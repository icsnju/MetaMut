import os
import pathlib

####### CONFIGS ######
homedir = pathlib.Path(__file__).parent.parent

muss_homedir = f"{homedir}/mutators"
muss_bin = f"{muss_homedir}/output/muss"

seeds_dir = f"{homedir}/seeds"

gcc_objdir = f"{homedir}/objects/gcc-project-trunk.obj"
gcc_bin = f"{gcc_objdir}/install/bin/gcc"

llvm_objdir = f"{homedir}/objects/llvm-project-trunk.obj"
clang_bin = f"{llvm_objdir}/bin/clang"

instrum_gcc_objdir = f"{homedir}/objects/gcc-project-trunk.instrum"
instrum_gcc_bin = f"{instrum_gcc_objdir}/install/bin/gcc"
instrum_gcc_nbrs = 1478061

instrum_llvm_objdir = f"{homedir}/objects/llvm-project-trunk.instrum"
instrum_clang_bin = f"{instrum_llvm_objdir}/bin/clang"
instrum_clang_nbrs = 4915140

cc_map = {
    gcc_bin: instrum_gcc_bin,
    clang_bin: instrum_clang_bin,
    instrum_gcc_bin: gcc_bin,
    instrum_clang_bin: clang_bin,
}

max_frames = 50
