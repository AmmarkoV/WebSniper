#include "HTMLAnalyzer.h"



HTMLAnalyzer::HTMLAnalyzer(unsigned char * file,unsigned char * file2)
{
  fprintf(stderr,"Allocating new HTMLAnalyzer\n");
  filename = file;
  filenameclean = file2;

  html_words =  TextAnalyzer_NewWordCollection();
  buffer = 0;
  failed_instance = false;

  /* FILE INPUT <<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<*/
  pFile = fopen ( (const char * ) file , "rb" );
  if (pFile==NULL) {fputs ("HTMLAnalyzer error opening file \n",stderr); failed_instance = true; return;}
  // obtain file size:
  fseek (pFile , 0 , SEEK_END);
  lSize = ftell (pFile);
  rewind (pFile);
  // allocate memory to contain the whole file:
  buffer = (char*) malloc (sizeof(char)*lSize);
  if (buffer == NULL) {fputs ("HTMLAnalyzer error allocating memory \n",stderr); failed_instance = true;  fclose (pFile); pFile=0; return;}
  // copy the file into the buffer:
  result = fread (buffer,1,lSize,pFile);
  fclose (pFile);

  pFile = 0;
  if (result != lSize)
  {
    fputs ("HTMLAnalyzer error reading file to memory \n",stderr);
    failed_instance = true;
    if ( buffer != 0 ) { free( buffer ); }
    pFile=0;
    return;
  }

  fprintf(stderr,"Allocated new HTMLAnalyzer\n");
  /* FILE INPUT */
}


void HTMLAnalyzer::CleanHTMLTags(char * filenameclean)
{
   if ( failed_instance == true ) { fprintf(stderr,"Error cleaning HTML tags.. \n"); return; }
   unsigned int sizeofbuffer=lSize;
   ClearTextFromHTMLTags(html_words,buffer,sizeofbuffer);

   FILE *fp_dbg;
   fp_dbg = fopen ( (const char * ) filenameclean , "wb" );
   if (fp_dbg==NULL) {  return; }
   fwrite (buffer,1,sizeofbuffer,fp_dbg);
   fclose (fp_dbg);
   fp_dbg=0;
}

unsigned int HTMLAnalyzer::WordOccurances(unsigned char * str , unsigned short length)
{
  if ( failed_instance  == true  )   { fprintf(stderr,"This instance of HTMLAnalyzer has no word occurances!.. \n"); return 0; }
  return TextAnalyzer_WordOccurances(html_words,str,length);
}

HTMLAnalyzer::~HTMLAnalyzer()
{
  TextAnalyzer_DeleteWordCollection(html_words);
  fprintf(stderr,"Survived Delete Word Collection\n");
  if ( buffer!= 0 )  free (buffer);
  fprintf(stderr,"Survived buffer free\n");
}
