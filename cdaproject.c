//Headers
#include <stdio.h>
#include <stdlib.h>

//Defines
#define MAX_PROGRAM_LENGTH 96

//Struct(s)
typedef struct {
    int op, r0, r1, r2;
} Instruction;

void getFileInfo(FILE* file, Instruction program[]);
void printProgram(Instruction program[]);

//Main method
int main(int argc, char *argv[]) {
    /* Helpful information
    * argv[0]: .exe location
    * argv[1]: argument count
    * argv[2]: 1st actual argument (should be file name)
    */

   Instruction* program = (Instruction*) calloc(MAX_PROGRAM_LENGTH/8, sizeof(Instruction));
   char* filename = "testInput.txt";
    FILE* ipf = fopen(filename, "r"); //Opens the command line text file given
    if (ipf == NULL) { //If the file name is wrong, or file does not exist, return 1 and exit
        printf("Filename not valid. FILE* ipf is NULL");
        return 1;
    }
    getFileInfo(ipf, program);
    printProgram(program);
    free(program);
    return 0;
}

void getFileInfo(FILE* file, Instruction program[]) {
    printf("Starting\n");
    Instruction ins[MAX_PROGRAM_LENGTH/8];
    int i;
    char s[MAX_PROGRAM_LENGTH];
    fgets(s, MAX_PROGRAM_LENGTH, file);
    
    int count = 0;
    for (i = 0; i < MAX_PROGRAM_LENGTH; i+=8) {
        //printf("%d * %d\n", count, i);
        //Fix this shit
        //Need to account for spaces in between numbers
        //Convert the ascii character to an int
        Instruction temp;
        temp.op = ((int) s[count*i]) - 30;
        temp.r0 = ((int) s[count*i+2]) - 30;
        temp.r1 = ((int) s[count*i+4]) - 30;
        temp.r2 = ((int) s[count*i+6]) - 30;
        printf("%d %d %d %d\n", temp.op, temp.r0, temp.r1, temp.r2);
        program[count] = temp;
        count++;
    }
    
    printf("Done\n");
}


void printProgram(Instruction program[]) {
    int i;
    for (i = 0; i < MAX_PROGRAM_LENGTH/8; i++) {
        printf("%d %d %d %d\n", program[i].op, program[i].r0, program[i].r1, program[i].r2);
    }
}
