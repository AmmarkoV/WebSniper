#include "GenerateReport.h"
#include "../InputParser/InputParser.h"
#include <stdio.h>
#include <stdlib.h>

FILE* fp=0;
int adds_called=0;

void WriteReportHeader(char * filename ,int live , int keynum,const char * keyword ,int sourcecount, wxListBox * Sources)
{
   fp = fopen(filename,"w");
   if (fp == 0) { return ; }

   fprintf(fp,"<html>\n<head>\n<title>WebSniper Report</title>\n");
   fprintf(fp,"<meta http-equiv=\"Content-Type\" content=\"text/html; charset=UTF-8\">\n");
   fprintf(fp,"</head>\n<body>\n");
   fprintf(fp,"<h2>A total of %u terms used for search : %s</h2>",keynum,keyword);
   fprintf(fp,"<h4>%u Websites scanned</h4>\n",sourcecount);
   if ( live ==0 ) fprintf(fp,"<h4>Results from the website cache</h4>\n");


   fprintf(fp,"<center><table border=0 width=98\% >");
   wxString state;
   char sourcetmp[2048]={0};
   for ( int i=0; i<sourcecount; i++)
   {
       if (i%2==0) fprintf(fp,"<tr bgcolor=\"CCCCCC\">"); else
                   fprintf(fp,"<tr bgcolor=\"EEEEEE\">");

     fprintf(fp,"<td>");
     fprintf(fp,"doc%u </td><td>",i+1);
     state.Clear();
     state<<Sources->GetString(i);

     strcpy(sourcetmp,state.mb_str(wxConvUTF8));
     fprintf(fp,"<a href=\"%s\">%s</a>",sourcetmp,sourcetmp);
     fprintf(fp,"</td></tr>");
   }
  fprintf(fp,"</table></center><br><br>");

  fprintf(fp,"<h4>Relevant Keywords for Websites</h4><br><br>\n");

  fprintf(fp,"<center><table border=0 width=98\% >");
}

void AddReportData(wxString keyword ,wxString docurl,unsigned int doc_id)
{
    if (fp == 0) { return ;}

    ++adds_called;
    if (adds_called%2==0) fprintf(fp,"<tr bgcolor=\"EEEEEE\">"); else
                          fprintf(fp,"<tr bgcolor=\"CCCCCC\">");


    fprintf(fp,"<td>");
      char sourcetmp[2048]={0};
      strcpy(sourcetmp,docurl.mb_str(wxConvUTF8));
      fprintf(fp,"<a href=\"%s\">doc%u</a></td><td>",sourcetmp,doc_id);
      strcpy(sourcetmp,keyword.mb_str(wxConvUTF8));
      fprintf(fp,"%s",sourcetmp);
    fprintf(fp,"</td></tr>");

}

void CloseReport()
{
  if (fp == 0) { return ; }


  fprintf(fp,"</table>");

  if ( adds_called==0 ) { fprintf(fp,"<br>No Results Found :(<br>"); }

  fprintf(fp,"</center></body></html>");
  fclose(fp);
  fp=0;
}
