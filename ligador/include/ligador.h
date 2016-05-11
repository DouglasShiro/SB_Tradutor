#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "tabelas.h"

void ler_objeto(FILE* modulo, Tab_lst **TabUso, Tab_lst **TabDef,
                Tab_lst **TabGlobalDef, int *fatorCorrecao, int i, int *relativo);
void corrige_endereco(FILE* modulo, FILE* out, Tab_lst* TabUso,
                Tab_lst* TabGlobalDef,  int fatorCorrecao, int *relativo);
