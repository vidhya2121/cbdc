#include <iostream>
#include "invokeHamilton.h"
#include <string>
#include <fcntl.h> //for files
#include <fstream>
#include <limits>
using std::cout, std::cin, std::getline, std::cerr, std::string, std::to_string, std::endl;

/*WHERE CHANGES BEEN MADE*/
#pragma region Write to var
//Using https://stackoverflow.com/questions/478898/how-do-i-execute-a-command-and-get-the-output-of-the-command-within-c-using-po
//AND https://www.onlinegdb.com/SJ5qIk2vm
// For Windows chage popen and pclose to _popen and _pclose
#include <cstdio>
#include <memory>
#include <stdexcept>
#include <array>

std::string exec(const char* cmd) {
    std::array<char, 128> buffer;
    std::string result;
    std::unique_ptr<FILE, decltype(&pclose)> pipe(popen(cmd, "r"), pclose);
    if (!pipe) {
        throw std::runtime_error("popen() failed!");
    }
    while (fgets(buffer.data(), buffer.size(), pipe.get()) != nullptr) {
        result += buffer.data();
    }
    return result;
}
#pragma endregion
    /*WHERE CHANGES BEEN MADE. END*/







#pragma region Helper Funcs
std::istream& ignoreline(std::ifstream& in, std::ifstream::pos_type& pos)
{
    pos = in.tellg();
    return in.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
}

std::string getLastLine(std::ifstream& in)
{
    std::ifstream::pos_type pos = in.tellg();

    std::ifstream::pos_type lastPos;
    while (in >> std::ws && ignoreline(in, lastPos))
        pos = lastPos;

    in.clear();
    in.seekg(pos);

    std::string line;
    std::getline(in, line);
    return line;
}

string readFromFile(string file_name) {
    std::ifstream file(file_name);
    if (file)
    {
        std::string line = getLastLine(file);
        return line;
    }
    return "";
}

std::string ReadNthLine(const std::string& filename, int N)
{
   std::ifstream in(filename.c_str());

   std::string s;  

   //skip N lines
   for(int i = 0; i < N; ++i)
       std::getline(in, s);

   std::getline(in,s);
   return s; 
}


std::string split(string str, char delim) {
    int index = str.find(delim);
    return str.substr(index+1, str.length());
}
#pragma endregion

#pragma region Hamilton


Hamilton::Hamilton() {
    numwallets = 0;
    max_mempool = 10;
}

bool Hamilton::RUN_HAMILTON(){
    try{
        //Try to run it
        return 1;
    }
    catch(...){ //failed for some reason
        cerr<<"Error: Failed to run OpenCBDC\n";
        return 0;
    }
}


//Minting coins and writing them in the first file
void Hamilton::MINT_NEW_COINS(string mempool, string wallet, double UTXOs, double atomic_value) { 
/*WHERE CHANGES BEEN MADE*/
    std::string command = "./build/src/uhs/client/client-cli 2pc-compose.cfg "+mempool+ " " +wallet+"  mint "+to_string(UTXOs)+" "+to_string(atomic_value)+"\n";
    exec(command.c_str());
    /*WHERE CHANGES BEEN MADE. END*/
}
//Get information on balance
string Hamilton::GET_BALANCE(string mempool, string wallet) {
    string cmd =  "./build/src/uhs/client/client-cli 2pc-compose.cfg "+mempool+ " "+wallet+" info "+"\n";
    string getbal = exec(cmd.c_str());
    string balance_value = getbal.substr(getbal.find('$')+1, getbal.find(',')-getbal.find('$')-1);
    return balance_value;
}
//Creating a wallet
std::pair<string, string> Hamilton::CREATE_WALLET() {
    numwallets++;
    string wallet = "wallet"+to_string(numwallets)+".dat";
    string mempool = "mempool"+to_string(numwallets % max_mempool)+".dat";
    string cmd = "./build/src/uhs/client/client-cli 2pc-compose.cfg "+mempool+" "+wallet+" newaddress\n";
    string create = exec(cmd.c_str());
    return std::make_pair(wallet, mempool);
}
//Get the wallet's address
string Hamilton::GET_WALLET_ADDRESS(string wallet, string mempool) {
    string cmd = "./build/src/uhs/client/client-cli 2pc-compose.cfg "+mempool+" "+wallet+" newaddress\n";
    string add = exec(cmd.c_str());
    return add;
}
// Send transaction from one wallet to the other
string Hamilton::TRANSACTION_TO_WALLET(string dest_mempool, string dest_wallet, double amount, string src_mempool, string src_wallet) {
    string key_of_receipient = GET_WALLET_ADDRESS(dest_wallet, dest_mempool);
    key_of_receipient.erase(std::remove(key_of_receipient.begin(), key_of_receipient.end(), '\n'), key_of_receipient.cend());
    string cmd = "./build/src/uhs/client/client-cli 2pc-compose.cfg "+src_mempool+ " " +src_wallet+"  send "+to_string(amount) +" "+key_of_receipient+"\n";
    string tran = exec(cmd.c_str());
    size_t position_of_s = tran.find("S"); // position of ";" in start_string
    //Continuing formula string proceeding      
    string formula1 = tran.substr (0, position_of_s); // Dividing formula from the initial string      
    string formula = formula1.erase(position_of_s); // Errasing ";" from the formula string
    size_t position_of_colon = formula.find("importinput:"); // position of ";" in start_string
// After ":"
      string assignment1 = formula.substr(position_of_colon);     // get from ";" to the end   
      string recepient = assignment1.erase(0, 12); // Errasing ";" from the assignment string  
          recepient.erase(std::remove(recepient.begin(), recepient.end(), '\n'), recepient.cend());
    return recepient;
}

//Import coins to the receiving wallet using the string after importinput from the currency transfer step above    
void Hamilton::IMPORTINPUT_OF_TRANSACTION (string data_for_receipient_to_import, string mempool, string wallet) {
    string cmd = "./build/src/uhs/client/client-cli 2pc-compose.cfg "+mempool+ " " +wallet+"  importinput "
    +data_for_receipient_to_import;
    system(cmd.c_str());
    SYNC_PREV_TRANSACTION(wallet, mempool);
}

void Hamilton::TRANSFER(string dest_mempool, string dest_wallet, double amount, string src_mempool, string src_wallet){
    // Send from user1 to user2
    amount = amount * UTXOS;
    string data_for_user2_import = TRANSACTION_TO_WALLET(dest_mempool, dest_wallet, amount, src_mempool, src_wallet);
    IMPORTINPUT_OF_TRANSACTION(data_for_user2_import, dest_mempool, dest_wallet);
}

//Syncing information above    
void Hamilton::SYNC_PREV_TRANSACTION (string wallet, string mempool) {
    string cmd = "./build/src/uhs/client/client-cli 2pc-compose.cfg "+mempool+ " " +wallet+"  sync";
    system(cmd.c_str());
}

#pragma endregion 


class TestEntity{
    bool canMint; //can be used for the restriction that only CB can mint. Done properly in classes.cpp; here just quick idea test
public:
    string wallet;
    string mempool;
    string address;
    double balance=0;
    TestEntity(bool _canMint=false){
        std::pair<string, string> user_wallet = hamilton.CREATE_WALLET();
        wallet = user_wallet.first;
        mempool = user_wallet.second;
        address = hamilton.GET_WALLET_ADDRESS(wallet, mempool);
        canMint = _canMint;
    }

    void printEntity(){
        cout<<"---\n";
        cout<<"wallet: "<<wallet<<'\n'
            <<"mempool: "<<mempool<<'\n'
            <<"address: "<<address<<'\n'
            <<"balance: "<<balance<<'\n';
    }
};

void InvokeManualTest(){
    //CB
    TestEntity CB = TestEntity(true);
    TestEntity UserA = TestEntity();
    TestEntity UserB = TestEntity();

    cout << "Address of the bank is :" << CB.address<<'\n';// hamilton.getCentralBankAddress() << endl;
    cout << "Address of the uA is :" << UserA.address<<'\n';// hamilton.getCentralBankAddress() << endl;
    cout << "Address of the uB is :" << UserB.address<<'\n';// hamilton.getCentralBankAddress() << endl;

    double initial_amount = 10000;
    double transfer_amount = 1000;
    // Mint coins - Central Banks only can do this
    hamilton.MINT_NEW_COINS(CB.mempool, CB.wallet, UTXOS, 1000000);
    hamilton.TRANSFER(UserA.mempool, UserA.wallet ,initial_amount, CB.mempool, CB.wallet);
    hamilton.TRANSFER(UserB.mempool, UserB.wallet, initial_amount, CB.mempool, CB.wallet);

    // Check balance of central bank
    string balance_central_bank = hamilton.GET_BALANCE(CB.mempool, CB.wallet);
    cout << "Balance of central bank is :" << balance_central_bank  << endl;

    // Check balance of user wallets
    string balance_user1 = hamilton.GET_BALANCE(UserA.mempool, UserA.wallet);
    cout << "Balance of user 1 :" << balance_user1  << endl;
    string balance_user2 = hamilton.GET_BALANCE(UserB.mempool, UserB.wallet);
    cout << "Balance of user 2 :" << balance_user2  << endl;

    // Send from user1 to user2
    hamilton.TRANSFER(UserB.mempool, UserB.wallet,transfer_amount, UserA.mempool, UserA.wallet);

    // Check balance of user wallets
    CB.balance = std::stod(hamilton.GET_BALANCE(CB.mempool, CB.wallet));
    UserA.balance = std::stod(hamilton.GET_BALANCE(UserA.mempool, UserA.wallet));
    UserB.balance = std::stod(hamilton.GET_BALANCE(UserB.mempool, UserB.wallet));

    //Print entities
    CB.printEntity();
    UserA.printEntity();
    UserB.printEntity();

}

// int main(){ //NOTE: MAKE SURE TO COMMENT THIS main() BEFORE RUNNING FULL VERSION
//     InvokeManualTest();
// }
