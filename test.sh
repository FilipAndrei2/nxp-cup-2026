#! /bin/bash

cd ./test

echo "Compiling tests. Please wait."

(
    while true; do
        echo -n "."
        sleep 1
    done
) &
LOADER_PID=$!

make -j$(nproc 2>/dev/null || echo 1)  > /dev/null 

kill $LOADER_PID
echo ""

./run_tests

make clean
