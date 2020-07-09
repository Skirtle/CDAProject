//Headers
#include <stdio.h>
#include <stdlib.h>

//Struct(s)
typedef struct {
    int op, r1, r2, r3;
} Instruction;


//Main method
int main(int argc, char *argv[]) {
    FILE* ipf = fopen(argv[0], "r"); //Opens the command line text file given
    if (ipf == NULL) { //If the file name is wrong, or file does not exist, return 1 and exit
        printf("Filename not valid. FILE* ipf is NULL");
        return 1;
    }

    return 0;
}
