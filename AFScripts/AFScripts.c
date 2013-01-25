#include "AFScripts.h"
#include <string.h>

unsigned int AFSVersion()
{
  return 5;
}

unsigned char AFSContextOK(struct AFSContext * afs)
{
    if ( afs == 0 ) { return 0; }
    return 1;
}

// <<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<
// FILE PARSING <<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<
// <<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<

unsigned char StartParsingFile(struct AFSContext * afs , char * filename)
{
   if (!AFSContextOK(afs)) { return 0; }
   afs->fp = fopen(filename,"r");
   if (afs->fp == NULL) { return 0; }

   afs->sanity_byte=AFS_SANITY_BYTE_VALUE;


   return 1;
}


unsigned char GetNextLineFile(struct AFSContext * afs ,char * line,unsigned int linelen)
{
    if (!AFSContextOK(afs)) { return 0; }
    if ( afs->fp != 0 ) {
                          unsigned int retres = (fgets(line,linelen,afs->fp)!=0);
                          if ( retres != 0 )
                          {
                           unsigned int len = strlen ( line );
                           if ( len > 0 )
                            {
                                 if (line[len-1]==10) { line[len-1]=0; /*fprintf(stderr,"-1 newline \n");*/ }
                                 if (line[len-1]==13) { line[len-1]=0; /*fprintf(stderr,"-1 newline \n");*/ }
                            }
                           if ( len > 1 )
                            {
                                 if (line[len-2]==10) { line[len-2]=0; /*fprintf(stderr,"-2 newline \n");*/ }
                                 if (line[len-2]==13) { line[len-2]=0; /*fprintf(stderr,"-2 newline \n");*/ }
                            }
                          }
                          return retres;
                        }
    return 0;
}


unsigned char StopParsingFile(struct AFSContext * afs)
{
  if (!AFSContextOK(afs)) { return 0; }
  if ( afs->fp!=0 ) fclose(afs->fp);
  return 1;
}

// <<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<
// STRING PARSING <<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<
// <<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<


unsigned char StartParsingString(struct AFSContext * afs , char * string , unsigned int string_len)
{
   afs->datastream = (unsigned char * ) string;
   afs->datastream_len = string_len;
   afs->datastream_pos = 0;
   afs->sanity_byte=AFS_SANITY_BYTE_VALUE;
   return 1;
}

unsigned char NoNextLineString(struct AFSContext * afs )
{
  if (  ( afs->datastream!=0 ) && ( afs->datastream_pos < afs->datastream_len ) && ( afs->datastream_len>0 ) ) { return 0; }
  return 1;
}

unsigned int GetNextLineString(struct AFSContext * afs ,char * line,unsigned int linelen)
{

    if (!AFSContextOK(afs)) { return 0; }

    if ( NoNextLineString(afs)==0 )
                        {
                           if ( ( line==0 ) || ( linelen == 0 ) ) { return 0; }

                           unsigned int start = afs->datastream_pos;
                           unsigned int i = start;

                           //printf("Starting parsing from %u ",afs->datastream_pos);
                           line[0]=0;
                           while  ( ( i-start<linelen ) && ( i < afs->datastream_len ) )
                           {
                             if ( afs->datastream[i]==0 )
                             {
                               // REACHED STRING END!
                               line[i-start] = 0;
                               break;
                             } else
                             if ( ( afs->datastream[i]!=10 ) && ( afs->datastream[i]!=13 ) )
                             {
                               line[i-start] = afs->datastream[i];
                             }
                               else
                             {
                               line[i-start] = 0;
                               break;
                               //printf("BREAKED! THIS SHOULD NOT BE VISIBLE\n");
                             }

                             //printf(" %c ",line[i-start]);

                             ++i;
                           }
                           line[i-start] = 0;


                           while ( ( i < afs->datastream_len ) && ( ( afs->datastream[i]==10 ) || ( afs->datastream[i]==13 ) ) )
                           {
                             //printf(" skip cr/lf ");
                             ++i;
                           }

                           afs->datastream_pos = i;
                           //printf("ending parsing at %u \n",afs->datastream_pos);
                           return i-start;
                        }
    return 0;
}

unsigned char MergeLineStringAtPos(struct AFSContext * afs ,unsigned int pos, char * line,unsigned int linelen)
{
  return 0;
}

unsigned char StopParsingString(struct AFSContext * afs)
{
    if (!AFSContextOK(afs)) { return 0; }
    return 1;
}
