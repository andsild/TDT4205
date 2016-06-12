#!/bin/bash

export LANG=en
# shellcheck disable=SC2002
make && \
    ( clear; (./src/vslc < vsl_programs/globals_and_args.vsl \
    | tee build/output.s) && gcc build/output.s -o build/output \
       && ./build/output 1 2)

    ( clear; ./src/vslc < vsl_programs/prime.vsl \
    | tee build/output.s && as -ggdb  build/output.s -o build/output.o \
       && cc ./build/output.o -o ./build/output && ./build/output 1 2 3 4 5 6 7 8)
    
    
    ( clear; ./src/vslc < vsl_programs/fibonacci_iterative.vsl \
    | tee build/output.s && as -ggdb  build/output.s -o build/output.o \
       && cc ./build/output.o -o ./build/output && ./build/output 20)


    ( clear; ./src/vslc < vsl_programs/fibonacci_recursive.vsl \
    | tee build/output.s && as -ggdb  build/output.s -o build/output.o \
       && cc ./build/output.o -o ./build/output && ./build/output 20)
    
    
    
    
    ( clear; ./src/vslc < vsl_programs/newton.vsl \
    | tee build/output.s && as -ggdb  build/output.s -o build/output.o \
       && cc ./build/output.o -o ./build/output && ./build/output 100)


    ( clear; ./src/vslc < vsl_programs/euclid.vsl \
    | tee build/output.s && as -ggdb  build/output.s -o build/output.o \
       && cc ./build/output.o -o ./build/output && ./build/output 15 12)

