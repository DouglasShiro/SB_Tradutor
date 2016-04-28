/* PROGRAMA MONTADOR
*  <COMANDO> ./montador.c <entrada.asm > saida.o 
*
*   Grupo:  Andressa Valadares          13/0042943
*           Douglas Shiro Yokoyama      13/0024902
*/

/*
 * TO DO list:
 * ->Analisador Lexico
 * */


#include "montador.h"

int main(int argc, char **argv)
{
	Tokens* tableTok = inicialize_token();
	
	if (argc != 3)
	{
		printf("Erro: número invalido de argumentos\n");
	}
	else 
	{
		tableTok = pre_processor(argv[1], tableTok); /* Le arquivo de entrada */
		print_tokens(tableTok);
	}
	
	while(tableTok != NULL) /*Libera a memoria alocada*/
		tableTok = remove_token(tableTok);
	
	return 0;
}
/*
 * Os tokens sao separados por espacos e salvos numa lista tableTok
 * */
Tokens* pre_processor(char* nameFile, Tokens* tableTok)
{
	FILE * pFile;
	char c;   
	char token[TOK_SIZE];
	int i, nLine; /* numero da linha */
	memset(token, '\0', sizeof(token)); /*Limpa a string*/

	
	i = 0;
	nLine = 0;	
	pFile = fopen (nameFile,"r");
	
	if (pFile ==  NULL) /* Verifica se o arquivo existe*/
	{
		perror ("The following error occurred");
		return NULL; /*Não tem lista de tokens, erro ao abrir o arquivo*/
	}
	
	printf("VOU LER O ARQUIVO!\n");
    while ((c = fgetc(pFile)) != EOF)
    {	
		if(c == ';')
		{
			while(((c = fgetc(pFile)) != EOF) && ((c = fgetc(pFile)) != '\n'));
			if(c == '\n')/* Se eh o final na linha incrementa o numero da linha*/
				nLine++;
		}
		else
		{
			if(c == '\n')/* Se eh o final na linha incrementa o numero da linha*/
				nLine++;
			
			if(!isspace(c))
			{	
				token[i] = c;
				i++;
			}
			/* Verifica se o token acabou*/
			if(isspace(c) && (token[0] != '\0'))
			{	token[i] = '\0';
				tableTok = insert_token(tableTok, token, nLine, 'u');
				memset(token, '\0', sizeof(token)); /*Limpa a string*/
				i = 0;
			}
			
		}
    }
    /*Se existir um token nao nulo, insere na lista*/
	if(token[0] != '\0')
	{	
		token[i] = '\0';
		tableTok = insert_token(tableTok, token, nLine, 'u');
	}
		
    printf("nlines: %d\n", nLine);

	fclose(pFile);
	return tableTok;
}

/* Insere na lista de tokens */
Tokens* insert_token(Tokens* tableTok, char* token, int line, char type)
{
    Tokens* aux;
    Tokens* new = (Tokens*) malloc(sizeof(Tokens));
 
	strcpy(new->tok, token);
    new->line = line;
    new->type = type;
 
    if(tableTok == NULL)
    {
        new->next = tableTok;
        return new;
    }
 
    for(aux = tableTok; aux->next != NULL; aux = aux->next);
    
    new->next = aux->next;
    aux->next = new;
 
    return tableTok;
}

/* Inicializa tabela de tokens*/
Tokens* inicialize_token(void)
{
    return NULL;
}

/* Imprime a lista de tokens*/
void print_tokens(Tokens* tableTok)
{
    Tokens* aux;
 
    for( aux = tableTok; aux != NULL; aux = aux->next)
        printf("%s\t%u\t%c\n", aux->tok, aux->line, aux->type);
}

/* Remove um token da lista*/
Tokens* remove_token(Tokens* tableTok)
{
    Tokens* aux = tableTok->next;
 
    free(tableTok);
 
    tableTok = aux;
 
    return tableTok;
}


