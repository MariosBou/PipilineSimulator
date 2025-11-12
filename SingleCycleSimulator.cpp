#include <fstream>
#include <iostream>
#include <unordered_map>
#include <string>
#include<vector>
#include <map>
#include <iomanip>

using namespace std;



class AluControl {

    //Inputs
    int aluOp,funct;

    //output
    int operation;


public:

    void activateOperation(bool aluop1, bool aluop0, int func){

        aluOp=0;
        funct=func;
        if(aluop1==true) { aluOp=1*2; }
        if(aluop0==true) { aluOp+=1; }

        if(aluOp==2 && (funct==32 || funct==33 || funct==-1 )) { operation=2; } //add
        else if((aluOp==2 && funct==36) || (aluOp==3 && funct==-1)) { operation=0; } //and
        else if((aluOp==2 && funct==37) || (aluOp==3 && funct==-2)) { operation=1; } //or
        else if(aluOp==1 || (aluOp==2 && (funct==34 || funct==35))) { operation=6; } //sub
        else if(aluOp==2 && funct==0) { operation=3; } //sll
        else if(aluOp==2 && funct==2) { operation=4; } //slr
        else if(aluOp==2 && funct==39) { operation=12; } //nor
        else if(aluOp==2 && (funct==42 || funct==43 || funct==-3 || funct==-4)) { operation=7; } //slt

    }

    int getOperation() { return operation; }



};







class ControlUnit {

    //inputs
    int opcode;

    //output
    bool regDest,aluSrc,memToReg,regWrite,memRead,memWrite,branch,aluOp0,aluOp1,jump;
public:

    void activateSignals(int code){
        opcode=code;
        regDest=0;
        aluSrc=0;
        memToReg=0;
        regWrite=0;
        memRead=0;
        memWrite=0;
        branch=0;
        aluOp0=0;
        aluOp1=0;

        //Eksisosi RegDest
        if(opcode==0) { regDest=1; }
        else { regDest=0; }

        //Eksisosi aluSrc
        if(opcode==8 || opcode==9 || opcode==12 || opcode==13 || opcode==10 || opcode==11 || opcode==43 || opcode==35) { aluSrc=1; }
        else { aluSrc=0; }

        //eksisosi gia memToReg
        if(opcode==35) { memToReg=1; }
        else { memToReg=0; }

        //eksisosi gia regWrite
        if(opcode==0 || opcode==9 || opcode==8 || opcode==12 || opcode==13 || opcode==10 || opcode==11 || opcode==35) { regWrite=1; }
        else { regWrite=0; }

        //eksisosi gia memRead
        if(opcode==35) { memRead=1; }
        else { memRead=0; }

        //eksisosi gia memWrite
        if(opcode==43) { memWrite=1; }
        else { memWrite=0; }

        //eksisosi gia branch
        if(opcode==4 || opcode==5) { branch=1; }
        else { branch=0; }

        //eksisosi gia aluop0
        if(opcode==12 || opcode==13 || opcode==4 || opcode==5) { aluOp0=1; }
        else { aluOp0=0; }

        //eksisosi gia aluop1
        if(opcode==0 || opcode==9 || opcode==8 || opcode==12 || opcode==13 || opcode==10 || opcode==11 || opcode==43 || opcode==35) { aluOp1=1; }
        else { aluOp1=0; }

        //eksisosi gia jump
        if(opcode==2) { jump=1; }
        else { jump=0; }

        //printSignals();

    }

    void printSignals(){
        cout <<regDest<<aluSrc<<memToReg<<regWrite<<memRead<<memWrite<<branch<<jump<<aluOp1<<aluOp0<<endl;
    }

    bool getRegDest() { return regDest; }
    bool getAluSrc() { return aluSrc; }
    bool getMemToReg() { return memToReg; }
    bool getRegWrite() { return regWrite; }
    bool getMemRead() { return memRead; }
    bool getMemWrite() { return memWrite; }
    bool getBranch() { return branch; }
    bool getAluOp0() { return aluOp0; }
    bool getAluOp1() { return aluOp1; }
    bool getJump() { return jump; }


};


class Alu {

    // Inputs
    int operation;
    int input0; //rs
    int input1; //rt

    // Outputs
    int result;
    bool zero=0;

public:

    void makeOperation(int inpu0,int inpu1, int operatio){
        zero=0;
        operation=operatio;
        input0=inpu0;
        input1=inpu1;
        if(operation==2) { result=input0+input1; }
        else if(operation==0) { result=input0&input1; }
        else if(operation==1) { result=input0|input1; }
        else if(operation==6) { result=input0-input1; }
        else if(operation==3) { result=input0<<input1; result=result&0xFFFFFFFF; }
        else if(operation==4) { result=input0>>input1; result=result&0xFFFFFFFF; }
        else if(operation==12) { result=~(input0 | input1); }
        else if(operation==7) { result=(input0<input1)? 1:0; }


        if(result==0 && operation==6) { zero=1; }

}




    // Getters
    int getInput0(){ return input0; }
    int getInput1(){ return input1; }
    int getResult(){ return result; }
    bool getZero(){ return zero; }



};







class DataMemory{


    unordered_map<int, int> memoryMap;
    vector<int> valuesOrder;
    int index=0;



public:

    int getMem(bool memRead,int address){ return memoryMap[address]; }

    void setMem(bool memWrite, int value, int addess){

        if(memWrite==1){ memoryMap[addess]=value; valuesOrder.push_back(value); index++; }
    }


    void writeToFile(ofstream& outputFile) {

        if (!outputFile) {
            cout << "Error opening file!" << endl;
            return;
        }

        for(int i=0;i<index;i++) {
            if(i!=0) { outputFile<<"\t"; }
            outputFile<<hex<<uppercase<<valuesOrder[i];
        }
    }




};





class RegisterFile{

    int registers[32];
    string rs,rt,rd;


public:

    RegisterFile(){
        for(int i=0;i<32;i++) { registers[i]=0; }
        registers[28]=268468224;
        registers[29]=2147483644;
    }

    void writeInRegister(int reg,int data){
        if(reg!=28 && reg!=29 && reg!=0 ) registers[reg]=data;
    }

    int getReadData(int reg){ return registers[reg]; }

    int getRegisterMatch(string reg){
        map<string, int> registerMap = {
            {"r0",0}, {"zero", 0}, {"at", 1},  {"v0", 2},  {"v1", 3},
            {"a0", 4},   {"a1", 5},  {"a2", 6},  {"a3", 7},
            {"t0", 8},   {"t1", 9},  {"t2", 10}, {"t3", 11},
            {"t4", 12},  {"t5", 13}, {"t6", 14}, {"t7", 15},
            {"s0", 16},  {"s1", 17}, {"s2", 18}, {"s3", 19},
            {"s4", 20},  {"s5", 21}, {"s6", 22}, {"s7", 23},
            {"t8", 24},  {"t9", 25}, {"k0", 26}, {"k1", 27},
            {"gp", 28},  {"sp", 29}, {"fp", 30}, {"ra", 31}
        };

        return registerMap.find(reg)->second;
    }

    // Setter methods
    void setRs(string value) { rs = value; }
    void setRt(string value) { rt = value; }
    void setRd(string value) { rd = value; }

    // Getter methods
    string getRs() const { return rs; }
    string getRt() const { return rt; }
    string getRd() const { return rd; }
};






class InstructionMem{

    vector <string> memory;
    vector <string> origMemory;
    int index=0;

public:

    //add instruction in memory
    void addInstruction (string instr){ memory.push_back(instr); index++; }

    void addOriginalInstruction(string instr){ origMemory.push_back(instr); }



    //get the number of instructions in the memory
    int getIndex() { return index; }

    //get a specific instruction
    string getInstruction(int num){

        if(num<0 && num>index){
            cout << "incorrect num" << endl;
        }
        else {
            return memory[num];
        }
    return nullptr;
    }

    string getOrigInstruction(int num){ return origMemory[num]; }

    //read from file and add them in memory
    void updateInstructions(){
        //open file

        int dec,hex=0,jump=0,pos;
        ifstream file("find_min2025.s");
        if(!file.is_open()){
            cerr<< "File Not Found!" << endl;
        }

        string fullinstruction,instruction;
        int jumplink;

        char ch;

        //add original instructions for print
        while(getline(file,fullinstruction)){
            instruction.clear();
            jumplink=0;
            for (char ch : fullinstruction) {

                //comment then break
                if(ch== '#') { break; }

                //if jump link do not write as instruction
                else if(ch==':') { jumplink=1; }

                //add only letters numbers , $
                else if((ch>96 && ch<123) || (ch==36) || (ch==44) || (ch>47 && ch <58 ) || (ch>=65 && ch<=70)){ instruction+=ch; }
                else if(ch==45 || ch==43 || ch==40 || ch==41) { instruction+=ch; }
            }

            //ignore this instructions
            if(instruction=="data" || instruction=="text" || instruction=="main"
               || instruction=="" ) {
                continue;
            }

            if(jumplink==0) { addOriginalInstruction(instruction); }
        }

        //close and reopen the file to save the instructions with addresses instead now
        file.close();
        ifstream fileinstr("find_min2025.s");
        if(!fileinstr.is_open()){
            cerr<< "File Not Found!" << endl;
        }


        //scan the file remove all the spaces, comments, and create instruction mem
        while(getline(fileinstr,fullinstruction)){
            hex=0;
            dec=0;
            if(ch!=':') { instruction.clear(); }
            else { instruction+=':'; }
            for (size_t i = 0; i < fullinstruction.length(); i++) {
                ch = fullinstruction[i];


                //if comment then break
                if(ch== '#') { break; }

                else if( ch==' '|| ch=='\t') { continue; }

                //hex found
                else if(ch=='x' && fullinstruction[i-1]=='0' && ((fullinstruction[i+1]>48 && fullinstruction[i+1]<58) || (fullinstruction[i+1]>=65 && fullinstruction[i+1]<=70))){ hex=1; }

                //if hex then do this
                else if(hex==1){

                    string hexvalue="";
                    for(size_t k=i; k<fullinstruction.length();k++){
                        ch=fullinstruction[k];
                        hexvalue+=ch;
                    }
                    dec=std::stoi(hexvalue,nullptr,16);
                    break;

                }

                //add only letters numbers , $ ( )
                else if((ch>96 && ch<123) || (ch==36) || (ch==44) || (ch>47 && ch <58 )){ instruction+=ch; }

                else if(ch==45 || ch==43 || ch==40 || ch==41) { instruction+=ch; }

                if(ch==':') break;
            }

            //ignore this instructions
            if(instruction=="data" || instruction=="text" || instruction=="main"
               || instruction=="" ) {
                ch='a';
                continue;
            }

            //if hex
            else if(hex==1){
                instruction.pop_back();
                instruction+=to_string(dec);
            }
            if(ch!=':'){ addInstruction(instruction); }
        }

        //in jump and branch make it numbers
        int found;
        vector <string> words;
        string compare;

        //search for jump or branch word
        for(int i=0;i<index;i++){
            compare.clear();
            found=0;
            for(char c: memory[i]){
                if(c==':'){
                    found=1;
                    break;
                }
            }
            //extract jump or branch word
            if(found==1){
                for(char c: memory[i]){
                    if(c==':') { break; }
                    compare+=c;
                }
                compare+=to_string(i);
                words.push_back(compare);


                //remove the jump or branch word
                size_t pos = memory[i].find(':');
                if (pos != std::string::npos) {
                    memory[i] = memory[i].substr(pos + 1);
                }
            }
        }

        //replace the words with the adrreses
        for(int i=0;i<index;i++){
            compare.clear();
            if(memory[i][0]=='j'){
                compare=memory[i].substr(1,memory[i].length()-1);
                for(string getnumber:words){
                    if(getnumber.substr(0,compare.length())==compare){
                        memory[i].clear();
                        memory[i] = "j" + to_string(stoi(getnumber.substr(compare.length(), getnumber.length())));
                    }
                }
            }
            else if(memory[i].substr(0,3)=="beq"){
                size_t firstComma = memory[i].find(',');
                size_t secondComma = memory[i].find(',', firstComma + 1); //find second comma
                compare=memory[i].substr(secondComma+1,memory[i].length()-secondComma); //get the word that we want to match
                for(string getnumber:words){
                    if(getnumber.substr(0,compare.length())==compare){
                        string rememberinstruction;
                        rememberinstruction=memory[i].substr(0,secondComma+1);
                        memory[i].clear();
                        //replace instruction with new instruction that has address instead of word
                        memory[i] =rememberinstruction+ to_string(stoi(getnumber.substr(compare.length(), getnumber.length())));
                    }
                }
            }
            //same as the branch
            else if(memory[i].substr(0,3)=="bne"){
                size_t firstComma = memory[i].find(',');
                size_t secondComma = memory[i].find(',', firstComma + 1);
                compare=memory[i].substr(secondComma+1,memory[i].length()-secondComma);
                for(string getnumber:words){
                    if(getnumber.substr(0,compare.length())==compare){
                        string rememberinstruction;
                        rememberinstruction=memory[i].substr(0,secondComma+1);
                        memory[i].clear();
                        memory[i] =rememberinstruction+ to_string(stoi(getnumber.substr(compare.length(), getnumber.length())));
                    }
                }
            }
        }
        file.close();
    }
};



//funstion for output on demand
void output(string instruction,Alu alu, AluControl aluControl,RegisterFile registerFile,ControlUnit controlUnit,int pc,int opcode,int funct,DataMemory dataMemory,int kikloi){
    ofstream outputFile("Marios_UC1067054.txt", ios::app);
    int count=0;

    if (outputFile.is_open()) {
        outputFile << "-----Cycle "<<kikloi<<"-----\n"<<"Registers:\n";

        //output for PC after instruction
        if(alu.getZero()==1 && controlUnit.getBranch()==1 && opcode==4) { outputFile<<hex<<uppercase<<funct*4; }
        else if(alu.getZero()==0 && controlUnit.getBranch()==1 && opcode==5) { outputFile<<hex<<uppercase<<funct*4; }
        else if(controlUnit.getJump()==1) { outputFile<<hex<<uppercase<<funct*4; }
        else { outputFile<<hex<<uppercase<<(pc-1)*4; }


        //instruction print
        for(int i=0;i<32;i++) { outputFile<<"\t"<<hex<<uppercase<<registerFile.getReadData(i); }
        outputFile<<"\n\nMonitors:\n"<<hex<<uppercase<<(pc-2)*4<<"\t";
        for(char ch: instruction){

                //periptosi lw, sw
                if((opcode==43 || opcode==35) && count==1 && ch==',') { outputFile<<ch<<" "; }
                else if((opcode==43|| opcode==35) && count==1 && ch!=',') { outputFile<<ch; }

                //periptosi jump
                else if(opcode==2 && count==0) {outputFile<<ch<<" "; count++;}

                //periptosi kanoniki
                else if(ch=='$' && count<2) { outputFile<<" "<<ch; count++; }
                else if(ch==',' && count==2) { outputFile<<ch<<" "; }
                else { outputFile<<ch; }
        }

        outputFile<<"\t";

        //addi addiu andi ori==================================================================================aluop ftixno====
        if(opcode==8 || opcode==9 || opcode==12 || opcode==13){

            outputFile<<"$"
            <<registerFile.getRs()<<"\t$"
            <<registerFile.getRt()<<"\t"
            <<"-\t"
            <<alu.getResult()<<"\t"
            <<registerFile.getReadData(registerFile.getRegisterMatch(registerFile.getRs()))<<"\t"
            <<"-\t"
            <<alu.getResult()<<"\t"
            <<"-\t"
            <<"-\t"
            <<"-\t"
            <<"-\t"
            <<controlUnit.getRegDest()<<"\t"
            <<controlUnit.getJump()<<"\t"
            <<controlUnit.getBranch()<<"\t"
            <<controlUnit.getMemRead()<<"\t"
            <<controlUnit.getMemToReg()<<"\t";
            if(opcode==8 || opcode==9) outputFile<<"10"<<"\t";
            else outputFile<<"00"<<"\t";
            outputFile<<controlUnit.getMemWrite()<<"\t"
            <<controlUnit.getAluSrc()<<"\t"
            <<controlUnit.getRegWrite()<<"\n\n";

        }

        //add addu sub subu and or xor slt sltu
        else if(opcode==0 && ((funct>=32 && funct<=37) || funct==39 || funct==42 || funct==43)){

            outputFile<<"$"
            <<registerFile.getRs()<<"\t$"
            <<registerFile.getRt()<<"\t$"
            <<registerFile.getRd()<<"\t"
            <<alu.getResult()<<"\t"
            <<registerFile.getReadData(registerFile.getRegisterMatch(registerFile.getRs()))<<"\t"
            <<registerFile.getReadData(registerFile.getRegisterMatch(registerFile.getRt()))<<"\t"
            <<alu.getResult()<<"\t"
            <<"-\t"
            <<"-\t"
            <<"-\t"
            <<"-\t"
            <<controlUnit.getRegDest()<<"\t"
            <<controlUnit.getJump()<<"\t"
            <<controlUnit.getBranch()<<"\t"
            <<controlUnit.getMemRead()<<"\t"
            <<controlUnit.getMemToReg()<<"\t"
            <<"00"<<"\t"
            <<controlUnit.getMemWrite()<<"\t"
            <<controlUnit.getAluSrc()<<"\t"
            <<controlUnit.getRegWrite()<<"\n\n";

        }

        //output for sll srl
        else if(opcode==0 && (funct==2 || funct==0)){
            outputFile
            <<"-"<<"\t$"
            <<registerFile.getRs()<<"\t$"
            <<registerFile.getRd()<<"\t"
            <<alu.getResult()<<"\t"
            <<"-"<<"\t"
            <<registerFile.getReadData(registerFile.getRegisterMatch(registerFile.getRs()))<<"\t"
            <<alu.getResult()<<"\t"
            <<"-\t"
            <<"-\t"
            <<"-\t"
            <<"-\t"
            <<controlUnit.getRegDest()<<"\t"
            <<controlUnit.getJump()<<"\t"
            <<controlUnit.getBranch()<<"\t"
            <<controlUnit.getMemRead()<<"\t"
            <<controlUnit.getMemToReg()<<"\t"
            <<"11"<<"\t"
            <<controlUnit.getMemWrite()<<"\t"
            <<controlUnit.getAluSrc()<<"\t"
            <<controlUnit.getRegWrite()<<"\n\n";

        }

        //sw
        else if(opcode==43){//===================================================================================

            outputFile<<"$"
            <<registerFile.getRs()<<"\t$"
            <<registerFile.getRt()<<"\t"
            <<"-"<<"\t"
            <<"-"<<"\t"
            <<registerFile.getReadData(registerFile.getRegisterMatch(registerFile.getRs()))<<"\t"
            <<registerFile.getReadData(registerFile.getRegisterMatch(registerFile.getRt()))<<"\t"
            <<alu.getResult()<<"\t"
            <<"-\t"
            <<alu.getResult()<<"\t"
            <<registerFile.getReadData(registerFile.getRegisterMatch(registerFile.getRt()))<<"\t"
            <<"-\t"
            <<"-"<<"\t"
            <<controlUnit.getJump()<<"\t"
            <<controlUnit.getBranch()<<"\t"
            <<controlUnit.getMemRead()<<"\t"
            <<"-"<<"\t"
            <<"00"<<"\t"
            <<controlUnit.getMemWrite()<<"\t"
            <<controlUnit.getAluSrc()<<"\t"
            <<controlUnit.getRegWrite()<<"\n\n";

        }

        //lw
        else if(opcode==35) {

            outputFile<<"$"
            <<registerFile.getRs()<<"\t"
            <<"-"<<"\t$"
            <<registerFile.getRt()<<"\t"
            <<dataMemory.getMem(1,alu.getResult())<<"\t"
            <<registerFile.getReadData(registerFile.getRegisterMatch(registerFile.getRs()))<<"\t"
            <<"-"<<"\t"
            <<alu.getResult()<<"\t"
            <<"-\t"<<
            alu.getResult()<<"\t"
            <<"-"<<"\t"
            <<dataMemory.getMem(1,alu.getResult())<<"\t"
            <<controlUnit.getRegDest()<<"\t"
            <<controlUnit.getJump()<<"\t"
            <<controlUnit.getBranch()<<"\t"
            <<controlUnit.getMemRead()<<"\t"
            <<controlUnit.getMemToReg()<<"\t"
            <<"00"<<"\t"
            <<controlUnit.getMemWrite()<<"\t"
            <<controlUnit.getAluSrc()<<"\t"
            <<controlUnit.getRegWrite()<<"\n\n";

        }

        //output for beq bne
        else if(opcode==4 || opcode==5){

            //to get the label name
            int getLabel=0;
            string label="";
            for(char ch:instruction){

                if(ch==',') getLabel++;
                else if(ch==' ') continue;
                else if(getLabel==2) label+=ch;

            }

            outputFile<<"$"
            <<registerFile.getRt()<<"\t$"
            <<registerFile.getRs()<<"\t"
            <<"-"<<"\t"
            <<"-"<<"\t"
            <<registerFile.getReadData(registerFile.getRegisterMatch(registerFile.getRt()))<<"\t"
            <<registerFile.getReadData(registerFile.getRegisterMatch(registerFile.getRs()))<<"\t";
            if(alu.getZero()==0 && controlUnit.getBranch()==1 && opcode==5) { outputFile<<"1"; }
            else {outputFile<<"0"; }
            outputFile<<"\t"
            <<label<<"\t"
            <<"-"<<"\t"
            <<"-"<<"\t"
            <<"-"<<"\t"
            <<"-"<<"\t"
            <<controlUnit.getJump()<<"\t"
            <<controlUnit.getBranch()<<"\t"
            <<controlUnit.getMemRead()<<"\t"
            <<"-"<<"\t"
            <<"01"<<"\t"
            <<controlUnit.getMemWrite()<<"\t"
            <<controlUnit.getAluSrc()<<"\t"
            <<controlUnit.getRegWrite()<<"\n\n";

        }

        //output for jump
        else if(opcode==2){

            //to get the label name
            int getLabel=0;
            string label="";
            for(char ch:instruction){

                if(ch=='j') getLabel++;
                else if(ch==' ') continue;
                else if(getLabel==1) label+=ch;

            }

            outputFile
            <<"-"<<"\t"
            <<"-"<<"\t"
            <<"-\t"
            <<"-\t"
            <<"-\t"
            <<"-"<<"\t"
            <<"-"<<"\t"
            <<"-"<<"\t"
            <<"-"<<"\t"
            <<"-"<<"\t"
            <<"-"<<"\t"
            <<"-"<<"\t"
            <<controlUnit.getJump()<<"\t"
            <<controlUnit.getBranch()<<"\t"
            <<controlUnit.getMemRead()<<"\t"
            <<"-"<<"\t"
            <<"-"<<"\t"
            <<controlUnit.getMemWrite()<<"\t"
            <<"-"<<"\t"
            <<controlUnit.getRegWrite()<<"\n\n";

        }

        //slt sltu
        else if(opcode==0 && ( funct==2 || funct==3)){

            outputFile<<"$"
            <<registerFile.getRs()<<"\t$"
            <<registerFile.getRt()<<"\t$"
            <<registerFile.getRd()<<"\t"
            <<alu.getResult()<<"\t"
            <<registerFile.getReadData(registerFile.getRegisterMatch(registerFile.getRs()))<<"\t"
            <<registerFile.getReadData(registerFile.getRegisterMatch(registerFile.getRt()))<<"\t"
            <<alu.getResult()<<"\t"
            <<"-\t"
            <<"-\t"
            <<"-\t"
            <<"-\t"
            <<controlUnit.getRegDest()<<"\t"
            <<controlUnit.getJump()<<"\t"
            <<controlUnit.getBranch()<<"\t"
            <<controlUnit.getMemRead()<<"\t"
            <<controlUnit.getMemToReg()<<"\t"
            <<"10"<<"\t"
            <<controlUnit.getMemWrite()<<"\t"
            <<controlUnit.getAluSrc()<<"\t"
            <<controlUnit.getRegWrite()<<"\n\n";

        }

        //slti sltiu
        else if(opcode==10 || opcode==11){

            outputFile<<"$"
            <<registerFile.getRs()<<"\t$"
            <<registerFile.getRt()<<"\t"
            <<"-"<<"\t"
            <<alu.getResult()<<"\t"
            <<registerFile.getReadData(registerFile.getRegisterMatch(registerFile.getRs()))<<"\t"
            <<"-"<<"\t"
            <<alu.getResult()<<"\t"
            <<"-\t"
            <<"-\t"
            <<"-\t"
            <<"-\t"
            <<controlUnit.getRegDest()<<"\t"
            <<controlUnit.getJump()<<"\t"
            <<controlUnit.getBranch()<<"\t"
            <<controlUnit.getMemRead()<<"\t"
            <<controlUnit.getMemToReg()<<"\t"
            <<"11"<<"\t"
            <<controlUnit.getMemWrite()<<"\t"
            <<controlUnit.getAluSrc()<<"\t"
            <<controlUnit.getRegWrite()<<"\n\n";

        }


        //output for memory state
        outputFile<<"Memory State:\n";
        dataMemory.writeToFile(outputFile);
        outputFile<<"\n\n";

        //close file
        outputFile.close();
    }

    else { cout<<"Error output.txt not Found" << endl; }

}

//function for final output
void finaloutput(Alu alu, AluControl aluControl,RegisterFile registerFile,ControlUnit controlUnit,int pc,int kikloi,DataMemory dataMemory){
    ofstream outputFile("Marios_UC1067054.txt", ios::app);

    if (outputFile.is_open()) {
            outputFile<<"-----Final State-----\nRegisters:\n"<<hex<<uppercase<<(pc)*4;
            for(int i=0;i<32;i++) { outputFile<<"\t"<<registerFile.getReadData(i); }
            outputFile<<"\n\nMemory State:\n";
            dataMemory.writeToFile(outputFile);
            outputFile<<"\n\nTotal Cycles:\n"<<dec<<kikloi;
    }
    else { cout<<"Error output.txt not Found" << endl; }
}


int main(){

    int rs,rt,rd,opcode,funct;

    ofstream outputFile("Marios_UC1067054.txt");
    if (outputFile.is_open()) {
        outputFile << "Name: your_name\n";
        outputFile.close();
    }
    else { cout<<"Error output.txt not Found" << endl; }

//to memory state tiponoume ola ta memory positions pou allasoun

    InstructionMem instructionMem;
    ControlUnit controlUnit;
    AluControl aluControl;
    Alu alu;
    DataMemory dataMemory;
    RegisterFile registerFile;


    //read file and add them in memory
    instructionMem.updateInstructions();

    int pc=0; //pou tha to tiposo pc *4
    int cycle=-1; // where to stop to print
    string instruction;
    int input_alu0,input_alu1,offset=0,i=0;
    string rs_temp,rt_temp,rd_temp;
    int count=0,index=0,negative=1,kikloi=0;
    char instroctiontype=' ';
    cout<<"Please enter the number where you want to stop"<<endl;
    cin>>cycle;

    while(true){

        instruction=instructionMem.getInstruction(pc);
        pc++;
        kikloi++;


        if (instruction.substr(0, 5)=="addiu") {
            opcode=9;
            instroctiontype='i';
            offset=0;
            funct=-1;
            for (char ch:instruction){
                if(ch==',') { index++; }
                else if (ch=='-') { negative=-1; }
                else if(index==2) { offset= offset*10 + (ch-'0'); }
            }
            index=0;
            offset*=negative;
            negative=1;
        }
        else if (instruction.substr(0, 4)=="addi"){
            opcode=8;
            offset=0;
            funct=-1;
            instroctiontype='i';
            for (char ch:instruction){
                if(ch==',') { index++; }
                else if (ch=='-') { negative=-1; }
                else if(index==2) { offset= offset*10 + (ch-'0'); }
            }
            index=0;
            offset*=negative;
            negative=1;
        }
        else if (instruction.substr(0, 4)=="slti"){
            opcode=10;
            offset=0;
            funct=-3;
            instroctiontype='i';
            for (char ch:instruction){
                if(ch==',') { index++; }
                else if (ch=='-') { negative=-1; }
                else if(index==2) { offset= offset*10 + (ch-'0'); }
            }
            index=0;
            offset*=negative;
            negative=1;
        }
        else if (instruction.substr(0, 5)=="sltiu"){
            opcode=11;
            offset=0;
            funct=-4;
            instroctiontype='i';
            for (char ch:instruction){
                if(ch==',') { index++; }
                else if (ch=='-') { negative=-1; }
                else if(index==2) { offset= offset*10 + (ch-'0'); }
            }
            index=0;
            offset*=negative;
            negative=1;
        }
        else if (instruction.substr(0, 2)=="sw"){
            opcode=43;
            offset=0;
            funct=-1;
            instroctiontype='i';
            for (char ch:instruction){
                if(ch==',' || ch=='(') { index++; }
                else if (ch=='-') { negative=-1; }
                else if(index==1) { offset= offset*10 + (ch-'0'); }
            }
            index=0;
            offset*=negative;
            negative=1;
        }
        else if (instruction.substr(0, 2)=="lw"){
            opcode=35;
            offset=0;
            funct=-1;
            instroctiontype='i';
            for (char ch:instruction){
                if(ch==',' || ch=='(') { index++; }
                else if (ch=='-') { negative=-1; }
                else if(index==1) { offset= offset*10 + (ch-'0'); }
            }
            index=0;
            offset*=negative;
            negative=1;
        }
        else if (instruction.substr(0, 4)=="andi"){
            opcode=12;
            offset=0;
            funct=-1;
            instroctiontype='i';
            for (char ch:instruction){
                if(ch==',') { index++; }
                else if (ch=='-') { negative=-1; }
                else if(index==2) { offset= offset*10 + (ch-'0'); }
            }
            index=0;
            offset*=negative;
            negative=1;
        }
        else if (instruction.substr(0, 3)=="ori"){
            opcode=13;
            offset=0;
            funct=-2;
            instroctiontype='i';
            for (char ch:instruction){
                if(ch==',') { index++; }
                else if (ch=='-') { negative=-1; }
                else if(index==2) { offset= offset*10 + (ch-'0'); }
            }
            index=0;
            offset*=negative;
            negative=1;
        }
        else if (instruction.substr(0, 3)=="beq"){
            opcode=4;
            offset=0;
            funct=-1;
            instroctiontype='i';
            for (char ch:instruction){
                if(ch==',') { index++; }
                else if (ch=='-') { negative=-1; }
                else if(index==2) { offset= offset*10 + (ch-'0'); }
            }
            index=0;
            offset*=negative;
            negative=1;
        }
        else if (instruction.substr(0, 3)=="bne"){
            opcode=5;
            offset=0;
            funct=-1;
            instroctiontype='i';
            for (char ch:instruction){
                if(ch==',') { index++; }
                else if (ch=='-') { negative=-1; }
                else if(index==2) { offset= offset*10 + (ch-'0'); }
            }
            index=0;
            offset*=negative;
            negative=1;
        }
        else if (instruction.substr(0, 4)=="addu") {
            opcode=0;
            instroctiontype='r';
            offset=-1;
            funct=33;
        }
        else if(instruction.substr(0, 3)=="add"){
            opcode=0;
            instroctiontype='r';
            offset=-1;
            funct=32;
        }
        else if(instruction.substr(0, 3)=="sub"){
            opcode=0;
            instroctiontype='r';
            offset=-1;
            funct=34;
        }
        else if(instruction.substr(0, 3)=="nor"){
            opcode=0;
            instroctiontype='r';
            offset=-1;
            funct=39;
        }
        else if(instruction.substr(0, 4)=="subu"){
            opcode=0;
            instroctiontype='r';
            offset=-1;
            funct=35;
        }
        else if(instruction.substr(0, 3)=="and"){
            opcode=0;
            instroctiontype='r';
            offset=-1;
            funct=36;
        }
        else if(instruction.substr(0, 3)=="slt"){
            opcode=0;
            instroctiontype='r';
            offset=-1;
            funct=42;
        }
        else if(instruction.substr(0, 4)=="sltu"){
            opcode=0;
            instroctiontype='r';
            offset=-1;
            funct=43;
        }
        else if(instruction.substr(0, 2  )=="or"){
            opcode=0;
            instroctiontype='r';
            offset=-1;
            funct=37;
        }
        else if(instruction=="sll$zero,$zero,0"){ break; }
        else if(instruction.substr(0, 3)=="sll"){
            opcode=0;
            instroctiontype='r';
            offset=0;
            funct=0;
            for (char ch:instruction){
                if(ch==',') { index++; }
                else if(index==2) { offset= offset*10 + (ch-'0'); }
            }
            index=0;
        }
        else if(instruction.substr(0, 3)=="srl"){
            opcode=0;
            instroctiontype='r';
            offset=0;
            funct=2;
            for (char ch:instruction){
                if(ch==',') { index++; }
                else if(index==2) { offset= offset*10 + (ch-'0'); }
            }
            index=0;
        }
        else if(instruction.substr(0,1)=="j"){
            opcode=2;
            instroctiontype='j';
            offset=0;
            funct=-1;
            for (char ch:instruction){
                if(ch=='j') { index++; }
                else if(index>=1) { offset= offset*10 + (ch-'0'); }
            }
            index=0;
        }

        //activate corresponding signals
        controlUnit.activateSignals(opcode);

        //calculate registers
        rs_temp="";
        rt_temp="";
        rd_temp="";
        if(opcode==0){
            for (char ch:instruction){
                if(ch==','){ count++; }
                else if (ch=='$'){ count++; }
                else if(count%2==1){
                        if(count==1) { rd_temp+=ch; }
                        else if(count==3) { rs_temp+=ch; }
                        else if(count==5) { rt_temp+=ch; }
                }
            }
        }

        else if(opcode==8 || opcode==9 || opcode==12 || opcode==13 || opcode==4 || opcode==5 || opcode==10){
            for (char ch:instruction){
                if(ch==','){ count++; }
                else if (ch=='$'){ count++; }
                else if(count%2==1){
                        if(count==1) { rt_temp+=ch;  }
                        else if(count==3) { rs_temp+=ch; }
                        else if(count==5) { rd_temp+=ch; }
                }
            }
        }
        else if(opcode==43 || opcode==35){
            for (char ch:instruction){
                if(ch==','){ count++; }
                else if (ch=='$'){ count++; }
                else if (ch==')') {count++; }
                else if(count%2==1){
                        if(count==1) { rt_temp+=ch;  }
                        else if(count==3) { rs_temp+=ch; }
                        else if(count==5) { rd_temp+=ch; }
                }
            }
        }


        count=0;
        registerFile.setRd(rd_temp);
        registerFile.setRs(rs_temp);
        registerFile.setRt(rt_temp);
        rs=registerFile.getRegisterMatch(rs_temp);
        rt=registerFile.getRegisterMatch(rt_temp);


        if(controlUnit.getRegDest()){ rd=registerFile.getRegisterMatch(rd_temp); }
        else { rd=rt; }


        //clalculate alu inputs
        if(opcode!=2) { input_alu0=registerFile.getReadData(rs); }


        if(controlUnit.getAluSrc()==0 && funct!=0 && funct!=2 && opcode!=2) { input_alu1=registerFile.getReadData(rt); }
        else { input_alu1=offset; }


        //calculate alu control output (operation)
        aluControl.activateOperation(controlUnit.getAluOp1(),controlUnit.getAluOp0(),funct);


        //calculate alu output (result)
        alu.makeOperation(input_alu0,input_alu1,aluControl.getOperation());

        //write in memory
        if(controlUnit.getMemWrite()==1){
            dataMemory.setMem(controlUnit.getMemWrite(),registerFile.getReadData(rt),alu.getResult());
        }

        //write alu or memory to register?1
        if(controlUnit.getMemToReg()){

            //write in register from memory
            if(controlUnit.getRegWrite()){ registerFile.writeInRegister(rd,dataMemory.getMem(controlUnit.getMemRead(),alu.getResult())); }
        }
        else {

            //write in register from alu
            if(controlUnit.getRegWrite()){ registerFile.writeInRegister(rd,alu.getResult()); }

        }


        //stop to print
        if(cycle==kikloi){
                cout<<instruction<<endl;
            if(opcode==2 || opcode==4 || opcode==5) { output(instructionMem.getOrigInstruction(pc-1),alu,aluControl,registerFile,controlUnit,pc+1,opcode,offset,dataMemory,cycle); }
            else { output(instructionMem.getOrigInstruction(pc-1),alu,aluControl,registerFile,controlUnit,pc+1,opcode,funct,dataMemory,cycle); }
            cout<<"Please enter the next number you want to stop. If you want no more prints give 0."<<endl;
            cin>>cycle;
        }


        if(alu.getZero()==1 && controlUnit.getBranch()==1 && opcode==4) { pc=offset; }
        else if(alu.getZero()==0 && controlUnit.getBranch()==1 && opcode==5) { pc=offset; }
        else if(controlUnit.getJump()==1) { pc=offset; }

    }
    finaloutput(alu,aluControl,registerFile,controlUnit,pc,kikloi,dataMemory);

    return 0;
}
