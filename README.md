# OpenCBDC extension
 Implementation of a CBDC system simulating a virtual economy on top of MIT's OpenCBDC project (from project Hamilton).

To check code run `clang-tidy *.cpp -- -std=c++17`

## Requirements:
This project is an extension of the [OpenCBDC project](https://github.com/mit-dci/opencbdc-tx) on commit `72b95a4` (latest tested commit that works) and depends on its docker container. Therefore, you need to set it up on your machine before running this extension.

Put the `opencbdc-tx` folder inside this folder and setup the docker steps.

### Usage
#### Case: versions (Recommended)
Choose the version you want to run in the file `runversions.py` by setting the variable `selected= *version*`.
Then run `python runversions.py` in shell.

#### Case: individual files
Compile using `g++ classes.cpp *filename*.cpp -o main`
OR `cmake . | make`
Run `./main`

### Testing pipe - mypipe file
Put this CBDC_F22 folder inside opencbdc-tx

Modify Dockerfile in opencbdc-tx to include RUN CBDC_F22/configure.sh after COPY . .

Then build opencbdc-tx - docker compose

Run docker container

Run ./CBDC_F22/mypipe inside the docker container.

### Testing invokeHamilton code
Put this CBDC_F22 folder inside opencbdc-tx

from within opencbdc-tx folder, run the following command

For building - ./CBDC_F22/configure.sh

For run - ./CBDC_F22/invokeHamilton
