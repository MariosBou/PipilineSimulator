#include <fstream>
#include <iostream>
#include <unordered_map>
#include <string>
#include<vector>
#include <map>
#include <iomanip>
#include <cstdint>
#include <bitset>
#include <algorithm>
#include <cctype>
#include<chrono>

using namespace std;




class Timer {
public:
    Timer() : elapsed_time(0), running(false) {}

    void start() {
        if (!running) {
            start_time = std::chrono::high_resolution_clock::now();
            running = true;
        }
    }

    void pause() {
        if (running) {
            auto now = std::chrono::high_resolution_clock::now();
            elapsed_time += std::chrono::duration_cast<std::chrono::nanoseconds>(now - start_time).count();
            running = false;
        }
    }

    void resume() {
        if (!running) {
            start_time = std::chrono::high_resolution_clock::now();
            running = true;
        }
    }


    long long elapsedNanoseconds() {
        if (running) {
            auto now = std::chrono::high_resolution_clock::now();
            return elapsed_time + std::chrono::duration_cast<std::chrono::nanoseconds>(now - start_time).count();
        }
        return elapsed_time;
    }

private:
    std::chrono::high_resolution_clock::time_point start_time;
    long long elapsed_time;
    bool running;
};


























class AluControl {

    //output
    int operation;

public:

    void activateOperation(int aluOp, int funct){

        if((aluOp==2 && (funct==32 || funct==33 || funct==-1 )) || (aluOp==0 && funct!=-100)) { operation=2; } //add
        else if((aluOp==2 && funct==36) || (aluOp==3 && funct==-1)) { operation=0; } //and
        else if((aluOp==2 && funct==37) || (aluOp==3 && funct==-2)) { operation=1; } //or
        else if(aluOp==1 || (aluOp==2 && (funct==34 || funct==35))) { operation=6; } //sub
        else if(aluOp==2 && funct==0) { operation=3; } //sll
        else if(aluOp==2 && funct==2) { operation=4; } //slr
        else if(aluOp==2 && funct==39) { operation=12; } //nor
        else if(aluOp==2 && (funct==42 || funct==43 || funct==-3 || funct==-4)) { operation=7; } //slt
        else if(aluOp==0 && funct==-100) { operation=8; }//lui

    }

    int getOperation() { return operation; }



};





class ControlUnit {
    //inputs
    int opcode;
    int currentstate=0;
    int previousState=0;

    //outputs
    bool aluSrcA,regWrite,regDest,PcWriteCond,PcWrite,IorD,memRead,memWrite,memToReg,IrWrite;
    int AluSrcB,Aluop,PcSource;


public:

    void getSignals(int code,int funct){
        opcode=code;
        regDest=0;
        aluSrcA=0;
        AluSrcB=0;
        memToReg=0;
        regWrite=0;
        memRead=0;
        memWrite=0;
        PcWriteCond=0;
        PcWrite=0;
        IorD=0;
        IrWrite=0;
        Aluop=0;
        PcSource=0;

        if(currentstate==4 || currentstate==5 || currentstate==8 || currentstate==7 || currentstate==9){
            if(currentstate==4){//Fifth state lw
                regDest=0;
                regWrite=1;
                memToReg=1;
            }
            else if(currentstate==5){//Forth state sw
                memWrite=1;
                IorD=1;
            }
            else if(currentstate==7){//Third state R-type & I-type
                regDest=1;
                regWrite=1;
                memToReg=0;
            }
            else if(currentstate==8){//Third state Beq & Bne
                aluSrcA=1;
                AluSrcB=0;
                Aluop=1;
                PcWriteCond=1;
                PcSource=1;
            }
            else if(currentstate==9){//Third state Jump
                PcWrite=1;
                PcSource=2;
            }
            previousState=currentstate;
            currentstate=0;
        }
        else if(currentstate==0){//First state all
            previousState=currentstate;
            currentstate=1;

            memRead=1;
            IorD=0;
            IrWrite=1;
            aluSrcA=0;
            AluSrcB=1;
            Aluop=0;
            PcWrite=1;
            PcSource=0;
        }
        else if(currentstate==1){//Second state all
            previousState=currentstate;
            if(opcode==43 || opcode==35) { currentstate=2; } //lw || sw
            else if(opcode==0 && funct==8) { currentstate=6; } //jr
            else if(opcode==0) { currentstate=6; }//R-type
            else if(opcode==4 || opcode==5) { currentstate=8; }//Beq || Bne
            else if(opcode==2) { currentstate=9; }//Jump
            else if((opcode>=8 && opcode<=13) || opcode==15) { currentstate=10; }//I-type
            else if(opcode==3) { currentstate=11; }//Jal

            aluSrcA=0;
            AluSrcB=3;
            Aluop=0;
        }
        else if(currentstate==2 && opcode==35){//Third state lw
            previousState=currentstate;
            currentstate=3;

            aluSrcA=1;
            AluSrcB=2;
            Aluop=0;
        }
        else if(currentstate==2 && opcode==43){//Third state sw
            previousState=currentstate;
            currentstate=5;

            aluSrcA=1;
            AluSrcB=2;
            Aluop=0;
        }
        else if(currentstate==3){//Forth state lw
            previousState=currentstate;
            currentstate=4;

            memRead=1;
            IorD=1;
        }
        else if(currentstate==6){//Third state R-type
            previousState=currentstate;
            if(funct==8) { currentstate=12; }
            else { currentstate=7; }

            aluSrcA=1;
            AluSrcB=0;
            Aluop=2;
        }
        else if(currentstate==10){//Third state I-type
            previousState=currentstate;
            currentstate=7;

            aluSrcA=1;
            AluSrcB=2;
            if(opcode==8 || opcode==9 ) { Aluop=2; }//addi addiu
            else if(opcode==10 || opcode==11 ) { Aluop=2; }//slti sltiu
            else if(opcode==12) { Aluop=3; }//andi
            else if(opcode==13) { Aluop=3; }//ori
            else if(opcode==15) { Aluop=0; }//lui
        }
        else if(currentstate==11){//Third state Jal
            previousState=currentstate;
            currentstate=0;

            PcSource=2;
            regDest=1;
            PcWrite=1;
            regWrite=1;

        }
        else if(currentstate==12){//Third state Jr
            previousState=currentstate;
            currentstate=0;

            PcSource=1;
            PcWrite=1;
        }
        else{ cout<<"Something went wrong with the states"<<endl; }
    }



    // Getters
    bool getAluSrcA() { return aluSrcA; }
    bool getRegWrite() { return regWrite; }
    bool getRegDest() { return regDest; }
    bool getPcWriteCond() { return PcWriteCond; }
    bool getPcWrite() { return PcWrite; }
    bool getIorD() { return IorD; }
    bool getMemRead() { return memRead; }
    bool getMemWrite() { return memWrite; }
    bool getMemToReg() { return memToReg; }
    bool getIrWrite() { return IrWrite; }
    int getAluSrcB() { return AluSrcB; }
    int getAluOp() { return Aluop; }
    int getPcSource() { return PcSource; }
    int getCurrent() { return currentstate; }
    int getPrevious() { return previousState; }

    void printSignals() {
        std::cout << "aluSrcA: " << aluSrcA << std::endl;
        std::cout << "regWrite: " << regWrite << std::endl;
        std::cout << "regDest: " << regDest << std::endl;
        std::cout << "pcWriteCond: " << PcWriteCond << std::endl;
        std::cout << "pcWrite: " << PcWrite << std::endl;
        std::cout << "iorD: " << IorD << std::endl;
        std::cout << "memRead: " << memRead << std::endl;
        std::cout << "memWrite: " << memWrite << std::endl;
        std::cout << "memToReg: " << memToReg << std::endl;
        std::cout << "irWrite: " << IrWrite << std::endl;
        std::cout << "aluSrcB: " << AluSrcB << std::endl;
        std::cout << "aluOp: " << Aluop << std::endl;
        std::cout << "pcSource: " << PcSource << std::endl;
        cout<<currentstate<<endl;
        cout<<previousState<<endl;
    }

};


































class Alu {

    // Inputs
    int operation;
    int input0; //rs
    int input1; //rt

    // Outputs
    int result;
    int prevRes=0;
    bool zero=0;

public:

    void makeOperation(int inpu0,int inpu1, int operatio){
        prevRes=result;
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
        else if(operation==8) { result=input1<<16; result=result&0xFFFFFFFF; }


        if(result==0 && operation==6) { zero=1; }

}




    // Getters
    int getInput0(){ return input0; }
    int getInput1(){ return input1; }
    int getResult(){ return result; }
    int getPrevResult(){ return prevRes; }
    bool getZero(){ return zero; }



};






































class Memory{

    //Data Memmory
    unordered_map<int, int> memoryMap;
    vector<int> valuesOrder;
    int index=0;

    //Instruction Memmory
    vector <string> memory;
    vector <string> origMemory;
    int count=0;





public:

    //Data Memory
    int getMem(int address){ return memoryMap[address]; }

    void setMem(int value, int addess){ memoryMap[addess]=value; valuesOrder.push_back(value); index++; }

    void getAllMemory(){

    for (size_t i = 0; i < valuesOrder.size(); i++) {
     cout<<hex<<valuesOrder[i]<<endl;
    }
    }


    void writeToFile(ofstream& outputFile) {

        if (!outputFile) {
            cout << "Error opening file!" << endl;
            return;
        }
        for(int i=0;i<valuesOrder.size();i++) {
            if(i!=0) { outputFile<<"\t"; }
            outputFile<<hex<<uppercase<<valuesOrder[i];
        }
    }

    //Instruction Memory

    //add instruction in memory
    void addInstruction (string instr){ memory.push_back(instr); index++; }

    //to save the original instructions
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

    bool isEmptyOrWhitespace(string fullinstruction) {
        for(char ch: fullinstruction){
            if((ch>='a' && ch<='z') || (ch>='A' && ch<='Z')){ return false;}
        }
        return true;
    }

    //read from file and add them in memory
    void updateInstructions(){
        //open file

        int dec,hex=0,jump=0,pos;
        ifstream file("mat_mul2024.txt");
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
        ifstream fileinstr("mat_mul2024.txt");
        if(!fileinstr.is_open()){
            cerr<< "File Not Found!" << endl;
        }


        //scan the file remove all the spaces, comments, and create instruction mem
        while(getline(fileinstr,fullinstruction)){
            hex=0;
            dec=0;
            if(ch!=':') { instruction.clear(); }
            else if(isEmptyOrWhitespace(fullinstruction)) { continue; }
            else { instruction+=':'; }
            for (size_t i = 0; i < fullinstruction.length(); i++) {
                ch = fullinstruction[i];


                //if comment then break
                if(ch== '#') { break; }

                else if( ch==' '|| ch=='\t') { continue; }

                //hex found
                else if(ch=='x' && fullinstruction[i-1]=='0' && ((fullinstruction[i+1]>=48 && fullinstruction[i+1]<58) || (fullinstruction[i+1]>=65 && fullinstruction[i+1]<=70))){ hex=1; }

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

            if(memory[i].substr(0,3)=="jal"){
                int metritis=0;
                for (char ch:memory[i]){
                    if(ch=='l' && metritis==0) { metritis++; }
                    else if(metritis>=1) { compare+=ch; }
                }
                for(string getnumber:words){
                    if(getnumber.substr(0,compare.length())==compare){
                        memory[i].clear();
                        memory[i] = "jal" + to_string(stoi(getnumber.substr(compare.length(), getnumber.length())));
                    }
                }
            }
            else if(memory[i][0]=='j'){
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






































class RegisterFile{

    int registers[32],pc;
    string rs,rt,rd,instruction,offset;
    int rsValue,rtValue;

public:

    RegisterFile(){
        for(int i=0;i<32;i++) { registers[i]=0; }
        registers[28]=268468224;
        registers[29]=2147483644;
    }

    void writeInRegister(int reg,int data){
        if(reg!=28 && reg!=29 && reg!=0 ) registers[reg]=data;
    }

    void setPc(int value) { pc=value; }

    int getPc()  { return pc; }

    int getReadData(int reg){ return registers[reg]; }

    void setRsValue(int value) { rsValue = value; }
    void setRtValue(int value) { rtValue = value; }

    int getRsValue() const { return rsValue; }
    int getRtValue() const { return rtValue; }


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
    void setInstruction(string value) { instruction = value; }





    // Getter methods
    string getRs() const { return rs; }
    string getRt() const { return rt; }
    string getRd() const { return rd; }
    string getInstruction() const { return instruction; }


};




















void output(ControlUnit controlUnit,int pc,Memory memory,RegisterFile registerFile,int opcode,int kikloi,Alu alu,int funct,int offset){
    ofstream outputFile("Marios_UC1067054.txt", ios::app);
    if (outputFile.is_open()) {

        outputFile << "-----Cycle "<<kikloi<<"-----\n"<<"Registers:\n"<<hex<<uppercase<<pc*4;

        //register print
        for(int i=0;i<32;i++) { outputFile<<"\t"<<hex<<uppercase<<registerFile.getReadData(i); }
        outputFile<<"\n\nMonitors:\n";
//allaso valo opcode==========================================================================================
        if(controlUnit.getPrevious()==0){ //All instruction
            outputFile<<hex<<uppercase
            <<(pc-1)*4<<"\t" //pc register
            <<(pc-1)*4<<"\t" //address memory
            <<"-"<<"\t" //write data
            <<memory.getOrigInstruction(pc-1)<<"\t" //memory Data
            <<"-"<<"\t" //Instruction
            <<"-"<<"\t" //Rs
            <<"-"<<"\t" //Rt
            <<"-"<<"\t" //Immediate
            <<"-"<<"\t" //Memory data Register
            <<"-"<<"\t" //Read Register 1
            <<"-"<<"\t" //Read Register 2
            <<"-"<<"\t" //Write Register
            <<"-"<<"\t" //Write Data
            <<"-"<<"\t" //Read Data 1
            <<"-"<<"\t" //Read Data 2
            <<"-"<<"\t" //Read Data 1 Register
            <<"-"<<"\t" //Read Data 2 Register
            <<(pc-1)*4<<"\t" //Alu Input 1
            <<"4"<<"\t" //Alu Input 2
            <<pc*4<<"\t" //Alu Result
            <<"-"<<"\t" //Alu Result Register
            <<controlUnit.getPcWriteCond()<<"\t" //Pc Write Condition
            <<controlUnit.getPcWrite()<<"\t" //Pc Write
            <<controlUnit.getIorD()<<"\t" //I or D
            <<controlUnit.getMemRead()<<"\t" //Memory Read
            <<controlUnit.getMemWrite()<<"\t" //Memory Write
            <<"-"<<"\t" //Memory to Regsiter
            <<controlUnit.getIrWrite()<<"\t" //Instruction Register Write
            <<bitset<2>(controlUnit.getPcSource())<<"\t" //PC Source
            <<bitset<2>(controlUnit.getAluOp())<<"\t" //Alu Op
            <<bitset<2>(controlUnit.getAluSrcB())<<"\t" //Alu Source B
            <<controlUnit.getAluSrcA()<<"\t" //Alu Source A
            <<controlUnit.getRegWrite()<<"\t" //Register Write
            <<"-"; //Register Destination
        }

        else if(controlUnit.getPrevious()==1){//All instructions

            int getLabel=0;
            string label="";
            if(opcode==4 || opcode==5){
                for(char ch:memory.getOrigInstruction(pc-1)){
                    if(ch==',') getLabel++;
                    else if(ch==' ') continue;
                    else if(getLabel==2) label+=ch;
                }
            }

            outputFile<<hex<<uppercase
            <<(pc)*4<<"\t" //pc register
            <<(pc-1)*4<<"\t" //address memory
            <<"-"<<"\t" //write data
            <<memory.getOrigInstruction(pc-1)<<"\t" //memory Data
            <<registerFile.getInstruction()<<"\t"; //Instruction
            if(opcode==4 || opcode==5){
                outputFile<<hex<<uppercase
                <<"$"<<registerFile.getRs()<<"\t" //Rs
                <<"$"<<registerFile.getRt()<<"\t" //Rt
                <<label<<"\t" //Immediate
                <<"-"<<"\t" //Memory data Register
                <<"$"<<registerFile.getRs()<<"\t" //Read Register 1
                <<"$"<<registerFile.getRt()<<"\t" //Read Register 2
                <<"-"<<"\t" //Write Register
                <<"-"<<"\t" //Write Data
                <<registerFile.getReadData(registerFile.getRegisterMatch(registerFile.getRs()))<<"\t" //Read Data 1
                <<registerFile.getReadData(registerFile.getRegisterMatch(registerFile.getRt()))<<"\t"; //Read Data 2
            }
            else if(opcode==0 && funct!=0 && funct!=2){
                outputFile<<hex<<uppercase
                <<"$"<<registerFile.getRs()<<"\t" //Rs
                <<"$"<<registerFile.getRt()<<"\t" //Rt
                <<"-"<<"\t" //Immediate
                <<"-"<<"\t" //Memory data Register
                <<"$"<<registerFile.getRs()<<"\t" //Read Register 1
                <<"$"<<registerFile.getRt()<<"\t" //Read Register 2
                <<"$"<<registerFile.getRd()<<"\t" //Write Register
                <<"-"<<"\t" //Write Data
                <<registerFile.getReadData(registerFile.getRegisterMatch(registerFile.getRs()))<<"\t" //Read Data 1
                <<registerFile.getReadData(registerFile.getRegisterMatch(registerFile.getRt()))<<"\t"; //Read Data 2
            }
            else if(opcode==0 && ( funct==0 || funct==2)){
                outputFile<<hex<<uppercase
                <<"-"<<"\t" //Rs
                <<"$"<<registerFile.getRs()<<"\t" //Rt
                <<"-"<<"\t" //Immediate
                <<"-"<<"\t" //Memory data Register
                <<"-"<<"\t" //Read Register 1
                <<"$"<<registerFile.getRs()<<"\t" //Read Register 2
                <<"$"<<registerFile.getRd()<<"\t" //Write Register
                <<"-"<<"\t" //Write Data
                <<"-"<<"\t" //Read Data 1
                <<hex<<"-"<<"\t"; //Read Data 2
            }
            else if(opcode==2 || opcode==3){
                outputFile<<hex<<uppercase
                <<"-"<<"\t" //Rs
                <<"-"<<"\t" //Rt
                <<"-"<<"\t" //Immediate
                <<"-"<<"\t" //Memory data Register
                <<"-"<<"\t" //Read Register 1
                <<"-"<<"\t" //Read Register 2
                <<"-"<<"\t" //Write Register
                <<"-"<<"\t" //Write Data
                <<"-"<<"\t" //Read Data 1
                <<"-"<<"\t"; //Read Data 2
            }
            else if(opcode==43){
                outputFile<<hex<<uppercase
                <<"$"<<registerFile.getRs()<<"\t" //Rs
                <<"$"<<registerFile.getRt()<<"\t" //Rt
                <<offset<<"\t" //Immediate
                <<"-"<<"\t" //Memory data Register
                <<"$"<<registerFile.getRs()<<"\t" //Read Register 1
                <<"-"<<"\t" //Write Register
                <<"$"<<registerFile.getRt()<<"\t" //Read Register 2
                <<"-"<<"\t" //Write Data
                <<registerFile.getReadData(registerFile.getRegisterMatch(registerFile.getRs()))<<"\t" //Read Data 1
                <<registerFile.getReadData(registerFile.getRegisterMatch(registerFile.getRt()))<<"\t"; //Read Data 2
            }
            else if(opcode==35){
                outputFile<<hex<<uppercase
                <<"$"<<registerFile.getRs()<<"\t" //Rs
                <<"$"<<registerFile.getRt()<<"\t" //Rt
                <<offset<<"\t" //Immediate
                <<"-"<<"\t" //Memory data Register
                <<"$"<<registerFile.getRs()<<"\t" //Read Register 1
                <<"-"<<"\t" //Read Register 2
                <<"$"<<registerFile.getRt()<<"\t" //Write Register
                <<"-"<<"\t" //Write Data
                <<registerFile.getReadData(registerFile.getRegisterMatch(registerFile.getRs()))<<"\t" //Read Data 1
                <<"-"<<"\t"; //Read Data 2
            }
            else if(opcode==15){
                outputFile<<hex<<uppercase
                <<"-"<<"\t" //Rs
                <<"$"<<registerFile.getRt()<<"\t" //Rt
                <<offset<<"\t" //Immediate
                <<"-"<<"\t" //Memory data Register
                <<"-"<<"\t" //Read Register 1
                <<"-"<<"\t" //Read Register 2
                <<"$"<<registerFile.getRt()<<"\t" //Write Register
                <<"-"<<"\t" //Write Data
                <<"-"<<"\t" //Read Data 1
                <<"-"<<"\t"; //Read Data 2
            }
            else{
                outputFile<<hex<<uppercase
                <<"$"<<registerFile.getRs()<<"\t" //Rs
                <<"$"<<registerFile.getRt()<<"\t" //Rt
                <<offset<<"\t" //Immediate
                <<"-"<<"\t" //Memory data Register
                <<"$"<<registerFile.getRs()<<"\t" //Read Register 1
                <<"-"<<"\t" //Read Register 2
                <<"$"<<registerFile.getRt()<<"\t" //Write Register
                <<"-"<<"\t" //Write Data
                <<registerFile.getReadData(registerFile.getRegisterMatch(registerFile.getRs()))<<"\t" //Read Data 1
                <<"-"<<"\t"; //Read Data 2
            }
            outputFile<<hex<<uppercase
            <<"-"<<"\t" //Read Data 1 Register
            <<"-"<<"\t";//Read Data 2 Register
            if(opcode!=4 && opcode!=5){
                outputFile<<hex<<uppercase
                <<"-"<<"\t" //Alu Input 1
                <<"-"<<"\t" //Alu Input 2
                <<"-"<<"\t" ;//Alu Result
            }
            else if(opcode==4 || opcode==5){
                outputFile<<hex<<uppercase
                <<pc*4<<"\t" //Alu Input 1
                <<label<<"\t" //Alu Input 2
                <<label<<"\t" ;//Alu Result
            }
            outputFile<<hex<<uppercase
            <<pc*4<<"\t" //Alu Result Register /////////////////////////prepi na fkali pcc!!!!!!!!!!!!!!!!!!!!!!!
            <<controlUnit.getPcWriteCond()<<"\t" //Pc Write Condition
            <<controlUnit.getPcWrite()<<"\t" //Pc Write
            <<"-"<<"\t" //I or D
            <<controlUnit.getMemRead()<<"\t" //Memory Read
            <<controlUnit.getMemWrite()<<"\t" //Memory Write
            <<"-"<<"\t" //Memory to Regsiter
            <<controlUnit.getIrWrite()<<"\t" //Instruction Register Write
            <<bitset<2>(controlUnit.getPcSource())<<"\t" //PC Source
            <<bitset<2>(controlUnit.getAluOp())<<"\t" //Alu Op
            <<bitset<2>(controlUnit.getAluSrcB())<<"\t" //Alu Source B
            <<controlUnit.getAluSrcA()<<"\t" //Alu Source A
            <<controlUnit.getRegWrite()<<"\t" //Register Write
            <<"-"; //Register Destination
        }
        else if(controlUnit.getPrevious()==2){
            outputFile<<hex<<uppercase
            <<(pc)*4<<"\t" //pc register
            <<(pc-1)*4<<"\t" //address memory
            <<"-"<<"\t" //write data
            <<memory.getOrigInstruction(pc-1)<<"\t" //memory Data
            <<registerFile.getInstruction()<<"\t" //Instruction
            <<"$"<<registerFile.getRs()<<"\t" //Rs
            <<"$"<<registerFile.getRt()<<"\t" //Rt
            <<offset<<"\t" //Immediate
            <<"-"<<"\t";//Memory data Register
            if(opcode==35){
                outputFile<<hex<<uppercase
                <<"$"<<registerFile.getRs()<<"\t" //Read Register 1
                <<"-"<<"\t" //Read Register 2
                <<"$"<<registerFile.getRt()<<"\t" //Write Register
                <<"-"<<"\t" //Write Data
                <<registerFile.getReadData(registerFile.getRegisterMatch(registerFile.getRs()))<<"\t" //Read Data 1
                <<"-"<<"\t" //Read Data 2
                <<registerFile.getReadData(registerFile.getRegisterMatch(registerFile.getRs()))<<"\t" //Read Data 1 Register
                <<"-"<<"\t"; //Read Data 2 Register
            }
            else if(opcode==43){
                outputFile<<hex<<uppercase
                <<"$"<<registerFile.getRs()<<"\t" //Read Register 1
                <<"-"<<"\t" //Read Register 2
                <<"$"<<registerFile.getRt()<<"\t" //Write Register
                <<"-"<<"\t" //Write Data
                <<registerFile.getReadData(registerFile.getRegisterMatch(registerFile.getRs()))<<"\t" //Read Data 1
                <<registerFile.getReadData(registerFile.getRegisterMatch(registerFile.getRt()))<<"\t" //Read Data 2
                <<registerFile.getReadData(registerFile.getRegisterMatch(registerFile.getRs()))<<"\t" //Read Data 1 Register
                <<"-"<<"\t"; //Read Data 2 Register
            }
            outputFile<<hex<<uppercase
            <<registerFile.getReadData(registerFile.getRegisterMatch(registerFile.getRs()))<<"\t" //Alu Input 1
            <<offset<<"\t" //Alu Input 2
            <<alu.getResult()<<"\t" //Alu Result
            <<"-"<<"\t" //Alu Result Register
            <<controlUnit.getPcWriteCond()<<"\t" //Pc Write Condition
            <<controlUnit.getPcWrite()<<"\t" //Pc Write
            <<"-"<<"\t" //I or D
            <<controlUnit.getMemRead()<<"\t" //Memory Read
            <<controlUnit.getMemWrite()<<"\t" //Memory Write
            <<"-"<<"\t" //Memory to Regsiter
            <<"-"<<"\t" //Instruction Register Write
            <<"-"<<"\t" //PC Source
            <<bitset<2>(controlUnit.getAluOp())<<"\t" //Alu Op
            <<bitset<2>(controlUnit.getAluSrcB())<<"\t" //Alu Source B
            <<controlUnit.getAluSrcA()<<"\t" //Alu Source A
            <<controlUnit.getRegWrite()<<"\t" //Register Write
            <<"-"; //Register Destination
        }
        else if(controlUnit.getPrevious()==3){
            outputFile<<hex<<uppercase
            <<(pc)*4<<"\t" //pc register
            <<alu.getResult()<<"\t" //address memory
            <<"-"<<"\t" //write data
            <<memory.getMem(alu.getResult())<<"\t" //memory Data
            <<registerFile.getInstruction()<<"\t" //Instruction
            <<"$"<<registerFile.getRs()<<"\t" //Rs
            <<"$"<<registerFile.getRt()<<"\t" //Rt
            <<offset<<"\t" //Immediate
            <<"-"<<"\t" //Memory data Register
            <<"$"<<registerFile.getRs()<<"\t" //Read Register 1
            <<"-"<<"\t" //Read Register 2
            <<"$"<<registerFile.getRt()<<"\t" //Write Register
            <<"-"<<"\t" //Write Data
            <<registerFile.getReadData(registerFile.getRegisterMatch(registerFile.getRs()))<<"\t" //Read Data 1
            <<"-"<<"\t" //Read Data 2
            <<registerFile.getReadData(registerFile.getRegisterMatch(registerFile.getRs()))<<"\t" //Read Data 1 Register
            <<"-"<<"\t" //Read Data 2 Register
            <<registerFile.getReadData(registerFile.getRegisterMatch(registerFile.getRs()))<<"\t" //Alu Input 1
            <<offset<<"\t" //Alu Input 2
            <<alu.getResult()<<"\t" //Alu Result
            <<alu.getResult()<<"\t" //Alu Result Register
            <<controlUnit.getPcWriteCond()<<"\t" //Pc Write Condition
            <<controlUnit.getPcWrite()<<"\t" //Pc Write
            <<controlUnit.getIorD()<<"\t" //I or D
            <<controlUnit.getMemRead()<<"\t" //Memory Read
            <<controlUnit.getMemWrite()<<"\t" //Memory Write
            <<"-"<<"\t" //Memory to Regsiter
            <<controlUnit.getIrWrite()<<"\t" //Instruction Register Write
            <<"-"<<"\t" //PC Source
            <<"-"<<"\t" //Alu Op
            <<"-"<<"\t" //Alu Source B
            <<"-"<<"\t" //Alu Source A
            <<controlUnit.getRegWrite()<<"\t" //Register Write
            <<"-"; //Register Destination
        }
        else if(controlUnit.getPrevious()==4){
            outputFile<<hex<<uppercase
            <<(pc)*4<<"\t" //pc register
            <<alu.getResult()<<"\t" //address memory
            <<"-"<<"\t" //write data
            <<memory.getMem(alu.getResult())<<"\t" //memory Data
            <<registerFile.getInstruction()<<"\t" //Instruction
            <<"$"<<registerFile.getRs()<<"\t" //Rs
            <<"$"<<registerFile.getRt()<<"\t" //Rt
            <<offset<<"\t" //Immediate
            <<memory.getMem(alu.getResult())<<"\t" //Memory data Register
            <<"$"<<registerFile.getRs()<<"\t" //Read Register 1
            <<"-"<<"\t" //Read Register 2
            <<"$"<<registerFile.getRt()<<"\t" //Write Register
            <<memory.getMem(alu.getResult())<<"\t" //Write Data
            <<registerFile.getReadData(registerFile.getRegisterMatch(registerFile.getRs()))<<"\t" //Read Data 1
            <<"-"<<"\t" //Read Data 2
            <<registerFile.getReadData(registerFile.getRegisterMatch(registerFile.getRs()))<<"\t" //Read Data 1 Register
            <<"-"<<"\t" //Read Data 2 Register
            <<registerFile.getReadData(registerFile.getRegisterMatch(registerFile.getRs()))<<"\t" //Alu Input 1
            <<offset<<"\t" //Alu Input 2
            <<alu.getResult()<<"\t" //Alu Result
            <<alu.getResult()<<"\t" //Alu Result Register
            <<controlUnit.getPcWriteCond()<<"\t" //Pc Write Condition
            <<controlUnit.getPcWrite()<<"\t" //Pc Write
            <<"-"<<"\t" //I or D
            <<controlUnit.getMemRead()<<"\t" //Memory Read
            <<controlUnit.getMemWrite()<<"\t" //Memory Write
            <<controlUnit.getMemToReg()<<"\t" //Memory to Regsiter
            <<controlUnit.getIrWrite()<<"\t" //Instruction Register Write
            <<"-"<<"\t" //PC Source
            <<"-"<<"\t" //Alu Op
            <<"-"<<"\t" //Alu Source B
            <<"-"<<"\t" //Alu Source A
            <<controlUnit.getRegWrite()<<"\t" //Register Write
            <<controlUnit.getRegDest(); //Register Destination
        }
        else if(controlUnit.getPrevious()==5){
            outputFile<<hex<<uppercase
            <<(pc)*4<<"\t" //pc register
            <<alu.getResult()<<"\t" //address memory
            <<registerFile.getReadData(registerFile.getRegisterMatch(registerFile.getRt()))<<"\t" //write data
            <<memory.getOrigInstruction(pc-1)<<"\t" //memory Data
            <<registerFile.getInstruction()<<"\t" //Instruction
            <<"$"<<registerFile.getRs()<<"\t" //Rs
            <<"$"<<registerFile.getRt()<<"\t" //Rt
            <<offset<<"\t" //Immediate
            <<"-"<<"\t" //Memory data Register
            <<"$"<<registerFile.getRs()<<"\t" //Read Register 1
            <<"-"<<"\t" //Write Register
            <<"$"<<registerFile.getRt()<<"\t" //Read Register 2
            <<"-"<<"\t" //Write Data
            <<registerFile.getReadData(registerFile.getRegisterMatch(registerFile.getRs()))<<"\t" //Read Data 1
            <<registerFile.getReadData(registerFile.getRegisterMatch(registerFile.getRt()))<<"\t" //Read Data 2
            <<registerFile.getReadData(registerFile.getRegisterMatch(registerFile.getRs()))<<"\t" //Read Data 1 Register
            <<registerFile.getReadData(registerFile.getRegisterMatch(registerFile.getRt()))<<"\t" //Read Data 2 Register
            <<registerFile.getReadData(registerFile.getRegisterMatch(registerFile.getRs()))<<"\t" //Alu Input 1
            <<offset<<"\t" //Alu Input 2
            <<alu.getResult()<<"\t" //Alu Result
            <<alu.getResult()<<"\t" //Alu Result Register
            <<controlUnit.getPcWriteCond()<<"\t" //Pc Write Condition
            <<controlUnit.getPcWrite()<<"\t" //Pc Write
            <<"-"<<"\t" //I or D
            <<controlUnit.getMemRead()<<"\t" //Memory Read
            <<controlUnit.getMemWrite()<<"\t" //Memory Write
            <<controlUnit.getMemToReg()<<"\t" //Memory to Regsiter
            <<controlUnit.getIrWrite()<<"\t" //Instruction Register Write
            <<"-"<<"\t" //PC Source
            <<"-"<<"\t" //Alu Op
            <<"-"<<"\t" //Alu Source B
            <<"-"<<"\t" //Alu Source A
            <<controlUnit.getRegWrite()<<"\t" //Register Write
            <<controlUnit.getRegDest(); //Register Destination
        }
        else if(controlUnit.getPrevious()==6){ //checkaro ti ginete jr kai slt ====================
            outputFile<<hex<<uppercase
            <<(pc)*4<<"\t" //pc register
            <<(pc-1)*4<<"\t" //address memory
            <<"-"<<"\t" //write data
            <<memory.getOrigInstruction(pc-1)<<"\t" //memory Data
            <<registerFile.getInstruction()<<"\t"; //Instruction
            if(funct==2 || funct==0){
                outputFile<<hex<<uppercase
                <<"-"<<"\t" //Rs
                <<"$"<<registerFile.getRs()<<"\t" //Rt
                <<dec<<"-"<<hex<<"\t" //Immediate
                <<"-"<<"\t" //Memory data Register
                <<"$"<<registerFile.getRs()<<"\t" //Read Register 1
                <<"-"<<"\t" //Read Register 2
                <<"$"<<registerFile.getRd()<<"\t" //Write Register
                <<"-"<<"\t" //Write Data
                <<registerFile.getReadData(registerFile.getRegisterMatch(registerFile.getRs()))<<"\t" //Read Data 1
                <<"-"<<"\t" //Read Data 2
                <<registerFile.getReadData(registerFile.getRegisterMatch(registerFile.getRs()))<<"\t" //Read Data 1 Register
                <<"-"<<"\t" //Read Data 2 Register
                <<registerFile.getReadData(registerFile.getRegisterMatch(registerFile.getRs()))<<"\t" //Alu Input 1
                <<offset<<"\t"; //Alu Input 2
            }
            else {
                outputFile<<hex<<uppercase
                <<"$"<<registerFile.getRs()<<"\t" //Rs
                <<"$"<<registerFile.getRt()<<"\t" //Rt
                <<"-"<<"\t" //Immediate
                <<"-"<<"\t" //Memory data Register
                <<"$"<<registerFile.getRs()<<"\t" //Read Register 1
                <<"$"<<registerFile.getRt()<<"\t" //Read Register 2
                <<"$"<<registerFile.getRd()<<"\t" //Write Register
                <<"-"<<"\t" //Write Data
                <<registerFile.getReadData(registerFile.getRegisterMatch(registerFile.getRs()))<<"\t" //Read Data 1
                <<registerFile.getReadData(registerFile.getRegisterMatch(registerFile.getRt()))<<"\t" //Read Data 2
                <<registerFile.getReadData(registerFile.getRegisterMatch(registerFile.getRs()))<<"\t" //Read Data 1 Register
                <<registerFile.getReadData(registerFile.getRegisterMatch(registerFile.getRt()))<<"\t" //Read Data 2 Register
                <<registerFile.getReadData(registerFile.getRegisterMatch(registerFile.getRs()))<<"\t" //Alu Input 1
                <<registerFile.getReadData(registerFile.getRegisterMatch(registerFile.getRt()))<<"\t"; //Alu Input 2
            }
            outputFile<<hex<<uppercase
            <<alu.getResult()<<"\t" //Alu Result
            <<"-"<<"\t" //Alu Result Register
            <<controlUnit.getPcWriteCond()<<"\t" //Pc Write Condition
            <<controlUnit.getPcWrite()<<"\t" //Pc Write
            <<"-"<<"\t" //I or D
            <<controlUnit.getMemRead()<<"\t" //Memory Read
            <<controlUnit.getMemWrite()<<"\t" //Memory Write
            <<"-"<<"\t" //Memory to Regsiter
            <<controlUnit.getIrWrite()<<"\t" //Instruction Register Write
            <<"-"<<"\t" //PC Source
            <<bitset<2>(controlUnit.getAluOp())<<"\t" //Alu Op
            <<bitset<2>(controlUnit.getAluSrcB())<<"\t" //Alu Source B
            <<controlUnit.getAluSrcA()<<"\t" //Alu Source A
            <<controlUnit.getRegWrite()<<"\t" //Register Write
            <<"-"; //Register Destination
        }
        else if(controlUnit.getPrevious()==7){
            if(opcode==0){
                outputFile<<hex<<uppercase
                <<(pc)*4<<"\t" //pc register
                <<(pc-1)*4<<"\t" //address memory
                <<"-"<<"\t" //write data
                <<memory.getOrigInstruction(pc-1)<<"\t" //memory Data
                <<registerFile.getInstruction()<<"\t"; //Instruction
                if(funct==0 || funct==2){
                    outputFile<<hex<<uppercase
                    <<"-"<<"\t" //Rs
                    <<"$"<<registerFile.getRs()<<"\t" //Rt
                    <<dec<<"-"<<hex<<"\t" //Immediate
                    <<"-"<<"\t" //Memory data Register
                    <<"$"<<registerFile.getRs()<<"\t" //Read Register 1
                    <<"-"<<"\t" //Read Register 2
                    <<"$"<<registerFile.getRd()<<"\t" //Write Register
                    <<alu.getResult()<<"\t" //Write Data
                    <<registerFile.getRsValue()<<"\t" //Read Data 1
                    <<"-"<<"\t" //Read Data 2
                    <<registerFile.getRsValue()<<"\t" //Read Data 1 Register
                    <<"-"<<"\t" //Read Data 2 Register
                    <<registerFile.getRsValue()<<"\t" //Alu Input 1
                    <<offset<<"\t" //Alu Input 2
                    <<alu.getResult()<<"\t" //Alu Result
                    <<alu.getResult()<<"\t"; //Alu Result Register
                }
                else{
                    outputFile<<hex<<uppercase
                    <<"$"<<registerFile.getRs()<<"\t" //Rs
                    <<"$"<<registerFile.getRt()<<"\t" //Rt
                    <<"-"<<"\t" //Immediate
                    <<"-"<<"\t" //Memory data Register
                    <<"$"<<registerFile.getRs()<<"\t" //Read Register 1
                    <<"$"<<registerFile.getRt()<<"\t" //Read Register 2
                    <<"$"<<registerFile.getRd()<<"\t" //Write Register
                    <<alu.getResult()<<"\t" //Write Data
                    <<registerFile.getRsValue()<<"\t" //Read Data 1
                    <<registerFile.getRtValue()<<"\t" //Read Data 2
                    <<registerFile.getRsValue()<<"\t" //Read Data 1 Register
                    <<registerFile.getRtValue()<<"\t" //Read Data 2 Register
                    <<registerFile.getRsValue()<<"\t" //Alu Input 1
                    <<registerFile.getRtValue()<<"\t" //Alu Input 2
                    <<alu.getResult()<<"\t" //Alu Result
                    <<alu.getResult()<<"\t"; //Alu Result Register
                }
                outputFile<<hex<<uppercase
                <<controlUnit.getPcWriteCond()<<"\t" //Pc Write Condition
                <<controlUnit.getPcWrite()<<"\t" //Pc Write
                <<"-"<<"\t" //I or D
                <<controlUnit.getMemRead()<<"\t" //Memory Read
                <<controlUnit.getMemWrite()<<"\t" //Memory Write
                <<controlUnit.getMemToReg()<<"\t" //Memory to Regsiter
                <<controlUnit.getIrWrite()<<"\t" //Instruction Register Write
                <<"-"<<"\t" //PC Source
                <<"-"<<"\t" //Alu Op
                <<"-"<<"\t" //Alu Source B
                <<"-"<<"\t" //Alu Source A
                <<controlUnit.getRegWrite()<<"\t" //Register Write
                <<controlUnit.getRegDest()<<"\t"; //Register Destination
            }
            else if(opcode==15){
                outputFile<<hex<<uppercase
                <<(pc)*4<<"\t" //pc register
                <<(pc-1)*4<<"\t" //address memory
                <<"-"<<"\t" //write data
                <<memory.getOrigInstruction(pc-1)<<"\t" //memory Data
                <<registerFile.getInstruction()<<"\t" //Instruction
                <<"-"<<"\t" //Rs
                <<"$"<<registerFile.getRt()<<"\t" //Rt
                <<offset<<"\t" //Immediate
                <<"-"<<"\t" //Memory data Register
                <<"-"<<"\t" //Read Register 1
                <<"-"<<"\t" //Read Register 2
                <<"$"<<registerFile.getRt()<<"\t" //Write Register
                <<alu.getResult()<<"\t" //Write Data
                <<"-"<<"\t" //Read Data 1
                <<"-"<<"\t" //Read Data 2
                <<"-"<<"\t" //Read Data 1 Register
                <<"-"<<"\t" //Read Data 2 Register
                <<"-"<<"\t" //Alu Input 1
                <<offset<<"\t" //Alu Input 2
                <<alu.getResult()<<"\t" //Alu Result
                <<alu.getResult()<<"\t" //Alu Result Register
                <<controlUnit.getPcWriteCond()<<"\t" //Pc Write Condition
                <<controlUnit.getPcWrite()<<"\t" //Pc Write
                <<"-"<<"\t" //I or D
                <<controlUnit.getMemRead()<<"\t" //Memory Read
                <<controlUnit.getMemWrite()<<"\t" //Memory Write
                <<controlUnit.getMemToReg()<<"\t" //Memory to Regsiter
                <<controlUnit.getIrWrite()<<"\t" //Instruction Register Write
                <<"-"<<"\t" //PC Source
                <<"-"<<"\t" //Alu Op
                <<"-"<<"\t" //Alu Source B
                <<"-"<<"\t" //Alu Source A
                <<controlUnit.getRegWrite()<<"\t" //Register Write
                <<"0"<<"\t"; //Register Destination
            }
            else{
                outputFile<<hex<<uppercase
                <<(pc)*4<<"\t" //pc register
                <<(pc-1)*4<<"\t" //address memory
                <<"-"<<"\t" //write data
                <<memory.getOrigInstruction(pc-1)<<"\t" //memory Data
                <<registerFile.getInstruction()<<"\t" //Instruction
                <<"$"<<registerFile.getRs()<<"\t" //Rs
                <<"$"<<registerFile.getRt()<<"\t" //Rt
                <<offset<<"\t" //Immediate
                <<"-"<<"\t" //Memory data Register
                <<"$"<<registerFile.getRs()<<"\t" //Read Register 1
                <<"-"<<"\t" //Read Register 2
                <<"$"<<registerFile.getRt()<<"\t" //Write Register
                <<alu.getResult()<<"\t" //Write Data
                <<registerFile.getReadData(registerFile.getRegisterMatch(registerFile.getRs()))<<"\t" //Read Data 1
                <<"-"<<"\t" //Read Data 2
                <<registerFile.getReadData(registerFile.getRegisterMatch(registerFile.getRs()))<<"\t" //Read Data 1 Register
                <<"-"<<"\t" //Read Data 2 Register
                <<registerFile.getReadData(registerFile.getRegisterMatch(registerFile.getRs()))<<"\t" //Alu Input 1
                <<offset<<"\t" //Alu Input 2
                <<alu.getResult()<<"\t" //Alu Result
                <<alu.getResult()<<"\t" //Alu Result Register
                <<controlUnit.getPcWriteCond()<<"\t" //Pc Write Condition
                <<controlUnit.getPcWrite()<<"\t" //Pc Write
                <<"-"<<"\t" //I or D
                <<controlUnit.getMemRead()<<"\t" //Memory Read
                <<controlUnit.getMemWrite()<<"\t" //Memory Write
                <<controlUnit.getMemToReg()<<"\t" //Memory to Regsiter
                <<controlUnit.getIrWrite()<<"\t" //Instruction Register Write
                <<"-"<<"\t" //PC Source
                <<"-"<<"\t" //Alu Op
                <<"-"<<"\t" //Alu Source B
                <<"-"<<"\t" //Alu Source A
                <<controlUnit.getRegWrite()<<"\t" //Register Write
                <<"0"; //Register Destination
            }
        }
        else if(controlUnit.getPrevious()==8){
                int getLabel=0;
                string label="";
                for(char ch:memory.getOrigInstruction(registerFile.getPc()-1)){
                    if(ch==',') getLabel++;
                    else if(ch==' ') continue;
                    else if(getLabel==2) label+=ch;
                }

                outputFile<<hex<<uppercase
                <<(registerFile.getPc())*4<<"\t" //pc register
                <<(registerFile.getPc()-1)*4<<"\t" //address memory
                <<"-"<<"\t" //write data
                <<memory.getOrigInstruction(registerFile.getPc()-1)<<"\t" //memory Data
                <<registerFile.getInstruction()<<"\t" //Instruction
                <<"$"<<registerFile.getRs()<<"\t" //Rs
                <<"$"<<registerFile.getRt()<<"\t" //Rt
                <<label<<"\t" //Immediate
                <<"-"<<"\t" //Memory data Register
                <<"$"<<registerFile.getRs()<<"\t" //Read Register 1
                <<"$"<<registerFile.getRt()<<"\t" //Read Register 2
                <<"-"<<"\t" //Write Register
                <<"-"<<"\t" //Write Data
                <<registerFile.getReadData(registerFile.getRegisterMatch(registerFile.getRs()))<<"\t" //Read Data 1
                <<registerFile.getReadData(registerFile.getRegisterMatch(registerFile.getRt()))<<"\t" //Read Data 2
                <<registerFile.getReadData(registerFile.getRegisterMatch(registerFile.getRs()))<<"\t" //Read Data 1 Register
                <<registerFile.getReadData(registerFile.getRegisterMatch(registerFile.getRt()))<<"\t" //Read Data 2 Register
                <<registerFile.getReadData(registerFile.getRegisterMatch(registerFile.getRs()))<<"\t" //Alu Input 1
                <<registerFile.getReadData(registerFile.getRegisterMatch(registerFile.getRt()))<<"\t" //Alu Input 2
                <<alu.getResult()<<"\t" //Alu Result
                <<"-"<<"\t" //Alu Result Register
                <<controlUnit.getPcWriteCond()<<"\t" //Pc Write Condition
                <<controlUnit.getPcWrite()<<"\t" //Pc Write
                <<"-"<<"\t" //I or D
                <<controlUnit.getMemRead()<<"\t" //Memory Read
                <<controlUnit.getMemWrite()<<"\t" //Memory Write
                <<controlUnit.getMemToReg()<<"\t" //Memory to Regsiter
                <<controlUnit.getIrWrite()<<"\t" //Instruction Register Write
                <<"-"<<"\t" //PC Source
                <<"01"<<"\t" //Alu Op
                <<"00"<<"\t" //Alu Source B
                <<"1"<<"\t" //Alu Source A
                <<controlUnit.getRegWrite()<<"\t" //Register Write
                <<controlUnit.getRegDest(); //Register Destination
        }
        else if(controlUnit.getPrevious()==9){
            outputFile<<hex<<uppercase
            <<(registerFile.getPc())*4<<"\t" //pc register
            <<(registerFile.getPc()-1)*4<<"\t" //address memory
            <<"-"<<"\t" //write data
            <<memory.getOrigInstruction(pc-1)<<"\t" //memory Data
            <<registerFile.getInstruction()<<"\t" //Instruction
            <<"-"<<"\t" //Rs
            <<"-"<<"\t" //Rt
            <<"-"<<"\t" //Immediate
            <<"-"<<"\t" //Memory data Register
            <<"-"<<"\t" //Read Register 1
            <<"-"<<"\t" //Read Register 2
            <<"-"<<"\t" //Write Register
            <<"-"<<"\t" //Write Data
            <<"-"<<"\t" //Read Data 1
            <<"-"<<"\t" //Read Data 2
            <<"-"<<"\t" //Read Data 1 Register
            <<"-"<<"\t" //Read Data 2 Register
            <<"-"<<"\t" //Alu Input 1
            <<"-"<<"\t" //Alu Input 2
            <<"-"<<"\t" //Alu Result
            <<"-"<<"\t" //Alu Result Register
            <<controlUnit.getPcWriteCond()<<"\t" //Pc Write Condition
            <<controlUnit.getPcWrite()<<"\t" //Pc Write
            <<"-"<<"\t" //I or D
            <<controlUnit.getMemRead()<<"\t" //Memory Read
            <<controlUnit.getMemWrite()<<"\t" //Memory Write
            <<"-"<<"\t" //Memory to Regsiter
            <<controlUnit.getIrWrite()<<"\t" //Instruction Register Write
            <<bitset<2>(controlUnit.getPcSource())<<"\t" //PC Source
            <<"-"<<"\t" //Alu Op
            <<"-"<<"\t" //Alu Source B
            <<"-"<<"\t" //Alu Source A
            <<controlUnit.getRegWrite()<<"\t" //Register Write
            <<"-"; //Register Destination
        }
        //I-type alu calculation output
        else if(controlUnit.getPrevious()==10){
            outputFile<<hex<<uppercase
            <<(pc)*4<<"\t" //pc register
            <<(pc-1)*4<<"\t" //address memory
            <<"-"<<"\t" //write data
            <<memory.getOrigInstruction(pc-1)<<"\t" //memory Data
            <<registerFile.getInstruction()<<"\t"; //Instruction
            if(opcode==15){
                outputFile<<hex<<uppercase
                <<"-"<<"\t"; //Rs
            }
            else{
                outputFile<<hex<<uppercase
                <<"$"<<registerFile.getRs()<<"\t"; //Rs
            }
            outputFile<<hex<<uppercase
            <<"$"<<registerFile.getRt()<<"\t" //Rt
            <<offset<<"\t" //Immediate
            <<"-"<<"\t"; //Memory data Register
            if(opcode==15){
                outputFile<<hex<<uppercase
                <<"-"<<"\t";
            }
            else{
                outputFile<<hex<<uppercase
                <<"$"<<registerFile.getRs()<<"\t"; //Read Register 1
            }
            outputFile<<hex<<uppercase
            <<"-"<<"\t" //Read Register 2
            <<"$"<<registerFile.getRt()<<"\t" //Write Register
            <<"-"<<"\t"; //Write Data
            if(opcode==15){
                outputFile<<hex<<uppercase
                <<"-"<<"\t";
            }
            else{
                outputFile<<hex<<uppercase
                <<registerFile.getReadData(registerFile.getRegisterMatch(registerFile.getRs()))<<"\t"; //Read Data 1
            }
            outputFile<<hex<<uppercase
            <<"-"<<"\t"; //Read Data 2
            if(opcode==15){
                outputFile<<hex<<uppercase
                <<"-"<<"\t";
            }
            else{
                outputFile<<hex<<uppercase
                <<registerFile.getReadData(registerFile.getRegisterMatch(registerFile.getRs()))<<"\t"; //Read Data 1 Register
            }
            outputFile<<hex<<uppercase
            <<"-"<<"\t"; //Read Data 2 Register
            if(opcode==15){
                outputFile<<hex<<uppercase
                <<"-"<<"\t" //Alu Input 1
                <<offset<<"\t"; //Alu Input 2
            }
            else{
                outputFile<<hex<<uppercase
                <<registerFile.getReadData(registerFile.getRegisterMatch(registerFile.getRs()))<<"\t" //Alu Input 1
                <<offset<<"\t"; //Alu Input 2
            }
            outputFile<<hex<<uppercase
            <<alu.getResult()<<"\t" //Alu Result
            <<"-"<<"\t" //Alu Result Register
            <<controlUnit.getPcWriteCond()<<"\t" //Pc Write Condition
            <<controlUnit.getPcWrite()<<"\t" //Pc Write
            <<"-"<<"\t" //I or D
            <<controlUnit.getMemRead()<<"\t" //Memory Read
            <<controlUnit.getMemWrite()<<"\t" //Memory Write
            <<"-"<<"\t" //Memory to Regsiter
            <<controlUnit.getIrWrite()<<"\t" //Instruction Register Write
            <<"-"<<"\t"; //PC Source
            if(opcode==8 || opcode==9 || opcode==12 || opcode==13 || opcode==15){
                outputFile<<hex<<uppercase
                <<"00"<<"\t"; //Alu Op
            }
            else if(opcode==10 || opcode==11){
                outputFile<<hex<<uppercase
                <<"01"<<"\t"; //Alu Op
            }
            outputFile<<hex<<uppercase
            <<bitset<2>(controlUnit.getAluSrcB())<<"\t" //Alu Source B
            <<controlUnit.getAluSrcA()<<"\t" //Alu Source A
            <<controlUnit.getRegWrite()<<"\t" //Register Write
            <<"-"; //Register Destination
        }
        //jal output
        else if(controlUnit.getPrevious()==11){
            outputFile<<hex<<uppercase
            <<(registerFile.getPc())*4<<"\t" //pc register
            <<(registerFile.getPc()-1)*4<<"\t" //address memory
            <<"-"<<"\t" //write data
            <<memory.getOrigInstruction(registerFile.getPc()-1)<<"\t" //memory Data
            <<registerFile.getInstruction()<<"\t" //Instruction
            <<"-"<<"\t" //Rs
            <<"-"<<"\t" //Rt
            <<"-"<<"\t" //Immediate
            <<"-"<<"\t" //Memory data Register
            <<"-"<<"\t" //Read Register 1
            <<"-"<<"\t" //Read Register 2
            <<"ra"<<"\t" //Write Register
            <<(registerFile.getPc())*4<<"\t" //Write Data
            <<"-"<<"\t" //Read Data 1
            <<"-"<<"\t" //Read Data 2
            <<"-"<<"\t" //Read Data 1 Register
            <<"-"<<"\t" //Read Data 2 Register
            <<"-"<<"\t" //Alu Input 1
            <<"-"<<"\t" //Alu Input 2
            <<"-"<<"\t" //Alu Result
            <<"-"<<"\t" //Alu Result Register
            <<controlUnit.getPcWriteCond()<<"\t" //Pc Write Condition
            <<controlUnit.getPcWrite()<<"\t" //Pc Write
            <<"-"<<"\t" //I or D
            <<controlUnit.getMemRead()<<"\t" //Memory Read
            <<controlUnit.getMemWrite()<<"\t" //Memory Write
            <<controlUnit.getMemToReg()<<"\t" //Memory to Regsiter
            <<controlUnit.getIrWrite()<<"\t" //Instruction Register Write
            <<bitset<2>(controlUnit.getPcSource())<<"\t" //PC Source
            <<"-"<<"\t" //Alu Op
            <<"-"<<"\t" //Alu Source B
            <<"-"<<"\t" //Alu Source A
            <<controlUnit.getRegWrite()<<"\t" //Register Write
            <<"-"; //Register Destination
        }
        //jr output
        else if(controlUnit.getPrevious()==12){
            outputFile<<hex<<uppercase
            <<(registerFile.getPc())*4<<"\t" //pc register
            <<(registerFile.getPc()-1)*4<<"\t" //address memory
            <<"-"<<"\t" //write data
            <<memory.getOrigInstruction(registerFile.getPc()-1)<<"\t" //memory Data
            <<registerFile.getInstruction()<<"\t" //Instruction
            <<"$"<<registerFile.getRs()<<"\t" //Rs
            <<"$"<<"zero"<<"\t" //Rt
            <<"-"<<"\t" //Immediate
            <<"-"<<"\t" //Memory data Register
            <<"$"<<registerFile.getRs()<<"\t" //Read Register 1
            <<"0"<<"\t" //Read Register 2
            <<"-"<<"\t" //Write Register
            <<"-"<<"\t" //Write Data
            <<registerFile.getReadData(registerFile.getRegisterMatch(registerFile.getRs()))*4<<"\t" //Read Data 1
            <<"0"<<"\t" //Read Data 2
            <<registerFile.getReadData(registerFile.getRegisterMatch(registerFile.getRs()))*4<<"\t" //Read Data 1 Register
            <<"0"<<"\t" //Read Data 2 Register
            <<registerFile.getReadData(registerFile.getRegisterMatch(registerFile.getRs()))*4<<"\t" //Alu Input 1
            <<"0"<<"\t" //Alu Input 2
            <<registerFile.getReadData(registerFile.getRegisterMatch(registerFile.getRs()))*4<<"\t" //Alu Result
            <<registerFile.getReadData(registerFile.getRegisterMatch(registerFile.getRs()))*4<<"\t" //Alu Result Register
            <<controlUnit.getPcWriteCond()<<"\t" //Pc Write Condition
            <<controlUnit.getPcWrite()<<"\t" //Pc Write
            <<"-"<<"\t" //I or D
            <<controlUnit.getMemRead()<<"\t" //Memory Read
            <<controlUnit.getMemWrite()<<"\t" //Memory Write
            <<controlUnit.getMemToReg()<<"\t" //Memory to Regsiter
            <<controlUnit.getIrWrite()<<"\t" //Instruction Register Write
            <<bitset<2>(controlUnit.getPcSource())<<"\t" //PC Source
            <<"-"<<"\t" //Alu Op
            <<"-"<<"\t" //Alu Source B
            <<"-"<<"\t" //Alu Source A
            <<controlUnit.getRegWrite()<<"\t" //Register Write
            <<"-"; //Register Destination
        }


        outputFile<<"\nMemory State:\n";
        memory.writeToFile(outputFile);
        outputFile<<"\n\n";


        outputFile.close();
    }
    else { cout<<"Error output.txt not Found" << endl; }
}

void begOutput(){
    ofstream outputFile("Marios_UC1067054.txt");
    if (outputFile.is_open()) {
            outputFile << "Name: your_name\n";
            outputFile.close();
        }
        else { cout<<"Error output.txt not Found" << endl; }
}


void finalOutput(int pc,RegisterFile registerFile,Memory memory, int kikloi,long long time){

    ofstream outputFile("Marios_UC1067054.txt", ios::app);

    if (outputFile.is_open()) {

            outputFile<<"-----Final State-----\nRegisters:\n"<<hex<<uppercase<<(pc)*4;
            for(int i=0;i<32;i++) { outputFile<<"\t"<<registerFile.getReadData(i); }
            outputFile<<"\n\nMemory State:\n";
            memory.writeToFile(outputFile);
            outputFile<<"\n\nTotal Cycles:\n"<<dec<<kikloi<<"\n\nUser Time:\n"<<time;
            outputFile.close();

    }
    else { cout<<"Error output.txt not Found" << endl; }
}





















int main(){

    Memory memory;
    ControlUnit controlUnit;
    AluControl aluControl;
    Alu alu;
    RegisterFile registerFile;
    Timer timer;


    begOutput();


    //read file and add them in memory
    memory.updateInstructions();


    string rs_temp,rt_temp,rd_temp,instruction;
    int pc=0,opcode=0,offset,funct,index=0,kikloi=0,cycle,negative=1,count=0,rs,rt,rd,input_alu0,input_alu1;
    string instroctiontype;
    cout<<"Please enter the number where you want to stop"<<endl;
    cin>>cycle;

    timer.start();

    while(true){
if(funct==0 || funct==2){
    cout<<"Epelekses na stamatisis edo: "<<memory.getInstruction(pc-1)<<endl;
    for(int i=0; i<32; i++) { cout<<registerFile.getReadData(i)<<endl; }
    controlUnit.printSignals();
}
        instruction=memory.getInstruction(pc);
        kikloi++;

        controlUnit.getSignals(opcode,funct);
        if(controlUnit.getPcWrite()==1 && controlUnit.getAluSrcA()==0 && controlUnit.getAluSrcB()==1 && controlUnit.getAluOp()==0 && controlUnit.getPcSource()==0) { cout<<instruction<<endl; pc++; }


        if(controlUnit.getIorD()==0 && controlUnit.getMemRead()==1){


            if (instruction.substr(0, 5)=="addiu"){
                opcode=9;
                instroctiontype="addiu";
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
            if(instruction.substr(0, 3)=="lui"){
                opcode=15;
                instroctiontype="lui";
                offset=0;
                funct=-100;
                for (char ch:instruction){
                    if(ch==',') { index++; }
                    else if (ch=='-') { negative=-1; }
                    else if(index==1) { offset= offset*10 + (ch-'0'); }
                }
                index=0;
                offset*=negative;
                negative=1;
            }
            else if (instruction.substr(0, 4)=="addi"){
                opcode=8;
                offset=0;
                funct=-1;
                instroctiontype="addi";
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
                instroctiontype="slti";
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
                instroctiontype="sltiu";
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
                instroctiontype="sw";
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
                instroctiontype="lw";
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
                instroctiontype="andi";
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
                instroctiontype="ori";
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
                instroctiontype="beq";
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
                instroctiontype="bne";
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
                instroctiontype="addu";
                offset=-1;
                funct=33;
            }
            else if(instruction.substr(0, 3)=="add"){
                opcode=0;
                instroctiontype="add";
                offset=-1;
                funct=32;
            }
            else if(instruction.substr(0, 3)=="sub"){
                opcode=0;
                instroctiontype="sub";
                offset=-1;
                funct=34;
            }
            else if(instruction.substr(0, 3)=="nor"){
                opcode=0;
                instroctiontype="nor";
                offset=-1;
                funct=39;
            }
            else if(instruction.substr(0, 4)=="subu"){
                opcode=0;
                instroctiontype="subu";
                offset=-1;
                funct=35;
            }
            else if(instruction.substr(0, 3)=="and"){
                opcode=0;
                instroctiontype="and";
                offset=-1;
                funct=36;
            }
            else if(instruction.substr(0, 3)=="slt"){
                opcode=0;
                instroctiontype="slt";
                offset=-1;
                funct=42;
            }
            else if(instruction.substr(0, 4)=="sltu"){
                opcode=0;
                instroctiontype="sltu";
                offset=-1;
                funct=43;
            }
            else if(instruction.substr(0, 2)=="or"){
                opcode=0;
                instroctiontype="or";
                offset=-1;
                funct=37;
            }
            else if(instruction.substr(0, 2)=="jr"){
                opcode=0;
                instroctiontype="jr";
                offset=-1;
                funct=8;
            }
            else if(instruction=="sll$zero,$zero,0"){ cout<<kikloi+3<<endl; break; }//============================================================
            else if(instruction.substr(0, 3)=="sll"){
                opcode=0;
                instroctiontype="sll";
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
                instroctiontype="srl";
                offset=0;
                funct=2;
                for (char ch:instruction){
                    if(ch==',') { index++; }
                    else if(index==2) { offset= offset*10 + (ch-'0'); }
                }
                index=0;
            }
            else if(instruction.substr(0,3)=="jal"){
                opcode=3;
                instroctiontype="jal";
                offset=0;
                funct=-1;
                for (char ch:instruction){
                    if(ch=='l') { index++; }
                    else if(index>=1) { offset= offset*10 + (ch-'0'); }
                }
                index=0;
            }
            else if(instruction.substr(0,1)=="j"){
                opcode=2;
                instroctiontype="j";
                offset=0;
                funct=-1;
                for (char ch:instruction){
                    if(ch=='j') { index++; }
                    else if(index>=1) { offset= offset*10 + (ch-'0'); }
                }
                index=0;
            }

        }

        if(controlUnit.getIrWrite()==1){

            rs_temp="";
            rt_temp="";
            rd_temp="";
            //get registers from instructions
            if(opcode==0){
                for (char ch:instruction){
                    if(ch==','){ count++; }
                    else if (ch=='$'){ count++; }
                    else if(count%2==1){
                            if(count==1) { if(funct==8) rs_temp+=ch; else rd_temp+=ch; }
                            else if(count==3) { rs_temp+=ch; }
                            else if(count==5) { rt_temp+=ch; }
                    }
                }
                if(funct==8) { rt_temp="zero"; rd_temp="zero"; }
            }

            else if(opcode==8 || opcode==9 || opcode==12 || opcode==13 || opcode==4 || opcode==5 || opcode==10 || opcode==15){
                for (char ch:instruction){
                    if(ch==','){ count++; }
                    else if (ch=='$'){ count++; }
                    else if(count%2==1){
                            if(count==1) { rt_temp+=ch;  }
                            else if(count==3) { rs_temp+=ch; }
                            else if(count==5) { rd_temp+=ch; }
                    }
                }
                rd_temp=rt_temp;
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


        }

        //STATE 1 ALL
        if(controlUnit.getAluSrcA()==0 && controlUnit.getAluSrcB()==3 && controlUnit.getAluOp()==0){
            registerFile.setInstruction(instroctiontype);
            registerFile.setRd(rd_temp);
            registerFile.setRs(rs_temp);
            registerFile.setRt(rt_temp);
            rs=registerFile.getRegisterMatch(rs_temp);
            rt=registerFile.getRegisterMatch(rt_temp);
        }

        //STATE 6 R-TYPE DO OPERATION
        else if(controlUnit.getAluSrcA()==1 && controlUnit.getAluSrcB()==0 && controlUnit.getAluOp()==2) {
            input_alu0=registerFile.getReadData(rs);
            if(funct==0 || funct==2) { input_alu1=offset; }
            else { input_alu1=registerFile.getReadData(rt); }

            //calculate alu control output (operation)
            aluControl.activateOperation(controlUnit.getAluOp(),funct);

            //calculate alu output (result)
            alu.makeOperation(input_alu0,input_alu1,aluControl.getOperation());
        }

        //STATE 7 R-TYPE & I-TYPE SAVE IN REGISTER
        else if(controlUnit.getRegDest()==1 && controlUnit.getRegWrite()==1 && controlUnit.getMemToReg()==0 && opcode!=3){
            registerFile.setRsValue(registerFile.getReadData(registerFile.getRegisterMatch(registerFile.getRs())));
            registerFile.setRtValue(registerFile.getReadData(registerFile.getRegisterMatch(registerFile.getRt())));
            registerFile.writeInRegister(registerFile.getRegisterMatch(registerFile.getRd()),alu.getResult());
        }

        //JUMP or JAl
        else if(controlUnit.getPcWrite()==1 && controlUnit.getPcSource()==2){
            registerFile.setPc(pc);

            if(opcode==3){
                registerFile.writeInRegister(31,pc);
            }
            pc=offset;
        }

        //Jr update pc
        else if(controlUnit.getPcSource()==1 && controlUnit.getPcWrite()==1 && controlUnit.getPcWriteCond()==0){
            registerFile.setPc(pc);
            pc=registerFile.getReadData(registerFile.getRegisterMatch(registerFile.getRs()));
        }

        //BEQ CALCULATE CONDITION IF TRUE BRANCH
        else if(controlUnit.getAluOp()==1 && controlUnit.getAluSrcA()==1 && controlUnit.getAluSrcB()==0 && controlUnit.getPcWriteCond()==1 && controlUnit.getPcSource()==1){
            registerFile.setPc(pc);

            input_alu0=registerFile.getReadData(rs);
            input_alu1=registerFile.getReadData(rt);

            //calculate alu control output (operation)
            aluControl.activateOperation(controlUnit.getAluOp(),funct);

            //calculate alu output (result)
            alu.makeOperation(input_alu0,input_alu1,aluControl.getOperation());

            if(opcode==4){ if(alu.getZero()==1) { pc=offset; } }
            else if(opcode==5) { if(alu.getResult()!=0) { pc=offset; } }
        }

        //CALCULATE OFFSET+ADDREESS FOR LW SW AND I-TYPE instructions
        else if(controlUnit.getAluSrcA()==1 && controlUnit.getAluSrcB()==2){

            input_alu0=registerFile.getReadData(rs);
            input_alu1=offset;

            //calculate alu control output (operation)
            aluControl.activateOperation(controlUnit.getAluOp(),funct);

            //calculate alu output (result)
            alu.makeOperation(input_alu0,input_alu1,aluControl.getOperation());

        }

        //LW READ FROM MEMORY
        else if(controlUnit.getMemRead()==1 && controlUnit.getIorD()==1){
            registerFile.writeInRegister(rt,memory.getMem(alu.getResult()));
        }

        //SW STORE IN MEMORY
        else if(controlUnit.getMemWrite()==1 && controlUnit.getIorD()==1){
            memory.setMem(registerFile.getReadData(rt),alu.getResult());
        }

        if(cycle==kikloi){
            timer.pause();
            output(controlUnit,pc,memory,registerFile,opcode,kikloi,alu,funct,offset);
            cout<<"Please enter the next number you want to stop. If you want no more prints give 0."<<endl;
            cin>>cycle;
            timer.resume();
        }

    }

    finalOutput(pc,registerFile,memory,kikloi+3,timer.elapsedNanoseconds());



    return 0;
}

