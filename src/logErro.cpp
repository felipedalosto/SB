#include <iostream>
#include <fstream>
#include <string>
#include <stdlib.h>
#include <vector>
#include <sstream>
#include "../include/assembler.hpp"
#include "../include/logErro.hpp"

void logReservedWord (string name, int isLabel){
	switch (isLabel){
		case 0:{
			cout << "Erro lexico - operando \""<< name <<"\" possui nome reservado:" <<endl;
			break;
		}
		case 1:{
			cout << "Erro lexico - rotulo \""<< name <<"\" possui nome reservado:" <<endl;
			break;
		}

	}
}

void logPrintLineNumber (int lineNumber, string lineText){
	cout  <<"Linha " << lineNumber << " :\"" << lineText << "\"" <<endl;
}

void logSectionDataBeforeText (){
	cout<<"Erro Semantico - SECTION DATA antes de SECTION TEXT."<<endl;
}


void logStartingWithNumber (string label, int number){
	cout << "Erro Lexico - rotulo \""<< label <<"\" comeca com o numero " << number << endl;
}

void logLabelIsTooBig (string label){
	cout << "Erro Lexico - rotulo \"" << label << "\" maior que o limite de 20 caracteres.\n"<<endl;
}

void logInvalidSection (string section){
	cout << "Erro Sintatico - secao \"" << section << "\" invalida. "<<endl;
}

void logInvalidDirective(string operand){
	cout << "Erro Sintatico - uso incorreto da diretiva pelo operando \""<< operand <<"\""<<endl;
}

void logMultipleLabels(){
	cout << "Erro Sintatico - mais de um rotulo na mesma linha." <<endl;
}

void logNoTextSection(){
    cout << "Erro Semantico - falta SECTION TEXT." <<endl;
}

void logCannotOpenFile(){
    cout << "Nao foi possivel abrir o arquivo." <<endl;
}

void logWrongArguments (){
    cout<< "Quantidade de argumentos incorreta. " <<endl;
}  

void logLabelRedefined (string label){
	cout << "Erro Semantico - rotulo \"" << label << "\" ja definido. "<<endl;
}

void logLabelUndefined (string label){
	cout << "Erro Semantico - rotulo \"" << label << "\" nao definido. "<<endl;
}

void logInvalidArgument (string label){
	cout << "Erro Lexico - \"" << label << "\" comando invalido. "<<endl;
}

void logWrongParameter (){
    cout<< "Erro Sintatico - quantidade incorreta de operandos. " <<endl;
}

void logWrongOption (){
    cout<< "Opção inválida para a extensão." <<endl;
}
      