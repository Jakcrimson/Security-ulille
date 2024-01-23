#!/bin/bash

#création des contenus observables
touch dir_a/file1
touch dir_a/file2
touch dir_a/file3
touch dir_b/file1
touch dir_b/file2
touch dir_b/file3
touch dir_c/file1
touch dir_c/file2
touch dir_c/file3

echo "this line is inside the file" > dir_a/file1
echo "this line is inside the file" > dir_b/file1
echo "this line is inside the file" > dir_c/file1


#compilation de notre script
gcc -o server script_francois_server.c

#execution
./server
