#include<bits/stdc++.h>
using namespace std;
class ProgramCounter{
private:
    int address;

public:
    ProgramCounter() : address(0) {

    }

    void increment() {
        address+=2;
    }

    void jump(int newAddress) {
        address = newAddress;
    }

    int getAddress() {
        return address;
    }

};

class Memory {
public:
    vector<string> memoryData;
    void loadMemory(string filename) {
        ifstream file(filename);
        if (file.is_open()) {
            string line;
            while (getline(file, line)) {
                istringstream iss(line);
                string hexValue;
                string data = "";

                while (iss >> hex >> hexValue) {
                    // Remove "0x" from the hex value
                    if (hexValue.substr(0, 2) == "0x") {
                        hexValue = hexValue.substr(2);
                    }

                    // Concatenate the hex values without spaces
                    data += hexValue;
                }
                memoryData.push_back(data.substr(0, 2));
                memoryData.push_back(data.substr(2));

            }
            file.close();
        } else
            cout << "Unable to open file: " << filename << endl;
    }

    string readMemory(int address) {
        return memoryData[address];
    }

    void writeMemory(int address, string data) {
        memoryData[address] = data;
    }

};
class Register{
public:
    vector<string>registerData;
    Register() : registerData(16) {};
    string readRegister(int address){
        return registerData[address];
    }
    void writeRegister(int address,string data){
        registerData[address]=data;
    }

};
class Machine{
public:
    Memory memory;
    ProgramCounter PC;
    Register registers;
    void DisplayPC(){
        cout<<"The content of the PC is: "<<PC.getAddress()<<endl;
    }
    void DisplayRegisters(int address){
        cout<<"The content of the Register is: "<<registers.readRegister(address)<<endl;
    }
    void DisplayMemory(int address){
        cout<<"The content of the memory is :"<<memory.readMemory(address)<<endl;
    }
    virtual void executeInstructions() = 0;
};


class ALU: public Machine{
    vector<string> instructions;
public:
    void loadInstructions(string filename){
        ifstream file(filename);
        if(!file.is_open()){
            cout<<"Error opening file"<<endl;
            return;
        }
        string hexa;
        string newHexa;
        while(file>>hexa){
            hexa=hexa.substr(2);
            newHexa+=hexa;
        }
        for(int i=0;i<newHexa.size();i+=4){
            instructions.push_back(newHexa.substr(i,4));
        }
    }
    void executeInstructions() {
         int i=0;
        while(i!=instructions.size()){
            string x = instructions[i];
            if (x[0] == '1') {
                i++;
                PC.increment();
                string M;
                int Maddress;
                string R;
                int Raddress;
                // converting the last three characters from hexadecimal to decimal
                M = x.substr(2);
                Maddress = stoi(M,nullptr,16);
                R = x[1];
                Raddress = stoi(R,nullptr,16);
                //loading data from a specific memory address into a specific register
                string datafrommemory = memory.readMemory(Maddress);
                registers.writeRegister(Raddress, datafrommemory);

            } else if (x[0] == '2') {
                i++;
                PC.increment();
                string registerAddress = x.substr(1,1);
                int registerAddresss = stoi(registerAddress,nullptr,16);
                string Data = x.substr(2,2);
                registers.writeRegister(registerAddresss,Data);

            } else if (x[0] == '3') {
                i++;
                PC.increment();
                string registerAddress = x.substr(1, 1);
                string memoryAddress = x.substr(2);
                int registerAddresss = stoi(registerAddress, nullptr, 16);
                int memoAddress = stoi(memoryAddress, nullptr, 16);
                string registerData = registers.readRegister(registerAddresss);
                if(memoAddress != 00) {
                    memory.writeMemory(memoAddress, registerData);
                }
                else
                    DisplayRegisters(registerAddresss);
            }else if (x[0] == '4') {
                i++;
                int R, S;
                if (x[2] >= '0' && x[2] <= '9') {
                    R = (int(x[2]) - '0');
                }
                else if (x[2] >= 'A' && x[2] <= 'F') {
                    R = (int(x[2]) - 'A' + 10);
                }

                if (x[3] >= '0' && x[3] <= '9') {
                    S = (int(x[3]) - '0');
                }
                else if (x[3] >= 'A' && x[3] <= 'F') {
                    S = (int(x[3]) - 'A' + 10);
                }

                registers.writeRegister(S, registers.readRegister(R));
                PC.increment();
            }
            else if (x[0] == '5') {
                i++;
                PC.increment();
                string R;
                string S;
                string T;
                int Raddresss;
                int Saddress;
                int Taddress;
                // converting the last 3 characters from hexadecimal to decimal
                R =x[1];
                Raddresss = stoi(R,nullptr,16);
                S =x[2];
                Saddress = stoi(S,nullptr,16);
                T =x[3];
                Taddress = stoi(T,nullptr,16);

                //Adding the data in registers S and T then storing the sum in register R
                string datafromS =  registers.readRegister(Saddress) ;
                string datafromT =  registers.readRegister(Taddress) ;

                //converting data inside registers S and T from hex to decimal
                int dec1 = stoi(datafromS, nullptr, 16);
                int dec2 = stoi(datafromT, nullptr, 16);

                // add the two numbers
                int sum = dec1 + dec2;

                //convert the sum from decimal to hex
                string hex = "";
                for (int i = 0; i < 2; i++) {
                    int remainder = sum % 16;
                    if (remainder <= 9) {
                        hex = char(remainder + '0') + hex;
                    } else {
                        hex = char(remainder + 55)  + hex;
                    }
                    sum = sum / 16;
                }
                //store the sum in register R
                registers.writeRegister(Raddresss,hex);

            } else if (x[0] == 'B') {
                i++;
                string R = x.substr(1, 1);
                string XY = x.substr(2);
                int Raddress = stoi(R, nullptr, 16);
                int jumpAddress = stoi(XY, nullptr, 16);
                if (registers.readRegister(Raddress) == registers.readRegister(0)) {
                    PC.jump(jumpAddress);
                }
                PC.increment();

            } else if (x[0] == 'C') {
                PC.increment();
                break;

            }
        }
    }
};
int main() {
    ALU program;
    program.memory.loadMemory("test.txt");
    program.loadInstructions("test.txt");
    program.executeInstructions();
    program.DisplayPC();

    program.DisplayRegisters(0);
    program.DisplayRegisters(1);
    program.DisplayRegisters(2);





    return 0;
}

