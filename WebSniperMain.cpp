/***************************************************************
 * Name:      WebSniperMain.cpp
 * Purpose:   Code for Application Frame
 * Author:    Ammar Qammaz (ammarkov@gmail.com)
 * Created:   2010-01-06
 * Copyright: Ammar Qammaz (http://62.103.22.50)
 * License:
 **************************************************************/

#include "WebSniperMain.h"
#include <wx/msgdlg.h>
#include <wx/url.h>
#include <wx/wfstream.h>
#include <wx/sound.h>
#include <wx/settings.h>
#include "HTMLAnalyzer.h"
#include "InputParser/InputParser.h"
#include "AFScripts/AFScripts.h"
#include "GenerateReport.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include <wx/html/htmlwin.h>
#include <wx/sizer.h>
#include <wx/dialog.h>
#include <wx/stdpaths.h>
#include <wx/filefn.h>

//(*InternalHeaders(WebSniperFrame)
#include <wx/string.h>
#include <wx/intl.h>
//*)

const unsigned int MAX_STR=1024;

wxString directory;
char directory_str[MAX_STR]={0};
char sources_str[MAX_STR]={0};
char keywords_str_contents[MAX_STR]={0};
char keywords_str[MAX_STR]={0};

char raw_directory[MAX_STR]={0};
char sound_directory[MAX_STR]={0};
char report_filename_str[MAX_STR]={0};
char directory_seperator[2]={'/'};


//helper functions
enum wxbuildinfoformat {
    short_f, long_f };

wxString wxbuildinfo(wxbuildinfoformat format)
{
    wxString wxbuild(wxVERSION_STRING);

    if (format == long_f )
    {
#if defined(__WXMSW__)
        wxbuild << _T("-Windows");
#elif defined(__UNIX__)
        wxbuild << _T("-Linux");
#endif

#if wxUSE_UNICODE
        wxbuild << _T("-Unicode build");
#else
        wxbuild << _T("-ANSI build");
#endif // wxUSE_UNICODE
    }

    return wxbuild;
}

//(*IdInit(WebSniperFrame)
const long WebSniperFrame::ID_LISTBOX1 = wxNewId();
const long WebSniperFrame::ID_TEXTCTRL1 = wxNewId();
const long WebSniperFrame::ID_BUTTON1 = wxNewId();
const long WebSniperFrame::ID_BUTTON2 = wxNewId();
const long WebSniperFrame::ID_TEXTCTRL2 = wxNewId();
const long WebSniperFrame::ID_TEXTCTRL3 = wxNewId();
const long WebSniperFrame::ID_BUTTON3 = wxNewId();
const long WebSniperFrame::ID_GAUGE1 = wxNewId();
const long WebSniperFrame::ID_STATICTEXT2 = wxNewId();
const long WebSniperFrame::ID_BUTTON4 = wxNewId();
const long WebSniperFrame::ID_STATICTEXT1 = wxNewId();
const long WebSniperFrame::ID_STATICTEXT3 = wxNewId();
const long WebSniperFrame::ID_CHECKBOX1 = wxNewId();
const long WebSniperFrame::ID_CHECKBOX2 = wxNewId();
const long WebSniperFrame::ID_CHECKBOX3 = wxNewId();
const long WebSniperFrame::ID_BUTTON5 = wxNewId();
const long WebSniperFrame::ID_BUTTON6 = wxNewId();
const long WebSniperFrame::idMenuQuit = wxNewId();
const long WebSniperFrame::idMenuAbout = wxNewId();
const long WebSniperFrame::ID_STATUSBAR1 = wxNewId();
//*)

BEGIN_EVENT_TABLE(WebSniperFrame,wxFrame)
    //(*EventTable(WebSniperFrame)
    //*)
END_EVENT_TABLE()

/*
>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>
>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>
>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>

              HELPER FUNCTIONS START

>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>
>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>
>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>
*/
inline wxString _U(const char String[] = "")
{
  return wxString(String, wxConvUTF8);
}


int FileExists(const char *fname)
{
    FILE *file= fopen(fname, "r");
    if (file!=0)
    {
        fclose(file);
        return 1;
    }
    return 0;
}

int FileCreate(const char *fname)
{
    if (FileExists(fname)) { return 1; }
    fprintf(stderr,"FileCreate(%s)\n",fname);
    FILE *file= fopen(fname, "w");
    if (file!=0)
    {
        fclose(file);
        return 1;
    }
    return 0;
}


int DirCreate(const char *fnamestr)
{
      wxString fnamewxstr;
      fnamewxstr<<_U(fnamestr);
      fnamewxstr<<_U(directory_seperator);//wxT("/");

      wxFileName fname(fnamewxstr);
      if (fname.DirExists())
        {
          fprintf(stderr," Directory %s Exists! ..!\n",fnamestr);
         } else
         {
           fprintf(stderr," Creating directory %s ..!\n",fnamestr);
           wxMkdir(_U(fnamestr),0777);
           return 1;
         }
   return  0;
}

int ReadAString(char * fname,char *str,unsigned int strlength)
{
    FILE *file= fopen(fname, "r");
    if (file!=0)
    {
        char * result = fgets(str,strlength,file);
        fclose(file);
        if (result==0) { return 0; } else
        if ( (strlen(str)>0) && ( str[strlen(str)-1]<=13) )
         {
            str[strlen(str)-1]=0;
         }

        return 1;
    }
    return 0;
}

int WriteAString(char * fname,char *str,unsigned int strlength)
{
    FILE *file= fopen(fname, "w");
    if (file!=0)
    {
        fwrite (str,1,strlength,file);
        fclose(file);
        return 1;
    }
    return 0;
}

void WebSniperFrame::CreateHomeFolderWithFiles()
{
    directory.clear();
    directory<< wxStandardPaths::Get().GetUserDataDir();
    strcpy(directory_seperator,"/");
/*
      int linux_dir=1;
      #if defined(__WXMSW__)
        linux_dir=0;
      #elif defined(__UNIX__)
        linux_dir=1;
      #endif


      char directory_seperator='/';
      if (linux_dir)
       {
         strcpy(directory_seperator,"/");
        //directory<<wxT("/.websniper");
       } else
       {
        strcpy(directory_seperator,"\\");
        //directory<<wxT("\\WebSniper");
       }
*/

      strncpy(directory_str,(const char*) directory.mb_str(wxConvUTF8),MAX_STR);
      DirCreate(directory_str);

      strncpy(raw_directory,directory_str,MAX_STR);
      strcat(raw_directory,directory_seperator);
      strcat(raw_directory,"raw");
      DirCreate(raw_directory);




    if (FileExists("/usr/share/websniper/Sounds/shot.wav")) {  strcpy(sound_directory,"/usr/share/websniper/Sounds/"); } else
                                                            {  strcpy(sound_directory,"Sounds/"); }



    strncpy(sources_str,directory_str,MAX_STR);
    strcat(sources_str,directory_seperator);
    strncat(sources_str,"sources.ini",MAX_STR);
    FileCreate(sources_str);

    strncpy(keywords_str,directory_str,MAX_STR);
    strcat(keywords_str,directory_seperator);
    strncat(keywords_str,"keywords.ini",MAX_STR);
    FileCreate(keywords_str);

    strncpy(report_filename_str,directory_str,MAX_STR);
    strcat(report_filename_str,directory_seperator);
    strncat(report_filename_str,"report.html",MAX_STR);




}

void WebSniperFrame::MessageCstr(char * msg)
{
    fprintf(stderr,"%s",msg);
    wxString wxmsgstr; wxmsgstr.clear();
    wxmsgstr<<_U(msg);
    Output->AppendText(wxmsgstr);

}

void  WebSniperFrame::PlaySound(wxString sndname)
{

  if ( !SoundOn->IsChecked() ) { return; }

  wxString fullcmd;  fullcmd.Clear();
  wxString fileused;  fileused.Clear();
  fileused<<_U(sound_directory);//wxT("Sounds/");
  fileused<<sndname;
  int linux_sound;

  #if defined(__WXMSW__)
        linux_sound=0;
  #elif defined(__UNIX__)
        linux_sound=1;
  #endif

  if ( linux_sound == 1 )
  {
   // aplay Sound/filename
   fullcmd<<wxT("aplay "); fullcmd<<fileused;
   if ( FileExists(fileused) ) { wxExecute(fullcmd); }
  } else
  {
   wxSound newsound(fileused);
   if ( newsound.IsOk() )
    {
      if ( newsound.Play(wxSOUND_ASYNC) )
      { /* OK */  } else
      { /*ERROR*/ }
    } else
    { /*ERROR*/ }
  }
}

int _C_FileCopy(char * frompath,char * topath)
{
  FILE * pFile;
  long lSize=0;
  char * buffer;
  int result;
  // READ FROMPATH
  // -----------------------------------
   pFile = fopen ( frompath , "rb" );
   if (pFile==0) { return 0; }
   // obtain file size:
    fseek (pFile , 0 , SEEK_END);
    lSize = ftell (pFile);
    rewind (pFile);
   // allocate memory to contain the whole file:
   buffer = (char*) malloc (sizeof(char)*lSize);
   if (buffer == 0 ) { fclose (pFile); return 0; }

   result = fread (buffer,1,lSize,pFile);
   fclose (pFile);
   // -----------------------------------

  // WRITE TOPATH
  // -----------------------------------
   pFile = fopen ( topath , "wb" );
   if (pFile==0) { free ( buffer ); return 0; }
   fwrite (buffer,1,lSize,pFile);
   fclose (pFile);
  // -----------------------------------

   free ( buffer );

  return 1;
}


void WebSniperFrame::MyCopyFile(wxString from,wxString to)
{
  wxString fullcmd;  fullcmd.Clear();


  int linux_syntax;
  #if defined(__WXMSW__)
        linux_syntax=0;
  #elif defined(__UNIX__)
        linux_syntax=1;
  #endif

  // ACTIVATE INLINE FILE COPYING ( NO CONSOLE WINDOW IN WINDOWS :P )
  //linux_syntax=2;

  if ( linux_syntax == 1 )
  {
   fullcmd.Clear();
   fullcmd<<wxT("cp "); fullcmd<<from; fullcmd<<wxT(" "); fullcmd<<to;
   if ( FileExists(from) ) { wxShell(fullcmd); }
  } else
  if ( linux_syntax == 0 )
  {
   fullcmd.Clear();
   fullcmd<<wxT("copy "); fullcmd<<from; fullcmd<<wxT(" "); fullcmd<<to;
   if ( FileExists(from) ) { wxShell(fullcmd); }
  } else
  {
     // C COPY FILE IMPLEMENTATION!
     char frompath[1024]={0};
     strcpy(frompath, (const char*) from.mb_str(wxConvUTF8) );
     char topath[1024]={0};
     strcpy(topath, (const char*) to.mb_str(wxConvUTF8) );

     if ( _C_FileCopy(frompath,topath)!= 1 )
      {
         MessageCstr((char*)"Error Copying File!\n");
      }
  }
}



void WebSniperFrame::ReplaceDownloadedWithDiff(wxString oldfile,wxString newfile)
{
  wxString fullcmd;  fullcmd.Clear();
  wxString diffedfile;  diffedfile.Clear();
  diffedfile<<newfile;
  diffedfile<<wxT("_diff");

  int linux_syntax;
  #if defined(__WXMSW__)
        linux_syntax=0;
  #elif defined(__UNIX__)
        linux_syntax=1;
  #endif

  if ( linux_syntax == 1 )
  {
   //diff _rawfile0 _rawfile0.orig > _rawfile_diff
   fullcmd.Clear(); fullcmd<<wxT("diff "); fullcmd<<oldfile; fullcmd<<wxT(" "); fullcmd<<newfile; fullcmd<<wxT(" > "); fullcmd<<diffedfile;
   if ( FileExists(oldfile)&&FileExists(newfile) ) { wxShell(fullcmd); }
  } else
  {
    fprintf(stderr,"Unable to get new differences for windows :( \n");
  }


  MyCopyFile(diffedfile,newfile);
}
/*
>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>
>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>
>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>

              HELPER FUNCTIONS END

>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>
>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>
>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>
*/



WebSniperFrame::WebSniperFrame(wxWindow* parent,wxWindowID id)
{
    //(*Initialize(WebSniperFrame)
    wxMenuItem* MenuItem2;
    wxMenuItem* MenuItem1;
    wxMenu* Menu1;
    wxMenuBar* MenuBar1;
    wxMenu* Menu2;

    Create(parent, id, _("WebSniper "), wxDefaultPosition, wxDefaultSize, wxDEFAULT_FRAME_STYLE, _T("id"));
    SetClientSize(wxSize(680,542));
    Sources = new wxListBox(this, ID_LISTBOX1, wxPoint(520,32), wxSize(160,304), 0, 0, 0, wxDefaultValidator, _T("ID_LISTBOX1"));
    Website = new wxTextCtrl(this, ID_TEXTCTRL1, wxEmptyString, wxPoint(520,344), wxSize(112,27), 0, wxDefaultValidator, _T("ID_TEXTCTRL1"));
    AddWebsite = new wxButton(this, ID_BUTTON1, _("+"), wxPoint(632,344), wxSize(24,24), 0, wxDefaultValidator, _T("ID_BUTTON1"));
    RemWebsite = new wxButton(this, ID_BUTTON2, _("-"), wxPoint(656,344), wxSize(24,24), 0, wxDefaultValidator, _T("ID_BUTTON2"));
    Output = new wxTextCtrl(this, ID_TEXTCTRL2, wxEmptyString, wxPoint(0,32), wxSize(512,336), wxTE_MULTILINE|wxTE_READONLY, wxDefaultValidator, _T("ID_TEXTCTRL2"));
    SearchTerms = new wxTextCtrl(this, ID_TEXTCTRL3, wxEmptyString, wxPoint(0,408), wxSize(680,27), wxTE_PROCESS_ENTER, wxDefaultValidator, _T("ID_TEXTCTRL3"));
    SearchButton = new wxButton(this, ID_BUTTON3, _("Search"), wxPoint(392,440), wxSize(160,48), 0, wxDefaultValidator, _T("ID_BUTTON3"));
    ProgressBar = new wxGauge(this, ID_GAUGE1, 100, wxPoint(0,464), wxSize(384,24), 0, wxDefaultValidator, _T("ID_GAUGE1"));
    StaticText1 = new wxStaticText(this, ID_STATICTEXT2, _("Websites to include :"), wxPoint(520,8), wxDefaultSize, 0, _T("ID_STATICTEXT2"));
    MailButton = new wxButton(this, ID_BUTTON4, _("Mail Results"), wxPoint(576,440), wxSize(104,24), 0, wxDefaultValidator, _T("ID_BUTTON4"));
    StaticText2 = new wxStaticText(this, ID_STATICTEXT1, _("Search Criteria"), wxPoint(8,384), wxDefaultSize, 0, _T("ID_STATICTEXT1"));
    StaticText3 = new wxStaticText(this, ID_STATICTEXT3, _("Progress : "), wxPoint(8,8), wxDefaultSize, 0, _T("ID_STATICTEXT3"));
    DownloadEnabled = new wxCheckBox(this, ID_CHECKBOX1, _("Download Live Data"), wxPoint(0,440), wxDefaultSize, 0, wxDefaultValidator, _T("ID_CHECKBOX1"));
    DownloadEnabled->SetValue(true);
    SoundOn = new wxCheckBox(this, ID_CHECKBOX2, _("Sound"), wxPoint(320,440), wxDefaultSize, 0, wxDefaultValidator, _T("ID_CHECKBOX2"));
    SoundOn->SetValue(true);
    ChangesOnly = new wxCheckBox(this, ID_CHECKBOX3, _("New Data Only"), wxPoint(176,440), wxDefaultSize, 0, wxDefaultValidator, _T("ID_CHECKBOX3"));
    ChangesOnly->SetValue(false);
    SeeButton = new wxButton(this, ID_BUTTON5, _("See Results"), wxPoint(576,464), wxSize(104,24), 0, wxDefaultValidator, _T("ID_BUTTON5"));
    Stop = new wxButton(this, ID_BUTTON6, _("S"), wxPoint(552,440), wxSize(24,48), 0, wxDefaultValidator, _T("ID_BUTTON6"));
    MenuBar1 = new wxMenuBar();
    Menu1 = new wxMenu();
    MenuItem1 = new wxMenuItem(Menu1, idMenuQuit, _("Quit\tAlt-F4"), _("Quit the application"), wxITEM_NORMAL);
    Menu1->Append(MenuItem1);
    MenuBar1->Append(Menu1, _("&File"));
    Menu2 = new wxMenu();
    MenuItem2 = new wxMenuItem(Menu2, idMenuAbout, _("About\tF1"), _("Show info about this application"), wxITEM_NORMAL);
    Menu2->Append(MenuItem2);
    MenuBar1->Append(Menu2, _("Help"));
    SetMenuBar(MenuBar1);
    StatusBar1 = new wxStatusBar(this, ID_STATUSBAR1, 0, _T("ID_STATUSBAR1"));
    int __wxStatusBarWidths_1[1] = { -1 };
    int __wxStatusBarStyles_1[1] = { wxSB_NORMAL };
    StatusBar1->SetFieldsCount(1,__wxStatusBarWidths_1);
    StatusBar1->SetStatusStyles(1,__wxStatusBarStyles_1);
    SetStatusBar(StatusBar1);

    Connect(ID_BUTTON1,wxEVT_COMMAND_BUTTON_CLICKED,(wxObjectEventFunction)&WebSniperFrame::OnAddWebsiteClick);
    Connect(ID_BUTTON2,wxEVT_COMMAND_BUTTON_CLICKED,(wxObjectEventFunction)&WebSniperFrame::OnRemWebsiteClick);
    Connect(ID_BUTTON3,wxEVT_COMMAND_BUTTON_CLICKED,(wxObjectEventFunction)&WebSniperFrame::OnSearchButtonClick);
    Connect(ID_BUTTON4,wxEVT_COMMAND_BUTTON_CLICKED,(wxObjectEventFunction)&WebSniperFrame::OnMailButtonClick);
    Connect(ID_BUTTON5,wxEVT_COMMAND_BUTTON_CLICKED,(wxObjectEventFunction)&WebSniperFrame::OnSeeButtonClick);
    Connect(ID_BUTTON6,wxEVT_COMMAND_BUTTON_CLICKED,(wxObjectEventFunction)&WebSniperFrame::OnStopClick);
    Connect(idMenuQuit,wxEVT_COMMAND_MENU_SELECTED,(wxObjectEventFunction)&WebSniperFrame::OnQuit);
    Connect(idMenuAbout,wxEVT_COMMAND_MENU_SELECTED,(wxObjectEventFunction)&WebSniperFrame::OnAbout);
    //*)

      #if defined(__WXMSW__)
      SetBackgroundColour(wxSystemSettings::GetColour(wxSYS_COLOUR_MENUBAR));
      #elif defined(__UNIX__)

      #endif

    Connect(ID_TEXTCTRL3, wxEVT_COMMAND_TEXT_ENTER, wxCommandEventHandler( WebSniperFrame::OnSearchButtonClick ) );

    CreateHomeFolderWithFiles();


    LoadSources();
    StopEnabled=0;
}

WebSniperFrame::~WebSniperFrame()
{
    //(*Destroy(WebSniperFrame)
    //*)
}


void WebSniperFrame::LoadSources()
{

    ReadAString(keywords_str,keywords_str_contents,MAX_STR);
    SearchTerms->SetValue(_U(keywords_str_contents));


    wxString state;

    fprintf(stderr,"Starting Source read \n");
    struct AFSContext aro;

    state.Clear();

    if ( StartParsingFile (&aro ,sources_str) )
    {
       char  line[1024]={0};
       unsigned int linelen=1024;
       CountSources = 0;

       while (GetNextLineFile(&aro,line,linelen) )
      {
       state.Clear(); state<<_U(line);
       if ( state.Length() >=6 )
        {
          Sources->InsertItems(1,&state,0);
          ++CountSources;
        }
      }

      StopParsingFile (&aro);
    } else
    {
      state<<wxT("Failed Parsing Sources File..!\n");
      state<<wxT("Check file sources.ini");
      Output->AppendText(state);
    }

    fprintf(stderr,"Source read Done \n");
}



void WebSniperFrame::OnQuit(wxCommandEvent& event)
{
    Close();
}

void WebSniperFrame::OnAbout(wxCommandEvent& event)
{
    wxString msg = wxbuildinfo(long_f);
    wxMessageBox(msg, _("Welcome to WebSniper! :) "));
}


void WebSniperFrame::DownloadSite(wxString sitename,wxString filename)
{
    wxString state;
    state.Clear();

    if ( !DownloadEnabled->IsChecked() )
     {
       state<<wxT(" will not download ") , state<<sitename , state<<wxT(" , downloads disabled\n ");
       Output->AppendText(state);
       return;
     }

    wxURL *http=0;
    http = new wxURL(sitename);
    http->GetProtocol().SetTimeout(20);
   if ( http == 0 ) { MessageCstr((char*)"Error Creating URL downloader \n"); }
     else
   if (http->GetError() == wxURL_NOERR)
  {
    fprintf(stderr,"Fetching Input Stream ..");
    wxInputStream* in = http->GetInputStream();
    fprintf(stderr,"got it\n");

    if ( in != 0 )
    {
      wxFileOutputStream outFile(filename);
      if ( outFile.Ok() )
      {
        outFile.Write(*in);

        state<<wxT("- Site `") ,  state<<sitename , state<<wxT("` opened!\n");

        Output->AppendText(state);
        PlaySound(wxT("load.wav"));
      } else
      { PlaySound(wxT("error.wav")); fprintf(stderr,"wxFileOutputStream not ready :( \n"); }
    } else
    { PlaySound(wxT("error.wav")); fprintf(stderr,"Null Input Stream Returned , saving segfault\n"); }
    delete in;
  } else
  {
    MessageCstr((char*)"Site could not be opened!\n");
  }
  delete http;
}



void WebSniperFrame::OnSearchButtonClick(wxCommandEvent& event)
{
    StopEnabled = 0;
    wxString state,keyword_hit;
    state.Clear() , state.Printf(wxT("Starting a new search on the internet..!\n"));
    Output->AppendText(state);

    InputParser search_criteria;

    wxString sterm = SearchTerms->GetValue();
    unsigned int sterm_count = search_criteria.SeperateWordsCC(sterm.mb_str(wxConvUTF8));
    char term_memory[1024]={0};
    unsigned int term_memory_length = 1024;

    state.Clear() , state<<wxT("A total of ");
    state<<sterm_count, state<<wxT(" search terms \n");
    Output->AppendText(state);

    if ( sterm_count == 0 )
    {
        state.Clear();
        state<<wxT(">>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>\n");
        state<<wxT("No Search terms were provided , cannot make a null search !! \n");
        state<<wxT("Please fill in the `Search Criteria` box with comma seperated keywords\n");
        state<<wxT("i.e. linux,unix,wxWidgets,C++,Programmer \n");
        state<<wxT(">>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>\n");
        Output->AppendText(state);
        return;
    }

    if ( ChangesOnly->IsChecked() )
    {
        #if defined(__WXMSW__)
        state.Clear();
        state<<wxT(">>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>\n");
        state<<wxT("Due to no capability of using diff in Windows\n");
        state<<wxT("Please uncheck the selection New Data Only\n");
        state<<wxT(">>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>\n");
        Output->AppendText(state);
        return;
        #elif defined(__UNIX__)

        #endif
    }

    strncpy(keywords_str_contents,sterm.mb_str(wxConvUTF8),1024);
    WriteAString(keywords_str,keywords_str_contents,1024);

    int live_websites = 1;
    if ( !DownloadEnabled->IsChecked() ) live_websites=0;

    WriteReportHeader(report_filename_str,live_websites,sterm_count,sterm.mb_str(wxConvUTF8),CountSources,Sources);


    wxString rawfile;
    wxString curr_rawfile;
    wxString last_rawfile;
    wxString cleanrawfile;
    char rawfile_s[512]={0};
    char cleanrawfile_s[512]={0};
    unsigned int total_keywords_present=0;

    HTMLAnalyzer * site=0;

    ProgressBar->SetRange(CountSources-1);
    unsigned int i=0;
    for (i=0; i<CountSources; i++ )
    {
      if ( StopEnabled == 1 ) { MessageCstr((char*)"Terminating Search!\n"); break;}

      fprintf(stderr,"Going for site %u \n",i);
      state.Clear();
      state << wxT(" `") , state << Sources->GetString(i) , state << wxT("` ");

      rawfile.Clear() , rawfile<<_U(raw_directory); rawfile<<wxT("/_rawfile") , rawfile<<i;
      curr_rawfile.Clear() , curr_rawfile<<rawfile , curr_rawfile<<wxT(".current");

      last_rawfile.Clear() , last_rawfile << rawfile , last_rawfile << wxT(".last");
      cleanrawfile.Clear() , cleanrawfile << rawfile , cleanrawfile << wxT(".clean");

      fprintf(stderr,"Copying File %u\n",i);
      MyCopyFile(curr_rawfile,last_rawfile);
      fprintf(stderr,"Downloading Site %u\n",i);
      DownloadSite(Sources->GetString(i),rawfile);
      fprintf(stderr,"Copying File %u\n",i);
      MyCopyFile(rawfile,curr_rawfile);
      if ( ChangesOnly->IsChecked() ) {  fprintf(stderr,"ReplaceDownloadedWithDiff %u\n",i);
                                          ReplaceDownloadedWithDiff(last_rawfile,rawfile);  }

      state << wxT(" stored to ") , state << rawfile , state << wxT(" \n");

      strcpy(rawfile_s, (const char*) rawfile.mb_str(wxConvUTF8) );
      strcpy(cleanrawfile_s, (const char*) cleanrawfile.mb_str(wxConvUTF8) );

      fprintf(stderr,"Reading rawfile %s !\n",rawfile_s);
      site = new HTMLAnalyzer((unsigned char *) rawfile_s,(unsigned char *) cleanrawfile_s);
      fprintf(stderr,"Cleaning HTML Tags !\n");

      char clean_file_cstr[512]={0};
      strcpy(clean_file_cstr,cleanrawfile.mb_str(wxConvUTF8));
      site->CleanHTMLTags(clean_file_cstr);

      total_keywords_present=0;
      unsigned int cur_occurances=0;
      unsigned int occurances[sterm_count+1];
      fprintf(stderr,"Checking out occurances on each of the %u terms of search supplied !\n",sterm_count);
      for (unsigned int z=0; z<sterm_count; z++)
      {
          search_criteria.GetWord(z,term_memory,term_memory_length);
          fprintf(stderr,"Checking out %s ",term_memory);
          cur_occurances = site->WordOccurances(( unsigned char *) term_memory ,  search_criteria.GetWordLength(z));
          occurances[z]=cur_occurances;
          if  ( cur_occurances > 0 ) ++total_keywords_present;
          fprintf(stderr,"Found in %u spots \n",cur_occurances);
          //char test = getchar();
      }
      if ( total_keywords_present > 0 )
      {
           PlaySound(wxT("shot.wav"));
           state.Clear();
           state << wxT("Found Results at website ");
           state << Sources->GetString(i);
           state << wxT(" ( ");


           keyword_hit.clear();
           for (unsigned int z=0; z<sterm_count; z++)
           {
             if ( occurances[z] > 0 )
             {
              keyword_hit << occurances[z];
              keyword_hit << wxT(" x ");
              search_criteria.GetWord(z,term_memory,term_memory_length);
              keyword_hit << _U(term_memory);
              if ( z >= sterm_count-1 ) keyword_hit << wxT(" "); else
                                      keyword_hit << wxT(" , ");
             }
           }

           state << keyword_hit;
           state << wxT(" ) \n");

           AddReportData(keyword_hit,Sources->GetString(i),i);
           Output->AppendText(state);

      }

    delete site;
    site =0;
    ProgressBar->SetValue(i);
    }

    CloseReport();
    PlaySound(wxT("pop.wav"));
    Refresh();
}

void WebSniperFrame::OnMailButtonClick(wxCommandEvent& event)
{
       #if defined(__WXMSW__)
       wxMessageBox(wxT("Function not implemented in Windows , please send the generated file in Reports folder manually"),wxT("WebSniper"));
       #elif defined(__UNIX__)
       wxMessageBox(wxT("Function not implemented in Linux , please send the generated file in Reports folder manually"),wxT("WebSniper"));
      #endif
}



void WebSniperFrame::OnSeeButtonClick(wxCommandEvent& event)
{

  if ( FileExists(report_filename_str) )
  {

    wxBoxSizer* topsizer;
    wxDialog dlg(this,wxID_ANY,wxString(_("Generated Results")));
    wxHtmlWindow * html;
    topsizer = new wxBoxSizer(wxVERTICAL);

    html = new wxHtmlWindow(&dlg,wxID_ANY,wxDefaultPosition,wxSize(600,400),wxHW_SCROLLBAR_AUTO );
    html ->LoadPage(_U(report_filename_str));
    html ->SetSize(590,390);
    topsizer->Add(html,1,wxALL,10);
    wxButton *but = new wxButton(&dlg,wxID_OK,wxT("OK"));
    but->SetDefault();
    topsizer->Add(but,0,wxALL|wxALIGN_RIGHT,15);

    dlg.SetSizer(topsizer);
    topsizer->Fit(&dlg);
    dlg.ShowModal();

  }
}

void WebSniperFrame::OnAddWebsiteClick(wxCommandEvent& event)
{
    wxString tmpstr = Website->GetValue();
    Sources->InsertItems(1,&tmpstr,0);
    tmpstr.Clear();
    Website->SetValue(tmpstr);
}

void WebSniperFrame::OnRemWebsiteClick(wxCommandEvent& event)
{
 if ( wxNOT_FOUND == Sources->GetSelection() )
  {
    wxMessageBox(wxT("Select a website from the drop down list to remove it!"),wxT("No selection"));
  } else
  {
   Sources->Delete(Sources->GetSelection());
  }
}



void WebSniperFrame::OnStopClick(wxCommandEvent& event)
{
}
