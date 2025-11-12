#include <fstream>
#include <iostream>
#include <unordered_map>
#include <string>
#include<vector>
#include <map>
#include <iomanip>
#include <cstdint>
#include <cctype>
#include<chrono>

using namespace std;





class MemoryRegister{

    int aluResult[2]={0,0};
    int readData[2]={0,0};
    string writeRegister[3]={"",""};
    string instruction[3]={"",""};
    int pc[2]={0,0};

    char memtoReg[3]={'0','0'},regWrite[3]={'0','0'}; //write back stage signals

public:

    void setRegister(int result, int data, string write, char memtoreg,char regwrite,string instr,int cc){
        aluResult[1]=aluResult[0];
        readData[1]=readData[0];
        writeRegister[1]=writeRegister[0];
        memtoReg[1]=memtoReg[0];
        regWrite[1]=regWrite[0];
        instruction[1]=instruction[0];
        pc[1]=pc[0];

        aluResult[0]=result;
        readData[0]=data;
        writeRegister[0]=write;
        memtoReg[0]=memtoreg;
        regWrite[0]=regwrite;
        instruction[0]=instr;
        pc[0]=cc;
    }

    int getAluResult()  { return aluResult[1]; }
    int getReadData() { return readData[1]; }
    int getPC()  { return pc[1];}

    string getWriteRegister()  { return writeRegister[1]; }
    char getMemToReg()  { return memtoReg[1]; }
    char getRegWrite()  { return regWrite[1]; }
    string getInstruction()  { return instruction[1]; }

    int getsAluResult()  { return aluResult[0]; }
    int getsReadData()  { return readData[0]; }
    int getsPC()  { return pc[0];}

    string getsWriteRegister()  { return writeRegister[0]; }
    char getsMemToReg()  { return memtoReg[0]; }
    char getsRegWrite()  { return regWrite[0]; }
    string getsInstruction()  { return instruction[0]; }


    void printRegister() const {
        std::cout << "Write-Back Register Values at Position [1]:" << std::endl;
        std::cout << "ALU Result: " << aluResult[1] << std::endl;
        std::cout << "Read Data: " << readData[1] << std::endl;
        std::cout << "Write Register: " << writeRegister[1] << std::endl;
        std::cout << "PC: " << pc[1] << std::endl;

        std::cout << "\nWrite-Back Stage Signals at Position [1]:" << std::endl;
        std::cout << "Mem to Reg: " << static_cast<int>(memtoReg[1]) << std::endl;
        std::cout << "Reg Write: " << static_cast<int>(regWrite[1]) << std::endl;
    }
};














class ExecuteRegister{

    //values
    int aluResult[2]={0,0};
    int rtData[2]={0,0};
    string writeRegister[3]={"",""};
    int offset[2]={0,0};
    int pc[2]={0,0};
    int opcode[2]={0,0};
    string instruction[3]={"",""};


    char memWrite[3]={'0','0'},memRead[3]={'0','0'},Branch[3]={'0','0'}; //memory stage signals

    char memtoReg[3]={'0','0'},regWrite[3]={'0','0'}; //write back stage signals

public:

    void setRegister(int result,int data,string write,int off, int c, char memwrite,char memread, char branch,char memtoreg,char regwrite,int code,string instr){

        aluResult[1]=aluResult[0];
        rtData[1]=rtData[0];
        writeRegister[1]=writeRegister[0];
        offset[1]=offset[0];
        pc[1]=pc[0];
        memWrite[1]=memWrite[0];
        memRead[1]=memRead[0];
        Branch[1]=Branch[0];
        memtoReg[1]=memtoReg[0];
        regWrite[1]=regWrite[0];
        opcode[1]=opcode[0];
        instruction[1]=instruction[0];

        aluResult[0]=result;
        rtData[0]=data;
        writeRegister[0]=write;
        offset[0]=off;
        pc[0]=c;
        opcode[0]=code;
        instruction[0]=instr;

        memWrite[0]=memwrite;
        memRead[0]=memread;
        Branch[0]=branch;

        memtoReg[0]=memtoreg;
        regWrite[0]=regwrite;
    }

    int getAluResult() const { return aluResult[1]; }
    int getRtData() const { return rtData[1]; }
    string getWriteRegister() const { return writeRegister[1]; }
    int getOffset() const { return offset[1]; }
    int getPC() const { return pc[1]; }
    int getOpcode() const { return opcode[1]; }
    string getInstruction() const { return instruction[1]; }

    char getMemWrite() const { return memWrite[1]; }
    char getMemRead() const { return memRead[1]; }
    char getBranch() const { return Branch[1]; }

    char getMemToReg() const { return memtoReg[1]; }
    char getRegWrite() const { return regWrite[1]; }

    int getsAluResult() const { return aluResult[0]; }
    int getsRtData() const { return rtData[0]; }
    string getsWriteRegister() const { return writeRegister[0]; }
    int getsOffset() const { return offset[0]; }
    int getsPC() const { return pc[0]; }
    int getsOpcode() const { return opcode[0]; }
    string getsInstruction() const { return instruction[0]; }

    char getMsemWrite() const { return memWrite[0]; }
    char getsMemRead() const { return memRead[0]; }
    char getsBranch() const { return Branch[0]; }

    char getsMemToReg() const { return memtoReg[0]; }
    char getsRegWrite() const { return regWrite[0]; }

    void printRegister() const {
        std::cout << "Values at position [1]:" << std::endl;
        std::cout << "aluResult: " << aluResult[1] << std::endl;
        std::cout << "rtData: " << rtData[1] << std::endl;
        std::cout << "writeRegister: " << writeRegister[1] << std::endl;
        std::cout << "offset: " << offset[1] << std::endl;
        std::cout << "pc: " << pc[1] << std::endl;

        std::cout << "\nMemory Stage Signals at position [1]:" << std::endl;
        std::cout << "memWrite: " << static_cast<int>(memWrite[1]) << std::endl;
        std::cout << "memRead: " << static_cast<int>(memRead[1]) << std::endl;
        std::cout << "Branch: " << static_cast<int>(Branch[1]) << std::endl;

        std::cout << "\nWrite-Back Stage Signals at position [1]:" << std::endl;
        std::cout << "memtoReg: " << static_cast<int>(memtoReg[1]) << std::endl;
        std::cout << "regWrite: " << static_cast<int>(regWrite[1]) << std::endl;
    }
};















class InstructionRegister {

    string instruction[3]={"",""};
    int pc[2]={0,0};
    int i=0;

public:

    void setRegister(const string &instr, const int c){
            instruction[1]=instruction[0];
            pc[1]=pc[0];
            instruction[0]=instr;
            pc[0]=c;
    }

    void setInstruction(string var) { instruction[0]=var; }
    void flashInstruction() { instruction[0]="addi$zero,$zero,0"; }

    string getInstruction(){ return instruction[1]; }
    int getPC(){ return pc[1]; }

    string getsInstruction(){ return instruction[0]; }
    int getsPC(){ return pc[0]; }

};












class DataRegister {

    //values
    int rsData[2]={0,0};
    int rtData[2]={0,0};
    int cc[2]={0,0};
    int offset[2]={0,0};
    string rt[3]={"",""};
    string rd[3]={"",""};
    string rs[3]={"",""};
    int opcode[2]={0,0};
    int funct[2]={0,0};
    string instruction[3]={"",""};


    char aluOp[3]={'0','0'},aluSrc[3]={'0','0'},regDest[3]={'0','0'}; //execute stage signals

    char memWrite[3]={'0','0'},memRead[3]={'0','0'},Branch[3]={'0','0'}; //memory stage signals

    char memtoReg[3]={'0','0'},regWrite[3]={'0','0'}; //write back stage signals


public:

    void setRegister(string tt, string d, int s,int t, int p, char aluop,char alusrc, char regdest, char memwrite,
                char memread, char branch, char memtoreg, char regwrite,string ss,int pp,int ff,int func,string isntr){
        //Move everything to the output
        rsData[1]=rsData[0];
        rtData[1]=rtData[0];
        cc[1]=cc[0];
        rt[1]=rt[0];
        rd[1]=rd[0];
        aluOp[1]=aluOp[0];
        aluSrc[1]=aluSrc[0];
        regDest[1]=regDest[0];
        memWrite[1]=memWrite[0];
        memRead[1]=memRead[0];
        Branch[1]=Branch[0];
        memtoReg[1]=memtoReg[0];
        regWrite[1]=regWrite[0];
        rs[1]=rs[0];
        opcode[1]=opcode[0];
        offset[1]=offset[0];
        funct[1]=funct[0];
        instruction[1]=instruction[0];

        //values stored
        offset[0]=ff;
        rsData[0]=s;
        rtData[0]=t;
        cc[0]=p;
        rt[0]=tt;
        rd[0]=d;
        rs[0]=ss;
        opcode[0]=pp;
        funct[0]=func;
        instruction[0]=isntr;

        //execute stage signals stored
        aluOp[0]=aluop;
        aluSrc[0]=alusrc;
        regDest[0]=regdest;

        //memory signals stored
        memWrite[0]=memwrite;
        memRead[0]=memread;
        Branch[0]=branch;

        //write back signals stored
        memtoReg[0]=memtoreg;
        regWrite[0]=regwrite;

    }

    void setRsData(int value)                { rsData[0] = value; }
    void setRtData(int value)                { rtData[0] = value; }
    void setCc(int value)                    { cc[0] = value; }
    void setOffset(int value)                { offset[0] = value; }
    void setRt(const string& value)          { rt[0] = value; }
    void setRd(const string& value)          { rd[0] = value; }
    void setRs(const string& value)          { rs[0] = value; }
    void setOpcode(int value)                { opcode[0] = value; }
    void setFunct(int value)                 { funct[0] = value; }
    void setInstruction(const string& value) { instruction[0] = value; }



    int getRsData()         { return rsData[1]; }
    int getRtData()         { return rtData[1]; }
    int getPC()             { return cc[1]; }
    string getRt()          { return rt[1]; }
    string getRd()          { return rd[1]; }
    string getRs()          { return rs[1]; }
    int getOpcode()         { return opcode[1];}
    int getOffset()         { return offset[1]; }
    int getFunct()          { return funct[1]; }
    string getInstruction() { return instruction[1]; }


    char getAluOp()  { return aluOp[1]; }
    char getAluSrc()  { return aluSrc[1]; }
    char getRegDest()  { return regDest[1]; }

    char getMemWrite()  { return memWrite[1]; }
    char getMemRead()  { return memRead[1]; }
    char getBranch()  { return Branch[1]; }

    char getMemToReg()  { return memtoReg[1]; }
    char getRegWrite()  { return regWrite[1]; }

    int getsRsData()  { return rsData[0]; }
    int getsRtData()  { return rtData[0]; }
    int getsPC()  { return cc[0]; }
    string getsRt()  { return rt[0]; }
    string getsRd()  { return rd[0]; }
    string getsRs()  { return rs[0]; }
    int getsOpcode()  { return opcode[0];}
    int getsOffset()  { return offset[0]; }
    int getsFunct()  { return funct[0]; }
    string getsInstruction()  { return instruction[0]; }

    char getsAluOp()  { return aluOp[0]; }
    char getsAluSrc() { return aluSrc[0]; }
    char getsRegDest() { return regDest[0]; }

    char getsMemWrite() { return memWrite[0]; }
    char getsMemRead() { return memRead[0]; }
    char getsBranch() { return Branch[0]; }

    char getsMemToReg() { return memtoReg[0]; }
    char getsRegWrite() { return regWrite[0]; }



    void printSignals() const {
        std::cout << "Values:" << std::endl;
        std::cout << "rsData: " << rsData[1] << std::endl;
        std::cout << "rtData: " << rtData[1] << std::endl;
        std::cout << "pc: " << cc[1] << std::endl;
        std::cout << "offset: " << offset[1] << std::endl;
        std::cout << "rt:  " << rt[1] << std::endl;
        std::cout << "rd:  "  << rd[1] << std::endl;
        std::cout << "rs:  " << rs[1] << std::endl;
        std::cout << "opcode: " << opcode[1] << std::endl;

        std::cout << "\nExecute Stage Signals:" << std::endl;
        std::cout << "aluOp: " << static_cast<int>(aluOp[0]) << " " << static_cast<int>(aluOp[1]) << std::endl;
        std::cout << "aluSrc: " << static_cast<int>(aluSrc[0]) << " " << static_cast<int>(aluSrc[1]) << std::endl;
        std::cout << "regDest: " << static_cast<int>(regDest[0]) << " " << static_cast<int>(regDest[1]) << std::endl;

        std::cout << "\nMemory Stage Signals:" << std::endl;
        std::cout << "memWrite: " << static_cast<int>(memWrite[0]) << " " << static_cast<int>(memWrite[1]) << std::endl;
        std::cout << "memRead: " << static_cast<int>(memRead[0]) << " " << static_cast<int>(memRead[1]) << std::endl;
        std::cout << "Branch: " << static_cast<int>(Branch[0]) << " " << static_cast<int>(Branch[1]) << std::endl;

        std::cout << "\nWrite-Back Stage Signals:" << std::endl;
        std::cout << "memtoReg: " << static_cast<int>(memtoReg[0]) << " " << static_cast<int>(memtoReg[1]) << std::endl;
        std::cout << "regWrite: " << static_cast<int>(regWrite[0]) << " " << static_cast<int>(regWrite[1]) << std::endl;
    }
};




















class Memory{

    //Data Memory
    unordered_map<int, int> memoryMap;
    vector<pair<int, int>> valuesOrder; // stores (address, value)
    int index=0;

    //Instruction Memory
    vector <string> memory;
    vector <string> origMemory;
    int count=0;





public:

    //Data Memory
    int getMem(int address){ return memoryMap[address]; }

    void setMem(int value, int address) {
        memoryMap[address] = value;

        // Check if the address already exists in valuesOrder
        bool updated = false;
        for (auto& pair : valuesOrder) {
            if (pair.first == address) {
                pair.second = value; // update existing value
                updated = true;
                break;
            }
        }

        // If not found, add a new entry
        if (!updated) {
            valuesOrder.push_back({address, value});
        }

        index++; // if index is still used elsewhere
    }

    void getAllMemory(){
        for (size_t i = 0; i < valuesOrder.size(); i++) {
             cout<<hex<<valuesOrder[i].second<<endl;
            }
        }


    void writeToFile(ofstream& outputFile) {
        if (!outputFile) {
            cout << "Error opening file!" << endl;
            return;
        }

        for (size_t i = 0; i < valuesOrder.size(); ++i) {
            if (i != 0) outputFile << "\t";
                outputFile << hex << uppercase << valuesOrder[i].second;
        }
    }

    int getAddressByValue(int value) {
    for (const auto& pair : valuesOrder) {
        if (pair.second == value) {
            return pair.first; // return the address
        }
    }
    return -1; // not found
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
        return "nullptr";
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
        ifstream file("pipeline_test2025.s");
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
                else if(ch==45 || ch==43 || ch==40 || ch==41 || ch=='_' || ch==' ') { instruction+=ch; }
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
        ifstream fileinstr("pipeline_test2025.s");
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
                else if(ch=='x' && fullinstruction[i-1]=='0' && ((fullinstruction[i+1]>=48 && fullinstruction[i+1]<58) || (fullinstruction[i+1]>=65 && fullinstruction[i+1]<=70) || (fullinstruction[i+1]>=97 && fullinstruction[i+1]<=102))){ hex=1; }

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

        if((aluOp==2 && (funct==32 || funct==33 )) || (aluOp==0 && funct==-7)) { operation=2; } //add
        else if((aluOp==2 && funct==36) || (aluOp==0 && funct==-1)) { operation=0; } //and
        else if((aluOp==2 && funct==37) || (aluOp==0 && funct==-2)) { operation=1; } //or
        else if((aluOp==1 && funct!=-3 && funct!=-4) || (aluOp==2 && (funct==34 || funct==35))) { operation=6; } //sub
        else if(aluOp==2 && funct==0) { operation=3; } //sll
        else if(aluOp==2 && funct==2) { operation=4; } //slr
        else if(aluOp==2 && funct==39) { operation=12; } //nor
        else if((aluOp==2 && (funct==42 || funct==43)) || (aluOp==1 && (funct==-3 || funct==-4))) { operation=7; } //slt
        else if(aluOp==0 && funct==-100) { operation=8; }//lui

    }

    int getOperation() { return operation; }



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


















class RegisterFile{

    int registers[32],pc;
    string rs,rt,rd,instruction;
    int offset;
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
       if(reg!="-"){
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
       return 0;
    }

    // Setter methods
    void setRs(string value) { rs = value; }
    void setRt(string value) { rt = value; }
    void setRd(string value) { rd = value; }
    void setOffset(int value) { offset=value; }
    void setInstruction(string value) { instruction = value; }





    // Getter methods
    string getRs() { return rs; }
    string getRt() { return rt; }
    string getRd() { return rd; }
    int getOffset() {return offset; }
    string getInstruction() { return instruction; }


};



















class HazardUnit{

//52
    //output
    int result=0;

public:

    void activateHazard(char memread,string rt,string rs, string writeregister,string instr,int op,string writeregister2){
        result=0;
        //cout<<"  rs"<<rs<<" rt  "<<rt<<" wr   "<<writeregister<<" wr2  "<<writeregister2<<instr<<endl;
        if(instr!="addi$zero,$zero,0"){
            if((memread==1) && ((rt==writeregister && rt!="zero") || (rs==writeregister && rs!="zero"))){
                result=1;
            }
        }
        if (op==4 || op==5) {
            if (((rs==writeregister || rs==writeregister2) && (rs!="zero")) || ((rt==writeregister || rt==writeregister2) && (rt!="zero"))) {
                result=1;
            }
        }

    }

    int getResult() { return result; }

};























class ForwardingUnit {

    //outputs
    int muxControl1=0;
    int muxControl2=0;


public:

    void activateForwarding(char regWrite1,char regWrite2,string writeRegister1,string writeRegister2,string rs,string rt){
         muxControl2=0;
         muxControl1=0;

        //mux signals for mem/wb register
        if(regWrite2==1 && writeRegister2!="zero" && writeRegister2==rs){ muxControl1=1; }

        if(regWrite2==1 && writeRegister2!="zero" && writeRegister2==rt){ muxControl2=1; }

        //mux signals  for ex/mem register
        if(regWrite1==1 && writeRegister1!="zero" && writeRegister1==rs){ muxControl1=2; }

        if(regWrite1==1 && writeRegister1!="zero" && writeRegister1==rt){ muxControl2=2; }
    }

    int getControl1(){ return muxControl1; }
    int getControl2(){ return muxControl2; }

};



























class ControlUnit {

    //outputs
    char aluSrc,regDest,Aluop,memRead,memWrite,memToReg,regWrite,branch;


public:

    void updateSignals(string type){//tha piani to code[2]
        regDest='-';
        aluSrc='-';
        memToReg='-';
        regWrite='-';
        memRead='-';
        memWrite='-';
        Aluop='-';
        branch='-';

        if(type == "lw"){
            Aluop=0;
            aluSrc=1;
            regDest=0;
            memRead=1;
            memWrite=0;
            regWrite=1;
            memToReg=1;
            branch=0;

        }
        else if (type == "sw") {
            Aluop=0;
            aluSrc=1;
            regDest=0;
            memRead=0;
            memWrite=1;
            regWrite=0;
            memToReg='-';
            branch=0;
        }
        else if (type == "addiu" || type == "addi") {
            Aluop=0;
            aluSrc=1;
            regDest=0;
            memRead=0;
            memWrite=0;
            regWrite=1;
            memToReg=0;
            branch=0;

        }
        else if (type == "addu" || type == "add" || type == "nor" || type == "and" ||
                 type == "slt" ||type == "sltu" || type == "or" || type == "srl" ||
                 type == "sub" || type == "subu" || type == "sll") {
            Aluop=2;
            if(type=="sll" || type=="srl"){ aluSrc=1; }
            else { aluSrc=0; }
            regDest=1;
            memRead=0;
            memWrite=0;
            regWrite=1;
            memToReg=0;
            branch=0;

        }
        else if (type == "slti" || type == "sltiu"){
            Aluop=1;
            aluSrc=1;
            regDest=0;
            memRead=0;
            memWrite=0;
            regWrite=1;
            memToReg=0;
            branch=0;

        }
        else if(type == "beq" || type == "bne"){
            Aluop=1;
            aluSrc=0;
            regDest='-';
            memRead=0;
            memWrite=0;
            regWrite=0;
            memToReg='-';
            branch=1;
        }
        else if(type == "andi" || type == "ori"){
            Aluop=0;
            aluSrc=1;
            regDest=0;
            memRead=0;
            memWrite=0;
            regWrite=1;
            memToReg=0;
            branch=0;
        }
    }


    char getAluSrc() { return aluSrc; }
    char getRegDest() { return regDest; }
    char getAluOp() { return Aluop; }
    char getMemRead() { return memRead; }
    char getMemWrite() { return memWrite; }
    char getMemToReg() { return memToReg; }
    char getRegWrite() { return regWrite; }
    char getBranch() { return branch; }


void printSignals() {
    std::cout << "ALU Src: " << static_cast<int>(aluSrc) << std::endl;
    std::cout << "Reg Dest: " << static_cast<int>(regDest) << std::endl;
    std::cout << "ALU Op: " << static_cast<int>(Aluop) << std::endl;
    std::cout << "Mem Read: " << static_cast<int>(memRead) << std::endl;
    std::cout << "Mem Write: " << static_cast<int>(memWrite) << std::endl;
    std::cout << "Mem To Reg: " << static_cast<int>(memToReg) << std::endl;
    std::cout << "Reg Write: " << static_cast<int>(regWrite) << std::endl;
    std::cout << "Branch: " << static_cast<int>(branch) << std::endl;
}

};







void begOutput(){
    ofstream outputFile("Marios_UC1067054.txt");
    if (outputFile.is_open()) {
            outputFile << "Name: Marios Bou-Mansour\nUniversity ID: UC1067054\n\n";
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


void midoutput(InstructionRegister ir,DataRegister dr, ExecuteRegister er, MemoryRegister mr,Memory mem,int kikloi,RegisterFile registerFile,int pc,string instr,bool hd, bool fd, bool branch){
    ofstream outputFile("Marios_UC1067054.txt", ios::app);
        if (outputFile.is_open()) {
            outputFile << "-----Cycle "<<kikloi<<"-----\n"<<"Registers:\n";
            if(dr.getsInstruction()=="addi$zero,$zero,0"){
                outputFile<<hex<<uppercase<<(pc-1)*4;
            }
            else {
                outputFile<<hex<<uppercase<<(pc)*4;
            }
            for(int i=0;i<32;i++) { outputFile<<"\t"<<hex<<uppercase<<registerFile.getReadData(i); }

            //monitors 1-3
            outputFile<<hex<<uppercase<<"\n\nMonitors:\n";
            if(dr.getsInstruction()=="addi$zero,$zero,0"){
                outputFile<<hex<<uppercase<<ir.getsPC()*4<<"\t"<<(ir.getsPC()-1)*4<<"\t";
                outputFile<<mem.getOrigInstruction((ir.getsPC()-2))<<"\t";
            }
            else{
                if(mem.getOrigInstruction(dr.getsPC()-1).substr(0, 3)=="beq"){
                    if(dr.getsRtData()==dr.getsRsData()){
                        outputFile<<pc<<"\t-\t-\t";
                    }
                    else {
                        outputFile<<(ir.getsPC()+1)*4<<"\t"<<
                        (ir.getsPC())*4<<"\t";
                        outputFile<<mem.getOrigInstruction((ir.getsPC()-1))<<"\t";
                    }
                }
                else if (mem.getOrigInstruction(dr.getsPC()-1).substr(0, 3)=="bne"){
                    if(dr.getsRtData()!=dr.getsRsData()){
                        outputFile<<pc<<"\t-\t-\t";
                    }
                    else {
                        outputFile<<(ir.getsPC()+1)*4<<"\t"<<
                        (ir.getsPC())*4<<"\t";
                        outputFile<<mem.getOrigInstruction((ir.getsPC()-1))<<"\t";
                    }
                }
                else {
                    outputFile<<(ir.getsPC()+1)*4<<"\t"<<
                    (ir.getsPC())*4<<"\t";
                    outputFile<<mem.getOrigInstruction((ir.getsPC()-1))<<"\t";
                }
            }

            //monitor 4
            if((mem.getOrigInstruction(dr.getsPC()-1).substr(0, 3)=="bne")||
               (mem.getOrigInstruction(dr.getsPC()-1).substr(0, 3)=="beq")){
                string label;
                int getLabel=0;
                for(char ch:mem.getOrigInstruction(dr.getsPC()-1)){
                    if(ch==',') getLabel++;
                    else if(ch==' ') continue;
                    else if(getLabel==2) label+=ch;
                }
                outputFile<<label<<"\t";
            }
            else {
                outputFile<<"-\t";
            }


            //monitors 5-10
            if(dr.getsInstruction()=="addi$zero,$zero,0"){
                outputFile<<"-\t-\t";
                if((mem.getOrigInstruction(mr.getPC()-1).substr(0, 3)=="bne")||
                   (mem.getOrigInstruction(mr.getPC()-1).substr(0, 3)=="beq")||
                   (mem.getOrigInstruction(mr.getPC()-1).substr(0, 2)=="sw")){
                    outputFile<<"-\t-\t";
                }
                else if(mem.getOrigInstruction(mr.getPC()-1).substr(0, 2)=="lw"){
                    outputFile<<
                    "$"<<mr.getWriteRegister()<<"\t"<<
                    mr.getReadData()<<"\t";
                }
                else {
                    outputFile<<
                    "$"<<mr.getWriteRegister()<<"\t"<<
                    mr.getAluResult()<<"\t";
                }
                outputFile<<"-\t-\t";
            }
            else if((mem.getOrigInstruction(dr.getsPC()-1).substr(0, 4)=="addi")||
               (mem.getOrigInstruction(dr.getsPC()-1).substr(0, 5)=="addiu")||
               (mem.getOrigInstruction(dr.getsPC()-1).substr(0, 4)=="slti")||
               (mem.getOrigInstruction(dr.getsPC()-1).substr(0, 5)=="sltiu")||
               (mem.getOrigInstruction(dr.getsPC()-1).substr(0, 4)=="andi")||
               (mem.getOrigInstruction(dr.getsPC()-1).substr(0, 3)=="ori")||
               (mem.getOrigInstruction(dr.getsPC()-1).substr(0, 2)=="lw")){
                outputFile<<
                "$"<<dr.getsRs()<<"\t"<<
                "-"<<"\t";
                if((mem.getOrigInstruction(mr.getPC()-1).substr(0, 3)=="bne")||
                   (mem.getOrigInstruction(mr.getPC()-1).substr(0, 3)=="beq")||
                   (mem.getOrigInstruction(mr.getPC()-1).substr(0, 2)=="sw")){
                    outputFile<<"-\t-\t";
                }
                else if(mem.getOrigInstruction(mr.getPC()-1).substr(0, 2)=="lw"){
                    outputFile<<
                    "$"<<mr.getWriteRegister()<<"\t"<<
                    mr.getReadData()<<"\t";
                }
                else {
                    outputFile<<
                    "$"<<mr.getWriteRegister()<<"\t"<<
                    mr.getAluResult()<<"\t";
                }
                outputFile<<
                dr.getsRsData()<<"\t"<<
                "-"<<"\t";
            }
            else if((mem.getOrigInstruction(dr.getsPC()-1).substr(0, 3)=="bne")||
               (mem.getOrigInstruction(dr.getsPC()-1).substr(0, 3)=="beq")){
                outputFile<<
                "$"<<dr.getsRt()<<"\t"<<
                "$"<<dr.getsRs()<<"\t";
                if((mem.getOrigInstruction(mr.getPC()-1).substr(0, 3)=="bne")||
                   (mem.getOrigInstruction(mr.getPC()-1).substr(0, 3)=="beq")||
                   (mem.getOrigInstruction(mr.getPC()-1).substr(0, 2)=="sw")){
                    outputFile<<"-\t-\t";
                }
                else if((mem.getOrigInstruction(mr.getPC()-1).substr(0, 2)=="lw")){
                    outputFile<<
                    "$"<<mr.getWriteRegister()<<"\t"<<
                    mr.getReadData()<<"\t";
                }
                else {
                    outputFile<<
                    "$"<<mr.getWriteRegister()<<"\t"<<
                    mr.getAluResult()<<"\t";
                }
                outputFile<<
                dr.getsRsData()<<"\t"<<
                dr.getsRtData()<<"\t";
            }
            else if(mem.getOrigInstruction(dr.getsPC()-1).substr(0, 2)=="sw"){
                outputFile<<
                "$"<<dr.getsRs()<<"\t"<<
                "$"<<dr.getsRt()<<"\t";
                if((mem.getOrigInstruction(mr.getPC()-1).substr(0, 3)=="bne")||
                   (mem.getOrigInstruction(mr.getPC()-1).substr(0, 3)=="beq")||
                   (mem.getOrigInstruction(mr.getPC()-1).substr(0, 2)=="sw")){
                    outputFile<<"-\t-\t";
                }
                else if((mem.getOrigInstruction(mr.getPC()-1).substr(0, 2)=="lw")){
                    outputFile<<
                    "$"<<mr.getWriteRegister()<<"\t"<<
                    mr.getReadData()<<"\t";
                }
                else {
                    outputFile<<
                    "$"<<mr.getWriteRegister()<<"\t"<<
                    mr.getAluResult()<<"\t";
                }
                outputFile<<
                dr.getsRsData()<<"\t"<<
                dr.getsRtData()<<"\t";
            }
            else if(dr.getsOpcode()==0 && (dr.getsOffset()==2 || dr.getsOffset()==0)){
                outputFile << "0" << "\t" << dr.getsRs() << "\t";
                if((mem.getOrigInstruction(mr.getPC()-1).substr(0, 3)=="bne")||
                   (mem.getOrigInstruction(mr.getPC()-1).substr(0, 3)=="beq")||
                   (mem.getOrigInstruction(mr.getPC()-1).substr(0, 2)=="sw")){
                    outputFile<<"-\t-\t";
                }
                else if((mem.getOrigInstruction(mr.getPC()-1).substr(0, 2)=="lw")){
                    outputFile<<
                    "$"<<mr.getWriteRegister()<<"\t"<<
                    mr.getReadData()<<"\t";
                }
                else {
                    outputFile<<
                    "$"<<mr.getWriteRegister()<<"\t"<<
                    mr.getAluResult()<<"\t";
                }
                outputFile<<
                "-"<<"\t"<<
                dr.getsRsData()<<"\t";
            }
            else{
                outputFile<<
                "$"<<dr.getsRs()<<"\t"<<
                "$"<<dr.getsRt()<<"\t";
                if((mem.getOrigInstruction(mr.getPC()-1).substr(0, 3)=="bne")||
                   (mem.getOrigInstruction(mr.getPC()-1).substr(0, 3)=="beq")||
                   (mem.getOrigInstruction(mr.getPC()-1).substr(0, 2)=="sw")){
                    outputFile<<"-\t-\t";
                }
                else if((mem.getOrigInstruction(mr.getPC()-1).substr(0, 2)=="lw")){
                    outputFile<<
                    "$"<<mr.getWriteRegister()<<"\t"<<
                    mr.getReadData()<<"\t";
                }
                else {
                    outputFile<<
                    "$"<<mr.getWriteRegister()<<"\t"<<
                    mr.getAluResult()<<"\t";
                }
                outputFile<<
                dr.getsRsData()<<"\t"<<
                dr.getsRtData()<<"\t";
            }

            //monitors 11
            if(dr.getsInstruction()=="addi$zero,$zero,0"){
                outputFile<<"-\t";
            }
            else if((mem.getOrigInstruction(dr.getsPC()-1).substr(0, 3)=="bne")||
               (mem.getOrigInstruction(dr.getsPC()-1).substr(0, 3)=="beq")){
                outputFile<<"-\t";
            }
            else if((mem.getOrigInstruction(dr.getsPC()-1).substr(0, 4)=="addi")||
                    (mem.getOrigInstruction(dr.getsPC()-1).substr(0, 5)=="addiu")||
                    (mem.getOrigInstruction(dr.getsPC()-1).substr(0, 4)=="slti")||
                    (mem.getOrigInstruction(dr.getsPC()-1).substr(0, 5)=="sltiu")||
                    (mem.getOrigInstruction(dr.getsPC()-1).substr(0, 4)=="andi")||
                    (mem.getOrigInstruction(dr.getsPC()-1).substr(0, 3)=="ori")||
                    (mem.getOrigInstruction(dr.getsPC()-1).substr(0, 2)=="lw")||
                    (mem.getOrigInstruction(dr.getsPC()-1).substr(0, 2)=="sw")){
                    outputFile<<dr.getsOffset()<<"\t";
            }
            else{
                outputFile<<"-"<<"\t";
            }

            //monitor 12-15
            if(dr.getsInstruction()=="addi$zero,$zero,0"){
                outputFile<<"-\t-\t-\t-\t";
            }
            else if((mem.getOrigInstruction(dr.getsPC()-1).substr(0, 4)=="addi")||
               (mem.getOrigInstruction(dr.getsPC()-1).substr(0, 5)=="addiu")||
               (mem.getOrigInstruction(dr.getsPC()-1).substr(0, 4)=="slti")||
               (mem.getOrigInstruction(dr.getsPC()-1).substr(0, 5)=="sltiu")||
               (mem.getOrigInstruction(dr.getsPC()-1).substr(0, 4)=="andi")||
               (mem.getOrigInstruction(dr.getsPC()-1).substr(0, 3)=="ori")||
               (mem.getOrigInstruction(dr.getsPC()-1).substr(0, 2)=="lw")){
                outputFile<<
                "$"<<dr.getsRs()<<"\t"<<
                "-"<<"\t"<<
                "$"<<dr.getsRt()<<"\t"<<
                "-\t";
            }
            else if((mem.getOrigInstruction(dr.getsPC()-1).substr(0, 3)=="bne")||
               (mem.getOrigInstruction(dr.getsPC()-1).substr(0, 3)=="beq")){
                outputFile<<
                "$"<<dr.getsRt()<<"\t"<<
                "$"<<dr.getsRs()<<"\t"<<
                "-"<<"\t"<<
                "-\t";
            }
            else if(mem.getOrigInstruction(dr.getsPC()-1).substr(0, 2)=="sw"){
                outputFile<<
                "$"<<dr.getsRs()<<"\t"<<
                "$"<<dr.getsRt()<<"\t"<<
                "-"<<"\t"<<
                "-\t";
            }
            else if(dr.getsOpcode()==0 && (dr.getsOffset()==2 || dr.getsOffset()==0)){
                outputFile<<
                "0\t"<<
                "$"<<dr.getsRt()<<"\t"<<
                "-"<<"\t"<<
                "$"<<dr.getsRd()<<"\t";
            }
            else{
                outputFile<<
                "$"<<dr.getsRs()<<"\t"<<
                "$"<<dr.getsRt()<<"\t"<<
                "-"<<"\t"<<
                "$"<<dr.getsRd()<<"\t";
            }

            //monitors 16-19
            if(dr.getInstruction()=="addi$zero,$zero,0"){
                outputFile<<"-\t-\t-\t-\t";
            }
            else if((mem.getOrigInstruction(dr.getPC()-1).substr(0, 4)=="addi")||
               (mem.getOrigInstruction(dr.getPC()-1).substr(0, 5)=="addiu")||
               (mem.getOrigInstruction(dr.getPC()-1).substr(0, 4)=="slti")||
               (mem.getOrigInstruction(dr.getPC()-1).substr(0, 5)=="sltiu")||
               (mem.getOrigInstruction(dr.getPC()-1).substr(0, 4)=="andi")||
               (mem.getOrigInstruction(dr.getPC()-1).substr(0, 3)=="ori")){
                outputFile<<
                dr.getRsData()<<"\t"<<
                dr.getOffset()<<"\t"<<
                er.getsAluResult()<<"\t"<<
                "-\t";
            }
            else if(mem.getOrigInstruction(dr.getPC()-1).substr(0, 2)=="lw"){
                outputFile<<
                dr.getRsData()<<"\t"<<
                dr.getOffset()<<"\t"<<
                er.getsAluResult()<<"\t"<<
                "-"<<"\t";
            }
            else if(mem.getOrigInstruction(dr.getPC()-1).substr(0, 2)=="sw"){
                outputFile<<
                dr.getRsData()<<"\t"<<
                dr.getOffset()<<"\t"<<
                er.getsAluResult()<<"\t"<<
                er.getsRtData()<<"\t";
            }
            else if((mem.getOrigInstruction(dr.getPC()-1).substr(0, 3)=="bne")||
               (mem.getOrigInstruction(dr.getPC()-1).substr(0, 3)=="beq")){
                outputFile<<
                "-"<<"\t"<<
                "-"<<"\t"<<
                "-"<<"\t"<<
                "-"<<"\t";
            }
            else if(dr.getOpcode()==0 && (dr.getOffset()==2 || dr.getOffset()==0)){
                outputFile<<
                "0\t"<<
                dr.getRsData()<<"\t"<<
                er.getsAluResult()<<"\t"<<
                "-"<<"\t";
            }
            else{
                outputFile<<
                dr.getRsData()<<"\t"<<
                dr.getRtData()<<"\t"<<
                er.getsAluResult()<<"\t"<<
                "-"<<"\t";
            }


            //monitors 20-24
            if(er.getInstruction()=="addi$zero,$zero,0"){
                outputFile<<"-\t-\t-\t-\t-\t";
            }
            else if((mem.getOrigInstruction(er.getPC()-1).substr(0, 4)=="addi")||
               (mem.getOrigInstruction(er.getPC()-1).substr(0, 5)=="addiu")||
               (mem.getOrigInstruction(er.getPC()-1).substr(0, 4)=="slti")||
               (mem.getOrigInstruction(er.getPC()-1).substr(0, 5)=="sltiu")||
               (mem.getOrigInstruction(er.getPC()-1).substr(0, 4)=="andi")||
               (mem.getOrigInstruction(er.getPC()-1).substr(0, 3)=="ori")){
                outputFile<<
                "-\t"<<"-\t"<<"-\t"<<
                er.getAluResult()<<"\t"<<
                "$"<<er.getWriteRegister()<<"\t";

            }
            else if(mem.getOrigInstruction(er.getPC()-1).substr(0, 2)=="lw"){
                outputFile<<
                er.getAluResult()<<"\t"<<
                "-"<<"\t"<<
                mr.getsReadData()<<"\t"<<
                "$"<<er.getWriteRegister()<<"\t";
            }
            else if(mem.getOrigInstruction(er.getPC()-1).substr(0, 2)=="sw"){
                outputFile<<
                er.getAluResult()<<"\t"<<
                er.getRtData()<<"\t"<<
                "-"<<"\t"<<"-\t"<<
                "-\t";
            }
            else if((mem.getOrigInstruction(er.getPC()-1).substr(0, 3)=="bne")||
               (mem.getOrigInstruction(er.getPC()-1).substr(0, 3)=="beq")){
                outputFile<<
                "-"<<"\t"<<
                "-"<<"\t"<<
                "-"<<"\t"<<"-\t"<<
                "-"<<"\t";
            }
            else{
                outputFile<<
                "-"<<"\t"<<
                "-"<<"\t"<<
                "-"<<"\t"<<
                er.getAluResult()<<"\t"<<
                "$"<<er.getWriteRegister()<<"\t";
            }

            //monitors 25 26
            if(mr.getInstruction()=="addi$zero,$zero,0"){
                outputFile<<"-\t-\t";
            }
            else if((mem.getOrigInstruction(mr.getPC()-1).substr(0, 4)=="addi")||
               (mem.getOrigInstruction(mr.getPC()-1).substr(0, 5)=="addiu")||
               (mem.getOrigInstruction(mr.getPC()-1).substr(0, 4)=="slti")||
               (mem.getOrigInstruction(mr.getPC()-1).substr(0, 5)=="sltiu")||
               (mem.getOrigInstruction(mr.getPC()-1).substr(0, 4)=="andi")||
               (mem.getOrigInstruction(mr.getPC()-1).substr(0, 3)=="ori")){
                outputFile<<
                "$"<<mr.getWriteRegister()<<"\t"<<
                mr.getAluResult()<<"\t";


            }
            else if(mem.getOrigInstruction(mr.getPC()-1).substr(0, 2)=="lw"){
                outputFile<<
                "$"<<mr.getWriteRegister()<<"\t"<<
                mr.getReadData()<<"\t";
            }
            else if(mem.getOrigInstruction(mr.getPC()-1).substr(0, 2)=="sw"){
                outputFile<<
                "-"<<"\t"<<
                "-"<<"\t";
            }
            else if((mem.getOrigInstruction(mr.getPC()-1).substr(0, 3)=="bne")||
               (mem.getOrigInstruction(mr.getPC()-1).substr(0, 3)=="beq")){
                outputFile<<
                "-"<<"\t"<<
                "-"<<"\t";
            }
            else{
                outputFile<<
                "$"<<mr.getWriteRegister()<<"\t"<<
                mr.getAluResult()<<"\t";

            }
            outputFile<<hd<<"\t"<<fd;

            outputFile<<"\n\nMemory State:\n";
            mem.writeToFile(outputFile);
            outputFile<<"\n";
            outputFile<<"\nPipeline Stages:\n";
            if(ir.getsPC()==1){
                outputFile<<
                mem.getOrigInstruction(ir.getsPC()-1)<<"\t"<<
                "-"<<"\t"<<
                "-"<<"\t"<<
                "-"<<"\t"<<
                "-"<<"\n\n";
            }
            else if(ir.getsPC()==2){
                outputFile<<
                mem.getOrigInstruction(ir.getsPC()-1)<<"\t"<<
                mem.getOrigInstruction(dr.getsPC()-1)<<"\t"<<
                "-"<<"\t"<<
                "-"<<"\t"<<
                "-"<<"\n\n";
            }
            else if(ir.getsPC()==3){
                outputFile<<
                mem.getOrigInstruction(ir.getsPC()-1)<<"\t"<<
                mem.getOrigInstruction(dr.getsPC()-1)<<"\t"<<
                mem.getOrigInstruction(dr.getPC()-1)<<"\t"<<
                "-"<<"\t"<<
                "-"<<"\n\n";
            }
            else if(ir.getsPC()==4){
                outputFile<<
                mem.getOrigInstruction(ir.getsPC()-1)<<"\t"<<
                mem.getOrigInstruction(ir.getsPC()-1)<<"\t"<<
                mem.getOrigInstruction(dr.getPC()-1)<<"\t"<<
                mem.getOrigInstruction(er.getPC()-1)<<"\t"<<
                "-"<<"\n\n";
            }
            else {
                if(dr.getsInstruction()=="addi$zero,$zero,0" && (er.getsOpcode()==4 || er.getsOpcode()==5)){ outputFile<<mem.getOrigInstruction(ir.getsPC()-1)<<"\t"; }
                else if(dr.getsInstruction()=="addi$zero,$zero,0"){ outputFile<<mem.getOrigInstruction(ir.getsPC()-2)<<"\t"; }
                else if(ir.getsInstruction()!="addi$zero,$zero,0"){ outputFile<<mem.getOrigInstruction(ir.getsPC()-1)<<"\t"; }
                else{ outputFile<<"bubble"<<"\t"; }
                if(dr.getsInstruction()!="addi$zero,$zero,0"){outputFile<<mem.getOrigInstruction(dr.getsPC()-1)<<"\t";}
                else{ outputFile<<"bubble"<<"\t"; }
                if(dr.getInstruction()!="addi$zero,$zero,0"){ outputFile<<mem.getOrigInstruction(dr.getPC()-1)<<"\t"; }
                else{ outputFile<<"bubble"<<"\t"; }
                if(er.getInstruction()!="addi$zero,$zero,0"){outputFile<<mem.getOrigInstruction(er.getPC()-1)<<"\t";}
                else{ outputFile<<"bubble"<<"\t"; }
                if(mr.getInstruction()!="addi$zero,$zero,0"){outputFile<<mem.getOrigInstruction(mr.getPC()-1);}
                else{ outputFile<<"bubble"<<"\t"; }
                outputFile<<"\n\n";
            }
            outputFile.close();
        }
}

















int main(){

    HazardUnit hu;
    Memory memory;
    ControlUnit controlUnit;
    AluControl aluControl;
    Alu alu;
    RegisterFile registerFile;
    Timer timer;
    InstructionRegister ir;
    DataRegister dr;
    MemoryRegister mr;
    ExecuteRegister er;
    ForwardingUnit fu;


    begOutput();


    //read file and add them in memory
    memory.updateInstructions();

    bool forward_taken=false;
    bool branch_taken=false;
    bool hazard_taken=false;
    bool branch[2]={false,false};
    string rs_temp,rt_temp,rd_temp,instruction;
    int pc=0,opcode=0,offset,funct,index=0,kikloi=0,cycle,negative=1,rs,rt,rd,input_alu0,input_alu1,b=0,count=0,memResult;
    string instroctiontype;
    cout<<"Please enter the number where you want to stop"<<endl;
    cin>>cycle;

    timer.start();

    while(true){
        kikloi++;
        //set the instruction register
        if (hu.getResult() == 0 && hazard_taken!=true) {
            if (branch_taken) {
                instruction=memory.getInstruction(pc);
                if (instruction != "sll$zero,$zero,0") { pc++; }
                ir.setRegister(instruction,pc);
                instruction = "addi$zero,$zero,0";  // Inject NOP
                branch_taken = false;               // Reset flag
            }
            else {
                instruction = memory.getInstruction(pc);
                if (instruction != "sll$zero,$zero,0") { pc++; }
                ir.setRegister(instruction, pc);
                instruction=ir.getInstruction();
            }

        }
        hazard_taken=false;
        //else { st=ir.getInstruction(); ir.setRegister("addi$zero,$zero,0",pc); ir.setRegister(instruction, pc); instruction=ir.getInstruction(); }
        //cout<<instruction<<endl;
        //cout<<instruction<<endl;
        //cout<<hu.getResult()<<endl;
        //if(kikloi==4533) {cout<<instruction<<endl;}


        if (instruction.substr(0, 5)=="addiu"){
            opcode=9;
            instroctiontype="addiu";
            offset=0;
            funct=-7;
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
            funct=-7;
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
        else if (instruction.substr(0, 2)=="sw"){
            opcode=43;
            offset=0;
            funct=-7;
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
            funct=-7;
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

        rs_temp="";
        rt_temp="";
        rd_temp="";


        //get registers from instructions
        if(instruction!="sll$zero,$zero,0"){
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
                else if(funct==0 || funct==2) { rt_temp="-"; }
            }
            else if(opcode==8 || opcode==9 || opcode==12 || opcode==13 || opcode==4 || opcode==5 || opcode==10 || opcode==15 || opcode==11 ){
                for (char ch:instruction){
                    if(ch==','){ count++; }
                    else if (ch=='$'){ count++; }
                    else if(count%2==1){
                            if(count==1) { rt_temp+=ch;  }
                            else if(count==3) { rs_temp+=ch; }
                            else if(count==5) { rd_temp+=ch; }
                    }
                }
                rd_temp="-";
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
        else if(instruction=="sll$zero,$zero,0"){ rs_temp="zero"; rt_temp="-"; rd_temp="zero"; }

        //instruction decode stage
        if(kikloi>1 && b<=2){
            if (kikloi>5 && dr.getsOpcode()!=0) {
                hu.activateHazard(dr.getsMemRead(),rt_temp,rs_temp,dr.getsRt(),instruction,opcode,er.getsWriteRegister());
            }
            else if(kikloi>5 && dr.getsOpcode()==0){
                hu.activateHazard(dr.getsMemRead(),rt_temp,rs_temp,dr.getsRd(),instruction,opcode,er.getsWriteRegister());
            }
            else {
                hu.activateHazard(dr.getsMemRead(),rt_temp,rs_temp,dr.getsRt(),instruction,opcode,"wow");
            }

            if(hu.getResult()==0){
                controlUnit.updateSignals(instroctiontype);

                //last stage
                if(kikloi>4 && b<=4){
                    if(mr.getsRegWrite()==1){
                        if(mr.getsMemToReg()==1) { registerFile.writeInRegister(registerFile.getRegisterMatch(mr.getsWriteRegister()),mr.getsReadData()); }
                        else if(mr.getsMemToReg()==0) { registerFile.writeInRegister(registerFile.getRegisterMatch(mr.getsWriteRegister()),mr.getsAluResult()); }
                    }
                    //if(kikloi==12) {mr.printRegister(); }
                }
                if (opcode==5 && (registerFile.getReadData(registerFile.getRegisterMatch(rs_temp))-registerFile.getReadData(registerFile.getRegisterMatch(rt_temp)))!=0) {
                    pc=offset;
                    ir.flashInstruction();
                    branch_taken=true;
                }
                else if (opcode==4 && (registerFile.getReadData(registerFile.getRegisterMatch(rs_temp))-registerFile.getReadData(registerFile.getRegisterMatch(rt_temp)))==0) {
                    pc=offset;
                    ir.flashInstruction();
                    branch_taken=true;
                }

                //if(instroctiontype=="sll") { cout<<"rs"<<rs_temp<<"    rt"<<rt_temp<<"    rd"<<rd_temp<<endl;}
                //cout<<instruction<<endl;
                dr.setRegister(rt_temp,rd_temp,registerFile.getReadData(registerFile.getRegisterMatch(rs_temp)), registerFile.getReadData(registerFile.getRegisterMatch(rt_temp)),ir.getPC(),
                               controlUnit.getAluOp(),controlUnit.getAluSrc(),controlUnit.getRegDest(),controlUnit.getMemWrite(),controlUnit.getMemRead(),controlUnit.getBranch(),
                               controlUnit.getMemToReg(),controlUnit.getRegWrite(),rs_temp,opcode,offset,funct,instruction);

                //if(kikloi==9) {dr.printSignals();cout<<dr.getInstruction()<<"instr"<<endl; }
            }
            else if(hu.getResult()==1){
                hazard_taken=true;
                controlUnit.updateSignals(instroctiontype);

                //last stage
                if(kikloi>4 && b<=4){
                    if(mr.getsRegWrite()==1){
                        if(mr.getsMemToReg()==1) { registerFile.writeInRegister(registerFile.getRegisterMatch(mr.getsWriteRegister()),mr.getsReadData()); }
                        else if(mr.getsMemToReg()==0) { registerFile.writeInRegister(registerFile.getRegisterMatch(mr.getsWriteRegister()),mr.getsAluResult()); }
                    }
                    //if(kikloi==12) {mr.printRegister(); }
                }
                //if(instroctiontype=="sll") { cout<<"rs"<<rs_temp<<"    rt"<<rt_temp<<"    rd"<<rd_temp<<endl;}
                dr.setRegister("zero","zero",0, 0,ir.getPC(),
                               0,0,0,0,0,0,0,0,"zero",opcode,offset,funct,"addi$zero,$zero,0");
                //if(kikloi==9) {dr.printSignals();cout<<dr.getInstruction()<<"instr"<<endl; }
            }
        }

        //alu stage
        if(kikloi>2 && b<=3){
            aluControl.activateOperation(dr.getAluOp(),dr.getFunct());

            fu.activateForwarding(er.getsRegWrite(),mr.getsRegWrite(),er.getsWriteRegister(),mr.getsWriteRegister(),dr.getRs(),dr.getRt());
            //cout<<"control1   "<<fu.getControl1()<<"    control2    "<<fu.getControl2()<<dr.getInstruction()<<endl;
            if(fu.getControl1()==0 && fu.getControl2()==0){ //replace nothing
                //rt in alu
                if(dr.getAluSrc()==0){
                    alu.makeOperation(registerFile.getReadData(registerFile.getRegisterMatch(dr.getRs())),registerFile.getReadData(registerFile.getRegisterMatch(dr.getRt())),aluControl.getOperation());
                }
                //offset in alu
                else if(dr.getAluSrc()==1){
                    alu.makeOperation(registerFile.getReadData(registerFile.getRegisterMatch(dr.getRs())),dr.getOffset(),aluControl.getOperation());
                }
            }

            else if(fu.getControl1()==2 && fu.getControl2()==0){ //replace rs
                //rt in alu
                if(dr.getAluSrc()==0){
                    alu.makeOperation(er.getsAluResult(),registerFile.getReadData(registerFile.getRegisterMatch(dr.getRt())),aluControl.getOperation());
                }
                //offset in alu
                else if(dr.getAluSrc()==1){
                    alu.makeOperation(er.getsAluResult(),dr.getOffset(),aluControl.getOperation());
                }
            }

            else if(fu.getControl1()==0 && fu.getControl2()==2){ //replace rt
                //rt in alu
                if(dr.getAluSrc()==0){
                    alu.makeOperation(registerFile.getReadData(registerFile.getRegisterMatch(dr.getRs())),er.getsAluResult(),aluControl.getOperation());
                }
                //offset in alu
                else if(dr.getAluSrc()==1){
                    alu.makeOperation(registerFile.getReadData(registerFile.getRegisterMatch(dr.getRs())),dr.getOffset(),aluControl.getOperation());
                }
            }

            else if(fu.getControl1()==2 && fu.getControl2()==2){ //replace both
                //rt in alu
                if(dr.getAluSrc()==0){
                    alu.makeOperation(er.getsAluResult(),er.getsAluResult(),aluControl.getOperation());
                }
                //offset in alu
                else if(dr.getAluSrc()==1){
                    alu.makeOperation(er.getsAluResult(),dr.getOffset(),aluControl.getOperation());
                }
            }

            else if(fu.getControl1()==1 && fu.getControl2()==0){
                //rt in alu
                if(dr.getAluSrc()==0){
                    if(er.getMemRead()==1){
                        alu.makeOperation(mr.getsReadData(),registerFile.getReadData(registerFile.getRegisterMatch(dr.getRt())),aluControl.getOperation());
                    }
                    else {
                        alu.makeOperation(mr.getsAluResult(),registerFile.getReadData(registerFile.getRegisterMatch(dr.getRt())),aluControl.getOperation());
                    }
                }
                //offset in alu
                else if(dr.getAluSrc()==1){
                    if(er.getMemRead()==1){
                        alu.makeOperation(mr.getsReadData(),dr.getOffset(),aluControl.getOperation());
                    }
                    else {
                        alu.makeOperation(mr.getsAluResult(),dr.getOffset(),aluControl.getOperation());
                    }
                }
            }

            else if(fu.getControl1()==0 && fu.getControl2()==1){
                //rt in alu
                if(dr.getAluSrc()==0){
                    if(er.getMemRead()==1){
                        alu.makeOperation(registerFile.getReadData(registerFile.getRegisterMatch(dr.getRs())),mr.getsReadData(),aluControl.getOperation());
                    }
                    else {
                        alu.makeOperation(registerFile.getReadData(registerFile.getRegisterMatch(dr.getRs())),mr.getsAluResult(),aluControl.getOperation());
                    }
                }
                //offset in alu
                else if(dr.getAluSrc()==1){
                    if(er.getMemRead()==1){
                        alu.makeOperation(registerFile.getReadData(registerFile.getRegisterMatch(dr.getRs())),dr.getOffset(),aluControl.getOperation());
                    }
                    else {
                        alu.makeOperation(registerFile.getReadData(registerFile.getRegisterMatch(dr.getRs())),dr.getOffset(),aluControl.getOperation());
                    }
                }

            }

            else if(fu.getControl1()==1 && fu.getControl2()==1){
                //rt in alu
                if(dr.getAluSrc()==0){
                    if(er.getMemRead()==1){
                        alu.makeOperation(mr.getsReadData(),mr.getsReadData(),aluControl.getOperation());
                    }
                    else {
                        alu.makeOperation(mr.getsAluResult(),mr.getsAluResult(),aluControl.getOperation());
                    }
                }
                //offset in alu
                else if(dr.getAluSrc()==1){
                    if(er.getMemRead()==1){
                        alu.makeOperation(mr.getsReadData(),dr.getOffset(),aluControl.getOperation());
                    }
                    else {
                        alu.makeOperation(mr.getsAluResult(),dr.getOffset(),aluControl.getOperation());
                    }
                }
            }

            else if(fu.getControl1()==2 && fu.getControl2()==1){
                //rt in alu
                if(dr.getAluSrc()==0){
                    if(er.getMemRead()==1){
                        alu.makeOperation(er.getsAluResult(),mr.getsReadData(),aluControl.getOperation());
                    }
                    else {
                        alu.makeOperation(er.getsAluResult(),mr.getsAluResult(),aluControl.getOperation());
                    }
                }
                //offset in alu
                else if(dr.getAluSrc()==1){
                    if(er.getMemRead()==1){
                        alu.makeOperation(er.getsAluResult(),dr.getOffset(),aluControl.getOperation());
                    }
                    else {
                        alu.makeOperation(er.getsAluResult(),dr.getOffset(),aluControl.getOperation());
                    }
                }
            }

            else if(fu.getControl1()==1 && fu.getControl2()==2){
                //rt in alu
                if(dr.getAluSrc()==0){
                    if(er.getMemRead()==1){
                        alu.makeOperation(mr.getsReadData(),er.getsAluResult(),aluControl.getOperation());
                    }
                    else {
                        alu.makeOperation(mr.getsAluResult(),er.getsAluResult(),aluControl.getOperation());
                    }
                }
                //offset in alu
                else if(dr.getAluSrc()==1){
                    if(er.getMemRead()==1){
                        alu.makeOperation(mr.getsReadData(),dr.getOffset(),aluControl.getOperation());
                    }
                    else {
                        alu.makeOperation(mr.getsAluResult(),dr.getOffset(),aluControl.getOperation());
                    }
                }
            }




            if(dr.getRegDest()==1){
                er.setRegister(alu.getResult(),dr.getRtData(),dr.getRd(),dr.getOffset(),dr.getPC(),dr.getMemWrite(),dr.getMemRead(),dr.getBranch(),dr.getMemToReg(),dr.getRegWrite(),dr.getOpcode(),dr.getInstruction());
            }
            else if(dr.getRegDest()==0){
                er.setRegister(alu.getResult(),dr.getRtData(),dr.getRt(),dr.getOffset(),dr.getPC(),dr.getMemWrite(),dr.getMemRead(),dr.getBranch(),dr.getMemToReg(),dr.getRegWrite(),dr.getOpcode(),dr.getInstruction());
            }
            else if(dr.getRegDest()=='-') {
                er.setRegister(alu.getResult(),dr.getRtData(),"-",dr.getOffset(),dr.getPC(),dr.getMemWrite(),dr.getMemRead(),dr.getBranch(),dr.getMemToReg(),dr.getRegWrite(),dr.getOpcode(),dr.getInstruction());
            }
            //.printRegister();
        }

        //write back stage
        if(kikloi>3 && b<=3) {
            if(er.getMemWrite()==1) { memory.setMem(registerFile.getReadData(registerFile.getRegisterMatch(er.getWriteRegister())),er.getAluResult()); }
        }
        if(kikloi>3 && b<=4) {
            if(er.getMemRead()==1){ memResult=memory.getMem(er.getAluResult()); } else { memResult=0; }
            mr.setRegister(er.getAluResult(),memResult,er.getWriteRegister(),er.getMemToReg(),er.getRegWrite(),er.getInstruction(),er.getPC());
        }

        if(fu.getControl1()==0 && fu.getControl2()==0){ forward_taken=false; }
        else{ forward_taken=true; }

        branch[0]=branch_taken;
        branch[1]=branch[0];

        if(kikloi==cycle){
            timer.pause();
            midoutput(ir,dr,er,mr,memory,kikloi,registerFile,pc,instruction,hazard_taken,forward_taken,branch[1]);
            cout<<"Please enter the number where you want to stop"<<endl; cin>>cycle;
            timer.resume();
        }
        if(mr.getInstruction()=="sll$zero,$zero,0"){break;}
    }

    finalOutput(pc,registerFile,memory,kikloi,timer.elapsedNanoseconds());
    return 0;
}
