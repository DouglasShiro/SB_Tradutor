#include "tabelas.h"

/*************************************************************************************/
/*          TABELA LISTA
    - Tabela de Uso
      Usado em: montador.c, ligador.c
      funcao: receber dados EXTERN

    - Tabela de Definicoes
      usado em: montador.c, ligador.c
      funcao: receber dados PUBLIC

    - Tabela fator de correcao
      usado em: ligador.c
      funcao: corrigir referencia de enderecos
*/
/* Insere na lista de tokens
Funcao:
  Inserir em determinada tabela os dados de

Entrada:
  char* simb;
  int valor;

  Para tabela de uso:
     simb = simb;
     valor = end;

  Para tabela de definicoes:
    simb = simb;
    valor = valor;

  Para tabela de fatores de correcao:
    simb = nome do arquivo objeto;
    valor = fator de correcao;

 Saida: Tab_lst*
*/
Tab_lst* insert_tab_lst(Tab_lst* tableLst, char* simb, int valor){
    Tab_lst* aux;
    Tab_lst* new = (Tab_lst*) malloc(sizeof(Tab_lst));

	  strcpy(new->simb, simb);
    new->valor = valor;

    if(tableLst == NULL)
    {
        new->next = tableLst;
        return new;
    }

    for(aux = tableLst; aux->next != NULL; aux = aux->next);

    new->next = aux->next;
    aux->next = new;

    return tableLst;
}

/* Inicializa tabela de tokens
  Igual para todas as tabelas
*/
Tab_lst* inicialize_tab_lst(void){
    return NULL;
}

/* Imprime a lista de tokens

*/
void print_tab_lst(Tab_lst* tableLst){
    Tab_lst* aux;

    for( aux = tableLst; aux != NULL; aux = aux->next)
        printf("%s\t%d\n", aux->simb, aux->valor);
}

/* Remove um token da lista*/
Tab_lst* remove_tab_lst(Tab_lst* tableLst){
    Tab_lst* aux = tableLst->next;

    free(tableLst);

    tableLst = aux;

    return tableLst;
}

/*************************************************************************************/
/*          TABELA HASH
    -Tabela de Instrucoes
    -Tabela de Diretivas

    Usado em: montador, ligador
    funcao: Referencia de instrucoes e diretivas validas
*/

/* Insere na lista de tokens */
// /Tab_lst* insert_tab_hash(Tab_hash* tableHash, char* minemonico, int op, int cod, int tam){
//
//     return tableTok;
// }
//
// /* Inicializa tabela de tokens*/
// Tab_hash* inicialize_tab_hash(void){
//     return NULL;
// }
//
// /* Imprime a lista de tokens*/
// void print_tab_hash(Tab_hash* tableHash){
//
// }
//
// /* Remove um token da lista*/
// Tab_hash* remove_tab_hash(Tab_hash* tableHash){
//     return tableTok;
// }
