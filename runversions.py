import os # subprocess.run doesn't work for OS based commands

# DESCRIPTION: This code is used by Developers to test individual functionalities of this system.

def runParser():
    print('running parser')
    os.system("g++ -std=c++17 commandParser.cpp classes.cpp invokeHamilton.cpp -o main")
    os.system("./main")

def runPrototype():
    print('running prototype')
    os.system("g++ -std=c++17 prototype.cpp classes.cpp invokeHamilton.cpp -o main")
    os.system("./main")

# def runPipe(): #NOT USED FOR NOW
#     print('running pipe')
#     os.system("g++ pipe.cpp -o main")
#     os.system("./main")


versions = {
    0: runParser,
    1: runPrototype,
    # 2: runPipe,
}


# Choose what version of the program you want to run by uncommentting ONLY ONE Function below
selected=1 # ONLY CHANGE THIS NUMBER, NOTHING ELSE

versions[selected]()