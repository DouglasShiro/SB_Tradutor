/*  Estrutura de dados TABELAS
*     - Tabela de simbolos
*     - Tabela de diretivas
*     - Tabela de definicoes
*     - Tabela uso
*/
#include <stdio.h>

#include <stdlib.h>
#include <string.h>

/*Estrutura de lista para armazenar os dados das tabelas:
  - Uso.
  - Definicao
*/
typedef struct TAB_LST{
  char* simb;
  int valor;
  struct TAB_USO* proximo;
}Tab_lst;

Tab_lst* insert_tab_lst(Tab_lst* tableLst, char* simb, int valor);
Tab_lst* inicialize_tab_lst(void);
void print_tab_lst(Tab_lst* tableLst);
Tab_lst* remove_tab_lst(Tab_lst* tableLst);

/*Estruta hash para armazenar os dados das tabelas
  - Instrucoes
  - Diretivas
*/
typedef struct TAB_HASH{
  char minemonico[10];
  int op;
  int cod;
  int tam;
}Tab_hash;

Tab_hash* insert_tab_hash(Tab_hash* tableHash, char* minemonico, int op, int cod, int tam);
Tab_hash* inicialize_tab_hash(void);
void print_tab_hash(Tab_hash* tableHash);
Tab_hash* remove_tab_hash(Tab_hash* tableHash);
