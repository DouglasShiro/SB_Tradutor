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
