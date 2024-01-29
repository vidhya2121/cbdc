#include <iostream>
using std::string, std::cout, std::cin;// using namespace std; causes problems "ambiguous" with vars
#include <vector>
#include "classes.h"
#include "invokeHamilton.h"

#include <stdlib.h> /* srand, rand */
#include <random> //"dev/urandom"
#include <time.h>   /* time, used to generate random seed */
#include <unistd.h> //in linux only. For windows it is windows.h
std::random_device rd1("/dev/urandom");

/*DESCRIPTION: 
    This code is used to test the Standalone implementation of "classes.h" functionality
*/

#pragma region Helper Random Funcs
int GenerateRandint(int lowerbound, int upperbound){
    std::uniform_int_distribution<int> d(lowerbound, upperbound);

    int num = d(rd1);
    return num;
}

Entity* getRandomEntity(string type=""){
    //get random one at first
    int index = GenerateRandint(0, Entity::getnum_Entities()-1); //-1 coz size starts counting from 1 not 0 so out of bound
    Entity* chosen = Entity::getEntityFromIndex(index);

    // if don't care about type return it
    if (type==""){
        return chosen;
    }
    
    // otherwise keep searching
    while(chosen->gettype()!=type){
        index = GenerateRandint(0, Entity::getnum_Entities()-1); //-1 coz size starts counting from 1 not 0 so out of bound
        chosen = Entity::getEntityFromIndex(index);
    }
    return chosen;
}
#pragma endregion


void EconRandomTest(int numactions){
    // start with a central bank
    CentralBank *Cen = new CentralBank();
    // generate default money 1 billion
    Cen->generateMoney(1000000000);

    CommercialBank *Com = new CommercialBank();
    Cen->Transfer(10000, Com->getid());
    User *user = new User();
    Cen->Transfer(10000, user->getid());
    
    // Do random actions
    auto start = std::chrono::steady_clock::now();
    while(numactions>0){
        numactions--;
        // choose action
        int action = GenerateRandint(0,2);
        
        if (action == 0){ //create random entity
            cout<<"Create\n";
            int create_type = GenerateRandint(0,1);
            if (create_type==0) {
                Com = new CommercialBank();
                // Default money from central to commercial bank 10K
                Cen->Transfer(10000, Com->getid());
            }
            if (create_type==1)
                user = new User();
                // Default money from central bank to user 1K
                Cen->Transfer(10000, user->getid());
            continue;
        }
        if (action == 1){ //create or remove money available in economy
            cout<<"Money available\n";
            // NOTE: UNminting money is currently UNSUPPORTED by OpenCBDC
            // int action_type = GenerateRandint(0,1);
            // if (action_type==0)
                Cen->generateMoney(GenerateRandint(100,1000));
            // if (action_type==1)
            //     Cen->removeMoney(GenerateRandint(100,200)); 
            continue;
        }
        if (action == 2){ //random transfer from random entity
            cout<<"Transfer\n";
            Entity* e = getRandomEntity();
            // printEntity(e);
            // Transfer 10 dollars
            e->Transfer(GenerateRandint(10,20), getRandomEntity()->getid());
            continue;
        }
    }
    auto end = std::chrono::steady_clock::now();
    cout << "Elapsed time in milliseconds: "
        << std::chrono::duration_cast<std::chrono::milliseconds>(end - start).count()
        << " ms" << std::endl;
}

void EconManualTest(){
    //start economy
    // create central bank
    CentralBank Cen;
    // //create 2 Commercial banks
    CommercialBank Com1;
    CommercialBank Com2;
    // //create 3 users
    User u1;
    User u2;
    User u3;

    cout<<"Central Banks created: "<<Entity::getnum_CentralBanks()<<'\n';
    cout<<"Commercial Banks created: "<<Entity::getnum_CommercialBanks()<<'\n';
    cout<<"Users created: "<<Entity::getnum_Users()<<'\n';

    //Cen generate cash
    Cen.generateMoney(3000);

    //Give money to Commercial Banks
    Cen.Transfer(1200, Com1.getid());
    Cen.Transfer(1000, Com2.getid());
    //Com1 and Com2 distributes cash to Users (pretend it's loans)
    Com1.Transfer(200, u1.getid());
    Com1.Transfer(800, u2.getid());
    Com2.Transfer(500, u3.getid());

    //Force transfer from U1 to U2
    Cen.ForceTransfer(u1.getid(), u2.getid(), 50);
}

void EconBenchmarkTest(){
    //start economy
    // create central bank
    CentralBank Cen;
    // //create 2 Commercial banks
    CommercialBank Com1;
    CommercialBank Com2;
    // //create 3 users
    User u1;

    // cout<<"Central Banks created: "<<Entity::getnum_CentralBanks()<<'\n';
    // cout<<"Commercial Banks created: "<<Entity::getnum_CommercialBanks()<<'\n';
    // cout<<"Users created: "<<Entity::getnum_Users()<<'\n';

    // cout << "Central Bank Balance: " << Cen.getBalance() << std::endl;
    // cout << "Commercial Bank Balance: " << Com1.getBalance() << std::endl;
    // cout << "User1 Balance: " << u1.getBalance() << std::endl;

    //Cen generate cash
    Cen.generateMoney(1000000000);
    // cout << "Central Bank Balance: " << Cen.getBalance() << std::endl;
    // cout << "Commercial Bank Balance: " << Com1.getBalance() << std::endl;
    // cout << "User1 Balance: " << u1.getBalance() << std::endl;

    //Give money to Commercial Banks
    Cen.Transfer(10000000, Com1.getid());
    // cout << "Central Bank Balance: " << Cen.getBalance() << std::endl;
    // cout << "Commercial Bank Balance: " << Com1.getBalance() << std::endl;
    // cout << "User1 Balance: " << u1.getBalance() << std::endl;
    auto start = std::chrono::steady_clock::now();
    //Com1 and Com2 distributes cash to Users (pretend it's loans)
    for (int i = 0; i < 2000; i++) {
        Com1.Transfer(10, u1.getid());
    }
    auto end = std::chrono::steady_clock::now();
    cout << "Elapsed time in milliseconds: "
        << std::chrono::duration_cast<std::chrono::milliseconds>(end - start).count()
        << " ms" << std::endl;
    cout << "Central Bank Balance: " << Cen.getBalance() << std::endl;
    cout << "Commercial Bank Balance: " << Com1.getBalance() << std::endl;
    cout << "User1 Balance: " << u1.getBalance() << std::endl;
}

int main(){
    // EconManualTest();
    EconBenchmarkTest();
    // EconRandomTest(1000);    

    // Entity::printallEntities();
    // Entity::showCurrentLedger();

    return 0;
}