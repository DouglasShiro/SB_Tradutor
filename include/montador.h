#include <ctype.h>
#include "tabelas.h"
#define TOK_SIZE  100

typedef struct TOKENS {
	int	line; 	/* Linha do programa que o token esta*/
	char tok[TOK_SIZE];	/* Token */
	char type;		/* Define o tipo, se é uma instrucao(i), operando(o) ou diretiva(d) ou indefinido(u)*/ 
	struct TOKENS*	next;  	/* Proximo token */
}Tokens;

/* Funcoes de Tokens */
Tokens* insert_token(Tokens* tableTok, char* token, int line, char type);
Tokens* inicialize_token(void);
void print_tokens(Tokens* tableTok);
Tokens* remove_token(Tokens* tableTok);

/* Funcoes do montador*/
Tokens* pre_processor(char* nameFile, Tokens* tableTok);
//int primeira_passagem(Tokens* tableTok, Tab_lst* tableDef);
int primeira_passagem(Tokens* tableTok, Tab_lst* tableSimb, Tab_lst* tableDir, Tab_lst* tableDef, Tab_lst* tableUso);
int token_valido(char* token, int nLine);


