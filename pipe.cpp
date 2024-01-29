#include<unistd.h> //for fork and pipe
#include<sys/wait.h> //for waitpid, kill, sigterm
#include<iostream>
using std::cout, std::cin, std::getline, std::cerr, std::string, std::to_string;
#include<string>
#include<vector>
using std::vector;
#include <fcntl.h> //for files

/*DESCRIPTION: 
    This code contains the logic needed to instantiate the Docker container of OpenCBDC
    and pipe strings from our customCLI "commandParser.cpp" to the Docker container.
    For now the pipe implemented between 2 C++ processes is just sending and recieving strings
*/


#pragma region  OpenCBDC COMMANDS:
const char* RUNCONTAINER = "docker compose --file docker-compose-2pc.yml --file docker-compose-prebuilt-2pc.yml up --no-build";

int numwallets=0;
string MINT_NEW_COINS(int UTXOs, int atomic_value, string wallet="wallet0.dat") {
    return "./build/src/uhs/client/client-cli 2pc-compose.cfg mempool0.dat "+wallet+" mint "+to_string(UTXOs)+" "+to_string(atomic_value)+"\n";
}
string GET_BALANCE(string wallet="wallet0.dat") {
    return "./build/src/uhs/client/client-cli 2pc-compose.cfg mempool0.dat "+wallet+" info\n";
}
string CREATE_WALLET() {
    numwallets++;
    string wallet = "wallet"+to_string(numwallets)+".dat";
    return "./build/src/uhs/client/client-cli 2pc-compose.cfg mempool1.dat "+wallet+" newaddress\n";
}
#pragma endregion

#pragma region Helper Funcs
void SendString(int mypipe[], string req){ //sends 2 things. FIRST numchars THEN string content
    req[req.size()] ='\n'; //transforming string to "command"
    // write(mypipe[1], req.c_str(), req.length()+1); //NOTE: MUST CONVERT STRING TO Cstring TO SEND TO PIPE

    //send string length
    int numchars = req.size()+1;
    write(mypipe[1], &numchars, sizeof(int));

    //send content
    write(mypipe[1], req.c_str(), numchars);  //NOTE: MUST CONVERT STRING TO Cstring TO SEND TO PIPE
    cout<<"Sending: "<<req<<' '<<req.size()<<'\n';
}

string ListenString(int mypipe[], int numchars){ //Here ONLY get string content, based on numchars read before calling this function
    string resp=""; //NOTE: not needed since can use either string or char array
    //get content
    char cs[numchars];
    read(mypipe[0], cs, numchars);   
    // printf("Read from printf: %s\n", cs); //prints entire array

    //Converting char array to string (Concatenate string). NOT NECESSARY IF GONNA USE char array
    for(int i=0; i<numchars; i++){
        resp += cs[i];// resp.push_back(c);
    }
    // cout<<resp<<'\n';
    return resp;
}
#pragma endregion

int main(){
    //Create pipes. First is to send input string from parent (this driver) to OpenCBDC docker.
    // CtoP[0] is READ end of the pipe
    // CtoP[1] is WRITE end of the pipe
    int ParsertoOpenCBDC[2];

    if (pipe(ParsertoOpenCBDC)==-1) {
        fprintf(stderr, "Pipe Failed" );
        return 1;
    }

    vector<pid_t> kids;

    auto pid1 = fork();
    
    if (pid1 == 0) { //child is OpenCBDC Docker containers
        //close unused pipes
        close(ParsertoOpenCBDC[1]); //only reading not writing (docker outputs to stdout)
        // cout<<"Hello from child\n";
        
        //Testing
        execlp("python3", "python3", "runTransactionDocker.py", NULL); //Need to write "python3" twice since it is argv[0] in OS
        close(ParsertoOpenCBDC[0]); //close reading end after use


        // auto pid2 = fork();

        // if (pid2==0){ //Transaction Processor Docker
        //     close(ParsertoOpenCBDC[0]); //close reading end since not used
        //     // cout<<"Hello from child 2 inside child 1\n";

        //     execlp("python3", "python3", "runTransactionDocker.py", NULL); //Need to write "python3" twice since it is argv[0] in OS
        // }
        // else if (pid2>0){ //Interface Docker
        //     kids.push_back(pid2);
        //     //redirecting pipe to stdin (read)
        //     dup2(ParsertoOpenCBDC[0], STDIN_FILENO);
        //     execlp("python3", "python3", "runInterfaceDocker.py", NULL); //Need to write "python3" twice since it is argv[0] in OS

        //     // //Child listens and repeats what it gets:
        //     // int numchars;
        //     // //NOTE: read pauses until something is written to pipe
        //     // while(read(ParsertoOpenCBDC[0], &numchars, sizeof(int)) > 0){ //On every iteration of writing, first sends numchars then content, so while still receiving numchars it means that there is still content to be read so continue
        //     //     string received = ListenString(ParsertoOpenCBDC, numchars);
        //     //     cout<<"Received: "<<received;//<<'\n';
        //     // }
        //     close(ParsertoOpenCBDC[0]); //close reading end after use
        // }
        // else
        // {
        //     cerr << "Error: fork of pid2 failed!\n";
        //     return 1;
        // } 
    }
    
    else if (pid1 > 0) { //parent is Parser (user-cli)
        kids.push_back(pid1);
        //close unused pipes
        close(ParsertoOpenCBDC[0]); //only writing not reading (docker outputs to stdout)
        cout<<"Hello from parent\n";

        //creating file to register stdout from docker (wallet info)
        int file = open("output.txt", O_WRONLY | O_CREAT, 0777);
        if (file==-1){
            cerr<<"Error: Unable to create file "<<"output.txt"<<"\n";
            return 1;
        }

        //redirecting STDOUT to file
        dup2(file, STDOUT_FILENO);
        // cout<<"Testing that STDOUT has been successfully redirected to file created\n";

        //Testing
        execlp("python3", "python3", "runInterfaceDocker.py", NULL); //Need to write "python3" twice since it is argv[0] in OS

        // //Talk to child
        // string req;
        // while(getline(cin, req)){ //while !std::cin.eof()
        //     // cout<<req<<'\n';
        //     if(req=="q")
        //         break;

        //     SendString(ParsertoOpenCBDC, req); //Send request
        //     fflush(stdin); //flushing remains of old input so it doesn't get buffered in new input
        //     cin.sync();
        // }

        close(ParsertoOpenCBDC[1]); //close writing end after use
    }
    else
    {
        cerr << "Error: fork of pid1 failed!\n";
        return 1;
    } 

    // cout<<MINT_NEW_COINS(3,4)<<GET_BALANCE()<<CREATE_WALLET()<<CREATE_WALLET();

    // send kill signal to all children
    for (pid_t k : kids) { //IMPORTANT OTHERWISE AFTER CTRL+D CHILD P2 INSIDE C1 STILL RUNNING
        // cout<<"killing "<<k<<endl;
        int status;
        kill(k, SIGTERM);
        waitpid(k, &status, 0);
    }

    return 0;
}