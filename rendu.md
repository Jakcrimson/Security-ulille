# Rendu "Les droits d’accès dans les systèmes UNIX"

## Binome

- Nom, Prénom, email: LAGUE, Pierre, pierre.lague.etu@univ-lille.fr

- Nom, Prénom, email: MULLER, François, francois.muller.etu@univ-lille.fr

## Question 1

On ne peux pas écrire le fichier car on ne dispose pas des droits d'écriture pour l'utilisateur actuel (-r--rw-r--). Ainsi, quand on essaye de sauvegarder le buffer d'écriture du fichier, on a un message de "Permission Denied".

## Question 2

La modification de permission '-x' pour un dossier signifie qu'on enlève la possibilité pour un utilisateur non sudoer et n'appartenant pas au groupe de pouvoir rentrer dans ce dossier.
En executant les operations suivantes :
```bash
>su ubuntu
>cd ~ 
>mkdir mydir
>chmod g-x mydir # on enlève les droits d'éxécution du dossier
>ls -al # on remarque que le x du groupe est parti
>cd mydir # on peut rentrer dans le dossier car pour l'utilisateur actuel, il y a toujours le droit x.
```

Quand on utilise l'utilisateur `toto`, il n'est pas possile d'entrer dans le repertoire `mydir`. Les membres du groupe auront ce message :

```bash
bash: cd: mydir: Permission denied
```

## Question 3

Notre script est le suivant :
 
```c
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>

int main() {
    // Affichage des identifiants d'utilisateur et de groupe
    printf("EUID: %d\n", geteuid());
    printf("EGID: %d\n", getegid());
    printf("RUID: %d\n", getuid());
    printf("RGID: %d\n", getgid());

    // Lecture du fichier mydir/mydata.txt
    const char *filename = "mydir/mydata.txt";
    int fd = open(filename, O_RDONLY);

    if (fd == -1) {
        perror("Erreur lors de l'ouverture du fichier");
        exit(EXIT_FAILURE);
    }

    /*
    Code associé au setup du tampon de lecture
    ... (cf suid.c)
    */

    // Lecture du contenu du fichier
    ssize_t bytesRead = read(fd, buffer, st.st_size);
    if (bytesRead == -1) {
        perror("Erreur lors de la lecture du fichier");
        free(buffer);
        close(fd);
        exit(EXIT_FAILURE);
    }

    // Affichage du contenu du fichier
    printf("\nContenu du fichier %s :\n", filename);
    write(STDOUT_FILENO, buffer, bytesRead);

    // Libération des ressources
    free(buffer);
    close(fd);

    return 0;
}
```

- Quand on execute le script avec l'utilsateur ubuntu, on obtient l'output suivant :
```
ubuntu@vm1:~$ ./a.out 
EUID: 1000
EGID: 1000
RUID: 1000
RGID: 1000

Contenu du fichier mydir/mydata.txt :
Hello World
``` 
Car le fichier est en droit de lecture pour l'utilisateur ubuntu et groupe ubuntu.

- Quand on execute le script avec l'utilisateur toto, on obtient l'output suivant :
```
toto@vm1:/home/ubuntu$ ./a.out 
EUID: 1001
EGID: 1001
RUID: 1001
RGID: 1001

Contenu du fichier mydir/mydata.txt :
Hello World
```
On remarque que les processus ID sont différents et que le contenu du fichier peut toujours être lu.
`mydir` et `mydata` sont en permission de lecture et d'éxécution pour le groupe `ubuntu`.


Quand on fait appel au flag `set-user-id`, il est possible qu'un utilisateur du groupe `ubuntu` puise lire le contenu du fichier `mydata.txt` même s'il n'est pas en mode executable pour les groupes.

```bash
chmod u+s mydata.txt  
ls -l
#out
-rwxrwxr-x 1 ubuntu ubuntu 16568 Jan 10 15:40 a.out
-rwSrw-r-- 1 ubuntu ubuntu    18 Jan 10 16:10 mydata.txt
-rw-rw-r-- 1 ubuntu ubuntu  1507 Jan 10 15:38 print_ids.c

root@vm1:/home/ubuntu# su toto
toto@vm1:/home/ubuntu$ ./a.out 
EUID: 1001
EGID: 1001
RUID: 1001
RGID: 1001

Contenu du fichier mydir/mydata.txt :
Hello World !!!!!
```
On retrouve les processus de l'utilisateur `toto`.


## Question 4

Notre script est le suivant :

```python
#!/usr/bin/env python3
import os

def print_user_group_ids():
    # Affichage des identifiants d'utilisateur et de groupe effectifs
    print(f"EUID: {os.geteuid()}")
    print(f"EGID: {os.getegid()}")

if __name__ == "__main__":
    # Exécution de la fonction pour afficher les identifiants d'utilisateur et de groupe
    print_user_group_ids()
```
On assigne le `set-user-id` au fichier python, et les permissions suivantes : `-rwsrw-r-- 1 ubuntu ubuntu   519 Jan 10 16:33 suuid.py`.

```bash
toto@vm1:/home/ubuntu$ ls -l 
total 28
-rwxrwxr-x 1 ubuntu ubuntu 16568 Jan 10 15:40 a.out
drwxrwxr-x 2 ubuntu ubuntu  4096 Jan 10 16:10 mydir
-rwsrw-r-- 1 ubuntu ubuntu   519 Jan 10 16:33 suuid.py
toto@vm1:/home/ubuntu$ python3 suuid.py 
EUID: 1001
EGID: 1001
```
Grace au `set-user-id`, l'utilisateur `toto` est en mesure d'éxécuter le script. On retrouve bien les ids de l'utilisateur `toto`.

Le flag set-user-id (setuid) est un mécanisme de sécurité dans les systèmes d'exploitation Unix/Linux. Cela permet à un utilisateur d'exécuter un programme avec des privilèges supérieurs à ceux qu'il possède normalement.

L'utilité du flag setuid est souvent associée à des situations où un programme nécessite des privilèges élevés pour effectuer certaines opérations, mais l'utilisateur normal ne devrait pas avoir ces privilèges en permanence.

En ce qui concerne la modification d'attributs sans demander à l'administrateur, cela peut être réalisé si l'administrateur a spécifiquement prévu cela dans la configuration du système. Par exemple, il pourrait y avoir un programme avec setuid qui permet à l'utilisateur de modifier certaines de ses propres informations dans le fichier `/etc/passwd` sans nécessiter les privilèges d'administration complets. 

Par exemple, si on essaye de modifier les `suid` dans un script python éxécuté par `ubuntu` sans les droits admin, on obtient l'output suivant :

```bash
ubuntu@vm1:~$ python3 suuid.py 
EUID: 1000
EGID: 1000
UIDs: (1000, 1000, 1000)
UIDs: (1000, 1000, 1000) #modification après affichage des ids
Traceback (most recent call last):
  File "/home/ubuntu/suuid.py", line 22, in <module>
    print_user_group_ids()
  File "/home/ubuntu/suuid.py", line 13, in print_user_group_ids
    os.setresuid(ruid, euid, suid) 
PermissionError: [Errno 1] Operation not permitted
```

```python
#le code en question :
ruid = 1001
euid = 1000
suid = 1001
os.setresuid(ruid, euid, suid) 
print(f"UIDs: {os.getresuid()}")
print(f"UIDs: {os.getresgid()}")
```


## Question 5

On obtient le résultat suivant :
```bash
toto:x:1001:1001::/home/toto:/bin/bash
```
de la forme `Nom:Mot de passe: UserID:PrincipleGroup:Gecos: HomeDirectory:Shell`

```bash
>ls -al /usr/bin/chfn
-rwsr-xr-x 1 root root 72712 Nov 24  2022 /usr/bin/chfn
```
On remarque que le `set-user-id` est activé, cela signifie que les utilisateurs n'ayant pas les super droits peuvent éxécuter la commande `chfn` afin de modifier leurs informations.

---

De manière naturelle, `chfn` éxécutée avec root permet de changer des informations sur un utilisateur (real name, username, information etc.).

Si on essaye de lancer la commande `chfn` avec l'utilisateur `toto`, l'utilisateur doit entrer un mot de passe, vraisemblablement, celui de l'utilisateur (en l'occurence `root`). Une fois rentré, nous pouvons modifier les informations de l'utilisateur `toto`.

```bash
toto@vm1:/home/ubuntu$ chfn
Password: 
Changing the user information for toto
Enter the new value, or press ENTER for the default
	Full Name: 
	Room Number []: 45
	Work Phone []: 06123485678
	Home Phone []: 031234678

```

Quand on vérifie le contenu de etc/passwd on remarque que les informations sont bien mise à jour :

```bash
toto:x:1001:1001:,45,06123485678,031234678:/home/toto:/bin/bash
```

## Question 6

 Le fichier `/etc/shadow` contient les mots de passe et est utilisé pour renforcer le niveau de sécurité des mots de passe en limitant l'accès aux données de mots de passe hachés à des utilisateurs ayant les super droits.


## Question 7

Pour la mise en place de la structure, on décide de créer un groupe en plus qui contient le `groupe_a` et le `groupe_b` afin de ne pas à avoir à utiliser les Access Control List.

Entre autres nous avons utilisé les fonctions suivantes :

- `groupadd` : pour la création de groupe
- `adduser` : pour la création d'utilisateurs
- `chgrp` : pour l'assignation de groupes à un utilisateur
- `chown` : pour le changement de proriétaire d'une entité (fichier, dossier)
- `chmod` : pour la modification des droits associés à un fichier/dossier
    - utilisation du `sticky-bit +t` pour limiter les actions au sein d'un dossier.


A la fin de la mise en place, voici les droits de l'arborescence de partage : 

```bash
lambda_a@vm1:/root/partage$ ls -l
total 20
-rwxr-xr-x 1 root  root         945 Jan 18 13:52 admin.sh
drwxrwx--T 4 admin groupe_a    4096 Jan 18 13:55 dir_a
drwxrws--T 3 admin groupe_b    4096 Jan 18 13:52 dir_b
drwsr-s--- 2 admin groupe_gene 4096 Jan 18 13:52 dir_c
-rwxr-xr-x 1 root  root         548 Jan 18 09:49 lambda_a.sh
```




Mettre les scripts bash dans le repertoire *question7*.

## Question 8

Nos programmes question 8 et 9 sont fusionnés. On prends en compte la création de mot de passe, la mise à jour du fichier passwd et le chiffrage des mots de passe dans passwd avec `id:passwd`. 

Les script executés donnent les résultats suivants :

>Avec l'utilisateur admin quand il veut supprimer un fichier dans `dir_a`.
```bash
admin@vm1:/root/partage$ ./rmg_test.sh
File to delete: dir_a/suppme
Your file exists!
Enter your password :
SYSTEM : Your userid and password have been added to the file 
Access granted
Deleting the file
```

>Avec l'utilisateur lambda_a quand il veut supprimer un fichier dans `dir_b`.
```bash
lambda_a@vm1:/root/partage$ ./rmg_test.sh
File to delete: dir_a/suppme
Your file exists!
Enter your password :
SYSTEM : Your userid and password have been added to the file 
Access denied
```



## Question 9

cf q8. et dossier q9.

>Un utilisateur est deja dans la base
```bash
affichage avant

15764:12Zrt0LAKAbjk
Enter your password :
SYSTEM : You are already registered in the passwd file
affichage après

15764:12Zrt0LAKAbjk
# id_utilisateur:crypt(password)
```

>Un utilisateur est ajouté à la base
```bash
affichage avant

Enter your password :
SYSTEM : Your userid and password have been added to the file 
affichage après

15764:12tJAuWgu.oe6
# id_utilisateur:crypt(password)
```

## Question 10

cf dossier q10. 

un exemple de l'éxécution :

>ici le serveur est en écoute
```bash

admin@vm1:/root/partage$ ./test-server.sh 
SERVER
Socket successfully created..
Socket successfully binded..
Server listening..
server accept the client...

```

>quand on lance le script de test client, le client se connecte au serveur et peut dialoguer.
Nous n'avons pas eu le temps de faire le lien entre les scripts des questions 8 et 9, ni d'implémenter proprement le fork()
```bash
admin@vm1:/root/partage$ ./test-client.sh 
CLIENT
Socket successfully created..
connected to the server..
Enter your username: pier
Enter your password: oui
From Server : You are connected!

Enter the string : list ./dir_a #le client veut lister dir_a
From Server : b.txt file1 file2 file3 lambda_a_file supp suppme test test.txt test_file 
Enter the string : list . #le client veut lister /root/partage/
From Server : admin.sh client client.c dir_a dir_b dir_c get_things get_things.c lambda_a.sh l��ü�
Enter the string : close # le client met fin à l'échange
Client Exit...

```
Dans l'exemple, et en se référrant à notre script de test, il est possible de lister le contenu d'un dossier (ici dir_a, avec l'utilisateur `admin`)






