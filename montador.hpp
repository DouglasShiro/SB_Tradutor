#include <iostream>
#include <string>
#include <sstream>
#include <fstream>
#include <vector>
#include <locale>
#include <algorithm>
#include <map>
#include <unordered_map>
#include <stdlib.h>

using namespace std;

int primeira_passagem(map<string,string> opTable, map<string,int> dirTable,
						vector<string> token,	map<string,int> simbTable,
						unordered_map<string,int> defTable, unordered_map<string,int>useTable);
int token_valido(string token);
