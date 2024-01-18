#!/bin/bash -x 
# Script vérifiant l'intégrité des utilisateurs du groupe a


# Lecture des dossiers et sous-dossiers de dir_a dir_c
ls -R dir_a			# pass
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

# Création et modification d'un fichier dans dir_a
touch dir_a/b.txt		# pass
echo "pass"
echo "Hoyaho" > dir_a/b.txt 	# pass
echo "pass"
mkdir dir_a/ma_lubulule		# pass
echo "pass"
rmdir dir_a/ma_lubulule 	# pass
echo "test" > dir_a/admin_file  # pass
echo "pass"

# Modification / supression de fichier qui n'appartiennent pas à lambda_a
mv dir_a/admin_file dir_a/test_file 	# denied
echo "denied"
rm dir_a/admin_file  			# denied
echo "denied"


# Modification / Supression / Lecture / Creation de fichiers dans dir_b
mv dir_b/a.txt dir_b/b.txt 		#denied
echo "denied"
rm dir_b/a.txt 				#denied
echo "denied"
cat dir_b/a.txt 			#denied
echo "denied"
touch dir_b/test.txt			#denied
echo "denied"
cd dir_b/ma_lubulule			#denied
echo "denied"



echo "Terminado"
