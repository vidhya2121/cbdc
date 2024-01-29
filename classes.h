#pragma once
#include <iostream>
using std::string, std::cout, std::cerr;// using namespace std; causes problems "ambiguous" with vars
#include <vector>
#include <string> //still have to put std::string but must include this
#include "invokeHamilton.h"


/*DESCRIPTION: 
    This code contains the classes needed in an economy
    1 base class "Entity" Parent of 3 classes "Central Bank", "Commercial Bank", "Users" which are actors in the economy
    2 classes for Logging: "Ledger" which takes "LedgerEntry" as input
*/

string generateID();
const string CENTRAL_BANK_TYPE = "Cen";
const string COMMERCIAL_BANK_TYPE = "Com";
const string USER_TYPE = "U";


class LedgerEntry {
    string timestamp;
    string transaction;
    string fromid, toid; 
    double amount;
public:
    LedgerEntry(string _time, string _tx, string _from, string _to, double _am){
        timestamp=_time; transaction=_tx; fromid=_from; toid=_to; amount=_am;
    }
    void printEntry();
};
class Ledger{
    static std::vector<LedgerEntry> ledger;
public:
    void addLedgerEntry(LedgerEntry entry);
    void showLedger();
};

#pragma region Base classes
/*Entity types: CENTRAL_BANK_TYPE=Central Bank; COMMERCIAL_BANK_TYPE=commercial bank; USER_TYPE=User
*/
class Entity{
    double balance=0;
    double bonds=0;

protected: //no need to be public since accessed by public functions + no misuse
    static std::vector<Entity*> allEntities; //initialized in .cpp
    string id;
    const string type;

    //OpenCBDC
    string walletdat;
    string mempool;
    string address;
    
    static int num_CentralBanks;
    static int num_CommercialBanks;
    static int num_Users;
    
    //add or remove balance from account;
    void changeBalance(double amount);
    void changeBonds(double amount);

    static void printallTypedEntities(string type);
    static Ledger* ledger;

public:
    Entity(string _type): type(_type){ //constructor which initiates constant var
        try{
            //OpenCBDC
            std::pair<string, string> user_wallet = hamilton.CREATE_WALLET();
            walletdat = user_wallet.first;
            mempool = user_wallet.second;
            address = hamilton.GET_WALLET_ADDRESS(walletdat, mempool);
            id = walletdat+"+"+mempool;
        }
        catch(...){
            cerr<<"Error generating wallet in OpenCBDC\n";
            //custom id
            id = generateID();
        }
        allEntities.push_back(this);
    }
    
    //gets id, const to make sure the function doesn't change id
    string getid() const{ return this->id;} 
    string gettype() const{ return this->type;}

    //OpenCBDC
    string getwalletdat() const{ return this->walletdat;} 
    string getmempool() const{ return this->mempool;} 
    string getaddress() const{ return this->address;} 
    void updateBalance(){this->balance = std::stod(hamilton.GET_BALANCE(mempool, walletdat));}

    //gets balance, const to make sure the function doesn't change value
    double getBalance() const{ return this->balance;}
    double getBonds() const{ return this->bonds;}

    static bool checkIDexists(string id);
    static Entity* getObjectfromID(string id);
    static void printallEntities();
    static void showCurrentLedger();

    static int getnum_CentralBanks(){return num_CentralBanks;}
    static int getnum_CommercialBanks(){return num_CommercialBanks;}
    static int getnum_Users(){return num_Users;}
    static int getnum_Entities(){return num_CentralBanks + num_CommercialBanks + num_Users;} //allEntities.size()

    static void printallUsers(){printallTypedEntities(USER_TYPE);}
    static void printallComBanks(){printallTypedEntities(COMMERCIAL_BANK_TYPE);}
    static void printallCenBanks(){printallTypedEntities(CENTRAL_BANK_TYPE);}

    // Used for Testing ONLY. Not secure for production
    static Entity* getEntityFromIndex(int index){return allEntities[index];}

    // //creates money by selling bonds from central bank
    // bool receiveMoney(double value);
    // bool sellBonds(double value);
    // //removes money by buying bonds to central bank
    // bool sendMoney(double value);
    // bool buyBonds(double value);

    // Transfer amount to entity_id
    bool Transfer(double amount, string to_id, string message="");
};

void printEntity(Entity * e);

#pragma endregion
class CentralBank: public Entity
{
    int totalMoneyinCirculation=0;

public:
	CentralBank(): Entity(CENTRAL_BANK_TYPE){num_CentralBanks++;}
    // ~CentralBank(); //Destructor

    //creates money
    bool generateMoney(double value);
    //removes money 
    bool removeMoney(double value);
    
    bool ForceTransfer(string fromid, string toid, double amount);
};

class CommercialBank: public Entity
{
public:
	CommercialBank(): Entity(COMMERCIAL_BANK_TYPE){num_CommercialBanks++;} // Constructor
    // ~CommercialBank(); //Destructor
    
};



class User: public Entity
{
public:
	User(): Entity(USER_TYPE){num_Users++;} // Constructor
    // ~User(); //Destructor

};

