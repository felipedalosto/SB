#ifndef LOGERRO
#define LOGERRO
using namespace std;


void logReservedWord (string,int);
void logPrintLineNumber (int, string);
void logStartingWithNumber (string, int);
void logLabelIsTooBig (string);
void logInvalidSection (string section);
void logInvalidDirective(string);
void logMultipleLabels();
void logSectionDataBeforeText();
void logCannotOpenFile();
void logNoTextSection();
void logLabelRedefined (string);
void logLabelUndefined (string);
void logInvalidArgument (string);
void logWrongParameter ();
void logWrongArguments ();
void logWrongOption ();

#endif
