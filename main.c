/*************************************************************************
CDA Programming Project. Code Written by Shawn Guydeene and Dalton Kajander
**************************************************************************/

//Headers
#include <stdio.h>
#include <stdlib.h>

//Globals and defines
#define MAX_PROGRAM_SIZE 125

//Struct(s)
typedef struct {
    int op, r0, r1, r2;
} Instruction;

//Function prototypes
void getFileInfo(FILE* file, Instruction* im, int *x);

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
        printf("Filename \"%s\" not valid. FILE* ipf is NULL", filename);
        return 1;
    }

    Instruction* IM;
    int programLength;
    getFileInfo(ipf, IM, &programLength);
    fclose(ipf);

    int DM[] = {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0};
    int RF[] = {0, 0, 0, 0, 0, 0, 0, 0};
    int halt = 0;
    int IFID[] = {0, 0, 0, 0};
    int IDEX[] = {0, 0, 0, 0};
    int EXDM[] = {0, 0, 0, 0};
    int DMWB[] = {0, 0, 0, 0};
    unsigned int PC = 0;
    unsigned int i;

    for(i = 0; i < programLength; i++) {
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
                printf("\t\tsio3 %d %d %d\n", IM[i].r0, IM[i].r1, IM[i].r2);
                break;
            case 8:
                printf("\t\tjmp %d %d %d\n", IM[i].r0, IM[i].r1, IM[i].r2);
                break;
            case 9:
                printf("\t\tbeqz %d %d %d\n", IM[i].r0, IM[i].r1, IM[i].r2);
                break;
        }
    }

    printf("\nAssembling Program...\nProgram Assembled\n\nRun.\n");

    while(halt == 0) {
        /* WRITING THE STATE OF THE EMULATOR */
        // Program counter and register file
        printf("\nPC = %d | RF = [", PC);
        for(i = 0; i < 8; i++)
            if(i != 7)
                printf("%d, ", RF[i]);
            else
                printf("%d", RF[i]);
        printf("] | DM = [");
        // printitng the data memory
        for(i = 0; i < 16; i++)
            if(i != 15)
                printf("%d, ", DM[i]);
            else
                printf("%d", DM[i]);
        // printing the contents of the buffer
        printf("]\nIFID = [%d, %d, %d, %d]", IFID[0], IFID[1], IFID[2], IFID[3]);
        printf(" | IDEX = [%d, %d, %d, %d]", IDEX[0], IDEX[1], IDEX[2], IDEX[3]);
        printf(" | EXDM = [%d, %d, %d, %d]", EXDM[0], EXDM[1], EXDM[2], EXDM[3]);
        printf(" | DMWB = [%d, %d, %d, %d]\n", DMWB[0], DMWB[1], DMWB[2], DMWB[3]);

        // Fetching the new instruction and storing inside of the IFID buffer.
        IFID[0] = IM[PC/4].op;
        IFID[1] = IM[PC/4].r0;
        IFID[2] = IM[PC/4].r1;
        IFID[3] = IM[PC/4].r2;
        // using a switch statement to decode which opcode was given for the instruction
        switch(IFID[0]) {
            case 1:
                // decoding opcode 1: load word operation
                printf("\n/* load word (lw %d %d %d) */\n", IFID[1], IFID[2], IFID[3]);
                // fetching operators and storing in IDEX buffer
                IDEX[0] = IFID[0];
                IDEX[1] = IFID[1];
                IDEX[2] = IFID[2];
                IDEX[3] = IFID[3];
                // Execute code
                RF[IDEX[1]] = DM[IDEX[3]];
                // Writing to EXDM and DMWB buffer
                EXDM[0] = IDEX[0];
                EXDM[1] = IDEX[1];
                EXDM[2] = IDEX[2];
                EXDM[3] = IDEX[3];
                DMWB[0] = EXDM[0];
                DMWB[1] = EXDM[1];
                DMWB[2] = EXDM[2];
                DMWB[3] = RF[EXDM[3]];
                break;
            case 2:
                // decoding opcode 2: addition operation
                printf("\n/* add (add %d %d %d) */\n", IFID[1], IFID[2], IFID[3]);
                 // fetching operators and storing in IDEX buffer
                IDEX[0] = IFID[0];
                IDEX[1] = IFID[1];
                IDEX[2] = RF[IFID[2]];
                IDEX[3] = RF[IFID[3]];
                // Execute code
                RF[IDEX[1]] = IDEX[2] + IDEX[3];
                // Writing to EXDM and DMWB buffer
                EXDM[0] = IDEX[0];
                EXDM[1] = RF[IDEX[1]];
                EXDM[2] = IDEX[2];
                EXDM[3] = IDEX[3];
                DMWB[0] = EXDM[0];
                DMWB[1] = EXDM[1];
                DMWB[2] = EXDM[2];
                DMWB[3] = EXDM[3];
                break;
            case 3:
                printf("\n/* store R%d to memory location %d (sw %d %d %d) */\n", IFID[1], IFID[3], IFID[1], IFID[2], IFID[3]);
                // fetching operators and storing in IDEX buffer
                IDEX[0] = IFID[0];
                IDEX[1] = RF[IFID[1]];
                IDEX[2] = IFID[2];
                IDEX[3] = IFID[3];
                // Execute code
                DM[IDEX[3]] = IDEX[1];
                // Writing to EXDM and DMWB buffer
                EXDM[0] = IDEX[0];
                EXDM[1] = IDEX[1];
                EXDM[2] = IDEX[2];
                EXDM[3] = IDEX[3];
                DMWB[0] = EXDM[0];
                DMWB[1] = EXDM[1];
                DMWB[2] = EXDM[2];
                DMWB[3] = EXDM[3];
                break;
            case 4:
                printf("\n/* subtract (sub %d %d %d) */\n", IFID[1], IFID[2], IFID[3]);
                // fetching operators and storing in IDEX buffer
                IDEX[0] = IFID[0];
                IDEX[1] = IFID[1];
                IDEX[2] = RF[IFID[2]];
                IDEX[3] = RF[IFID[3]];
                // Execute code
                RF[IDEX[1]] = IDEX[2] - IDEX[3];
                // Writing to EXDM and DMWB buffer
                EXDM[0] = IDEX[0];
                EXDM[1] = IDEX[1];
                EXDM[2] = IDEX[2];
                EXDM[3] = IDEX[3];
                DMWB[0] = EXDM[0];
                DMWB[1] = EXDM[1];
                DMWB[2] = EXDM[2];
                DMWB[3] = EXDM[3];
                break;
            case 5:
                printf("\n/* input value (sio1 %d %d %d) */\n", IFID[1], IFID[2], IFID[3]);
                // fetching operators and storing in IDEX buffer
                IDEX[0] = IFID[0];
                IDEX[1] = IFID[1];
                IDEX[2] = IFID[2];
                IDEX[3] = 0;
                // Execute code
                scanf("%d", &RF[IDEX[1]]);
                // Writing to EXDM and DMWB buffer
                EXDM[0] = IDEX[0];
                EXDM[1] = IDEX[1];
                EXDM[2] = IDEX[2];
                EXDM[3] = RF[IDEX[3]];
                DMWB[0] = EXDM[0];
                DMWB[1] = EXDM[1];
                DMWB[2] = EXDM[2];
                DMWB[3] = EXDM[3];
                break;
            case 6:
                printf("\n/* outputting R0 to screen (sio2 %d %d %d) */\n", IFID[1], IFID[2], IFID[3]);
                // fetching operators and storing in IDEX buffer
                IDEX[0] = IFID[0];
                IDEX[1] = RF[0];
                IDEX[2] = IFID[2];
                IDEX[3] = 0;
                // Execute code
                printf("%d\n", IDEX[1]);
                // Writing to EXDM and DMWB buffer
                EXDM[0] = IDEX[0];
                EXDM[1] = IDEX[1];
                EXDM[2] = IDEX[2];
                EXDM[3] = IDEX[3];
                DMWB[0] = EXDM[0];
                DMWB[1] = EXDM[1];
                DMWB[2] = EXDM[2];
                DMWB[3] = EXDM[3];
                break;
            case 7:
                printf("\n/* end of program (sio3 %d %d %d) */\n", IFID[1], IFID[2], IFID[3]);
                // fetching operators and storing in IDEX buffer
                IDEX[0] = IFID[0];
                IDEX[1] = IFID[1];
                IDEX[2] = IFID[2];
                IDEX[3] = IFID[3];
                // Execute code
                halt = 1;
                // Writing to EXDM and DMWB buffer
                EXDM[0] = IDEX[0];
                EXDM[1] = IDEX[1];
                EXDM[2] = IDEX[2];
                EXDM[3] = IDEX[3];
                DMWB[0] = EXDM[0];
                DMWB[1] = EXDM[1];
                DMWB[2] = EXDM[2];
                DMWB[3] = EXDM[3];
                break;
            case 8:
                printf("\n/* jump to %d (jmp %d %d %d) */\n", IFID[3], IFID[1], IFID[2], IFID[3]);
                // fetching operators and storing in IDEX buffer
                IDEX[0] = IFID[0];
                IDEX[1] = IFID[1];
                IDEX[2] = IFID[2];
                IDEX[3] = IFID[3];
                // Execute code
                PC = IDEX[3];
                // Writing to EXDM and DMWB buffer
                EXDM[0] = IDEX[0];
                EXDM[1] = IDEX[1];
                EXDM[2] = IDEX[2];
                EXDM[3] = IDEX[3];
                DMWB[0] = EXDM[0];
                DMWB[1] = EXDM[1];
                DMWB[2] = EXDM[2];
                DMWB[3] = EXDM[3];
                break;
            case 9:
                printf("\n/* check if register is zero (beq %d %d %d) */\n", IFID[1], IFID[2], IFID[3]);
                // fetching operators and storing in IDEX buffer
                IDEX[0] = IFID[0];
                IDEX[1] = RF[IFID[1]];
                IDEX[2] = IFID[2];
                IDEX[3] = IFID[3];
                // Execute code
                if (IDEX[1] == 0)
                    PC += 4;
                // Writing to EXDM and DMWB buffer
                EXDM[0] = IDEX[0];
                EXDM[1] = IDEX[1];
                EXDM[2] = IDEX[2];
                EXDM[3] = IDEX[3];
                DMWB[0] = EXDM[0];
                DMWB[1] = EXDM[1];
                DMWB[2] = EXDM[2];
                DMWB[3] = EXDM[3];
                break;
        }
        // Increment program counter
        PC += 4;
    }

    // Program counter and register file
    printf("\nPC = %d | RF = [", PC);
    for(i = 0; i < 8; i++)
        if(i != 7)
            printf("%d, ", RF[i]);
        else
            printf("%d", RF[i]);
    printf("] | DM = [");
    // printitng the data memory
    for(i = 0; i < 16; i++)
        if(i != 15)
            printf("%d, ", DM[i]);
        else
            printf("%d", DM[i]);
    // printing the contents of the buffer
    printf("]\nIFID = [%d, %d, %d, %d]", IFID[0], IFID[1], IFID[2], IFID[3]);
    printf(" | IDEX = [%d, %d, %d, %d]", IDEX[0], IDEX[1], IDEX[2], IDEX[3]);
    printf(" | EXDM = [%d, %d, %d, %d]", EXDM[0], EXDM[1], EXDM[2], EXDM[3]);
    printf(" | DMWB = [%d, %d, %d, %d]\n", DMWB[0], DMWB[1], DMWB[2], DMWB[3]);
    
    // Program completed
    printf("\nProgram complete.\n");

    //Finished, free and close pointers and files. End.
    free(IM);
    return 0;
}

void getFileInfo(FILE* file, Instruction* im, int *x) {
    //Initial variables
    int i, j, c, actualLen;
    int count = 0, length = 0;

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
    length = count/4;
    x = &length;
    //Instruction* tempProgram = (Instruction*) calloc(length, sizeof(Instruction));
    Instruction tempProgram[MAX_PROGRAM_SIZE];
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

    im = tempProgram;
}
