#ifndef GENERATEREPORT_H_INCLUDED
#define GENERATEREPORT_H_INCLUDED

#include <wx/listbox.h>


void WriteReportHeader(char * filename ,int live,int keynum,const char * keyword ,int sourcecount, wxListBox * Sources);
void AddReportData(wxString keyword ,wxString docurl,unsigned int doc_id);
void CloseReport();


#endif // GENERATEREPORT_H_INCLUDED
