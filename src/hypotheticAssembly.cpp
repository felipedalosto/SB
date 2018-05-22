#include <iostream>
#include <fstream>
#include <string>
#include <stdlib.h>
#include <vector>
#include <sstream>
#include "../include/assembler.hpp"

vector<instr_format> generateInstructionsTable(vector<instr_format> Instr){

  struct instr_format Instr_p;

  Instr_p.code = 1;
  Instr_p.tam = 2;
  Instr_p.name = "ADD";
  Instr.push_back(Instr_p);
  Instr_p.code = 2;
  Instr_p.tam = 2;
  Instr_p.name = "SUB";
  Instr.push_back(Instr_p);
  Instr_p.code = 3;
  Instr_p.tam = 2;
  Instr_p.name = "MULT";
  Instr.push_back(Instr_p);
  Instr_p.code = 4;
  Instr_p.tam = 2;
  Instr_p.name = "DIV";
  Instr.push_back(Instr_p);
  Instr_p.code = 5;
  Instr_p.tam = 2;
  Instr_p.name = "JMP";
  Instr.push_back(Instr_p);
  Instr_p.code = 6;
  Instr_p.tam = 2;
  Instr_p.name = "JUMPN";
  Instr.push_back(Instr_p);
  Instr_p.code = 7;
  Instr_p.tam = 2;
  Instr_p.name = "JUMPP";
  Instr.push_back(Instr_p);
  Instr_p.code = 8;
  Instr_p.tam = 2;
  Instr_p.name = "JMPZ";
  Instr.push_back(Instr_p);
  Instr_p.code = 9;
  Instr_p.tam = 3;
  Instr_p.name = "COPY";
  Instr.push_back(Instr_p);
  Instr_p.code = 10;
  Instr_p.tam = 2;
  Instr_p.name = "LOAD";
  Instr.push_back(Instr_p);
  Instr_p.code = 11;
  Instr_p.tam = 2;
  Instr_p.name = "STORE";
  Instr.push_back(Instr_p);
  Instr_p.code = 12;
  Instr_p.tam = 2;
  Instr_p.name = "INPUT";
  Instr.push_back(Instr_p);
  Instr_p.code = 13;
  Instr_p.tam = 2;
  Instr_p.name = "OUTPUT";
  Instr.push_back(Instr_p);
  Instr_p.code = 14;
  Instr_p.tam = 1;
  Instr_p.name = "STOP";
  Instr.push_back(Instr_p);

  return Instr;
}

vector<diret_format> generateDirectivesTable(vector<diret_format>Diret_r){

  diret_format Diret;

  Diret.label = "SECTION";
  Diret.args = 1;
  Diret.tam = 0;
  Diret_r.push_back(Diret);
  Diret.label = "SPACE";
  Diret.args = 0;  //**** MUDAAAAAAAAAAAAAAAAAAAAR*//
  Diret.tam = 1;
  Diret_r.push_back(Diret);
  Diret.label = "CONST";
  Diret.args = 1;
  Diret.tam = 1;
  Diret_r.push_back(Diret);
  Diret.label = "EQU";
  Diret.args = 1;
  Diret.tam = 0;
  Diret_r.push_back(Diret);
  Diret.label = "IF";
  Diret.args = 1;
  Diret.tam = 0;
  Diret_r.push_back(Diret);
  Diret.label = "PUBLIC";
  Diret.args = 0;
  Diret.tam = 0;
  Diret_r.push_back(Diret);
  Diret.label = "EXTERN";
  Diret.args = 0;
  Diret.tam = 0;
  Diret_r.push_back(Diret);
  Diret.label = "BEGIN";
  Diret.args = 0;
  Diret.tam = 0;
  Diret_r.push_back(Diret);
  Diret.label = "END";
  Diret.args = 0;
  Diret.tam = 0;
  Diret_r.push_back(Diret);

  return Diret_r;
}