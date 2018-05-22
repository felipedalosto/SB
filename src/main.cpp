#include <iostream>
#include <string>
#include <fstream>
#include <algorithm>
#include <list>
#include <sstream>
#include <vector>
#include <stdlib.h>
#include "../include/preprocessador.h"
#include "../include/assembler.hpp"
#include "../include/logErro.hpp"

using namespace std;

// Variaveis para manipulacao de arquivos 
ifstream inputFile;
fstream  tmpFile;

// Definicoes das instrucoes e diretivas.
hypotheticAssembly tables;

int main(int argc, char const *argv[])
{
	string filename, option, outputFile, pre_temp;

	// Lanca erro caso a funcao seja chamada com quantidade de arqumentos diferente de 4. 
	if (!(argc == 4)){		
		logCannotOpenFile();
		logWrongArguments();
		return -1;
	}
    
    // Variaveis locais recebem os parametros. 
	option = argv[1];
	filename = argv[2];	
	outputFile = argv[3];

	// Analisa o nome do arquivo a ser montado.
	if ((filename.substr(filename.find(".")+1) == "asm") && option == "-o") {
		
		inputFile.open(argv[2]);
		if( !inputFile.is_open() ) {
			logCannotOpenFile();
			return -1;
		}

		tmpFile.open(argv[3],ios::out|ios::trunc);
		if( !tmpFile.is_open() ) {
			logCannotOpenFile();
			return -1;
		}

		// Realiza o pre-processamento do arquivo. 
		pre_proc(inputFile,tmpFile);

		// Carrega o Assembly Hipotetico na memoria. 
		tables.instructions = generateInstructionsTable(tables.instructions);
		tables.directives = generateDirectivesTable(tables.directives);

		// Executa a montagem. 
		assembler(outputFile,outputFile,tables.instructions,tables.directives);

		// Remove o arquivo intermediario. 
		remove(argv[3]);								
	}
	// Caso o arquivo de entrada seja .pre e necessario so monta-lo. 
	else if ((filename.substr(filename.find(".")+1) == "pre") && option == "-o" ) {	
		// Executa a montagem
		assembler(filename, outputFile, tables.instructions, tables.directives);
	}
	// Caso a opcao seja apenas pre-processamento. 
	else if((filename.substr(filename.find(".")+1) == "asm") && option == "-p")	{
		
		string ext (".pre");
		inputFile.open( argv[2] );
		
		if( !inputFile.is_open() ) {
			logCannotOpenFile();
			return -1;
		}
		outputFile += ext;
		tmpFile.open(outputFile.c_str(),ios::out|ios::trunc);
		
		if(!tmpFile.is_open()){
			logCannotOpenFile();
			return -1;
		}
		pre_proc(inputFile,tmpFile);
	}
	// Caso tente utiliza a opcao -p para um arquivo .pre
	else
	{
		logWrongOption();
		return -1;
	}
	return 0;
}