#!/bin/bash
#: Title        :checkCorrect
#: Desctiption  : Verify output with .table.correct files
#: Options   	: none

# Since my hash algorithm is different, the ordering
# of the output is different.
# Therefore I compare sorted files instead of regular outputs

diffsmooth="--side-by-side --suppress-common-lines --ignore-trailing-space --ignore-space-change --ignore-all-space"

shopt -s globstar
suffix=".table.correct"
for file in **/*.vsl 
do
    ./src/vslc < "${file}" 2>/dev/null | sort  > "${file}.txt"
    # file.txt now contains sorted output
    correctInFile="${file%%.*}${suffix}"
    sortedCorrectOutfile="${correctInFile}.sorted"
    sort "${correctInFile}" > "${sortedCorrectOutfile}"
    # sortedCorrectOutfile now contains sorted output from lecturer
    
    # compare the two files
    eval diff "${diffsmooth}" "${file}.txt" "${sortedCorrectOutfile}" || echo diff found for file "${file}"
    
    #cleanup
    rm  "${file}.txt" "${sortedCorrectOutfile}"
done
