#!/bin/bash

#compilation
gcc -o pwg_test pwg.c -lcrypt 

# affichage de pwd
echo "affichage avant"
cat passwd

# execution 
./pwg_test

# affichage après 
echo "affichage après"
cat passwd