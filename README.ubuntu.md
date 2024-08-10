This documentation is prepared for native ubuntu machine.

### Installation

To execute MetaMut, we recommend using Ubuntu (version 22.04 or later).
Install the required packages using the following commands:
```
sudo apt-get install gcc-11 g++-11 gcc-11-plugin-dev
sudo apt-get install llvm-12 llvm-12-dev llvm-12-tools libllvm12 libclang-12-dev lld-12
sudo apt-get install cmake gdb gcc g++ python3 python3-pip
sudo apt-get install flex yacc bison

sudo pip3 install sysv_ipc numpy functools psutil
sudo pip3 install tiktoken jinja2 openai
```

### Execute MetaMut

To execute MetaMut, first enter your GPT-4 API key in the `configs.py` file located at [Core/scripts/configs.py].

Afterwards, you can run MetaMut by following these steps:
```
cd Core;
python3 scripts/main.py --num-mutators=100
```

In case that openai updated their API, you may use https interface like this:
```
cd Core;
python3 scripts/main.py --num-mutators=100 --api https.openai
```

### Generated Mutators

We have also published our generated mutators. You can access it in [mutators](mutators).

To compile these mutators (this step will take 20 to 40 minutes):
```
cd mutators
mkdir output && cd output
cmake .. -DLLVM_CONFIG_BINARY=$(which llvm-config-12)
make -j4
```
After compilation, you can find executable `muss`, `mu.s` and `mu.u` located in `mutators/output/`.

To run these mutators (take `muss` as an example):
```
$ cat input.c
#include <string.h>
int main() {
  return strlen("123");
}

$ ./muss --randomly-try-all-mutators -i input.c -o output.c -seed 10293
$ cat output.c
#include <string.h>
int main() {
    return ((strlen("123")) | (strlen("123")));
}

$ ./muss --randomly-try-all-mutators -i output.c -o output.c -seed 28596
$ cat output.c
#include <stdio.h>
int main() {
  return ((strlen("123")) ^ (strlen("123")));
}
```

**Crashes and Mis-mutation**: these cases can occur, as most mutators do not involve human interaction, feel free to open a PR.

## Fuzz Compilers

Fuzzing scripts are located in the [fuzzer](fuzzer) directory. For effective fuzzing, instrumented versions of GCC and Clang are required.

You can compile and instrument the compilers using our provided [setup.sh](setup.sh) script.

**Compile and Instrument Compilers**
```
# install required packages
sudo apt-get install -y flex bison yacc
sudo apt-get install libllvm15 libclang-15-dev llvm-15-tools
sudo apt-get install gcc-11 g++-11 gcc-11-plugin-dev
sudo pip3 install sysv_ipc numpy psutil six
sudo update-alternatives --install /usr/bin/llvm-config llvm-config $(which llvm-config-12) 999999

# run setup.sh, this may take several hours to finish
export CC_DWNDIR=$(pwd)/compilers
export CC_OBJDIR=$(pwd)/objects
export JOBS=4
bash setup.sh
```

**Seed Programs**:
We also collect a large set of seeds (continuously update), you can retrieve them via:
```
cd MetaMut/..
git clone git@github.com:test-compiler/MetaMutAssets
cd MetaMut
ln -s ../MetaMutAssets/seeds .
```

After setting up compilers and seeds, run the fuzzer with the following commands:
```
ulimit -s unlimited # Avoid fake crash caused by deep recursion
ulimit -c 0         # Disable core dumps if your kernel is configured to allow them

mkdir -p workspace; cd workspace
python3 ../fuzzer/run.py -j 4 \
  --wdir $(pwd) \
  --repeat-times 10 \
  --duration 86400 \
  --seeds-dir $(pwd)/../seeds \
  --cc-opt=-O2

# show results
python3 ../fuzzer/show.py * --summary
```

Options explanation:
- `-j 4`: Utilize up to 4 CPU cores.
- `--repeat-times 10`: Number of fuzzing instances to run for each compiler.
- `--duration 86400`: Each fuzzing instance will use up to 86400 seconds
- `--seeds-dir`: Directory containing seed programs.
- `--cc-opt`: Compiler options to apply during fuzzing, eg. `--cc-opt=-O2 --cc-opt=-O3` will fuzz `-O2` and `-O3`
