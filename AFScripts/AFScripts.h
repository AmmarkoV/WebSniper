#ifndef AFSCRIPTS_H_INCLUDED
#define AFSCRIPTS_H_INCLUDED

#ifdef __cplusplus
extern "C" {
#endif


#include <stdio.h>

#define AFS_SANITY_BYTE_VALUE 666069;

struct AFSContext
{
   FILE* fp;

   unsigned char * datastream;
   unsigned int datastream_pos;
   unsigned int datastream_len;

   unsigned int sanity_byte;
};

unsigned int AFSVersion();

unsigned char StartParsingFile(struct AFSContext * afs , char * filename);
unsigned char GetNextLineFile(struct AFSContext * afs ,char * line,unsigned int linelen);
unsigned char StopParsingFile(struct AFSContext * afs);

unsigned char StartParsingString(struct AFSContext * afs , char * string , unsigned int string_len);
unsigned char NoNextLineString(struct AFSContext * afs );
unsigned int GetNextLineString(struct AFSContext * afs ,char * line,unsigned int linelen);
unsigned char MergeLineStringAtPos(struct AFSContext * afs ,unsigned int pos, char * line,unsigned int linelen);
unsigned char StopParsingString(struct AFSContext * afs);

#ifdef __cplusplus
}
#endif


#endif // AFSCRIPTS_H_INCLUDED
