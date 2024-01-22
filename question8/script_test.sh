#!/bin/bash

#compilation
gcc -o rmg_test rmg.c -lcrypt && clear


#création de suppme pour d'autres essais
touch dir_a/suppme
touch dir_b/suppme
touch dir_c/suppme


#execution sur le fichier "suppme"
./rmg_test dir_a/suppme

