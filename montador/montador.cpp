#include "montador.hpp"

int main(int argc, char **argv)
{

	int tamanho,  i = 0;
	locale loc;
	ifstream inFile;
	ofstream outFile;

	string  in(argv[1]);
	string	out(argv[2]);
	string line, str, aux;
	/*ESTRUTURA DE DADOS*/
	vector<string> arq, token;
	map<string,int> simbTable;
	unordered_map<string,int> defTable;
	unordered_multimap<string,int> useTable;
	unordered_multimap<string,int> reverso;
	map<string,string> opTable;
	map<string,int> dirTable;
	vector<string> code;				/*vetor que ira receber os codigo objeto a ser salvo em arquivo*/
	vector<int> relativo;
	/*ITERADORES*/
	map<string,int>::iterator it;		/*iterador para tabelas formato string, int*/
	map<string,string>::iterator itOp;	/*iterador para tabelas formato string, string*/
	unordered_map<string,int>::iterator itMod;
	unordered_multimap<string,int>::iterator itUse;


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
		in = in + ".asm";
		inFile.open(in.c_str());	// Abre o arquivo
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

	/* Programa ja foi pre processado */
	/* Realiza a primeira passagem e verifica alguns erros de diretivas e definicoes */
	primeira_passagem(opTable, dirTable,  token, simbTable, defTable, useTable);

	/* Realiza a segunda passagem e cria o arquivo de saida */
	segunda_passagem(opTable, dirTable,  token, simbTable, defTable, useTable, code, relativo);

	/* Printa o programa com as alteracoes */
	for(vector<string>::iterator it = token.begin(); it != token.end(); it++)
		cout << *it << " ";
	}

	cout << "****************	TABELA DE SIMBOLOS	****************\n";
		for (it=simbTable.begin(); it!=simbTable.end(); ++it)
			cout << it->first << "=>" << it->second<<"\n";


	cout << "****************	TABLE USE		****************\n";
	for (itUse=useTable.begin(); itUse!=useTable.end(); ++itUse)
		cout << itUse->first << "=>" << itUse->second<<"\n";
	reverso.insert(useTable.begin(), useTable.end());
	 for (itUse=reverso.begin(); itUse!=reverso.end(); ++itUse)
	 	cout << itUse->first << "=>" << itUse->second<<"\n";

	cout << "****************	TABLE DEFINITION	****************\n";
	for (itMod=defTable.begin(); itMod!=defTable.end(); ++itMod){
		cout << itMod->first << "=>" << itMod->second<<"\n";
	}

	cout << "****************	RELATIVE	****************\n";
		for(vector<int>::iterator it = relativo.begin(); it != relativo.end(); it++){
			cout << *it << " ";
		}
		cout << "\n";

	cout << "****************	CODE	****************\n";
		for(vector<string>::iterator it = code.begin(); it != code.end(); it++){
			cout << *it << " ";
		}
		cout << "\n";
	/*ABRIR ARQUIVO SAIDA*/

	if(_erro == FALSE){
		out = out + ".o";
		outFile.open(out, fstream::out);	// Abre o arquivo

		/*IMPRIMIR ARQUIVO SAIDA*/
		outFile << "TABLE USE\n";
		reverso.insert(useTable.begin(), useTable.end());
		for (itUse=reverso.begin(); itUse!=reverso.end(); ++itUse)
			outFile << itUse->first << "\t" << itUse->second<<"\n";
		outFile << "\n";

		outFile << "TABLE DEFINITION\n";
		for (itMod=defTable.begin(); itMod!=defTable.end(); ++itMod)
			outFile << itMod->first << "\t" << itMod->second<<"\n";
		outFile << "\n";

		outFile << "RELATIVE\n";
		for(vector<int>::iterator it = relativo.begin(); it != relativo.end(); it++)
			outFile << *it << " ";
		outFile << "\n";
		outFile << "\n";

		outFile << "CODE\n";
		for(vector<string>::iterator it = code.begin(); it != code.end(); it++)
			outFile << *it << " ";
		outFile << "\n";
		outFile << "\n";

		/*FECHA ARQUIVO SAIDA*/
		outFile.close();
	}
	return 0;
}

/* Verifica se o token nao possui caracteres invalidos */
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

/* Primeira Passagem */
int primeira_passagem(map<string,string>& opTable, map<string,int>& dirTable,
						vector<string>& token,	map<string,int>& simbTable,
						unordered_map<string,int>& defTable, unordered_multimap<string,int>& useTable){

	char c;
	int existe = 0,  posCount = 0, lineCount = 1,
		begin = 0, end = 0;
	string rotulo, str, valor;
	map<string,int>::iterator it;
	map<string,string>::iterator itOp;
	unordered_map<string,int>::iterator itMod;
	unordered_multimap<string,int>::iterator itUse;
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


				/* Verifica se a secao de dados existe */
				if(_sectionData == -1)
				{
					cout << "ERRO SEMANTICO: SECTION DATA faltante \n";
					_erro = TRUE;
				}/* Verifica de a instrucao esta na secao DATA */
				else if((_sectionData < _sectionText) || (_sectionData > posCount)){
						cout << "ERRO SEMANTICO:" << lineCount << ": A diretiva " << token[j] << " está fora da seção DATA\n";
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

				/* Verifica se a secao de dados existe */
				if(_sectionData == -1)
				{
					cout << "ERRO SEMANTICO: SECTION DATA faltante \n";
					_erro = TRUE;
				}/* Verifica de a instrucao esta na secao DATA */
				else if((_sectionData < _sectionText) || (_sectionData > posCount)){
						cout << "ERRO SEMANTICO:" << lineCount << ": A diretiva " << token[j] << " está fora da seção DATA\n";
						_erro = TRUE;
				}

				for (string::size_type i = 0; i < str.length(); ++i)
				{
					c=str[i];
					if(!isdigit(c) && c != '-'){

						cout << "ERRO SINTATICO:" << lineCount << ": Operando inválido: 1 passagem \"" << str << "\"\n";
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

					cout << "ERRO SINTATICO:" << lineCount << ": Rótulo não identificado ou ausente\"" << rotulo << "\"\n";
					_erro = TRUE;

				}

			}
			/* Diretiva PUBLIC */
			else if(!it->first.compare("PUBLIC")){
				rotulo = token[++j];
				/* Verifica se o rotulo ja foi definido */
				 if(!rotulo.compare("\n")){

					cout << "ERRO SINTATICO:" << lineCount << ": Declaração ausente \n";
					_erro = TRUE;
				}
				else{
					if((itMod = defTable.find(rotulo)) != defTable.end()){

						cout << "ERRO SEMANTICO:" << lineCount << ": Rótulo redefinido \"" << rotulo << "\"\n";
						_erro = TRUE;


					}
					else{

						defTable[rotulo] = -1;
					}
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

	/* Verifica se a SECTION TEXT esta antes da SECTION DATA */
	if((_sectionData != -1) && _sectionData < _sectionText)
	{
		cout << "ERRO SEMANTICO: SECTION TEXT deve vir antes da SECTION DATA \n";
		_erro = TRUE;
	}
	//cout << "****************	MONTADOR	****************\n";
	//cout << "****************	TABELA DE DEFINIÇÕES	****************\n";
		//for (itMod=defTable.begin(); itMod!=defTable.end(); ++itMod)
			//cout << posCount << ": " << itMod->first << "=>" << itMod->second<<"\n";

	//cout << "****************	TABELA DE USO		****************\n";
		//for (itMod=useTable.begin(); itMod!=useTable.end(); ++it)
			//cout << posCount << ": " << itMod->first << "=>" << itMod->second<<"\n";

	//cout << "****************	TABELA DE SIMBOLOS	****************\n";
		//for (it=simbTable.begin(); it!=simbTable.end(); ++it)
			//cout << posCount << ": " << it->first << "=>" << it->second<<"\n";

	return 0;
}

/* Segunda Passagem */
int segunda_passagem(map<string,string> &opTable, map<string,int> &dirTable,
						vector<string> &token,	map<string,int> &simbTable,
						unordered_map<string,int> &defTable, unordered_multimap<string,int> &useTable,
						vector<string>&code, vector<int>&relativo){

		string memOp, memOp2; 				/*Operandos para operacoes*/
		string str;							/*string auxiliar*/
		string end;							/*string auxiliar para salvar endereco das operacoes e operandos*/
		string opCode;						/*string auxiliar para salvar o op code das operacoes*/
		char c;								/*char auxiliar*/
		int posCount = 0, lineCount = 1;
		int space = 0, posArray = 0;						/*int para receber valor da diretiva SPACE*/
		int auxCode;
		map<string,int>::iterator it;		/*iterador para tabelas formato string, int*/
		map<string,string>::iterator itOp;	/*iterador para tabelas formato string, string*/
		unordered_map<string,int>::iterator itMod;
		unordered_multimap<string,int>::iterator itUse;

		/*percorre arquivo fonte*/
    for(string::size_type j= 0; j < token.size(); ++j){
        if(token[j].find(":") != string::npos){
        	/*Verifica ROTULOS*/
			/*ignora rotulos*/
        }else if((itOp = opTable.find(token[j])) != opTable.end()){
            /*verifica OPERACOES*/
            if(!itOp->first.compare("COPY")){
                /*se operacao for COPY verifica os proximos 2 tokens
                  e verifica se sao operandos*/
                    memOp = token[j+1]; /*Operando 1*/
                    memOp2 = token[j+2]; /*Operando 2*/
                    if(memOp.find(",") != string::npos){
                        memOp = memOp.substr(0, memOp.length()-1);
						if((it = simbTable.find(memOp)) != simbTable.end()){
							//end = posCount + 1;
							auxCode = it->second;
							if(memOp2.find(",") != string::npos){
								_erro = TRUE;
								cout << "ERRO SINTATICO:" << lineCount << ": formato invalido: COPY A, B :\"" << memOp2 << "\"\n";
							}else{
								if((it = simbTable.find(memOp2)) != simbTable.end()){
									/*se tudo der certo sexta tem mais um show*/
									cout << "end" << posCount << ":\t" << itOp->first << ", ";
									cout << memOp << ", ";
									cout << memOp2 << "\n";
									/*ARQUIVO SAIDA*/
									/*salva OP CODE e end dos simbolos dos operandos de COPY*/
									code.push_back(itOp->second);
									code.push_back(to_string(auxCode));
									code.push_back(to_string(it->second));
									relativo.push_back(posCount+1);
									relativo.push_back(posCount+2);

									posCount += 3;

								}else{
									_erro = TRUE;
									cout << "ERRO SEMANTICO:" << lineCount << ": Simbolo inexistente :\"" << memOp2 << "\"\n";
								}
							}
						}else{
							_erro = TRUE;
							cout << "ERRO SEMANTICO:" << lineCount << ": Simbolo inexistente :\"" << memOp << "\"\n";
						}
                  }else{
						_erro = TRUE;
                		cout << "ERRO SINTATICO:" << lineCount << ": formato invalido: COPY A, B :\"" << memOp << "\"\n";
                  }

            } else if(!itOp->first.compare("STOP")){
                /*se operacao for STOP verifica proximo token para ver se STOP nao possui operandos*/

				if((it = simbTable.find(token[j+1])) == simbTable.end()){
					//*se tudo der certo sexta tem mais um show*/
					posCount += 1;
					cout << "end" << posCount << ":\t" <<  itOp->first << "\n";
					/*ARQUIVO SAIDA*/
					code.push_back(itOp->second);

				}else{
					cout << "ERRO SINTATICO:" << lineCount << ": formato invalido: STOP :\"" << token[j+1] << "\"\n";
				}
            // }else if((!itOp->first.compare("ADD")) || (!itOp->first.compare("SUB")) || (!itOp->first.compare("MUL")) || (!itOp->first.compare("DIV"))){
			// 	if((it = simbTable.find(token[j+1])) != simbTable.end()){
			// 		if(){
			//
			// 		}else{
			// 			cout << "ERRO SINTATICO:" << lineCount << ": formato invalido: OP :\"" << token[j+1] << "\"\n";
			// 		}
			// 		cout << "ERRO SINTATICO:" << lineCount << ": formato invalido: STOP :\"" << token[j+1] << "\"\n";
			// 	}
			//
			// }else if(){
			//
			// }else if(){
			//
			// }else if(){

			}else {
				posArray = 0;
				memOp = token[j+1];
				/* Verifica se possui mais de um operando */
				if(simbTable.find(token[j+2]) != simbTable.end()){
					_erro = TRUE;
					cout << "ERRO SINTATICO:" << lineCount << ": Operando inválido \n";
				}
				if(token[j+2] == "+"){
					if(token[j+3] != "\n"){
						posArray = stoi(token[j+3]);
						if((it = simbTable.find(memOp)) != simbTable.end()){
							posArray += it->second;
							/* Verifica se a posicao eh valida consultando se exite alguma
							 * posicao maior que a posicao do memOp e menor que a posArray*/
							for(map<string,int>::iterator itArray = simbTable.begin(); itArray != simbTable.end(); itArray++){
								//cout << ""
								if((itArray->second > it->second) && (itArray->second <= posArray))
								{
									_erro = TRUE;
									cout << "ERRO SEMANTICO:" << lineCount << ": Endereço de memória não reservado \n";
									itArray = simbTable.end();
								}
							}
							posArray = stoi(token[j+3]);
						}
					}
					else{
						_erro = TRUE;
						cout << "ERRO SEMANTICO:" << lineCount << ": Declaração ausente\n";
					}

				}
                /*se operacao nao for COPY ou STOP verifica o proximo token
                  e verifica se eh operando*/
				if((itOp = opTable.find(token[j])) != opTable.end()){
					/*Caso o proximo token seja um simbolo entao a operacao esta com os operandos corretos*/
					if((it = simbTable.find(memOp)) != simbTable.end()){
						cout << "end" << posCount << ":\t" <<  itOp->first << ", ";
						cout <<  it->first << "\n";
						/*ARQUIVO SAIDA*/
						//cout <<" -- " << itOp->second << " - " << it->second;
						code.push_back(itOp->second);
						code.push_back(to_string(it->second));
						relativo.push_back(posCount+1);

						posCount += 2;
					}else{
						str = memOp + "#";
						if((it = simbTable.find(str)) != simbTable.end()){
							/*Atualiza tabela de USO*/
							char * strAux = new char[memOp.size() + 1];
							copy(memOp.begin(), memOp.end(), strAux);
							strAux[memOp.size()] = '\0'; //finaliza string com '\0'

							useTable.insert(make_pair<string,int>(strAux, (posCount+1)));
							//cout << "Table Use: " << memOp << ", " << posCount+1 << "\n";
							cout << "end" << ":\t" <<  itOp->first << ", ";
							cout <<  it->first << "\n";
							/*ARQUIVO SAIDA*/
							code.push_back(itOp->second);
							code.push_back(to_string(it->second));
							relativo.push_back(posCount+1);
							posCount += 2;
							delete[] strAux;
						}else{
							_erro = TRUE;
							cout << "ERRO SINTATICO:" << lineCount << ": Operando invalido: :\"" << memOp << "\"\n";
						}
					}
				}else{
					_erro = TRUE;
					cout << "ERRO SINTATICO:" << lineCount << ": Operando invalido: :\"" << itOp->first << "\"\n";
				}
            }

        }else if((it = dirTable.find(token[j])) != dirTable.end()){
			/*Verifica DIREIVAS*/
			if(!it->first.compare("SPACE")){
				/*verifica token */
				str = token[j-1];
				str = str.substr(0, str.length()-1);
				if((it = simbTable.find(str)) != simbTable.end()){
					/*Faz a leitura do espaco a ser reservado para SPACE*/
					str = token[j+1];
					for (string::size_type i = 0; i < str.length(); ++i){
						c=str[i];
						space = space *10 + (c -'0');
					}
					/* salva no vetor 00 space vezes*/
					while(space > 0)
					{

						space--;
						cout << "end" << posCount << ":\t 00 \n";
						/*Arquivo Saida*/
						code.push_back("00");
						posCount+= 1;
					}

					/*salva no arquivo de saida o endereco do SPACE e 0's*/
				}else{
					cout << "ERRO SINTATICO:" << lineCount << ": Simbolo inválido: \"" << token[j-1] << "\"\n";
				}
			}else if(!it->first.compare("CONST")){
						/* */
						str = token[j+1];
						cout << "end" << posCount << ":\t"<< str << "\n";
						posCount+= 1;
						/*Arquivo Saida*/
						code.push_back(str);

			}else if(!it->first.compare("PUBLIC")){
				/* coloca o valor certo na tabela de uso */
				memOp = token[j+1];

				if(memOp.compare("\n")){
					cout << "PUBLIC -> memOp:" << memOp << "\n";
					if((it = simbTable.find(memOp)) != simbTable.end()){

						cout << "PUBLIC -> memOp:" << memOp << "\n";
						if((itMod = defTable.find(memOp)) != defTable.end()){
							defTable[memOp] = it->second;
						}

					}
					else{
						_erro = TRUE;
						cout << "ERRO SEMANTICO:" << lineCount << ": Operando invalido: \"" << itOp->first << "\"\n";
					}
			}
			else{
				_erro = TRUE;
				cout << "ERRO SINTATICO:" << lineCount << ": Declaração ausente\n";
			}

			}else if(!it->first.compare("SECTION") || !it->first.compare("EQU")
					|| !it->first.compare("BEGIN") || !it->first.compare("END")
					|| !it->first.compare("EXTERN")){
				/**/

			}
			/* Diretiva IF */
			else if(!it->first.compare("IF")){
				string valor = "";
				valor = token[j+1];
				/* Se o rotulo existe na tabela de simbolos */
				if( simbTable.find(valor) != simbTable.end()){

					cout << "ERRO SINTATICO:" << lineCount << ": IF possui argumento inválido: \"" << valor << "\"\n";
					_erro = TRUE;
				}
				else if(!valor.compare("\n")){

					cout << "ERRO SINTATICO:" << lineCount << ": Declaração ausente \n";
					_erro = TRUE;
				}
				string::size_type n = j;
				while (token[n] != "\n")
				{
					token[n].erase();
					n++;
				}
				if(!valor.compare("0"))
				{

					//cout << "VALORIF: "<< rotulo << "\n";
					//cout<< "SOME LINHA\n"<< token[n];

					n++;
					while (token[n] != "\n")
					{
						token[n].erase();
						n++;
					}

				}

			}

		}
		if(token[j] == "\n"){
			lineCount++;
		}
    }
	// cout << "****************	TABELA DE SIMBOLOS	****************\n";
	// 	for (it=simbTable.begin(); it!=simbTable.end(); ++it)
	// 		cout << posCount << ": " << it->first << "=>" << it->second<<"\n";
	//
	//
	// cout << "****************	TABELA DE USO		****************\n";
	// 	unordered_map<string, int> reverso;
	// 	for (itUse=useTable.begin(); itUse!=useTable.end(); ++itUse)
	// 		reverso[itUse->first] = itUse->second;
	//
	// 	for (itUse=reverso.begin(); itUse!=reverso.end(); ++itUse)
	// 		cout << posCount << ": " << itUse->first << "=>" << itUse->second<<"\n";
	//
	//
	// cout << "****************	TABELA DE DEFINIÇÕES	****************\n";
	// 	for (itMod=defTable.begin(); itMod!=defTable.end(); ++itMod)
	// 		cout << posCount << ": " << itMod->first << "=>" << itMod->second<<"\n";
	//
	// cout << "****************	RELATIVO	****************\n";
	// 	for(vector<int>::iterator it = relativo.begin(); it != relativo.end(); it++){
	// 		cout << *it << " ";
	// 	}
	// 	cout << "\n";
	//
	// cout << "****************	CODIGO OBJETO	****************\n";
	// 	for(vector<string>::iterator it = code.begin(); it != code.end(); it++){
	// 		cout << *it << " ";
	// 	}
	// 	cout << "\n";
return 0;
}
