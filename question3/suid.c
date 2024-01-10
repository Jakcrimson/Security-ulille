#include <stdio.h>
#include <stdlib.h>
int main(int argc, char *argv[])
{
    FILE *f;
    if (argc < 2) {
        printf("Missing argument\n");
        exit(EXIT_FAILURE);
    }
    printf("Hello world\n");
    f = fopen(argv[1], "r");
    if (f == NULL) {
        perror("Cannot open file");
        exit(EXIT_FAILURE);
    }
    printf("File opens correctly\n");
    fclose(f);
    exit(EXIT_SUCCESS);
}


