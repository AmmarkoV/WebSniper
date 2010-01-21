#ifndef HTMLANALYZER_H
#define HTMLANALYZER_H


#include "../TextAnalyzer/TextAnalyzer.h"
#include <stdio.h>
#include <stdlib.h>

class HTMLAnalyzer
{
    public:
        HTMLAnalyzer(unsigned char * file,unsigned char * file2);
        virtual ~HTMLAnalyzer();

        void CleanHTMLTags();
        unsigned int WordOccurances(unsigned char * str , unsigned short length);
    protected:
    private:
      unsigned char * filename;
      unsigned char * filenameclean;
      FILE * pFile;
      long lSize;
      char * buffer;
      size_t result;
      word_collection * html_words;
      bool failed_instance;

};

#endif // HTMLANALYZER_H
