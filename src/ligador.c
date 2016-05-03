/* PROGRAMA LIGADOR
* <COMANDO> ./ligador.c <entrada_1.o entrada_2.o >saida.e
*
*   Grupo:  Andressa Valadares          13/0042943
*           Douglas Shiro Yokoyama      13/0024902
*/

/*
TASK -
ler tabela uso OK
ler tabela definicao OK
gerar tabela de fatores de correcao OK
criar tabela global de definicoes OK

1.− Alinha os codigos objeto a serem ligados OK
2.− Construir tabela com fatores de correcao OK
3.− Construir tabela global de definicoes, utilizando os fatores de correcao. OK
4.− Para cada objeto:
4.1.− Corrigir os enderecos das entradas da tabela de uso, utilizando a tabela global de definicoes
4.2.− Corrigir os enderecos do codigo usando os fatores de correcao
4.3.− Corrigir os enderecos relativos usando os fatores de correcao
5.− Gerar codigo executavel e salvar em arquivo
*/

#include "ligador.h"

int main(int argc, char** argv){

    //int fatorCorrecao[2];
    int fatCorAux = 0;

    char* nomeArquivo;

    /*MODULOS*/
    FILE *modA;
    FILE *modB;
    FILE *out;

    /*TABELAS*/
    Tab_lst* TabUsoA = inicialize_tab_lst();
    Tab_lst* TabDefA = inicialize_tab_lst();
    Tab_lst* TabUsoB = inicialize_tab_lst();
    Tab_lst* TabDefB = inicialize_tab_lst();
    Tab_lst* TabGlobalDef = inicialize_tab_lst();

    if( argc == 4){
        nomeArquivo = argv[1];
        modA = fopen(nomeArquivo, "r");
        nomeArquivo = argv[2];
        modB = fopen(nomeArquivo, "r");
        nomeArquivo = argv[3];
        out = fopen(nomeArquivo, "w");

        /*verifica erro na abertura do arquivo*/
        if((modA != NULL) && (modB != NULL) && (out != NULL)){

            /*Leitura do codigo objeto dos modulos A e B
                - geracao tabelas de uso e definicao
                - leitura enderecos relativos
                - uniao codigos objetos modA e modB
            */
            //fatorCorrecao[0] = 0;
            ler_objeto(modA, &TabUsoA, &TabDefA, &TabGlobalDef, &fatCorAux, 0);
            //fatorCorrecao[1] = fatCorAux;
            ler_objeto(modB, &TabUsoB, &TabDefB, &TabGlobalDef, &fatCorAux, 1);

            rewind(modA);
            rewind(modB);

            corrige_endereco(modA, out, TabUsoA, TabGlobalDef);
            corrige_endereco(modB, out, TabUsoB, TabGlobalDef);

            fclose(out);
        }else{
            printf("ERRO: falha ao abrir arquivo\n\n");
        }
    }else{
        printf("ERRO: argumento(s) invalido(s).");
    }

    // printf("\nTab uso MODA\n");
    // print_tab_lst(TabUsoA);
    // printf("\nTab def MODA\n");
    // print_tab_lst(TabDefA);
    // printf("\nTab uso MODB\n");
    // print_tab_lst(TabUsoB);
    // printf("\nTab def MODB\n");
    // print_tab_lst(TabDefB);
    // printf("\nTab global\n");
    // print_tab_lst(TabGlobalDef);

    fclose(modA);
    fclose(modB);

    return 0;
}

void ler_objeto(FILE* modulo, Tab_lst **TabUso, Tab_lst **TabDef, Tab_lst **TabGlobalDef, int *fatCorAux, int i){
  int dado;

  char rotulo[10];
  char secao[10];

  /*LEITURA DAS SECOES DO CODIGO OBJ DO MODULO*/
  fscanf(modulo, "%s", secao);
  if(strcmp(secao, "TABLE") == 0){

      /*TABLE USE*/
      fscanf(modulo, "%s", secao);
      while(strcmp(secao, "TABLE") != 0){
        fscanf(modulo, "%s", secao);
           /*salva dado na tabela de uso*/
           if(strcmp(secao, "TABLE") != 0){
              strcpy(rotulo, secao);
              fscanf(modulo, "%d", &dado);
              *TabUso = insert_tab_lst(*TabUso, rotulo, dado);
           }
      }

      fscanf(modulo, "%s", secao);

      /*TABLE DEFINITION*/
      while(strcmp(secao, "RELATIVE") != 0){
           /*salva dado da tabela de uso*/
           fscanf(modulo, "%s", secao);
           if(strcmp(secao, "RELATIVE") != 0){
                strcpy(rotulo, secao);
                fscanf(modulo, "%d", &dado);
                *TabDef = insert_tab_lst(*TabDef, rotulo, dado);
                *TabGlobalDef = insert_tab_lst(*TabGlobalDef, rotulo, (dado+(*fatCorAux)));
           }
      }

      /*RELATIVE*/
      while(strcmp(secao, "CODE") != 0){
          /*salva dado dos enderecos relativos*/
          fscanf(modulo, "%s ", secao);
          if(strcmp(secao, "CODE") != 0){
              dado = atoi(secao);
          }
      }

      /*CODE*/
      while(fscanf(modulo, "%d", &dado) != EOF){
          *fatCorAux+=1;
      }

    }
return;
}

void corrige_endereco(FILE* modulo, FILE* out, Tab_lst* TabUso, Tab_lst* TabGlobalDef){

    int end = 0;
    int dado;
    char secao[10];
    Tab_lst* TabUsoAux = TabUso;

    fscanf(modulo, "%s", secao);
    while(strcmp(secao, "CODE") != 0){
      fscanf(modulo, "%s", secao);
      //printf("%s", secao);
    }
    //printf("%s", secao);
    while(fscanf(modulo, "%d", &dado) != EOF){
        //printf("\ndado : %d\n", dado);
        if(TabUsoAux != NULL){
            if(TabUsoAux->valor == end){
              dado += procura_tab_lst(TabGlobalDef, TabUsoAux->simb);
              TabUsoAux = TabUsoAux->next;
            }
        }
        end++;
        /*salva dado da secao code*/
        fprintf(out, "%d ", dado);
    }

    return;
}
