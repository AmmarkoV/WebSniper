#include "InputParser.h"
/* InputParser..
   A small generic library for parsing a string and tokenizing it!
   URLs: http://62.103.22.50
   Written by Ammar Qammaz a.k.a. AmmarkoV 2006

 This program is free software; you can redistribute it and/or modify
 it under the terms of the GNU General Public License as published by
 the Free Software Foundation; either version 3 of the License, or
 (at your option) any later version.

 This program is distributed in the hope that it will be useful,
 but WITHOUT ANY WARRANTY; without even the implied warranty of
 MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
 GNU General Public License for more details.

 You should have received a copy of the GNU General Public License
 along with this program. If not, see <http://www.gnu.org/licenses/>.
*/


const char * ver=" VERSION 1.30 - 7/1/10 \0";

const char * Version()
{
 return InputParserC_Version();
}



void InputParser::DefaultDelimeterSetup()
{
   InputParser_DefaultDelimeters(ipc);
}

InputParser::InputParser()
{
   ipc = InputParser_Create(256,5);
}

InputParser::~InputParser()
{
   InputParser_Destroy(ipc);
}


void InputParser::SetDelimeter(int num,char tmp)
{
   InputParser_SetDelimeter(ipc,num,tmp);
}

char InputParser::GetDelimeter(int num)
{
   return InputParser_GetDelimeter(ipc,num);
}

/*
void InputParser::SetWord(int num,char * tmp)
{

}
*/
unsigned int InputParser::GetWord(int num,char * thestr,unsigned int thestrsize)
{
  return InputParser_GetWord(ipc,num,thestr,thestrsize);
}

unsigned int InputParser::GetUpcaseWord(int num,char * thestr,unsigned int thestrsize)
{
  return InputParser_GetUpcaseWord(ipc,num,thestr,thestrsize);
}

unsigned int InputParser::GetLowercaseWord(int num,char * thestr,unsigned int thestrsize)
{
  return InputParser_GetLowercaseWord(ipc,num,thestr,thestrsize);
}

signed int InputParser::GetWordInt(int num)
{
 return InputParser_GetWordInt(ipc,num);
}

char InputParser::GetWordChar(int num,int chr)
{
 return InputParser_GetWordChar(ipc,num,chr);
}


unsigned short InputParser::GetWordLength(int num)
{
  return InputParser_GetWordLength(ipc,num);
}


int InputParser::SeperateWords(char * inpt)
{
  return InputParser_SeperateWords(ipc,inpt,1);
}//END OF SEPERATE_WORDS

int InputParser::SeperateWordsCC(const char * inpt)
{
  return SeperateWords((char * )inpt);
}


int InputParser::SeperateWordsUC(unsigned char * inpt)
{
  return SeperateWords((char * )inpt);
}
