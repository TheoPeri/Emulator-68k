#!/bin/bash

if [ -z ${M68kPath} ];
    then echo "The path variable is not set (\$M68kPath)";
    exit 1;
else
    echo "Changing directory ..."; fi
    cd $M68kPath
    ./target/build/exec
