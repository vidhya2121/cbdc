import os

# DESCRIPTION: This code is called in "pipe.cpp" to run the OpenCBDC Interface docker image to communicate with the Transaction processor.
# Our custom-CLI commands will be piped to this interface 
# So make sure to run the transaction processor first

# depend_dir = "opencbdc-tx/"
RUN_Cli_CONTAINER = "docker run --network 2pc-network -ti ghcr.io/mit-dci/opencbdc-tx-twophase /bin/bash"

# os.system("cd "+depend_dir) #Problem: returns to this file's directory after command ends
# os.chdir(depend_dir)
# os.system("pwd")

#Testing
os.system(RUN_Cli_CONTAINER)

