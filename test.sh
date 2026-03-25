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

make  > /dev/null 2>&1

kill $LOADER_PID
echo ""

./run_tests

make clean
