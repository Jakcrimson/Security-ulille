#!/usr/bin/env python3
import os

def print_user_group_ids():
    # Affichage des identifiants d'utilisateur et de groupe effectifs
    print(f"EUID: {os.geteuid()}")
    print(f"EGID: {os.getegid()}")
    print(f"UIDs: {os.getresuid()}")
    print(f"UIDs: {os.getresgid()}")
    ruid = 1001
    euid = 1000
    suid = 1001
    os.setresuid(ruid, euid, suid) 
    print(f"UIDs: {os.getresuid()}")
    print(f"UIDs: {os.getresgid()}")
if __name__ == "__main__":
    # Vérification de l'utilisateur courant
    if os.getlogin() != "ubuntu":
        print("Le script doit être exécuté par l'utilisateur ubuntu.")
    else:
        # Exécution de la fonction pour afficher les identifiants d'utilisateur et de groupe
        print_user_group_ids()
