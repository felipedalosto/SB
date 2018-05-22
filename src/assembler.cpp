#include <iostream>
#include <fstream>
#include <string>
#include <stdlib.h>
#include <vector>
#include <sstream>
#include "../include/assembler.hpp"
#include "../include/logErro.hpp"

using namespace std;
 
int assembler(string inputName,string outputName, vector<instr_format> instructionsTable, vector<diret_format> directivesTable ){
  
  //------------------------------------------------------------------------------------------------------------  //
  //------------------------------------------------------------------------------------------------------------  //
  //------------------------------------*************************************-----------------------------------  //
  //------------------------------------**       VARIAVEIS E ARQUIVOS      **-----------------------------------  //
  //------------------------------------*************************************-----------------------------------  //
  //------------------------------------------------------------------------------------------------------------  //
  //------------------------------------------------------------------------------------------------------------  //

  // Declaracao das Strings utilizadas para ler as instrucoes e opera_las.
  string lineText, memTmp, memTmpArray;
  string label, op, mem1, mem2;


  // Contadores para memoria, linha e tamanho.
  int memoryCounter =0, lineCounter=1;

  // Manipulacao das intrucoes/diretivas/simbolos
  int instructionSize = -1, directiveSize = -1;
  int directiveIsFound = -1 ,symbolIsFound=1;
  int inst_code;

  // Flag para a presenca de "SECTION TEXT"
  int set_TEXT =0; 

  // Flag de erro 
  int *flag_EXTERN = (int*)calloc(1,sizeof(int)), compensate1=0, pos, compensate2 =0, memTmpi=0;
  int *flag_ERROR = (int*)calloc(1,sizeof(int));

  // Operadores para converter Hexadecimal
  stringstream convertHexadecimal;
  ostringstream convert; 
  
  // Cria Vetor de Structs do tipo Simbolos para receber a tabela de simbolos.
  vector<st_tab_simb> symbolTable;
  vector<st_tab_simb>::iterator symbolTable_it;
  st_tab_simb symbol;

  // Cria Vetor de Structs do tipo Instrucoes para receber a tabela de instruções.
  vector <instr_format> instructions;
  vector <instr_format>::iterator instructions_it;
  instructions = instructionsTable;

  // Cria Vetor de Structs do tipo Diretivas para para receber a tabela de diretivas.
  vector<diret_format> directives;
  vector<diret_format>::iterator directives_it;
  directives = directivesTable;

  // Variaveis para a manipulacao dos arquivos fonte e de saida. 
  ifstream myfile (inputName.c_str());  
  outputName = outputName + ".o"; 
  ofstream fp_fileout (outputName.c_str());

  // Declara uma Struct do tipo instrucao.
  struct sep_instr instruction;

  // Verifica se o arquivo foi aberto corretamente. 
  if ( !fp_fileout.is_open() || ! myfile.is_open() ){  
    logCannotOpenFile();
    return 1;
  } 

  //------------------------------------------------------------------------------------------------------------  //
  //------------------------------------------------------------------------------------------------------------  //
  //------------------------------------*************************************-----------------------------------  //
  //------------------------------------**        PRIMEIRA PASSAGEM        **-----------------------------------  //
  //------------------------------------*************************************-----------------------------------  //
  //------------------------------------------------------------------------------------------------------------  //
  //------------------------------------------------------------------------------------------------------------  //


  // Percorre o arquivo linha por linha.
  while ( getline (myfile,lineText) ){ 

    // Separa a linha de instrucao em "label,op,mem1,mem2".
    instruction = splitLine(lineText,lineCounter,flag_ERROR );

    // Caso a flag de erro esteja ativada, imprime o numero da linha em que o erro foi gerado e o seu texto.
    if(*flag_ERROR == 1){
      logPrintLineNumber (lineCounter, lineText);
      exit(1);
    }

    // Variaveis locais recebem a instrucao. 
    label = instruction.label;
    op = instruction.op;
    mem1 = instruction.mem1;    
    mem2 = instruction.mem2;


    // Realiza a etapa de Scanner, avaliando se os tokens usados sao validos.
    if ( scanner (label, op, mem1, mem2, instructions, directives) ) {
      logPrintLineNumber (lineCounter, lineText);
      exit(1);
     }
    
    // Realiza a etapa de Parsing, avaliando se as estruturas logicas sao validas.
    if ( parsing (op , mem1 , mem2) ){
      logPrintLineNumber (lineCounter, lineText);
      exit(1);
    }

    // Verifica se o rotulo existe. 
    if( !label.empty() ){

      symbolIsFound = searchSymbolTable (symbolTable, label, flag_EXTERN);

      // Caso o rotulo ja exista, lanca erro de redefinicao de rotulos. 
      if(symbolIsFound > 0 ){
        logLabelRedefined (label);
        logPrintLineNumber (lineCounter, lineText);
        exit(1);
      } 
      // Caso o rotulo nao exista, adiciona na tabela de simbolos. 
      else {
        symbol.label = label;
        symbol.ender = memoryCounter;                
        symbolTable.push_back (symbol);    
        
      }
    }

    // Verifica se e uma instrucao. 
    instructionSize = searchInstructionsTable (instructions, op, 2);

    // Caso a instrucao seja valida, movimenta o contador de posicao de memoria o tamanho exato da instrucao. 
    if ( instructionSize > 0 ){
      memoryCounter = memoryCounter + instructionSize;
    }

    // Caso contrario, verifica se e uma diretiva. 
    else {
      directiveIsFound = searchDirectivesTable(directives, op);
      // Se for uma diretiva, executa e movimenta o contador de posicao de memoria o tamanho exato da diretiva. 
      if ( directiveIsFound >= 0 ){ 
        directiveSize = doDirective( op, mem1, &set_TEXT, lineCounter, lineText );
        memoryCounter= memoryCounter + directiveSize;
      }
      // Caso nao seja rotulo, instrucao ou diretiva, lanca erro de operacao invalida. 
      else {
        logInvalidArgument (op);
        logPrintLineNumber (lineCounter, lineText);
        exit(1);
      }      
    }
    // Limpa a variavel local para receber uma nova linha. 
    op.clear();
    mem1.clear();
    label.clear();
    mem2.clear();

    // Incrementa o contador de linhas. 
    lineCounter++;
    
  }
  // Lanca erro caso durante a leitura nao encontre a "SECTION TEXT".
  if(!set_TEXT){
    logNoTextSection();
    exit(1);
  } 

  myfile.clear();
  myfile.seekg(0, ios::beg);

  //------------------------------------------------------------------------------------------------------------  //
  //------------------------------------------------------------------------------------------------------------  //
  //------------------------------------*************************************-----------------------------------  //
  //------------------------------------**        SEGUNDA PASSAGEM         **-----------------------------------  //
  //------------------------------------*************************************-----------------------------------  //
  //------------------------------------------------------------------------------------------------------------  //
  //------------------------------------------------------------------------------------------------------------  //

  // Cria os vetores para manipulacao dos arquivos.   
  vector<int> realocation,code_to_file;
  vector<int>::iterator realocation_it, code_to_file_it;

  // Reinicia os contadores de memoria e linha para uma nova passagem. 
  memoryCounter = 0;
  lineCounter=1;

  // Percorre o arquivo pela segunda vez.
  while ( getline (myfile, lineText) ) {

    // Separa novamente os elementos de cada linha.
    instruction = splitLine (lineText, lineCounter, flag_ERROR);

    // Caso a instrucao seja a diretiva SECTION pula a linha e pega a proxima.
    while(instruction.op == "SECTION"){      
      getline (myfile, lineText);
      lineCounter ++;
      instruction = splitLine (lineText, lineCounter, flag_ERROR);
    }

    // Variaveis locais recebem a instrucao. 
    label = instruction.label;
    op = instruction.op;
    mem1 = instruction.mem1;
    mem2 = instruction.mem2;

    // Caso encontre o indicador de Hexadecimal "0x" realiza a conversao para inteiros. 
    pos = (int)mem1.find("0x");
    if (pos == 0){
      memTmp.assign(mem1, 2, mem1.length() );
      memTmpi = atoi(memTmp.c_str());
      convertHexadecimal << memTmpi;
      convertHexadecimal >> std::hex >> memTmpi;
      convert << memTmpi;
      mem1 = convert.str();
    }

    // Verifica se cada um dos operandos nao vazios e nao constantes sao um rotulo valido ou vetor. 
    notNumberOperand (mem1, memTmpArray, compensate1, symbolIsFound, lineCounter, lineText, symbolTable, flag_EXTERN);
    notNumberOperand (mem2, memTmpArray, compensate2, symbolIsFound, lineCounter, lineText, symbolTable, flag_EXTERN);
    
    // Procura a instrucao na tabela de instrucoes.
    instructionSize = searchInstructionsTable (instructions, op, 2);

    // Caso a instrucao exista, movimenta o contador de memoria 
    if(instructionSize>0){
      memoryCounter = memoryCounter+instructionSize; 
      
      // Verifica se a quantidade de operandos por instrucao esta correta. Caso correta, acha a instrucao na tabela.  
      if (instructionSize == 1 + ( !mem1.empty() ) + ( !mem2.empty() ) ){
        inst_code = searchInstructionsTable(instructions,op,1);
        realocation.push_back(0);
        code_to_file.push_back(inst_code);

        // Acha o operando buscando seu rotulo na tabela de simbolos. 
        if(!mem1.empty()){          
          symbolIsFound = searchSymbolTable(symbolTable,mem1,flag_EXTERN);
          realocation.push_back(1);
          code_to_file.push_back(symbolIsFound+compensate1);
          
        }
        if(!mem2.empty()){
          symbolIsFound = searchSymbolTable(symbolTable,mem2,flag_EXTERN);
          realocation.push_back(1);
          code_to_file.push_back(symbolIsFound+compensate2);
        }
      }
      // Lanca erro caso a quantidade de parametros esteja errada. 
      else {
        logWrongParameter();
        logPrintLineNumber(lineCounter,lineText);
        exit(1);
      }
    }
    // Se nao for instrucao, procura na tabela de diretivas. 
    else {
      directiveIsFound = searchDirectivesTable(directives,op); 

      // Caso ache a operacao na lista de diretivas. 
      if(directiveIsFound>=0){ 
        // Para a diretiva "CONST" apenas transforma a string para int e cria na memoria o valor. 
        if(op =="CONST"){
          realocation.push_back(0);
          code_to_file.push_back(atoi(mem1.c_str()));
        }
        // Para a diretiva "SPACE" verifica o tamanho necessario e aloca o espaco na memoria. 
        else if(op == "SPACE"){

          int space_tam = atoi(mem1.c_str());
          int cont=1;
          
          // Alocacao de variaveis.
          if(space_tam==0){
            code_to_file.push_back(0);
            realocation.push_back(0);
          }

          // Alocacao de vetores.
          for(cont=1;cont<=space_tam;cont++){
            code_to_file.push_back(0);
            realocation.push_back(0);
          }
        }
      }      
    }
    lineCounter++;
  }
  // Escreve o codigo gerado e um " " no arquivo de saida. 
  for (code_to_file_it = code_to_file.begin(); code_to_file_it != code_to_file.end(); code_to_file_it++) {
    fp_fileout << *code_to_file_it << " ";
  }
    
  myfile.close();
  fp_fileout.close();

  free(flag_EXTERN);
  free(flag_ERROR);

  return 0;
}

// Funcao utilizada para separar as linhas em tokens. 
struct sep_instr splitLine(string lineText, int lineCounter,int* error){

  // Declara uma Struct do tipo instrucao. 
  struct sep_instr instruction;

  int pos;
  *error =0;

  // Encontra rotulos utilizando o separador ":".
  pos = (int)lineText.find(":");
  // Caso ache, elimina os espacos e salva o rotulo. 
  if(pos > 0){
    instruction.label.assign(lineText,0,pos);
    lineText.erase(0,pos+1);

    // Checa se existe mais de um rotulo por linha. 
    if( (int)lineText.find(":") > 0 ) {
      logMultipleLabels();
      *error = 1;
      return instruction;
    }
  }
  // Encontra instrucoes utilizando o separador " ".
  pos = (int)lineText.find(" "); //espaço para pegar codigo de instrução
  
  // Caso encontre, elimina os espacos. 
  if(pos>0){
    instruction.op.assign(lineText,0,pos);  // pega codigo de instrução
    lineText.erase(0,pos+1); //+1 para eliminar espaço
  }
  // Para tratar as operacoes que nao tem operandos. 
  else { 
    instruction.op=lineText;
    lineText.erase(0,lineText.length());
  }

  // Captura o primeiro operando. 
  pos = (int)lineText.find(",");
  
  if( pos > 0 ) {
    instruction.mem1.assign(lineText,0,pos);
    lineText.erase(0,pos+1);

    // Caso ainda tenham tokens a ser lidos, captura o segundo operando.
    if( lineText.length() > 0 ){ 
      instruction.mem2 = lineText;
      lineText.erase(0,instruction.mem2.length());
    }
  }  
  else if(lineText.length()>0){
    pos = (int)lineText.find(" ");
    
    if( pos < 0 ){
      instruction.mem1 = lineText;
      lineText.erase(0,pos);
    }
    else{
        instruction.mem1.assign(lineText,0,pos);
        lineText.erase(0,pos+1); 
        instruction.mem2.assign(lineText,0,pos);
        lineText.erase(0,pos);
      }
    }
    // Lanca erro caso apos capturar todos os tokens necessarios ainda haja algo na linha. 
    if(!lineText.empty()){
      logWrongParameter();
      logPrintLineNumber(lineCounter,lineText);
      *error = 1;
    }
  return instruction;
}

// Funcao utilizada para buscar as instrucos na tabela de instrucoes. 
int searchInstructionsTable(vector<instr_format> Instr, string instruction,int flag){

  int j=0;
  vector<instr_format>::iterator Instr_it;

  for( Instr_it = Instr.begin(); Instr_it != Instr.end(); Instr_it ++ ){
    if(Instr[j].name == instruction){
      switch(flag){
        case(1):
          return Instr[j].code;
        case(2):
          return Instr[j].tam;
      }
    }
    j++;
  }
  return -1;
}

// Funcao utilizada para buscar as diretivas na tabela de diretivas.
int searchDirectivesTable(vector<diret_format> Diret, string directive){

  int i=0;
  vector<diret_format>::iterator Diret_it;

  for( Diret_it = Diret.begin(); Diret_it != Diret.end(); Diret_it ++){
    if(Diret[i].label == directive){
      return Diret[i].tam;
    }
    i++;
  }
  return -1;
}

// Funcao utilizada para buscar rotulos na tabela de simbolos. 
int searchSymbolTable(vector<st_tab_simb> symbolTable, string label,int* flag_EXTERN){

  int symbolIsFound =-1;
  st_tab_simb symbol;  
  vector<st_tab_simb>::iterator symbolTable_it;

    for(symbolTable_it = symbolTable.begin(); symbolTable_it != symbolTable.end(); symbolTable_it ++){
      symbol = *symbolTable_it;
      if(label == symbol.label){
        symbolIsFound = symbol.ender;
      }
    }
    return symbolIsFound;
}

// Funcao utilizada para que o montador execute as diretivas durante a montagem. 
int doDirective(string op,string mem1, int *set_TEXT, int lineCounter, string lineText){

  int tam = 0;
  
  if(op == "SECTION" && mem1 == "TEXT"){
    *set_TEXT = 1;    
  }
  // Avalia se a flag para a "SECTION TEXT" e verdadeira quando a "SECTION DATA" e definida.
  else if(op == "SECTION" && mem1 == "DATA"){
    if(*set_TEXT==0){
      logSectionDataBeforeText();
      logPrintLineNumber (lineCounter, lineText );
      exit(1);
    }
  }
  if ( op == "CONST" ){    
    return 1;
  }
  if ( op == "SPACE" ){
    tam = 1;

    if (!mem1.empty()){
      tam  = atoi( mem1.c_str() );      
    }
  }  
  return tam;
}

// Funcao utilizada para realizar a etapa de Scanner, checando se os tokens utilizados sao validos.
int scanner(string label,string op,string mem1,string mem2,vector<instr_format>Instr,vector<diret_format> Diret){

  vector<instr_format>::iterator Instr_it;
  int flag, number=0;

  if(!label.empty()){
    // Checa se os rotulos tem nome reservado.
    if(reservedWordScanner(label,Instr, Diret, 1 ) == 1){
      return 1;
    }
    // Checa se os rotulos sao "TEXT" ou "DATA".
    if(label=="TEXT" || label == "DATA" ){
      logReservedWord(label,1);
      return 1;
    } 
    // Checa se os rotulos comecam com numeros.
    number = atoi(label.c_str() );
    if(number!=0){
      logStartingWithNumber(label,number);
      return 1;
    }
    // Checa se os rotulos comecam com 0.
    number=(int)label.find("0");
    if(number==0){
      logStartingWithNumber(label,number);
      return 1;
    }
    // Checa se os rotulos tem no maximo 20 caracteres.
    if( label.length() > 20 ){
      logLabelIsTooBig(label);
      return 1;
    }
  }
  // Checa se o primeiro operando tem nome reservado. 
  if( !mem1.empty() ){

    if(reservedWordScanner(mem1,Instr, Diret, 0) == 1){
      return 1;
    }
  }
  // Checa se o segundo operando tem nome reservado. 
  if(!mem2.empty()){

    if(reservedWordScanner(mem2,Instr, Diret, 0) == 1){
      return 1;
    }
  }
  return 0;
}
// Funcao utilizada para realizar a etapa de Parser, checando se as estruturas logicas sao validas.
int parsing(string op,string mem1,string mem2){

  int pos;

  pos = mem1.find(" ");
  if(pos>=0){
    logWrongParameter();
    return 1;
  }
  pos = mem2.find(" ");
  if(pos>=0){
    logWrongParameter();
    return 1;
  }
  // Checa o nome das "SECTION"s.
  if(op == "SECTION"){
    if(mem1 != "TEXT" && mem1 != "DATA"){
      logInvalidSection(mem1);
      return 1;
    }
    if(!mem2.empty()){
      logInvalidDirective(mem2);
      return 1;
    } 
  }
  // Checa instrucoes que so tem um operando. 
  if(op == "CONST" || op == "SPACE"){
    if(!mem2.empty()){
      logInvalidDirective(mem2);
      return 1;
    }
  }
   return 0;
}

// Realiza das operacoes que tem rotulos como operandos. 
void notNumberOperand (string mem, string mem_array, int compensate, int symbolIsFound, int lineCounter, string lineText, vector<st_tab_simb> symbolTable, int *flag_EXTERN ){
  
  int pos;

  if(!mem.empty() && (atoi(mem.c_str()) == 0 )){

    // Verifica se foi feito o uso de vetores por meio do operador "+".
    pos = (int)mem.find("+");
    compensate = 0;
    // Para vetores e necessario reservar o espaco de memoria dado pela string retirada do arquivo fonte. 
    if( pos > 0){
      mem_array.assign(mem,pos+1,mem.length());
      compensate = atoi(mem_array.c_str());
      mem.assign(mem,0,pos);
    }

    // Encontra o rotulo utilizado na tabela de simbolos. 
    symbolIsFound = searchSymbolTable(symbolTable,mem,flag_EXTERN);
    
    // Lanca erro caso nao esteja na tabela de simbolos. 
    if(symbolIsFound==-1){
      logLabelUndefined(mem);
      logPrintLineNumber (lineCounter,lineText);
      exit(1);
    }
 }
}

// Checa se os rotulos utilizados possuem nomes reservados. 
int reservedWordScanner (string mem, vector<instr_format> Instr, vector<diret_format> Diret, int opLabel){
  
  //Flags para instrucao e diretivas. 
  int isInstruction, isDirective;

  // Procura nas tabelas de instrucao e diretivas. 
  isInstruction = searchInstructionsTable(Instr,mem,1);
  isDirective = searchDirectivesTable(Diret,mem);

  // Lanca erro se o nome for reservado. 
  if(isInstruction >= 0  || isDirective >= 0){
    logReservedWord(mem,opLabel);    
    return 1;
  }
}