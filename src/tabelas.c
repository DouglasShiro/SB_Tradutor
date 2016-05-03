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

int procura_tab_lst(Tab_lst* tableLst, char* simb){
    Tab_lst* aux = tableLst;

    while(strcmp(aux->simb, simb) != 0){
        aux = aux->next;
    }
return aux->valor;
}
/*************************************************************************************/
/*          TABELA HASH
    -Tabela de Instrucoes
    -Tabela de Diretivas

    Usado em: montador, ligador
    funcao: Referencia de instrucoes e diretivas validas
*/


/* Funcoes do hash */

/* Cria e adiciona cada instrucao e diretiva a tabela hash*/
int inicialize_dir_inst(Tab_hash* tableDI)
{
	char* inst[14] = {"ADD", "SUB", "MULT", "DIV",
					"JMP", "JMPN", "JMPP","JMPZ",
					"COPY", "LOAD", "STORE", "INPUT",
					"OUTPUT", "STOP"};

	char* dir_0[4] = {"PUBLIC","EXTERN", "BEGIN", "END"};

	char* dir_1[5] = {"SECTION", "SPACE", "CONST","EQU", "IF"};


	int i, aux = 0;
	tableDI = (Tab_hash*) malloc(sizeof(Tab_hash));
	for(i = 0; i < 14; i++)
	{
		aux = return_key(inst[i]);

		if(tableDI[aux].name != NULL)
			printf("CHAVE REPETIDA - [%d]", aux);

		tableDI[aux].name = inst[i];
		tableDI[aux].cod = i+1;

		/* Para o COPY*/
		if(strcmp(inst[i],"COPY"))
		{
			tableDI[aux].tam = 3;
			tableDI[aux].op = 2;
		}/* Para o STOP */
		else if(strcmp(inst[i],"STOP"))
		{
			tableDI[aux].tam = 1;
			tableDI[aux].op = 0;
		}
		else
		{
			tableDI[aux].tam = 2;
			tableDI[aux].op = 1;
		}

	}

	/* Diretivas com OP = 0*/
	for(i = 0; i < 4; i++)
	{
		aux = return_key(dir_0[i]);

		if(tableDI[aux].name != NULL)
			printf("CHAVE REPETIDA - [%d]", aux);

		tableDI[aux].name = dir_0[i];
		tableDI[aux].tam = 0;
		tableDI[aux].op = 0;
	}

	/* Diretivas com OP = 1*/
	for(i = 0; i < 5; i++)
	{
		aux = return_key(dir_1[i]);
		if(tableDI[aux].name != NULL)
			printf("CHAVE REPETIDA - [%d]", aux);

		tableDI[aux].name = dir_1[i];
		tableDI[aux].op = 1;

		if(strcmp(dir_1[i], "SPACE") || strcmp(dir_1[i], "SPACE"))
		{
			tableDI[aux].tam = 1;
		}
		else
		{
			tableDI[aux].tam = 0;
		}
	}

	return 0;
}
/* Calcula a chave para o hash */
int return_key(char* str)
{
	int i, key = 0;

	for(i = 0; i < strlen(str); i++)
	{
		key += str[i];
	}
	key = (key*257)%67;
	return key;
}

/* Inicializa tabela de tokens*/
Tab_hash* inicialize_tab_hash(void){
     return NULL;
	}





/* Insere na lista de tokens */
// /Tab_lst* insert_tab_hash(Tab_hash* tableHash, char* minemonico, int op, int cod, int tam){
//
//     return tableTok;
// }
//

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
