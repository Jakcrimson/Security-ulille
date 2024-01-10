# Rendu "Les droits d’accès dans les systèmes UNIX"

## Binome

- Nom, Prénom, email: ___

- Nom, Prénom, email: ___

## Question 1

On ne peux pas ecrire le fichier car on ne dispose pas des droits d'écriture pour l'utilisateur actuel (-r--rw-r--). Ainsi, quand on essaye de sauvegarder le buffer d'écriture du fichier, on a un message de "Permission Denied".

## Question 2

Le droit 'x' pour un dossier signifie qu'on ne peut pas l'ouvrir.
En executant les operations suivantes :
```bash
>su ubuntu
>cd ~ 
>mkdir mydir
>chmod g-x mydir # on enlève les droits d'éxécution du dossier
>ls -al # on remarque que le x du groupe est parti
>cd mydir # on peut rentrer dans le dossier car pour l'utilisateur actuel, il y a toujours le droit x.
```

Quand on utilise l'utilisateur toto, il n'est pas possile d'entrer dans le repertoire mydir. Les membres du groupe auront ce message :

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

    // Obtention de la taille du fichier pour allouer un tampon de lecture approprié
    struct stat st;
    if (fstat(fd, &st) == -1) {
        perror("Erreur lors de la récupération de la taille du fichier");
        close(fd);
        exit(EXIT_FAILURE);
    }

    // Allouer un tampon de lecture
    char *buffer = malloc(st.st_size);
    if (buffer == NULL) {
        perror("Erreur lors de l'allocation du tampon de lecture");
        close(fd);
        exit(EXIT_FAILURE);
    }

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
mydir et mydata sont en permission de lecture et d'éxécution pour le groupe ubuntu.


Quand on fait appel au flag `set-user-id`, il est possible qu'un utilisateur du groupe ubuntu puise lire le contenu du fichier `mydata.txt` même s'il n'est pas en mode executable pour les groupes.

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
On retrouve les processus de l'utilisateur toto.


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


## Question 5

Réponse

## Question 6

Réponse

## Question 7

Mettre les scripts bash dans le repertoire *question7*.

## Question 8

Le programme et les scripts dans le repertoire *question8*.

## Question 9

Le programme et les scripts dans le repertoire *question9*.

## Question 10

Les programmes *groupe_server* et *groupe_client* dans le repertoire
*question10* ainsi que les tests. 








