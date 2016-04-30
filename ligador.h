#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef struct TabFatorCorrecao {
  char ObjFile[20];
  int Tam;
  struct TabFatorCorrecao* next;
}FatorCorrecao;

Tab_lst* insert_tab_lst(Tab_lst* tableLst, char* simb, int valor);
Tab_lst* inicialize_tab_lst(void);
void print_tab_lst(Tab_lst* tableLst);
Tab_lst* remove_tab_lst(Tab_lst* tableLst);
