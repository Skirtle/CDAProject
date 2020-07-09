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
    FILE* ipf = fopen(filename, "r"); //Opens the command line text file given

    if (ipf == NULL) { //If the file name is wrong, or file does not exist, return 1 and exit
        printf("Filename not valid. FILE* ipf is NULL");
        return 1;
    }

    Instruction* program = getFileInfo(ipf);


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
        j++;
    }
    
    return tempProgram;
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