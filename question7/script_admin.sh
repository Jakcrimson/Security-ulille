#!/bin/bash -x
# script de validation des possibilitées d'opérations de l\'admin
# veuillez être onnecté en temps que "admin" pour lancer ce script


# Lecture des dossiers et sous-dossiers de dir_a dir_c
ls -R dir_a			# pass
ls -R dir_b
ls -R dir_c			# pass


# Lire fichier dans dir_c
cat dir_c/a.txt 		# pass

# Création et modification d'un fichier dans dir_a
touch dir_a/oui.txt		# pass
echo "Hoyaho" > dir_a/oui.txt 	# pass
mkdir dir_a/ma_lubulule		# pass

# Création et modification d'un fichier dans dir_b
touch dir_b/oui.txt		# pass
echo "Hoyaho" > dir_b/oui.txt 	# pass
mkdir dir_b/ma_lubulule		# pass

# Modifier des fichiers ou en créer dans dir_c
touch dir_c/supp.txt
echo "oui" > dir_c/supp.txt

# Effacer et renommer des fichiers dans chaque dossier
mv dir_a/oui.txt dir_a/supp.txt
rm dir_a/supp.txt

mv dir_b/oui.txt dir_b/supp.txt
rm dir_b/supp.txt

mv dir_c/supp.txt dir_c/supp2.txt
rm dir_c/supp2.txt

echo "Terminado"
