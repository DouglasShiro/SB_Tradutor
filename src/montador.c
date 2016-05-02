/* PROGRAMA MONTADOR
*  <COMANDO> ./montador.c <entrada.asm > saida.o 
*
*   Grupo:  Andressa Valadares          13/0042943
*           Douglas Shiro Yokoyama      13/0024902
*/

/*
 * TO DO list:
 * ->Primeira passagem
 * 1. Verificar se é um token valido
 * 2. Salvar rotulos nas tabelas de diretivas
 * 3. Verificar se esse rotulo ja existe, se sim ERRO
 * 
 * */


#include "montador.h"

int main(int argc, char **argv)
{
	Tokens* tableTok = inicialize_token();
	Tab_lst* tableSimb = inicialize_tab_lst();
	Tab_lst* tableDef = inicialize_tab_lst();
	Tab_lst* tableUso = inicialize_tab_lst();
	Tab_hash* tableDI = inicialize_tab_hash();
	
	inicialize_dir_inst(tableDI);
	
	if (argc != 3)
	{
		printf("Erro: numero invalido de argumentos\n");
	}
	else 
	{
		tableTok = pre_processor(argv[1], tableTok); /* Le arquivo de entrada */
		print_tokens(tableTok);
		//primeira_passagem(tableTok, tableSimb);
	}
	
	while(tableTok != NULL) /*Libera a memoria alocada*/
		tableTok = remove_token(tableTok);
	
	while(tableSimb != NULL) /*Libera a memoria alocada*/
		tableSimb = remove_tab_lst(tableSimb);
	
	
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
	strcat(nameFile, ".asm");
	pFile = fopen (nameFile,"r");
	
	if (pFile ==  NULL) /* Verifica se o arquivo existe*/
	{
		perror ("The following error occurred");
		return NULL; /*Não tem lista de tokens, erro ao abrir o arquivo*/
	}
	
	printf("VOU LER O ARQUIVO!\n");
    while (((c = fgetc(pFile)) != EOF) && i < 100)
    {	
		c = toupper(c); /* Converte todods os caracteres para maiusculo */
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
    if(i >= 100)
		printf("ERROR:%d: token ultrapassa o limite maximo de caracteres\n", nLine);
		
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

/*
int primeira_passagem(Tokens* tableTok, Tab_lst* tableSimb, Tab_lst* tableDir, Tab_lst* tableDef, Tab_lst* tableUso)
{
	
	Tokens* aux = tableTok;
	
	for(aux = tableTok; aux != NULL; aux = aux->next)
	{
		token_valido(aux->tok, aux->line);
	}
	return 0;
}
*/
int token_valido(char* token, int nLine)
{
	int i = 0;
		
	if(isdigit(token[0]))
	{
		printf("ERRO LEXICO:%d: %s\n", nLine, token);
	}
	while(token[i] != '\0')
	{
			if(!isalnum(token[i]) && !(token[i] == '_'))
			{
					printf("ERRO LEXICO:%d: %s\n", nLine, token);
			}
		i++;
	}
	
return 0; 	
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






