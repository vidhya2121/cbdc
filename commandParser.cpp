#include <iostream>
using std::cin, std::cout, std::string, std::getline;
#include <string>
#include <sstream>
#include "classes.h"

/*DESCRIPTION: 
    This code is the custom CLI used to trigger functionalities introduced in classes.h
    Will be later used to pipe custom commands to OpenCBDC commands (docker container)
*/

#pragma region COMMANDS:
const string HELP_COMMAND = "help";
const string RUN_PROTOTYPE_COMMAND = "run prototype";
const string CREATE_CentralBank_COMMAND = "create cen";
const string CREATE_CommercialBank_COMMAND = "create com";
const string CREATE_User_COMMAND = "create user";
const string SHOW_ALL_COMMAND = "show all";
const string SHOW_CentralBank_COMMAND = "show cen";
const string SHOW_CommercialBank_COMMAND = "show com";
const string SHOW_Users_COMMAND = "show users";
const string SHOW_Ledger_COMMAND = "show ledger";
const int CenB_LIMIT=1;
#pragma endregion

// DECLARATIONS:
void helpCommand();
void getInputCommand(string input);
void runPrototype();

int main(){
    string input;
    cout<<"Welcome to the CBDC economy user-interface!\nEnter command (or type 'help'): ";
    while (getline(cin, input)) { // read from stdin until EOF
        if (input=="q")
            break;
        // cout<<input<<'\n';

        if (input==HELP_COMMAND)
            helpCommand();
        else
            getInputCommand(input);

        cout<<"Enter command (or 'q' to quit): ";
    }
    return 0;
}

void helpCommand(){
    cout<<"--- Commands \t | Descripton\n"
        <<"'"<<CREATE_CentralBank_COMMAND<<"': \t creates central bank\n"
        <<"'"<<CREATE_CommercialBank_COMMAND<<"': \t creates commercial bank\n"
        <<"'"<<CREATE_User_COMMAND<<"': \t creates user\n"
        <<"'"<<SHOW_ALL_COMMAND<<"': \t shows details of all entities created\n"
        <<"'"<<SHOW_CentralBank_COMMAND<<"': \t shows details of central banks created\n"
        <<"'"<<SHOW_CommercialBank_COMMAND<<"': \t shows details of commercial banks created\n"
        <<"'"<<SHOW_Users_COMMAND<<"': \t shows details of users created\n"
        <<"'"<<SHOW_Ledger_COMMAND<<"': \t shows Ledger\n"
        // <<"'"<<RUN_PROTOTYPE_COMMAND<<"': \t runs prototype\n"
        <<"";
}

void getInputCommand(string input){
    //CREATE:
    if (input==CREATE_CentralBank_COMMAND){
        if (Entity::getnum_CentralBanks() == CenB_LIMIT){
            cout<<"A Central Bank Already exists. To continue, you must remove limit in the code.\n";
            return;
        }
        CentralBank *Cen = new CentralBank();
        cout<<"Central Bank created\n";
        return;
    }

    if (input==CREATE_CommercialBank_COMMAND){
        CommercialBank *Com = new CommercialBank();
        cout<<"Commercial Bank created\n";
        return;
    }

    if (input==CREATE_User_COMMAND){
        User *user = new User();
        cout<<"User created\n";
        return;
    }

    //SHOW:
    if (input==SHOW_ALL_COMMAND){
        Entity::printallEntities();
        return;
    }
    if (input==SHOW_CentralBank_COMMAND){
        cout<<"Central Banks created: "<<Entity::getnum_CentralBanks()<<'\n';
        return;
    }
    if (input==SHOW_CommercialBank_COMMAND){
        cout<<"Commercial Banks created: "<<Entity::getnum_CommercialBanks()<<'\n';
        return;
    }
    if (input==SHOW_Users_COMMAND){
        cout<<"Users created: "<<Entity::getnum_Users()<<'\n';
        return;
    }
    if (input==SHOW_Ledger_COMMAND){
        Entity::showCurrentLedger();
        return;
    }
    

    //DEFAULT:
    cout<<"Error: Command Not understood. Type 'help' for a list of commands.\n";
}

void runPrototype(){
    cout<<"Prototype\n";
}