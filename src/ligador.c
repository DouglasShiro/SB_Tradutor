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

int main(int argc, char** argv){

    char* nomeArquivo;

    /*MODULOS*/
    FILE *modA;
    FILE *modB;
    FILE *out;

    /*TABELAS*/
    Tab_lst* TabUsoA = inicialize_tab_lst();
    Tab_lst* TabDefA = inicialize_tab_lst();
    // Tab_lst* TabRelA = inicialize_tab_lst();
    Tab_lst* TabUsoB = inicialize_tab_lst();
    Tab_lst* TabDefB = inicialize_tab_lst();
    // Tab_lst* TabRelB = inicialize_tab_lst();
    // Tab_lst* TabCodeA = inicialize_tab_lst();
    // Tab_lst* TabCodeB = inicialize_tab_lst();
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

            /*Leitura codigo objeto A e B
                - geracao tabelas de uso e definicao
                - leitura enderecos relativos
                - uniao codigos objetos modA e modB
            */
            Alinhar_codigo(modA, modB, out, &TabUsoA, &TabDefA, &TabUsoB, &TabDefB, &TabGlobalDef);

            fclose(out);
        }else{
            printf("ERRO: falha ao abrir arquivo\n\n");
        }
    }else{
        printf("ERRO: argumento(s) invalido(s).");
    }

    printf("\nTab uso MODA\n");
    print_tab_lst(TabUsoA);
    printf("\nTab def MODA\n");
    print_tab_lst(TabDefA);
    printf("\nTab uso MODB\n");
    print_tab_lst(TabUsoB);
    printf("\nTab def MODB\n");
    print_tab_lst(TabDefB);
    printf("\nTab global\n");
    print_tab_lst(TabGlobalDef);
    //printf("\nTab rel MODA\n");
    // print_tab_lst(TabRelA);
    // printf("\nTab rel MODB\n");
    //print_tab_lst(TabRelB);
    // printf("\nFator de correcao A\n");
    // printf("%d", fatorCorrecao[0]);
    // printf("\nFator de correcao B\n");
    // printf("%d", fatorCorrecao[1]);


    fclose(modA);
    fclose(modB);

    return 0;
}

void Alinhar_codigo(FILE* modA, FILE* modB, FILE* out, Tab_lst **TabUsoA, Tab_lst **TabDefA, Tab_lst **TabUsoB, Tab_lst **TabDefB, Tab_lst **TabGlobalDef){

  int end;
  int dado;
  int fatorCorrecao[2];

  char rotulo[10];
  char secao[10];

  /*MODULO A*/
  fscanf(modA, "%s", secao);
  //printf("%s", secao);
  if(strcmp(secao, "TABLE") == 0){
      /*TABLE USE*/
      fscanf(modA, "%s", secao);
      //printf(" %s", secao);
      while(strcmp(secao, "TABLE") != 0){
           /*salva dado da tabela de uso*/
           fscanf(modA, "%s", secao);
           if(strcmp(secao, "TABLE") != 0){
             strcpy(rotulo, secao);
             fscanf(modA, "%d", &dado);
             *TabUsoA = insert_tab_lst(*TabUsoA, rotulo, dado);
            //  printf("\nrot :%s", rotulo);
            //  printf("\ndado:%d", dado);
           }
      }

      fscanf(modA, "%s", secao);
      // printf("\n\n%s", secao);
      /*TABLE DEFINITION*/
      fatorCorrecao[0] = 0;
      while(strcmp(secao, "RELATIVE") != 0){
           /*salva dado da tabela de uso*/
           fscanf(modA, "%s", secao);
           //printf("secao: %s", secao);
           if(strcmp(secao, "RELATIVE") != 0){
             strcpy(rotulo, secao);
             fscanf(modA, "%d", &dado);
             *TabDefA = insert_tab_lst(*TabDefA, rotulo, dado);
            //  printf("\nrot : %s", rotulo);
            //  printf("\ndado: %d", dado);
            *TabGlobalDef = insert_tab_lst(*TabGlobalDef, rotulo, (dado+fatorCorrecao[0]));
           }
      }

      // printf("\n\nRELATIVE");
      /*RELATIVE*/
      while(strcmp(secao, "CODE") != 0){
          /*salva dado dos enderecos relativos*/
          fscanf(modA, "%s ", secao);
          if(strcmp(secao, "CODE") != 0){
            dado = atoi(secao);
            // TabRelA = insert_tab_lst(TabRelA, "endRel", dado);
            //printf("\ndado: %d", dado);
          }
      }

      // printf("\n\nCODE");
      /*CODE*/
      while(fscanf(modA, "%d", &dado)!= EOF){
          /*salva dado da secao code*/
          fprintf(out, "%d ", dado);
          //printf("\ndado: %d", dado);
          fatorCorrecao[1]++;
      }
      // printf("\nend: %d\n", fatorCorrecao[1]);

      /*MODULO B*/
      fscanf(modB, "%s", secao);
      //printf("%s", secao);
      if(strcmp(secao, "TABLE") == 0){
          /*TABLE USE*/
          fscanf(modB, "%s", secao);
          //printf(" %s", secao);
          while(strcmp(secao, "TABLE") != 0){
               /*salva dado da tabela de uso*/
               fscanf(modB, "%s", secao);
               if(strcmp(secao, "TABLE") != 0){
                 strcpy(rotulo, secao);
                 fscanf(modB, "%d", &dado);
                 *TabUsoB = insert_tab_lst(*TabUsoB, rotulo, dado);
                //  printf("\nrot :%s", rotulo);
                //  printf("\ndado:%d", dado);
               }
          }

          fscanf(modB, "%s", secao);
          // printf("\n\n%s", secao);
          /*TABLE DEFINITION*/
          //fatorCorrecao[0] = 0;
          while(strcmp(secao, "RELATIVE") != 0){
               /*salva dado da tabela de uso*/
               fscanf(modB, "%s", secao);
               //printf("secao: %s", secao);
               if(strcmp(secao, "RELATIVE") != 0){
                 strcpy(rotulo, secao);
                 fscanf(modB, "%d", &dado);
                 *TabDefB = insert_tab_lst(*TabDefB, rotulo, dado);
                //  printf("\nrot : %s", rotulo);
                //  printf("\ndado: %d", dado);
                *TabGlobalDef = insert_tab_lst(*TabGlobalDef, rotulo, (dado+fatorCorrecao[1]));
               }
          }

          // printf("\n\nRELATIVE");
          /*RELATIVE*/
          while(strcmp(secao, "CODE") != 0){
              /*salva dado dos enderecos relativos*/
              fscanf(modB, "%s ", secao);
              if(strcmp(secao, "CODE") != 0){
                dado = atoi(secao);
                // TabRelB = insert_tab_lst(TabRelB, "endRel", dado);
                //printf("\ndado: %d", dado);
              }
          }

          // printf("\n\nCODE");
          /*CODE*/
          end = 0;
          while(fscanf(modB, "%d", &dado)!= EOF){
              /*Verifica se endereco e relativo*/
              /*salva dado da secao code*/
              fprintf(out, "%d ", dado);
              end++;
              //printf("\ndado: %d", dado);
          }
          // printf("\nend: %d\n", fatorCorrecao[1]);
        }

  }else{
      printf("arquivo nao possui outro modulo para ser ligado\n\n");
  }

  return;
}
