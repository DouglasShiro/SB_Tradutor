#include "montador.hpp"

#define TRUE 1
#define FALSE 0

int _erro;

int main(int argc, char **argv)
{
	int tamanho,  i = 0;
	char c;
	locale loc;
	ifstream inFile;
	string  nameFile(argv[1]);
	string line, str, aux;
	vector<string> arq, token;
	map<string,int> simbTable;
	unordered_map<string,int> defTable;
	unordered_map<string,int> useTable;
	map<string,string> opTable;
	map<string,int> dirTable;

	opTable["ADD"] = "01";
	opTable["SUB"] = "02";
	opTable["MULT"] = "03";
	opTable["DIV"] = "04";
	opTable["JMP"] = "05";
	opTable["JMPN"] = "06";
	opTable["JMPP"] = "07";
	opTable["JMPZ"] = "08";
	opTable["COPY"] = "09";
	opTable["LOAD"] = "10";
	opTable["STORE"] = "11";
	opTable["INPUT"] = "12";
	opTable["OUTPUT"] = "13";
	opTable["STOP"] = "14";

	dirTable["SECTION"] = 1;
	dirTable["SPACE"] = 1;
	dirTable["CONST"] = 1;
	dirTable["PUBLIC"] = 0;
	dirTable["EQU"] = 1;
	dirTable["IF"] = 1;
	dirTable["EXTERN"] = 0;
	dirTable["BEGIN"] = 0;
	dirTable["END"] = 0;


	if (argc != 3)
	{
		printf("Erro: numero invalido de argumentos\n");
	}
	else
	{
		nameFile = nameFile + ".asm";
		inFile.open(nameFile.c_str());	// Abre o arquivo
		/*  Insere cada linha no vetor string arq */
		while(getline(inFile,line)){
			arq.push_back(line);
			i++; /* numero de linhas */
		}
		inFile.close();

		tamanho = i; /* tamanho = numero de linhas */
		for(i = 0; i < tamanho ;i++){
			str = arq[i]; /* atribuo a str a string arq[i]- linha inteira do arquivo */
			istringstream ss(str);

			aux = "";
			while(ss >> aux){
				if(aux[0] == ';')
				{
					break;
				}else{
					token.push_back(aux);
				}
			}
			token.push_back("\n");



		}
			for(vector<string>::iterator i = token.begin(); i < token.end(); i++)
				cout << *i;


	/* ja pre processou, -- falta if equ*/

	primeira_passagem(opTable, dirTable,  token, simbTable, defTable, useTable);

  //segunda_passagem(opTable, dirTable,  token, simbTable, defTable, useTable);

	}

	return 0;
}



int primeira_passagem(map<string,string> opTable, map<string,int> dirTable,
						vector<string> token,	map<string,int> simbTable,
						unordered_map<string,int> defTable, unordered_map<string,int>useTable){

	char c;
	int existe = 0,  posCount = 0, lineCount = 1, ok = 1,  k = 0;
	string rotulo, str;
	map<string,int>::iterator it;
	map<string,string>::iterator itOp;
	vector<string>::iterator itVec;

	for(string::size_type j = 0; j < token.size(); ++j){
		/* Verifica se eh um rotulo */
		if(token[j].find(":") != string::npos){

			rotulo = token[j].substr(0, token[j].length()-1);
			if(existe == 1){

				cout << "ERRO SINTATICO:" << lineCount << ": Rótulo depois de um rótulo: "<< rotulo << "\n";
			}
			else if(token_valido(rotulo)){

				it = simbTable.find(rotulo);
				existe = 1;
				if(it != simbTable.end()){

					cout << "ERRO SEMANTICO:" << lineCount << ": Rótulo redefinido: \"" << rotulo << "\"\n";
				}
				else{
					simbTable[rotulo] = posCount;
				}

			}
			else{

				cout << "ERRO LEXICO:" << lineCount << ": Token inválido: \"" << token[j] << "\"\n";
			}


		}/* Verifica se eh um operacao */
		else if((itOp = opTable.find(token[j])) != opTable.end()){

			if(!itOp->first.compare("COPY")){
				posCount += 3;
				cout << itOp->first << "\n";
			}
			else if(!itOp->first.compare("STOP")){
				posCount += 1;
			}
			else{
				posCount += 2;
			}
			existe = 0;

		}
		/* Verifica se eh uma diretiva */
		else if((it = dirTable.find(token[j])) != dirTable.end()){

			if(!it->first.compare("SPACE")){

				str = token[++j];
				for (string::size_type i = 0; i < str.length(); ++i)
				{
					c=str[i];

					if(!isdigit(c)){

						cout << "ERRO SINTATICO:" << lineCount << ": Operando inválido: \"" << str << "\"\n";
						i = str.length();
					}

				}
				string::size_type sz;
				//posCount+= stoi(str, &sz);
			}
			else if(!it->first.compare("CONST")){

				str = token[++j];
				for (string::size_type i = 0; i < str.length(); ++i)
				{
					c=str[i];
					if(!isdigit(c)){

						cout << "ERRO SINTATICO:" << lineCount << ": Operando inválido: \"" << str << "\"\n";
						i = str.length();
					}

				}

				posCount+= 1;
			}
			else if(!it->first.compare("EXTERN")){

				rotulo = token[j-1].substr(0, token[j-1].length()-1);
				if((it = simbTable.find(rotulo)) != simbTable.end()){

					rotulo = rotulo + "#";
					simbTable[rotulo] = 0;
					simbTable.erase(it);

				}
				else{

					cout << "ERRO SINTATICO:" << lineCount << ": Rótulo não identificado \"" << rotulo << "\"\n";

				}

			}
			else if(!it->first.compare("PUBLIC")){
			}

			existe = 0;

		}
		str = "";
		if(token[j] == "\n"){
			lineCount++;
		}
	}


		for (it=simbTable.begin(); it!=simbTable.end(); ++it)
			cout << posCount << ": " << it->first << "=>" << it->second<<"\n";


	return 0;
}

/* VERIFICA SE O TOKEN POSSUI CARACTERES VALIDOS*/
int token_valido(string token)
{
	int i = 0;

	if(isdigit(token[0]))
	{
		return false;
	}
	while(token[i] != '\0')
	{
			if(!isalnum(token[i]) && !(token[i] == '_'))
			{
					return false;
			}
		i++;
	}

return true;
}

int segunda_passagem(map<string,string> opTable, map<string,int> dirTable,
						vector<string> token,	map<string,int> simbTable,
						unordered_map<string,int> defTable, unordered_map<string,int>useTable)
{
    int posCount = 0, lineCount = 1;

		string memOp1, memOp2;
		string operando;

		map<string,int>::iterator it;
		map<string,string>::iterator itOp;
		printf("oi2");
		/*percorre arquivo fonte*/
    for(string::size_type j= 0; j < token.size(); ++j){
        /*Verifica se eh um rotulo, caso seja ignora*/
        if(token[j].find(":") != string::npos){
              //*Ignora token que eh rotulo*//

        }else if((itOp = opTable.find(token[j])) != opTable.end()){
            /*verifica OPERACOES*/

            if(!itOp->first.compare("COPY")){
                /*se operacao for COPY verifica os proximos 2 tokens
                  e verifica se sao operandos*/
                  memOp1 = token[j+1];
                  memOp2 = token[j+2];
                  if(memOp1.find(",") != string::npos){
                      memOp1 = memOp1.substr(0, memOp1.length()-1);
											if((it = simbTable.find(memOp1)) != simbTable.end()){
													if((it = simbTable.find(memOp2)) != simbTable.end()){
															/*se tudo der certo sexta tem mais um show*/
															posCount += 3;
															/*ARQUIVO SAIDA*/
													}else{
															_erro = TRUE;
															cout << "ERRO SEMANTICO:" << lineCount << ": Simbolo inexistente :\"" << memOp2 << "\"\n";
													}
											}else{
													_erro = TRUE;
													cout << "ERRO SEMANTICO:" << lineCount << ": Simbolo inexistente :\"" << memOp1 << "\"\n";
											}
                  }else{
											_erro = TRUE;
                      cout << "ERRO SINTATICO:" << lineCount << ": formato invalido: COPY A, B :\"" << memOp1 << "\"\n";
                  }

            } else if(!itOp->first.compare("STOP")){
                /*se operacao for STOP verifica proximo token para ver se STOP nao possui operandos*/
								if((it = simbTable.find(token[j+1])) != simbTable.end()){
										//*se tudo der certo sexta tem mais um show*/
										posCount += 0;
										/*ARQUIVO SAIDA*/
								}else{
									cout << "ERRO SINTATICO:" << lineCount << ": formato invalido: STOP :\"" << token[j+1] << "\"\n";
								}
            } else {
                /*se operacao nao for COPY ou STOP verifica o proximo token
                  e verifica se eh operando*/
									operando = token[j+1];
									if((it = simbTable.find(operando)) != simbTable.end()){
												/*se tudo der certo sexta tem mais um show*/
												posCount += 2;
												/*ARQUIVO SAIDA*/
									}else{
											_erro = TRUE;
											cout << "ERRO SEMANTICO:" << lineCount << ": Simbolo inexistente :\"" << operando << "\"\n";
									}
            }

        }else if((itOp = opTable.find(token[j])) != opTable.end()){
			 	/*Verifica DIREIVAS*/

				}
				if(token[j] == "\n"){
					lineCount++;
				}
    }

return 0;
}
