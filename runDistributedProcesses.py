import os

# DESCRIPTION: Run these 3 processes in the background sequentially (since they depend on each other). Then write wallet commands directly in shell (no need to pipe)
# depend_dir = "opencbdc-tx/"

RUN_SHARD0 = "./build/src/uhs/twophase/locking_shard/locking-shardd 2pc-compose.cfg 0 0 | echo Shards Running"

RUN_COORD0 = "./build/src/uhs/twophase/coordinator/coordinatord 2pc-compose.cfg 0 0"

RUN_SENTINEL0 = "./build/src/uhs/twophase/sentinel_2pc/sentineld-2pc 2pc-compose.cfg 0"
# os.system("cd "+depend_dir) #Problem: returns to this file's directory after command ends
# os.chdir(depend_dir)
# os.system("pwd")

#Testing
print("Starting shard...")
os.system(RUN_SHARD0)
# print("Starting coordinator...")
# os.system(RUN_COORD0)
# print("Starting sentinel...")
# os.system(RUN_SENTINEL0)

