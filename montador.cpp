#include "montador.hpp"

int main(int argc, char **argv)
{
	int tamanho,  i = 0;
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


	/* TABELA DE INSTRUCOES */
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

	/* TABELA DE DIRETIVAS */
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
			aux = ""; /* Zera a string auxiliar*/
			
			/* Separa as linhas em tokens tendo espaco como delimitadores*/
			while(ss >> aux){
				transform(aux.begin(), aux.end(), aux.begin(), ::toupper);
				/* Caso seja um comnetario */
				if(aux[0] == ';'){
					break;
				}
				else{			
					token.push_back(aux);
				}
			}
			token.push_back("\n");			
		}
					
	/* ja pre processou, -- falta if equ*/
	
	primeira_passagem(opTable, dirTable,  token, simbTable, defTable, useTable);	
	
	}
	
	return 0;
}



int primeira_passagem(map<string,string> opTable, map<string,int> dirTable,
						vector<string> token,	map<string,int> simbTable,
						unordered_map<string,int> defTable, unordered_map<string,int>useTable){

	char c;
	int existe = 0,  posCount = 0, lineCount = 1, 
		begin = 0, end = 0;
	string rotulo, str, valor;
	map<string,int>::iterator it;
	map<string,string>::iterator itOp;
	unordered_map<string,int>::iterator itMod;
	vector<string>::iterator itVec;
	
	for(string::size_type j = 0; j < token.size(); ++j){
						
		/* Verifica se eh um rotulo */				
		if(token[j].find(":") != string::npos){
			
			rotulo = token[j].substr(0, token[j].length()-1);
			if(existe == 1){
				
				cout << "ERRO SINTATICO:" << lineCount << ": Rótulo depois de um rótulo: "<< rotulo << "\n";
				_erro = TRUE;
			}
			
			if(token_valido(rotulo)){
				
				it = simbTable.find(rotulo);
				if(it != simbTable.end()){
					
					cout << "ERRO SEMANTICO:" << lineCount << ": Rótulo redefinido: \"" << rotulo << "\"\n";
					_erro = TRUE;
				}
				else if(existe == 0){
					simbTable[rotulo] = posCount;	
				}
				
				existe = 1;
					
			}
			else{
				
				cout << "ERRO LEXICO:" << lineCount << ": Token inválido: \"" << token[j] << "\"\n";
				_erro = TRUE;
			}
			
			
		}
		/* Verifica se eh um operacao */
		else if((itOp = opTable.find(token[j])) != opTable.end()){
		/* Verifica se a instrucao esta na secao TEXT*/
			if(_sectionText > posCount)	{
					cout << "ERRO SEMANTICO:" << lineCount << ": A operação está fora da seção de texto \n";
					_erro = TRUE;
				
			} /* Verifica de a instrucao esta na secao DATA*/
			else if((_sectionData < posCount) && (_sectionData != -1)){
					cout << "ERRO SEMANTICO:" << lineCount << ": A operação está fora da seção de texto \n";
					_erro = TRUE;
				
			}
			
			/* Caso seja COPY-> tam 3*/
			if(!itOp->first.compare("COPY")){
				posCount += 3;
				cout << itOp->first << "\n";
			}/* Caso seja STOP -> tam 1*/
			else if(!itOp->first.compare("STOP")){
				posCount += 1;
			} /* Todas as outras tam = 2*/
			else{
				posCount += 2;
			}
			existe = 0;
			
		}
		/* Verifica se eh uma diretiva */				
		else if((it = dirTable.find(token[j])) != dirTable.end()){
			/* Diretiva SPACE */		
			if(!it->first.compare("SPACE")){
				str = token[++j];
				int space = 0;
		
				/* verifica se o space esta dentro da secao de dados*/
				if(_sectionData > posCount)
				{
					cout << "ERRO SEMANTICO:" << lineCount << ": Diretiva SPACE está fora da seção de dados \n";
					_erro = TRUE;
				
				}
				/* Verifica se a secao de dados existe*/
				if(_sectionData == -1)
				{
					cout << "ERRO SEMANTICO: SECTION DATA faltante \n";
					_erro = TRUE;
				}
				
				for (string::size_type i = 0; i < str.length(); ++i)
				{
					c=str[i]; 
					
					if(!isdigit(c)){
					
						cout << "ERRO SINTATICO:" << lineCount << ": Operando inválido: \"" << str << "\"\n";
						_erro = TRUE;
						i = str.length();
					}else{
						space = space *10 + (c -'0');
					}

				}
				posCount+= space;
			}
			/* Diretiva CONST */
			else if(!it->first.compare("CONST")){
				str = token[++j];
				
				/* verifica se o const esta dentro da secao de dados*/
				if(_sectionData > posCount)
				{
					cout << "ERRO SEMANTICO:" << lineCount << ": Diretiva SPACE está fora da seção de dados \n";
					_erro = TRUE;
				
				}
				/* Verifica se a secao de dados existe*/
				if(_sectionData == -1)
				{
					cout << "ERRO SEMANTICO: SECTION DATA faltante \n";
					_erro = TRUE;
				}
		
				for (string::size_type i = 0; i < str.length(); ++i)
				{
					c=str[i]; 	
					if(!isdigit(c)){
					
						cout << "ERRO SINTATICO:" << lineCount << ": Operando inválido: \"" << str << "\"\n";
						_erro = TRUE;
						i = str.length();
					}

				}
				
				posCount+= 1;	
			}
			/* Diretiva EXTERN */
			else if(!it->first.compare("EXTERN")){
				
				rotulo = token[j-1].substr(0, token[j-1].length()-1);
				if((it = simbTable.find(rotulo)) != simbTable.end()){
					
					rotulo = rotulo + "#";
					simbTable[rotulo] = 0;
					simbTable.erase(it);
					
				}
				else{
					
					cout << "ERRO SINTATICO:" << lineCount << ": Rótulo não identificado \"" << rotulo << "\"\n";
					_erro = TRUE;

				}
				
			}
			/* Diretiva PUBLIC */
			else if(!it->first.compare("PUBLIC")){
				rotulo = token[++j];
				/* Verifica se o rotulo ja foi definido */	
				if((itMod = defTable.find(rotulo)) != defTable.end()){
					
					cout << "ERRO SEMANTICO:" << lineCount << ": Rótulo redefinido \"" << rotulo << "\"\n";
					_erro = TRUE;
					
					
				}
				else{
					
					defTable[rotulo] = -1;

				}
			}
			/* Diretiva EQU */
			else if(!it->first.compare("EQU")){
				valor = "";
				valor = token[j+1];	
				 rotulo = token[j-1].substr(0, token[j-1].length()-1);
				 if((it = simbTable.find(rotulo)) != simbTable.end()){
					
					 simbTable.erase(it);
					 if(find(token.begin(), token.end(), rotulo) != token.end())
					 {
						 cout << "valor:	\"" << valor << "\"	rotulo:	\""<< rotulo << "\"\n";
						 replace(token.begin(), token.end(), rotulo, valor);
						 		
					 }
										
				}
				else{
					cout << "ERRO SINTATICO:" << lineCount << ": Rótulo não definido\n";
					_erro = TRUE;					
				}
			}
			/* Diretiva BEGIN */
			else if(!it->first.compare("BEGIN")){
				begin = 1;				
			}
			/* Diretiva END */
			else if(!it->first.compare("END")){
				end = 1;
				if(begin == 0)
				{
					cout << "ERRO SEMANTICO: Diretiva END declarada sem BEGIN\n";
					_erro = TRUE;
				}
			}
			/* Diretiva SECTION */
			else if(!it->first.compare("SECTION")){
				string operando = token [++j];
				/* Verifica se o operando eh TEXT */
				if(!operando.compare("TEXT")){
					_sectionText = posCount;
				}
				/* Verifica se o operando eh DATA */
				else if(!operando.compare("DATA")){
					_sectionData = posCount;
				} /* Senao erro */
				else{
					cout << "ERRO SINTATICO:"<< lineCount <<":"<< posCount <<": Operando inválido, deve ser TEXT ou DATA\n";
					_erro = TRUE;					
				}
			
			}
			/* Diretiva IF */
			else if(!it->first.compare("IF")){
				rotulo = "";
				rotulo = token[j+1];
				/* Se o rotulo existe na tabela de simbolos */			
				if( simbTable.find(rotulo) != simbTable.end()){
					
					cout << "ERRO SEMANTICO:" << lineCount << ": Rótulo redefinido: \"" << rotulo << "\"\n";
					_erro = TRUE;
				}
				else if(!valor.compare("\n")){
					
					cout << "ERRO SINTATICO:" << lineCount << ": IF deve possuir um rotulo válido\n";
					_erro = TRUE;					
				}
				else{
					
					if(!valor.compare("0"))
					{
						
						cout << "VALORIF: "<< rotulo << "\n";
						cout<< "SOME LINHA\n";
					}
					
				}
					
				
				
			}
						
			existe = 0;
		
		}
		str = "";
		/* Conta as linhas do programa */
		if(token[j] == "\n"){
			lineCount++;
		}
	}
	
	/* Verifica erro do BEGIN sem END */
	if((begin == 1) && (end == 0)){
		
		cout << "ERRO SEMANTICO: Diretiva BEGIN declarada sem END\n";
		_erro = TRUE;
	}
	
	/* Verifica se o STOP existe */
	if((begin == 0) && (end == 0)){
		if(find(token.begin(), token.end(), "STOP") == token.end()){
							
			cout << "ERRO SEMANTICO: o programa não possui nenhuma instrução STOP\n";
			_erro = TRUE;
		}
	}

	/* Verifica se a SECTION TEXT existe */
	if(_sectionText == -1)
	{
		cout << "ERRO SEMANTICO: SECTION TEXT faltante \n";
					_erro = TRUE;
	}
	
	cout << "****************	TABELA DE DEFINIÇÕES	****************\n";
		for (itMod=defTable.begin(); itMod!=defTable.end(); ++itMod)
			cout << posCount << ": " << itMod->first << "=>" << itMod->second<<"\n";
		
	cout << "****************	TABELA DE USO		****************\n";
		for (itMod=useTable.begin(); itMod!=useTable.end(); ++it)
			cout << posCount << ": " << itMod->first << "=>" << itMod->second<<"\n";
		
	cout << "****************	TABELA DE SIMBOLOS	****************\n";
		for (it=simbTable.begin(); it!=simbTable.end(); ++it)
			cout << posCount << ": " << it->first << "=>" << it->second<<"\n";
		


	for(vector<string>::iterator it = token.begin(); it != token.end(); it++)
		cout << *it;
	
	return 0;
}

/* VERIFICA SE O TOKEN POSSUI CARACTERES VALIDOS*/
int token_valido(string token)
{
	int i = 0;
	/* Se o primeiro caractere eh um digito -> ERRO*/
	if(isdigit(token[0]))
	{
		return false;
	}
	while(token[i] != '\0')
	{		/* Se o nao e numero, letra ou underscore -> ERRO*/	
			if(!isalnum(token[i]) && !(token[i] == '_'))
			{
					return false;
			}
		i++;
	}
	
return true; 	
}





