/* PROGRAMA LIGADOR
* <COMANDO> ./ligador.c <entrada_1.o entrada_2.o >saida.e
*
*   Grupo:  Andressa Valadares          13/0042943
*           Douglas Shiro Yokoyama      13/0024902
*/

/*
TASK -
ler tabela uso
ler tabela definicao
gerar tabela de fatores de correcao
criar tabela global de definicoes

1.− Alinha os codigos objeto a serem ligados OK
2.− Construir tabela com fatores de correcao
3.− Construir tabela global de definicoes, utilizando os fatores de correcao.
4.− Para cada objeto:
4.1.− Corrigir os enderecos das entradas da tabela de uso, utilizando a tabela global de definicoes
4.2.− Corrigir os enderecos do codigo usando os fatores de correcao
4.3.− Corrigir os enderecos relativos usando os fatores de correcao
5.− Gerar codigo executavel e salvar em arquivo
*/

#include "ligador.h"
#include "tabelas.h"

int main(int argc, char** argv){

    char rotulo[10];
    char secao[10];
    char* nomeArquivo;

    int dado;
    int fatorCorrecao = 0;

    /*MODULOS*/
    FILE *modA;
    FILE *modB;
    FILE *out;

    /*TABELAS*/
    Tab_lst* FatorCorrecao;
    Tab_lst* TabGlobalDef;
    Tab_lst* TabUso;
    Tab_lst* TabDef;
    Tab_lst* TabRel;

    if( argc == 4){
        nomeArquivo = argv[1];
        modA = fopen(nomeArquivo, "r");
        nomeArquivo = argv[2];
        modB = fopen(nomeArquivo, "r");
        nomeArquivo = argv[3];
        out = fopen(nomeArquivo, "w");

        /*verifica erro na abertura do arquivo*/
        if((modA != NULL) && (modB != NULL) && (out != NULL)){

            /*Leitura codigo objeto A e B
                - geracao tabelas de uso e definicao
                - leitura enderecos relativos
                - uniao codigos objetos modA e modB
            */


            fscanf(modA, "%s", secao);
            //printf("%s", secao);
            if(strcmp(secao, "TABLE") == 0){
                /*TABLE USE*/
                //TabUso = inicialize_tab_lst();
                fscanf(modA, "%s", secao);
                printf(" %s", secao);
                while(strcmp(secao, "TABLE") != 0){
                     /*salva dado da tabela de uso*/
                     fscanf(modA, "%s", secao);
                     if(strcmp(secao, "TABLE") != 0){
                       strcpy(rotulo, secao);
                       fscanf(modA, "%d", &dado);
                       //TabUso = insert_tab_lst(TabUso, rotulo, dado);
                       printf("\nrot :%s", rotulo);
                       printf("\ndado:%d", dado);
                     }
                }

                printf("\n");
                fscanf(modA, "%s", secao);
                printf("\n\n%s", secao);
                /*TABLE DEFINITION*/
                //TabDef = inicialize_tab_lst();
                while(strcmp(secao, "RELATIVE") != 0){
                     /*salva dado da tabela de uso*/
                     fscanf(modA, "%s", secao);
                     //printf("secao: %s", secao);
                     if(strcmp(secao, "RELATIVE") != 0){
                       strcpy(rotulo, secao);
                       fscanf(modA, "%d", &dado);
                       //TabDef = insert_tab_lst(TabDef, rotulo, dado);
                       printf("\nrot : %s", rotulo);
                       printf("\ndado: %d", dado);
                     }
                }

                printf("\n\nRELATIVE");
                /*RELATIVE*/
                //TabRel = inicialize_tab_lst();
                while(strcmp(secao, "CODE") != 0){
                    /*salva dado dos enderecos relativos*/
                    fscanf(modA, "%s ", secao);
                    if(strcmp(secao, "CODE") != 0){
                      dado = atoi(secao);
                      //TabRel = insert_tab_lst(TabRel, NULL, dado);
                      printf("\ndado: %d", dado);
                    }
                }
                printf("\n\nCODE");
                /*CODE*/
                while(fscanf(modA, "%d", &dado)!= EOF){
                    /*salva dado da secao code*/

                    printf("\ndado: %d", dado);
                    fatorCorrecao++;
                }
                printf("\nend: %d\n", fatorCorrecao);

            }else{
                printf("arquivo nao possui outro modulo para ser ligado\n\n");
            }
            fclose(out);
        }else{
            printf("ERRO: falha ao abrir arquivo\n\n");
        }
    }else{
        printf("ERRO: argumento(s) invalido(s).");
    }
    fclose(modA);
    fclose(modB);

    return 0;
}
