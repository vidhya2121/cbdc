#include<unistd.h>
#include<sys/wait.h>
#include<iostream>
#include <signal.h>
using std::cout, std::cin, std::getline, std::cerr, std::string, std::to_string;
#include<string>

int numwallets=0;
int MINT_NEW_COINS(int UTXOs, int atomic_value, string wallet="wallet0.dat") {
    char *args[8];
    args[0] = (char *) "./build/src/uhs/client/client-cli";
    args[1] = (char *) "2pc-compose.cfg";
    args[2] = (char *) "mempool0.dat";
    args[3] = (char *) wallet.c_str();
    args[4] = (char *) "mint";
    args[5] = (char *) to_string(UTXOs).c_str();
    args[6] = (char *) to_string(atomic_value).c_str();
    args[7] = nullptr;
    execv("./build/src/uhs/client/client-cli", args);
    return 0;
}

int GET_BALANCE(string wallet="wallet0.dat") {
    char *args[6];
    args[0] = (char *) "./build/src/uhs/client/client-cli";
    args[1] = (char *) "2pc-compose.cfg";
    args[2] = (char *) "mempool0.dat";
    args[3] = (char *) wallet.c_str();
    args[4] = (char *) "info";
    args[5] = nullptr;
    execv("./build/src/uhs/client/client-cli", args);
    return 0;
}
int CREATE_WALLET() {
    numwallets++;
    string wallet = "wallet"+to_string(numwallets)+".dat";
    char *args[6];
    args[0] = (char *) "./build/src/uhs/client/client-cli";
    args[1] = (char *) "2pc-compose.cfg";
    args[2] = (char *) "mempool0.dat";
    args[3] = (char *) wallet.c_str();
    args[4] = (char *) "newaddress";
    args[5] = nullptr;
    execv("./build/src/uhs/client/client-cli", args);
    return 0;
}

int main() {
    int CtoP[2];
    pipe(CtoP);
    auto pid = fork();
    if (pid == 0) {
        dup2(CtoP[1], STDOUT_FILENO);
        close(CtoP[0]);
        close(CtoP[1]);
        return MINT_NEW_COINS(10,1000);
    } 

    // only parent gets to here
    dup2(CtoP[0], STDIN_FILENO);
    close(CtoP[0]);
    close(CtoP[1]);

    // read input from the child until eof
    while (!std::cin.eof())
    {
        std::string line;
        std::getline(std::cin, line);
        if (line.size() == 0) {
            continue;
        }
        std::cout << line << std::endl;
    }
    int status;
    waitpid(pid, &status, 0);

    pid = fork();
    if (pid == 0) {
        dup2(CtoP[1], STDOUT_FILENO);
        close(CtoP[0]);
        close(CtoP[1]);
        return GET_BALANCE();
    } 

    // only parent gets to here
    dup2(CtoP[0], STDIN_FILENO);
    close(CtoP[0]);
    close(CtoP[1]);

    // read input from the child until eof
    while (!std::cin.eof())
    {
        std::string line;
        std::getline(std::cin, line);
        if (line.size() == 0) {
            continue;
        }
        std::cout << line << std::endl;
    }
    waitpid(pid, &status, 0);

    pid = fork();
    if (pid == 0) {
        dup2(CtoP[1], STDOUT_FILENO);
        close(CtoP[0]);
        close(CtoP[1]);
        return CREATE_WALLET();
    } 

    // only parent gets to here
    dup2(CtoP[0], STDIN_FILENO);
    close(CtoP[0]);
    close(CtoP[1]);

    // read input from the child until eof
    while (!std::cin.eof())
    {
        std::string line;
        std::getline(std::cin, line);
        if (line.size() == 0) {
            continue;
        }
        std::cout << line << std::endl;
    }
    waitpid(pid, &status, 0);
    return 0;
}