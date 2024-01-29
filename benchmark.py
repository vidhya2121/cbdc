import subprocess
import timeit
pk = str(input("Public Key of rx wallet:"))
n = int(input("Number of Tx to run: "))
# dir = (subprocess.check_output(['ls','-la'])).decode()
SETUP_CODE = "import subprocess"
TC = 'subprocess.check_output([\'./build/src/uhs/client/client-cli\', \'2pc-compose.cfg\', \'mempool0.dat\',\' wallet0.dat\',\'send\', \'30\',\''+pk+'\'])'
time = timeit.timeit(setup = SETUP_CODE,
                     stmt = TC,
                     number = n)
print(time)