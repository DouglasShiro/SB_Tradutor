#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <ctype.h>
#define TOK_SIZE  100






typedef struct TOKENS {
	int	line; 	/* Linha do programa que o token esta*/
	char tok[TOK_SIZE];	/* Token */
	char type;		/* Define o tipo, se é uma instrucao(i), operando(o) ou diretiva(d) ou indefinido(u)*/ 
	struct TOKENS*	next;  	/* Proximo token */
}Tokens;


Tokens* pre_processor(char* nameFile, Tokens* tableTok);
Tokens* insert_token(Tokens* tableTok, char* token, int line, char type);
Tokens* inicialize_token(void);
void print_tokens(Tokens* tableTok);
Tokens* remove_token(Tokens* tableTok);


