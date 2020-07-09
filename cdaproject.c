//Headers
#include <stdio.h>
#include <stdlib.h>

//Defines and globals
#define MAX_PROGRAM_LENGTH 120
int programLength = 0;

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

    Instruction* program = NULL;
    char* filename = "testInput.txt";
    FILE* ipf = fopen(filename, "r"); //Opens the command line text file given
    if (ipf == NULL) { //If the file name is wrong, or file does not exist, return 1 and exit
        printf("Filename not valid. FILE* ipf is NULL");
        return 1;
    }
    getFileInfo(ipf, program);
    printf("Size: %d\n", sizeof(program) / sizeof(program[0]));
    printProgram(program);
    free(program);
    return 0;
}

void getFileInfo(FILE* file, Instruction program[]) {
    //Initial variables
    int i, j, progLen, c, actualLen;
    int count = 0;

    //Get length of file and reset file pointer when done
    while(1) {
        c = fgetc(file);
        if (c == EOF || c == '\n') {
            break;
        }
        if (c != ' ') {
            count++;
        }
    }
    progLen = count/4;
    Instruction* tempProgram = (Instruction*) calloc(progLen, sizeof(Instruction));
    actualLen = count * 2;
    rewind(file);

    //Create new variables and reset file pointer
    Instruction ins[progLen];
    char fullString[actualLen];
    char regStr[count];
    fgets(fullString, actualLen, file);
    rewind(file);

    //Normalize string
    j = 0;
    for (i = 0; i < actualLen; i++) {
        if (fullString[i] != ' ') {
            regStr[j] = fullString[i];
            j++;
        }
    }

    //Convert normalized string to instructions
    j = 0;
    for (i = 0; i < count; i+=4) {
        Instruction temp;
        temp.op = regStr[i+0] - '0';
        temp.r0 = regStr[i+1] - '0';
        temp.r1 = regStr[i+2] - '0';
        temp.r2 = regStr[i+3] - '0';
        tempProgram[j] = temp;
        j++;
    }
    programLength = progLen;
}


void printProgram(Instruction program[]) {
    int i;
    printf("Starting print\n");
    for (i = 0; i < programLength; i++) {
        printf("%d %d %d %d\n", program[i].op, program[i].r0, program[i].r1, program[i].r2);
    }
}
