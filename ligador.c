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
2.− Constroir tabela com fatores de correcao
3.− Constroir tabela global de definicoes, utilizando os fatores de correcao.
4.− Para cada objeto:
4.1.− Corrigir os enderecos das entradas da tabela de uso, utilizando a tabela global de definicoes
4.2.− Corrigir os enderecos do c ́odigo usando os fatores de correcao
4.3.− Corrigir os enderecos relativos usando os fatores de correcao
5.− Gerar codigo executavel e salvar em arquivo
*/

#include "ligador.h"
#include "tabelas.h"

int main(int argc, char** argv){

    char endData[10];
    char* nomeArquivo;
    int endA = 0;
    int endB = 0;
    FILE *moda;
    FILE *modb;
    FILE *out;

    if( argc == 4){
      nomeArquivo = argv[1];
      moda = fopen(nomeArquivo, "r");
      nomeArquivo = argv[2];
      modb = fopen(nomeArquivo, "r");
      nomeArquivo = argv[3];
      out = fopen(nomeArquivo, "w");

      /*verifica erro na abertura do arquivo*/
      if((moda != NULL) && (modb != NULL) && (out != NULL)){

        /*Alinhamento codigos objeto*/
        while(fscanf(moda, "%s ", endData) != EOF){
            printf("%s ", endData);
            fprintf (out, "%s ", endData);
            //tamanho arquivo obj
            endA++;
        }
        fprintf(out, "||" );
        while(fscanf(modb, "%s ", endData) != EOF){
            printf("end %s ", endData);
            fprintf (out, "%s ", endData);
            //tamanho arquivo obj
            endB++;
        }
        fclose(out);

      }
      else{
        printf("ERRO: falha ao abrir arquivo\n\n");
      }
    }
    else{
      printf("ERRO: argumento(s) invalido(s).");
    }
    fclose(moda);
    fclose(modb);

    return 0;
}
