#!/usr/bin/env bash

# shellcheck disable=SC2002
make && \
    # ( clear;  ./src/vslc < ./vsl_programs/custom.vsl )
    # ( clear;  ./src/vslc < ./vsl_programs/prec.vsl )
    # ( clear;  ./src/vslc < ./vsl_programs/simplify.vsl )
    # ( clear;  ./src/vslc < ./vsl_programs/euclid.vsl )
    ( clear;  ./src/vslc < ./vsl_programs/nested_scopes.vsl )
    # ( clear;  ./src/vslc < ./vsl_programs/prime.vsl )
