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

/* Inicializa tabela*/
Tab_lst* inicialize_tab_lst(void){
    return NULL;
}

/* Imprime a tabela*/
void print_tab_lst(Tab_lst* tableLst){
    Tab_lst* aux;

    for( aux = tableLst; aux != NULL; aux = aux->next)
        printf("%s\t%d\n", aux->simb, aux->valor);
}

/* Remove um elemento da lista*/
Tab_lst* remove_tab_lst(Tab_lst* tableLst){
    Tab_lst* aux = tableLst->next;

    free(tableLst);

    tableLst = aux;

    return tableLst;
}

/*procura elemento na tabela
    entrada: Tabela, simbolo a ser procurado
    saida: valor do simbolo procurado
*/
int procura_tab_lst(Tab_lst* tableLst, char* simb){
    Tab_lst* aux = tableLst;

    while(strcmp(aux->simb, simb) != 0){
        aux = aux->next;
        if(aux->next == NULL){
            return -1;
        }
    }
return aux->valor;
}
