# dcc888-2022s1

## Assignment 1

### Instructions

1. Check no folder named Ufmg inside `llvm-project/llvm/lib/Transforms`

```bash
$ Transforms> echo "add_subdirectory(Ufmg)" >> ../CMakeLists.txt
```

2. Unpack zip or git clone repo inside above mentioned directory
3. Execute

```bash
$ Transfomrs> cd XYZ/llvm-project/build
$ build> cmake -G "Ninja" ../llvm \
-DBUILD_SHARED_LIBS=on \
-DLLVM_ENABLE_PROJECTS=clang
$ build> cmake --build .
$ build> bin/opt -load lib/LLVMUfmg.so -ufmg -enable-new-pm=0 < "<FILENAME.bc>" > /dev/null
```

### OPTIONAL AUTO PDF GENERATE: ENSURE XPDF installed or change to your pdf viewer inside opt_run.sh

4. Execute the commends the commands, this will link the opt runner shell scripts inside the llvm-project build and run the tests:

```bash
$ build> cd XYZ/llvm-project/llvm/lib/Transforms/Ufmg
$ Ufmg> mkdir -p XYZ/llvm-project/build/udot/{bc,pdf,tmps}
$ Ufmg> cp -r src XYZ/llvm-project/build/udot/
$ Ufmg> ln opt_run.sh XYZ/llvm-project/build/udot/opt_run.sh
$ Ufmg> ln opt_test.sh XYZ/llvm-project/build/udot/opt_test.sh
$ Ufmg> cd XYZ/llvm-project/build/udot
$ udot> ./opt_run.sh main.c
$ udot> ./opt_test.sh
```