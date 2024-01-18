#!/bin/bash -x 
# Script vérifiant l'intégrité des utilisateurs du groupe a


# Lecture des dossiers et sous-dossiers de dir_b dir_c
ls -R dir_b			# pass
echo "pass"
ls -R dir_c			# pass
echo "pass"

# Lire fichier dans dir_c
cat dir_c/u.txt 		# pass
echo "pass"

# Modifier fichier dans dir_c
echo "Hoyaho" > dir_c/u.txt	# denied
echo "denied"
mv dir_c/u.txt dir_c/b.txt	# denied
echo "denied"
rm dir_c/u.txt		# denied
echo "denied"
touch dir_c/b.txt		# denied
echo "denied"

# Création et modification d'un fichier dans dir_b
touch dir_b/b.txt		# pass
echo "pass"
echo "Hoyaho" > dir_b/b.txt 	# pass
echo "pass"
mkdir dir_b/ma_lubulule		# pass
echo "pass"
rmdir dir_b/ma_lubulule 	# pass
echo "test" > dir_b/admin_file  # pass
echo "pass"

# Modification / supression de fichier qui n'appartiennent pas à lambda_a
mv dir_b/admin_file dir_b/test_file 	# denied
echo "denied"
rm dir_b/admin_file  			# denied
echo "denied"


# Modification / Supression / Lecture / Creation de fichiers dans dir_a
mv dir_a/a.txt dir_a/b.txt 		#denied
echo "denied"
rm dir_a/a.txt 				#denied
echo "denied"
cat dir_a/a.txt 			#denied
echo "denied"
touch dir_a/test.txt			#denied
echo "denied"
cd dir_a/ma_lubulule			#denied
echo "denied"



echo "Terminado"
