/* PROGRAMA LIGADOR
* <COMANDO> ./ligador.c entrada_1.o entrada_2.o saida.e
*
*   Grupo:  Andressa Valadares          13/0042943
*           Douglas Shiro Yokoyama      13/0024902
*/

#include "ligador.h"

int main(int argc, char** argv){

    int fatorCorrecao = 0;

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

        /*verifica erro ao abrir arquivos*/
        if((modA != NULL) && (modB != NULL) && (out != NULL)){

            /*Leitura do codigo objeto dos modulos A e B
                - geracao tabelas de uso e definicao
                - leitura enderecos relativos
                - uniao codigos objetos modA e modB*/

            ler_objeto(modA, &TabUsoA, &TabDefA, &TabGlobalDef, &fatorCorrecao, 0);
            ler_objeto(modB, &TabUsoB, &TabDefB, &TabGlobalDef, &fatorCorrecao, 1);

            /*Retornando ao inicio do arquivo*/
            rewind(modA);
            rewind(modB);

            /*Corrige e grava o codigo objeto no arquivo de saida*/
            corrige_endereco(modA, out, TabUsoA, TabGlobalDef);
            corrige_endereco(modB, out, TabUsoB, TabGlobalDef);

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

void ler_objeto(FILE* modulo, Tab_lst **TabUso, Tab_lst **TabDef, Tab_lst **TabGlobalDef, int *fatorCorrecao, int i){

  int dado;
  char rotulo[10];
  char secao[10];

  /*LEITURA DAS SECOES DO CODIGO OBJ DO MODULO*/
  fscanf(modulo, "%s", secao);
  if(strcmp(secao, "TABLE") == 0){

      /*SECAO TABLE USE*/
      fscanf(modulo, "%s", secao);
      while(strcmp(secao, "TABLE") != 0){
        fscanf(modulo, "%s", secao);
           /*salva simbolos e valores na tabela de uso*/
           if(strcmp(secao, "TABLE") != 0){
              strcpy(rotulo, secao);
              fscanf(modulo, "%d", &dado);
              *TabUso = insert_tab_lst(*TabUso, rotulo, dado);
           }
      }

      fscanf(modulo, "%s", secao);

      /*SECAO TABLE DEFINITION*/
      while(strcmp(secao, "RELATIVE") != 0){
           /*salva simbolos e enderecos na tabela de definicoes*/
           fscanf(modulo, "%s", secao);
           if(strcmp(secao, "RELATIVE") != 0){
                strcpy(rotulo, secao);
                fscanf(modulo, "%d", &dado);
                *TabDef = insert_tab_lst(*TabDef, rotulo, dado);
                *TabGlobalDef = insert_tab_lst(*TabGlobalDef, rotulo, (dado+(*fatorCorrecao)));
           }
      }

      /*SECAO RELATIVE*/
      while(strcmp(secao, "CODE") != 0){
          /*Percorre secao dos enderecos relativos*/
          fscanf(modulo, "%s ", secao);
          // if(strcmp(secao, "CODE") != 0){
          //     dado = atoi(secao);
          // }
      }

      /*SECAO CODE*/
      while(fscanf(modulo, "%d", &dado) != EOF){
          *fatorCorrecao+=1;
      }

    }
return;
}

void corrige_endereco(FILE* modulo, FILE* out, Tab_lst* TabUso, Tab_lst* TabGlobalDef){

    int end = 0; /*Inicializado como zero, pois indica o inicio do codigo obj*/
    int dado;
    char secao[10];
    Tab_lst* TabUsoAux = TabUso;

    /*Percorre modulo ate secao de CODE(codigo objeto)*/
    fscanf(modulo, "%s", secao);
    while(strcmp(secao, "CODE") != 0){
      fscanf(modulo, "%s", secao);
    }
    /*Faz a leitura do codigo objeto*/
    while(fscanf(modulo, "%d", &dado) != EOF){
        /*Verifica tabela de uso*/
        if(TabUsoAux != NULL){
            /*Verifica se o endereco do codigo obj esta na tabela de uso*/
            if(TabUsoAux->valor == end){
                /*Caso esteja este eh atualizado de acordo com o
                  valor presente na tabela global de definicoes*/
                dado += procura_tab_lst(TabGlobalDef, TabUsoAux->simb);
                TabUsoAux = TabUsoAux->next;
            }
        }
        end++;
        /*salva valor no arquivo de saida, que eh o codigo objeto ligado*/
        fprintf(out, "%d ", dado);
    }

    return;
}
