//Headers
#include <stdio.h>
#include <stdlib.h>

//Defines and globals
#define MAX_PROGRAM_LENGTH 120

//Struct(s)
typedef struct {
    int op, r0, r1, r2;
} Instruction;

void getFileInfo(FILE* file, Instruction program[], int length);
void printProgram(Instruction program[], int length);
int getProgramLength(FILE* file);

//Main method
int main(int argc, char *argv[]) {
    /* Helpful information
    * argv[0]: .exe location
    * argv[1]: argument count
    * argv[2]: 1st actual argument (should be file name)
    */

    Instruction* program = (Instruction*) malloc(0);
    int programLength;
    char* filename = "testInput.txt";
    FILE* ipf = fopen(filename, "r"); //Opens the command line text file given

    if (ipf == NULL) { //If the file name is wrong, or file does not exist, return 1 and exit
        printf("Filename not valid. FILE* ipf is NULL");
        return 1;
    }

    programLength = getProgramLength(ipf);
    getFileInfo(ipf, program, programLength);
    printProgram(program, programLength);
    free(program);
    return 0;
}

void getFileInfo(FILE* file, Instruction program[], int length) {
    //Initial variables
    int i, j, c, actualLen;
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
    Instruction* tempProgram = (Instruction*) calloc(length, sizeof(Instruction));
    actualLen = count * 2;
    rewind(file);

    //Create new variables and reset file pointer
    Instruction ins[length];
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
        printf("%d: %d %d %d %d\n", i, temp.op, temp.r0, temp.r1, temp.r2);
        j++;
    }
    realloc(program, length*sizeof(Instruction));
    for (i = 0; i < length; i++) {
        program[i] = tempProgram[i];
    }
}


void printProgram(Instruction program[], int length) {
    int i;
    printf("Starting print of %d instructions\n", length);
    for (i = 0; i < length; i++) {
        //printf("%d: %d %d %d %d\n", i, program[i].op, program[i].r0, program[i].r1, program[i].r2);
        printf("%d, ", i);
    }
}


int getProgramLength(FILE* file) {
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
    rewind(file);
    return count/4;
}