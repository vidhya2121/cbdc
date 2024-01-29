#pragma once
#include <string>
#include <fcntl.h> //for files
#include <fstream>
#include <limits>
using std::string;

static const int UTXOS = 100;

class Hamilton {
    int numwallets;
    int max_mempool;
    // string central_bank_wallet;
    // string central_bank_mempool;
    // string central_bank_address;
    bool RUN_HAMILTON(); //Run the project automatically and return status
    bool isRunning = RUN_HAMILTON();

    string TRANSACTION_TO_WALLET(string dest_mempool, string dest_wallet, double amount, string src_mempool, string src_wallet);
    void IMPORTINPUT_OF_TRANSACTION (string data_for_receipient_to_import, string mempool, string wallet);
    void SYNC_PREV_TRANSACTION (string wallet, string mempool="mempool1.dat");
        
    public:
        Hamilton();
        // string getCentralBankAddress();
        // std::pair<string, string> getCentralBankWalletMempool();
        std::pair<string, string> CREATE_WALLET();
        string GET_WALLET_ADDRESS(string wallet, string mempool="mempool1.dat");
        void MINT_NEW_COINS(string mempool, string wallet, double UTXOs=100, double atomic_value=1);
        string GET_BALANCE(string mempool="mempool0.dat", string wallet="wallet0.dat");
        void TRANSFER(string dest_mempool, string dest_wallet, double amount=10, string src_mempool="mempool0.dat", string src_wallet="wallet0.dat");
        //read output:
        string getBalanceOutput();
        string getMintOutput();
        string getWalletAddressOutput();
        string getTransactionOutput();

};
#ifndef _hamilton
#define _hamilton
static Hamilton hamilton;
#endif


std::istream& ignoreline(std::ifstream& in, std::ifstream::pos_type& pos);
std::string getLastLine(std::ifstream& in);
string readFromFile(string file_name);
std::string ReadNthLine(const std::string& filename, int N);
std::string split(string str, char delim);