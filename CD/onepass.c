#include <stdio.h>
#include <string.h>
#include <stdlib.h>

// Function to find the address of a label in SYMTAB
int findInSYMTAB(char findLabel[]) {
    FILE *SYMTAB;
    char label[30], addr[30];
    SYMTAB = fopen("symtab.dat", "r");
    if (SYMTAB == NULL) {
        printf("Error: Unable to open SYMTAB file.\n");
        exit(1);
    }

    while (fscanf(SYMTAB, "%s %s", label, addr) != EOF) {
        if (strcmp(findLabel, label) == 0) {
            fclose(SYMTAB);
            return atoi(addr);
        }
    }

    fclose(SYMTAB);
    return -1; // Label not found
}

// Function to get the mnemonic code
int getMnemonicCode(char mnemonic[]) {
    if (strcmp(mnemonic, "LDA") == 0)
        return 33;
    else if (strcmp(mnemonic, "STA") == 0)
        return 44;
    else if (strcmp(mnemonic, "LDCH") == 0)
        return 53;
    else if (strcmp(mnemonic, "STCH") == 0)
        return 57;
    else
        return -1; // Invalid mnemonic
}

void main() {
    FILE *INPUT, *OUTPUT, *SYMTAB, *INTERMEDIATE, *FINAL, *OBJ;
    char mnemonic[10][10] = {"START", "LDA", "STA", "LDCH", "STCH", "END"};
    int LOCCTR, start = 0, i, j, length, Tlength, count = 0, finalAddress, startAddr;
    char label[20], opcode[20], operand[20], address[20];

    INPUT = fopen("input.dat", "r");
    OUTPUT = fopen("output.dat", "w");
    SYMTAB = fopen("symtab.dat", "w");

    if (INPUT == NULL || OUTPUT == NULL || SYMTAB == NULL) {
        printf("Error: Unable to open necessary files.\n");
        exit(1);
    }

    fscanf(INPUT, "%s %s %s", label, opcode, operand);

    if (strcmp(opcode, "START") == 0) {
        start = atoi(operand);
        LOCCTR = start;
        fprintf(OUTPUT, "%s\t%s\t%s\n", label, opcode, operand);
        fscanf(INPUT, "%s %s %s", label, opcode, operand);
    } else {
        LOCCTR = 0;
    }

    while (strcmp(opcode, "END") != 0) {
        fprintf(OUTPUT, "%d\t", LOCCTR);

        if (strcmp(label, "**") != 0) {
            fprintf(SYMTAB, "%s\t%d\n", label, LOCCTR);
        }

        for (j = 0; j < 6; j++) {
            if (strcmp(mnemonic[j], opcode) == 0) {
                LOCCTR += 3;
                break;
            }
        }

        if (strcmp(opcode, "WORD") == 0) {
            LOCCTR += 3;
        } else if (strcmp(opcode, "RESW") == 0) {
            LOCCTR += 3 * atoi(operand);
        } else if (strcmp(opcode, "RESB") == 0) {
            LOCCTR += atoi(operand);
        } else if (strcmp(opcode, "BYTE") == 0) {
            LOCCTR += strlen(operand) - 3;
        }

        fprintf(OUTPUT, "%s\t%s\t%s\n", label, opcode, operand);
        fscanf(INPUT, "%s %s %s", label, opcode, operand);
    }

    fprintf(OUTPUT, "%d\n", LOCCTR);
    fprintf(OUTPUT, "%s\t%s\t%s\n", label, opcode, operand);
    printf("\nTHE LENGTH OF THE PROGRAM IS %d\n", LOCCTR - start);

    length = LOCCTR - start;
    finalAddress = LOCCTR;
    Tlength = length - count;

    fclose(INPUT);
    fclose(OUTPUT);
    fclose(SYMTAB);

    // Second pass to generate the object code
    INTERMEDIATE = fopen("output.dat", "r");
    OBJ = fopen("obj.dat", "w");
    FINAL = fopen("final.dat", "w");

    if (INTERMEDIATE == NULL || OBJ == NULL || FINAL == NULL) {
        printf("Error: Unable to open intermediate or output files.\n");
        exit(1);
    }

    fscanf(INTERMEDIATE, "%s %s %s", label, opcode, operand);

    if (strcmp(opcode, "START") == 0) {
        fprintf(FINAL, "%s\t%s\t%s\n", label, opcode, operand);
        fprintf(OBJ, "H^%s^00%s^00%d\n", label, operand, length);
        fscanf(INTERMEDIATE, "%s %s %s %s", address, label, opcode, operand);
        fprintf(OBJ, "T^%06d^%d^", atoi(address), Tlength);
    }

    while (strcmp(opcode, "END") != 0) {
        if (strcmp(label, "**") == 0) {
            fprintf(OBJ, "%d%d^", getMnemonicCode(opcode), findInSYMTAB(operand));
            fprintf(FINAL, "%s\t%s\t%s\t%s\t%d\t%d\n", address, label, opcode, operand, getMnemonicCode(opcode), findInSYMTAB(operand));
            fscanf(INTERMEDIATE, "%s %s %s %s", address, label, opcode, operand);
        } else if (strcmp(opcode, "BYTE") == 0) {
            fprintf(FINAL, "%s\t%s\t%s\t%s", address, label, opcode, operand);
            for (i = 2; i < (strlen(operand) - 1); i++) {
                fprintf(OBJ, "%X", operand[i]);
                fprintf(FINAL, "%X", operand[i]);
            }
            fprintf(FINAL, "\n");
            fscanf(INTERMEDIATE, "%s %s %s %s", address, label, opcode, operand);
        } else if (strcmp(opcode, "WORD") == 0) {
            fprintf(OBJ, "%06X^", atoi(operand));
            fprintf(FINAL, "%s\t%s\t%s\t%s\t%06X\n", address, label, opcode, operand, atoi(operand));
            fscanf(INTERMEDIATE, "%s %s %s %s", address, label, opcode, operand);
        } else {
            fscanf(INTERMEDIATE, "%s %s %s %s", address, label, opcode, operand);
        }
    }

    fprintf(FINAL, "%s\t%s\t%s\t%s\n", address, label, opcode, operand);
    fprintf(OBJ, "\nE^%06d\n", start);

    fclose(INTERMEDIATE);
    fclose(OBJ);
    fclose(FINAL);

    // Cleanup
    remove("output.dat");
}
