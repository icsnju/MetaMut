<p align="center">
  <h1 align="center"><strong>MetaMut</strong></h1>
</p>

<p align="center">
    <a href="https://semver.org" alt="Version">
        <img src="https://img.shields.io/badge/release-v0.1.0-blue" />
    </a>
    <a href="https://google.github.io/styleguide/javaguide.html" alt="Code style">
        <img src="https://img.shields.io/badge/style-Google-blue" />
    </a>
    <a href="https://opensource.org/licenses/MIT" alt="License">
        <img src="https://img.shields.io/github/license/test-compiler/MetaMut" />
    </a>
</p>

MetaMut is a generator to generate semantic aware mutators for testing C/C++ compilers.

Our project has already generated over 200 semantic aware mutators. Leveraging these mutators, we have successfully discovered over 130 bugs in two extensively tested modern C/C++ compilers, namely GCC and Clang.

**GCC/Clang Bugs**: We have reported over 130 bugs to GCC/Clang. A comprehensive list of these reported bugs is available [here](bugs.md).

## Run This Project

### Installation

To get started, you can pull our Docker image from Docker Hub:
```
docker pull metamut/metamut:latest
docker run -it metamut /bin/bash
```

*note*: If you prefer to compile and execute directly on your native machine, follow instructions [here](README.ubuntu.md).

### Execute MetaMut

To run MetaMut within the Docker container, use the following commands:
```
cd /root/MetaMut/Core;
# fill in openai key in Core/scripts/configs.py
python3 scripts/main.py --num-mutators=100
```

The mutators will be generated in the `Core/lib/mutators` subdirectory.

### Generated Mutators

You can find the generated mutators as binaries in the Docker image:

* Supervised Version: `/usr/bin/mu.s`
* Unsupervised Version: `/usr/bin/mu.u`
* Fused Version: `/usr/bin/muss`

List all supported mutators:
```
$ muss --list-mutators
[
  "AddBitwiseOperator",
  "AddIntegerArray",
  "AddNestedLoop",
  "AddRandomAssignment",
  "AddRandomConditionalExpr",
  ...
]
```

Execute a specific mutator:
```
$ cat /root/MetaMut/seeds/28507.c
extern void foo (int);

void bar (unsigned long l)
{
    foo(l == 0);
}

$ muss -i /root/MetaMut/seeds/28507.c -mutator s.duplicate-stmt -o -
extern void foo (int);

void bar (unsigned long l)
{
    foo(l == 0);foo(l == 0);
}
```

Alternatively, run all mutators until a successful mutation occurs:
```
$ muss --randomly-try-all-mutators -i /root/MetaMut/seeds/28507.c -o - -seed 123
extern void foo (int);

void bar (unsigned long l)
{
    foo(l == 0 & 0);
}
```

### Fuzz Compilers

To run the fuzzer:
```
cd /root/MetaMut; mkdir -p workspace; cd workspace
python3 ../fuzzer/run.py -j 4 \
  --repeat-times 10 \
  --duration 86400 \
  --seeds-dir $(pwd)/../seeds \
  --cc-opt=-O2 \
  --wdir $(pwd)
```

*note*: You may use Docker's -v option to map a local directory to a directory inside the Docker container (`-v local_dir:docker_dir`), allowing the fuzzer's output to be stored on your local machine.

Review the fuzzer's results with:
```
cd /root/MetaMut
python3 ../fuzzer/show.py * --summary
```
