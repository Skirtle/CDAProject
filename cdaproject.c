//Headers
#include <stdio.h>
#include <stdlib.h>

//Defines and globals
#define MAX_PROGRAM_LENGTH 125

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
    * argv[1]: 1st actual argument (should be file name)
    */

    //char* filename = "testInput.txt";
    char* filename = argv[1];
    FILE* ipf = fopen(filename, "r"); //Opens the command line text file given

    if (ipf == NULL) { //If the file name is wrong, or file does not exist, return 1 and exit
        printf("Filename \"%s\"not valid. FILE* ipf is NULL", filename);
        return 1;
    }

    Instruction* IM = getFileInfo(ipf);
    int programLength = getProgramLength(ipf);
    fclose(ipf);

    int DM[] = {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0};
    int RF[] = {0, 0, 0, 0, 0, 0, 0, 0};
    int halt = 0;
    unsigned int PC = 0;
    
    for(int i = 0; i < programLength; i++) {
        printf("%d %d %d %d", IM[i].op, IM[i].r0, IM[i].r1, IM[i].r2);
        switch(IM[i].op) {
            case 1:
                printf("\t\tlw %d %d %d\n", IM[i].r0, IM[i].r1, IM[i].r2);
                break;
            case 2:
                printf("\t\tadd %d %d %d\n", IM[i].r0, IM[i].r1, IM[i].r2);
                break;
            case 3:
                printf("\t\tsw %d %d %d\n", IM[i].r0, IM[i].r1, IM[i].r2);
                break;
            case 4:
                printf("\t\tsub %d %d %d\n", IM[i].r0, IM[i].r1, IM[i].r2);
                break;
            case 5:
                printf("\t\tsio1 %d %d %d\n", IM[i].r0, IM[i].r1, IM[i].r2);
                break;
            case 6:
                printf("\t\tsio2 %d %d %d\n", IM[i].r0, IM[i].r1, IM[i].r2);
                break;
            case 7:
                printf("\t\thalt\n");
                break;
            case 8:
                printf("\t\tjmp %d %d %d\n", IM[i].r0, IM[i].r1, IM[i].r2);
                break;
            case 9:
                printf("\t\tbeqz %d %d %d\n", IM[i].r0, IM[i].r1, IM[i].r2);
                break;
        }
    }
    
    printf("\nAssembling Program...\nProgram Assembled\n\nRun.\n\n");
    
    while(1) {
        
        printf("PC = %d | RF = [", PC * 4);
        for(int i = 0; i < 8; i++) {
            if(i != 7) {
                printf("%d, ", RF[i]);
            }
            else {
                printf("%d", RF[i]);
            }
        }
        printf("] | DM = [");
        for(int i = 0; i < 16; i++) {
            if(i != 15) {
                printf("%d, ", DM[i]);
            }
            else {
                printf("%d", DM[i]);
            }
        }
        printf("]\n");
        
        switch(IM[PC].op) {
            case 1:
                RF[IM[PC].r0] = DM[IM[PC].r2];
                PC += 1;
                break;
            case 2:
                RF[IM[PC].r0] = RF[IM[PC].r1] + RF[IM[PC].r2];
                PC += 1;
                break;
            case 3:
                DM[IM[PC].r2] = RF[IM[PC].r0];
                PC += 1;
                break;
            case 4:
                RF[IM[PC].r0] = RF[IM[PC].r1] - RF[IM[PC].r2];
                PC += 1;
                break;
            case 5:
                scanf("%d", &RF[IM[PC].r0]);
                PC += 1;
                break;
            case 6:
                printf("%d\n", RF[IM[PC].r0]);
                PC += 1;
                break;
            case 7:
                halt = 1;
                PC += 1;
                break;
            case 8:
                PC = IM[PC].r2;
                break;
            case 9:
                if (IM[PC].r0 == 0) {
                    PC += 4;
                }
                break;
        }
        if(halt) {
            break;
        }
    }

    //Finished, free and close pointers and files. End.
    free(IM);
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
