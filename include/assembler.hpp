#ifndef ASSEMBLER
#define ASSEMBLER
using namespace std;


typedef struct sep_instr{
      string label,op,mem1,mem2;
      void reset() {
        label.clear();
        op.clear();
        mem1.clear();
        mem2.clear();
    }
} sep_instr;

typedef struct st_tab_simb{
      string label;
      int ender;
} st_tab_simb;


typedef struct diret_format{
      string label;
      int args,tam;

} diret_format;

typedef struct instr_format{
      string name;
      int code,tam;
     

} instr_format;

typedef struct hypotheticAssembly
{
  vector<instr_format> instructions;
  vector<diret_format> directives;

} instrucoes_e_diretivas;

struct sep_instr splitLine(string, int, int*); // ponteiro de error

vector<instr_format> generateInstructionsTable(vector<instr_format> );
vector<diret_format> generateDirectivesTable(vector<diret_format> );

int searchInstructionsTable(vector<instr_format>, string,int);
int searchDirectivesTable(vector<diret_format>,string);
int searchSymbolTable(vector<st_tab_simb>, string, int*);

int scanner(string,string,string,string, vector<instr_format>,vector<diret_format>);
int parsing(string,string,string);


int doDirective(string,string,int*,int,string);

int assembler(string,string,vector<instr_format>,vector<diret_format>);

void notNumberOperand (string, string, int, int, int, string, vector<st_tab_simb>, int*);
int reservedWordScanner (string, vector<instr_format>, vector<diret_format>, int);

#endif
