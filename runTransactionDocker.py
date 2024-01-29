import os

# DESCRIPTION: This code is called in "pipe.cpp" to run the OpenCBDC Transaction docker as a process (to start the economy backbone).

depend_dir = "opencbdc-tx/"
RUN_2PC_CONTAINER = "docker compose --file docker-compose-2pc.yml --file docker-compose-prebuilt-2pc.yml up --no-build"

# os.system("cd "+depend_dir) #Problem: returns to this file's directory after command ends
os.chdir(depend_dir)
# os.system("pwd")

#Testing
os.system(RUN_2PC_CONTAINER)
