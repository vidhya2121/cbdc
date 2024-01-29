#include "classes.h"

/*DESCRIPTION: 
    This code is the implementation of functionalities introduced in classes.h (so read .h first to get a better understanding)
    1 base class "Entity" Parent of 3 classes "Central Bank", "Commercial Bank", "Users" which are actors in the economy
    2 classes for Logging: "Ledger" which takes "LedgerEntry" as input
*/

#pragma region Helper Funcs
int count=0;
string generateID(){ 
    count++;
    return "id"+std::to_string(count);
}


#pragma endregion

#pragma region Ledger
void LedgerEntry::printEntry(){
    cout<<
        "timestamp: "<<timestamp<<'\n'<<
        "transaction: "<<transaction<<'\n'<<
        "from: "<<fromid<<'\n'<<
        "to: "<<toid<<'\n'<<
        "amount: "<<amount<<'\n';
}

std::vector<LedgerEntry> Ledger::ledger={};
void Ledger::addLedgerEntry(LedgerEntry entry){
    Ledger::ledger.push_back(entry);
}
void Ledger::showLedger(){
    cout<<"{\n";
    for (int i = 0; i < Ledger::ledger.size(); i++)
    {
        LedgerEntry* e = &Ledger::ledger[i];
        cout<<"---"<<i<<'\n';
        e->printEntry();
    }
    cout<<"}\n";
}
#pragma endregion

#pragma region Entity
#pragma region static

int Entity::num_CentralBanks=0;
int Entity::num_CommercialBanks=0;
int Entity::num_Users=0;

std::vector<Entity*> Entity::allEntities={};
Ledger* Entity::ledger= new Ledger();

bool Entity::checkIDexists(string id){ //checks if id is in vector
    for (int i = 0; i < Entity::allEntities.size(); i++)
    {
        if (id == Entity::allEntities[i]->getid())
            return true;
    }
    return false; //not exist
}
Entity* Entity::getObjectfromID(string id){ //returns object of id in vector
    for (int i = 0; i < Entity::allEntities.size(); i++)
    {
        if (id == Entity::allEntities[i]->getid())
            return Entity::allEntities[i];
    }
    return NULL; //not exist
}

void printEntity(Entity * e){
    cout<<"Id: "<<e->getid()<<'\n';
    cout<<"Type: "<<e->gettype()<<'\n';
    cout<<"Balance: "<<e->getBalance()<<'\n';
    cout<<"Bonds: "<<e->getBonds()<<'\n';
}

void Entity::printallEntities(){
    cout<<"[*] Current State of Entities:\n";
    cout<<"{\n";
    for (int i = 0; i < Entity::allEntities.size(); i++)
    {
        cout<<"---"<<i<<'\n';
        printEntity(Entity::allEntities[i]);
    }
    cout<<"}\n";
}
void Entity::printallTypedEntities(string type){
    cout<<"[*] Current State of "<<type<<" entities:\n";
    cout<<"{\n";
    for (int i = 0; i < Entity::allEntities.size(); i++)
    {
        if (Entity::allEntities[i]->gettype() == type){
            cout<<"---"<<i<<'\n';
            printEntity(Entity::allEntities[i]);
        }
    }
    cout<<"}\n";
}

void Entity::showCurrentLedger(){
    cout<<"[*] Ledger:\n";
    ledger->showLedger();
}
#pragma endregion

void Entity::changeBalance(double amount){ 
    //DO it in OpenCBDC
    try{
        this->updateBalance();
    }
    catch(...){
        cerr<<"Error Getting balance in OpenCBDC\n";
        this->balance+=amount;
    }
}
void Entity::changeBonds(double amount){ this->bonds+=amount;}

bool Entity::Transfer(double amount, string to_id, string message){
    if (amount<=0){
        cout<<"Error: Cannot transfer amount below 1\n";
        return 0;
    }
    if (amount > this->balance){
        cout<<"Error: Your balance "<<this->balance<<" is Not enough to transfer "<<amount<<'\n';
        return 0;
    }
    
    if (!Entity::checkIDexists(to_id)){
        cout<<"Error: Receipient ID NOT found "<<to_id<<'\n';
        return 0;
    }

    //DO it in OpenCBDC
    try{
        string dest_wallet = to_id.substr(0, to_id.find("+"));
        string dest_mempool = to_id.substr(2, to_id.find("+"));
        // Send from user1 to user2
        // cout << "transfer debug " << dest_wallet << " " << this->getwalletdat() << std::endl;
        hamilton.TRANSFER(dest_mempool, dest_wallet, amount, this->getmempool(), this->getwalletdat());
    }
    catch(...){
        cerr<<"Error Transfering in OpenCBDC\n";
    }
    
    this->changeBalance(-amount);
    Entity::getObjectfromID(to_id)->changeBalance(amount);
    
    //add ledger entry
    if (message==""){
        LedgerEntry entry = LedgerEntry("now","Transfer money", this->getid(), to_id, amount);
        ledger->addLedgerEntry(entry);
    }
    else{
        LedgerEntry entry = LedgerEntry("now", message, this->getid(), to_id, amount);
        ledger->addLedgerEntry(entry);
    }

    return 1; //sucess
}



// bool Entity::receiveMoney(double value){
//     this->changeBalance(value);
//     return 1;
// }
// bool Entity::sendMoney(double value){
//     if (value > this->getBalance()){
//         cout<<"Error: Amount "<<value<<" is greater than balance "<<this->getBalance()<<'\n';
//         return 0;
//     }
//     this->changeBalance(-value);
//     return 1;
// }
// bool Entity::sellBonds(double value){
//     if (value > this->getBonds()){
//         cout<<"Error: Amount "<<value<<" is greater than bonds "<<this->getBonds()<<'\n';
//         return 0;
//     }
//     this->changeBonds(-value);
//     return 1;
// }
// bool Entity::buyBonds(double value){
//     this->changeBonds(value);
//     return 1;
// }
#pragma endregion

#pragma region CentralBank
    bool CentralBank::generateMoney(double value){   
        //DO it in OpenCBDC
        try{
            hamilton.MINT_NEW_COINS(this->getmempool(), this->getwalletdat(), UTXOS, value);
        }
        catch(...){
            cerr<<"Error minting new coins in OpenCBDC\n";
        }

        //Cen
        this->changeBalance(value);
        this->totalMoneyinCirculation+=value;

        //add ledger entry
        LedgerEntry entry = LedgerEntry("now","Generated money", this->getid(), this->getid(), value);
        ledger->addLedgerEntry(entry);
        return 1;
    }

    bool CentralBank::removeMoney(double value){
        if (value > this->totalMoneyinCirculation){
            cout<<"Error: value "<<value<<" is greater than total money in circulation "<<this->totalMoneyinCirculation<<'\n';
            return 0;
        }

        // QUESTION: HOW TO UNMINT MONEY IN OPENCBDC?

        //Cen
        if (value > this->getBalance()){
            cout<<"Error: "<<value<<" is greater than current money in Central Bank "<<this->getBalance()<<". Gather money from the market\n";
            return 0;
        }
        this->changeBalance(-value);
        this->totalMoneyinCirculation-=value;

        //add ledger entry
        LedgerEntry entry = LedgerEntry("now","Removed money", this->getid(), this->getid(), value);
        ledger->addLedgerEntry(entry);
        return 1;
    }

    bool CentralBank::ForceTransfer(string fromid, string toid, double amount){
        if (!(Entity::checkIDexists(fromid)&&Entity::checkIDexists(toid))){
            cout<<"Error: id not found\n";
            return 0;
        }
        Entity* fromobj = Entity::getObjectfromID(fromid);
        return fromobj->Transfer(amount, toid, "FORCED Transfer");
    }
#pragma endregion

#pragma region CommercialBank

#pragma endregion