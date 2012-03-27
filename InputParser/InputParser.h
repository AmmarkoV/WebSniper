#ifndef _INPUTPARSER_H_
#define _INPUTPARSER_H_

#include "InputParser_C.h"
#include <stdio.h>
#include <string.h>
#include <ctype.h>

using namespace std;


class InputParser
{
  private:

    struct InputParserC * ipc;

    public:

    const char * Version();
    void DefaultDelimeterSetup();

    InputParser();
    ~InputParser();

    void SetDelimeter(int num,char tmp);
    char GetDelimeter(int num);

    //void SetWord(int num,char * tmp);
    unsigned int GetWord(int num,char * thestr,unsigned int thestrsize);
    unsigned int GetUpcaseWord(int num,char * thestr,unsigned int thestrsize);
    unsigned int GetLowercaseWord(int num,char * thestr,unsigned int thestrsize);
    char GetWordChar(int num,int chr);
    signed int GetWordInt(int num);

    unsigned short GetWordLength(int num);

    int SeperateWords(char * inpt);
    int SeperateWordsCC(const char * inpt);
    int SeperateWordsUC(unsigned char * inpt);
};

#endif
