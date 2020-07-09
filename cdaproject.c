//Headers
#include <stdio.h>
#include <stdlib.h>

//Defines and globals
#define MAX_PROGRAM_LENGTH 120

//Struct(s)
typedef struct {
    int op, r0, r1, r2;
} Instruction;

Instruction* getFileInfo(FILE* file);
int getProgramLength(FILE* file);

//Main method
int main(int argc, char *argv[]) {
    /* Helpful information
    * argv[0]: .exe location
    * argv[1]: argument count
    * argv[2]: 1st actual argument (should be file name)
    */

    int programLength;
    char* filename = "testInput.txt";
    //char* filename = argv[2];
    FILE* ipf = fopen(filename, "r"); //Opens the command line text file given

    if (ipf == NULL) { //If the file name is wrong, or file does not exist, return 1 and exit
        printf("Filename \"%s\"not valid. FILE* ipf is NULL", filename);
        return 1;
    }

    Instruction* program = getFileInfo(ipf);


    //Finished, free and close pointers and files. End.
    free(program);
    fclose(ipf);
    return 0;
}

Instruction* getFileInfo(FILE* file) {
    //Initial variables
    int i, j, c, actualLen;
    int count = 0;
    int length = getProgramLength(file);

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
    actualLen = length * 8; //Funky math, but this is right.
    rewind(file);

    //Create new variables and reset file pointer
    Instruction ins[length];
    char fullString[actualLen];
    char regStr[length*4];
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
    for (i = 0; i < 4*length; i+=4) {
        Instruction temp;
        temp.op = regStr[i+0] - '0';
        temp.r0 = regStr[i+1] - '0';
        temp.r1 = regStr[i+2] - '0';
        temp.r2 = regStr[i+3] - '0';
        tempProgram[j] = temp;
        j++;
    }
    
    return tempProgram;
}


int getProgramLength(FILE* file) {
    //Initial variables
    int c;
    int count = 0;

    //Get length of file and reset file pointer when done
    while(1) {
        c = fgetc(file);
        if (c == EOF || c == '\n') { //Break when new line is found, code should be on one line
            break;
        }
        if (c != ' ') { //Ignore spaces
            count++;
        }
    }
    rewind(file); //Reset the pointer to the start of the file
    return count/4; //count/4 is the amount of instructions given, as every 4 digits is one instruction
}