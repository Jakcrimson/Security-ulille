#!/bin/bash -x 
# Script vérifiant l'intégrité des utilisateurs du groupe a


# Lecture des dossiers et sous-dossiers de dir_a dir_c
ls -R dir_b			                # pass
ls -R dir_c			                # pass


# Lire fichier dans dir_c
cat dir_c/a.txt 		            # pass

# Modifier fichier dans dir_c
echo "Hoyaho" > dir_c/a.txt	        # denied
mv dir_c/a.txt dir_c/b.txt	        # denied
rm dir_c/a.txt			            # denied
touch dir_c/b.txt		            # denied

# Création et modification d'un fichier dans dir_a
touch dir_b/b.txt		            # pass
echo "Hoyaho" > dir_b/b.txt 	    # pass
mkdir dir_b/ma_lubulule		        # pass

# Effacer 
