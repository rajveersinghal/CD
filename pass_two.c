#include<stdio.h>
#include<string.h>

void main()
{
  char a[10], ad[10], label[10], opcode[10], operand[10], symbol[10], ch;  
  int st, diff, i, address = 0, add, len, actual_len, finaddr, prevaddr = 0, j = 0;
  char mnemonic[15][15] = {"LDA", "STA", "LDCH", "STCH"};
  char code[15][15] = {"33", "44", "53", "57"};
  FILE *fp1, *fp2, *fp3, *fp4;

  fp1 = fopen("ASSMLIST.DAT", "w");
  if (fp1 == NULL) { printf("Error opening ASSMLIST.DAT\n"); return; }
  fp2 = fopen("SYMTABB.DAT", "r");
  if (fp2 == NULL) { printf("Error opening SYMTAB.DAT\n"); fclose(fp1); return; }
  fp3 = fopen("INTERMED.DAT", "r");
  if (fp3 == NULL) { printf("Error opening INTERMED.DAT\n"); fclose(fp1); fclose(fp2); return; }
  fp4 = fopen("OBJCODE.DAT", "w");
  if (fp4 == NULL) { printf("Error opening OBJCODE.DAT\n"); fclose(fp1); fclose(fp2); fclose(fp3); return; }

  fscanf(fp3, "%s %s %s", label, opcode, operand);

  while (strcmp(opcode, "END") != 0) {
    prevaddr = address;
    fscanf(fp3, "%d %s %s %s", &address, label, opcode, operand);
  }
  finaddr = address;
  fclose(fp3);
  fp3 = fopen("INTERMED.DAT", "r");

  fscanf(fp3, "%s %s %s", label, opcode, operand);
  if (strcmp(opcode, "START") == 0) {
    fprintf(fp1, "\t%s\t%s\t%s\n", label, opcode, operand);
    fprintf(fp4, "H^%s^00%s^00%d\n", label, operand, finaddr);
    fscanf(fp3, "%d %s %s %s", &address, label, opcode, operand);
    st = address;
    diff = prevaddr - st;
    fprintf(fp4, "T^00%d^%d", address, diff);
  }

  while (strcmp(opcode, "END") != 0) {
    if (strcmp(opcode, "BYTE") == 0) {
      fprintf(fp1, "%d\t%s\t%s\t%s\t", address, label, opcode, operand);
      len = strlen(operand);
      actual_len = len - 3;
      fprintf(fp4, "^");
      for (i = 2; i < (actual_len + 2); i++) {
        sprintf(ad, "%X", operand[i]);
        fprintf(fp1, "%s", ad);
        fprintf(fp4, "%s", ad);
      }
      fprintf(fp1, "\n");
    } else if (strcmp(opcode, "WORD") == 0) {
      len = strlen(operand);
      sprintf(a, "%d", atoi(operand));
      fprintf(fp1, "%d\t%s\t%s\t%s\t00000%s\n", address, label, opcode, operand, a);
      fprintf(fp4, "^00000%s", a);
    } else if (strcmp(opcode, "RESB") == 0 || strcmp(opcode, "RESW") == 0) {
      fprintf(fp1, "%d\t%s\t%s\t%s\n", address, label, opcode, operand);
    } else {
      j = 0;
      while (strcmp(opcode, mnemonic[j]) != 0) j++;
      if (strcmp(operand, "COPY") == 0)
        fprintf(fp1, "%d\t%s\t%s\t%s\t%s0000\n", address, label, opcode, operand, code[j]);
      else {
        rewind(fp2);
        fscanf(fp2, "%s %d", symbol, &add);
        while (strcmp(operand, symbol) != 0)
          fscanf(fp2, "%s %d", symbol, &add);
        fprintf(fp1, "%d\t%s\t%s\t%s\t%s%d\n", address, label, opcode, operand, code[j], add);
        fprintf(fp4, "^%s%d", code[j], add);
      }
    }
    fscanf(fp3, "%d %s %s %s", &address, label, opcode, operand);
  }

  fprintf(fp1, "%d\t%s\t%s\t%s\n", address, label, opcode, operand);
  fprintf(fp4, "\nE^00%d", st);

  printf("\nIntermediate file is converted into object code");

  fclose(fp1);
  fclose(fp2);
  fclose(fp3);
  fclose(fp4);
}
