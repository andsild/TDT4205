#!/bin/bash

# Verify output with .tree.correct files

shopt -s globstar
for file in **/*.vsl 
do
    cat "${file}" | ./src/vslc > "${file}.txt"
    diff "${file}.txt" "${file%%.*}.tree.correct" || echo diff found for file ${file}
    rm  "${file}.txt" 
done
