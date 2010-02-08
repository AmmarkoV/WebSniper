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
#include "../InputParser/InputParser.h"
#include "../AFScripts/AFScripts.h"

//(*InternalHeaders(WebSniperFrame)
#include <wx/string.h>
#include <wx/intl.h>
//*)

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
const long WebSniperFrame::ID_BUTTON5 = wxNewId();
const long WebSniperFrame::ID_STATICTEXT1 = wxNewId();
const long WebSniperFrame::ID_STATICTEXT3 = wxNewId();
const long WebSniperFrame::ID_CHECKBOX1 = wxNewId();
const long WebSniperFrame::ID_CHECKBOX2 = wxNewId();
const long WebSniperFrame::idMenuQuit = wxNewId();
const long WebSniperFrame::idMenuAbout = wxNewId();
const long WebSniperFrame::ID_STATUSBAR1 = wxNewId();
//*)

BEGIN_EVENT_TABLE(WebSniperFrame,wxFrame)
    //(*EventTable(WebSniperFrame)
    //*)
END_EVENT_TABLE()


inline wxString _U(const char String[] = "")
{
  return wxString(String, wxConvUTF8);
}

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
    SearchButton = new wxButton(this, ID_BUTTON3, _("Search"), wxPoint(392,440), wxSize(184,48), 0, wxDefaultValidator, _T("ID_BUTTON3"));
    ProgressBar = new wxGauge(this, ID_GAUGE1, 100, wxPoint(0,464), wxSize(384,24), 0, wxDefaultValidator, _T("ID_GAUGE1"));
    StaticText1 = new wxStaticText(this, ID_STATICTEXT2, _("Websites to include :"), wxPoint(520,8), wxDefaultSize, 0, _T("ID_STATICTEXT2"));
    MailButton = new wxButton(this, ID_BUTTON4, _("Mail Results"), wxPoint(576,440), wxSize(104,24), 0, wxDefaultValidator, _T("ID_BUTTON4"));
    SaveButton = new wxButton(this, ID_BUTTON5, _("Save Results"), wxPoint(576,464), wxSize(104,24), 0, wxDefaultValidator, _T("ID_BUTTON5"));
    StaticText2 = new wxStaticText(this, ID_STATICTEXT1, _("Search Criteria"), wxPoint(8,384), wxDefaultSize, 0, _T("ID_STATICTEXT1"));
    StaticText3 = new wxStaticText(this, ID_STATICTEXT3, _("Progress : "), wxPoint(8,8), wxDefaultSize, 0, _T("ID_STATICTEXT3"));
    DownloadEnabled = new wxCheckBox(this, ID_CHECKBOX1, _("Download Live Data"), wxPoint(0,440), wxDefaultSize, 0, wxDefaultValidator, _T("ID_CHECKBOX1"));
    DownloadEnabled->SetValue(true);
    SoundOn = new wxCheckBox(this, ID_CHECKBOX2, _("Sound"), wxPoint(320,440), wxDefaultSize, 0, wxDefaultValidator, _T("ID_CHECKBOX2"));
    SoundOn->SetValue(true);
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
    Connect(idMenuQuit,wxEVT_COMMAND_MENU_SELECTED,(wxObjectEventFunction)&WebSniperFrame::OnQuit);
    Connect(idMenuAbout,wxEVT_COMMAND_MENU_SELECTED,(wxObjectEventFunction)&WebSniperFrame::OnAbout);
    //*)

      #if defined(__WXMSW__)
      SetBackgroundColour(wxSystemSettings::GetColour(wxSYS_COLOUR_MENUBAR));
      #elif defined(__UNIX__)

      #endif

    Connect(ID_TEXTCTRL3, wxEVT_COMMAND_TEXT_ENTER, wxCommandEventHandler( WebSniperFrame::OnSearchButtonClick ) );
    LoadSources();
}

WebSniperFrame::~WebSniperFrame()
{
    //(*Destroy(WebSniperFrame)
    //*)
}


void WebSniperFrame::LoadSources()
{
    wxString state;

    fprintf(stderr,"Starting Source read \n");
    struct AFSContext aro;

    state.Clear();
    if ( StartParsingFile (&aro ,"sources.ini\0") )
    {
       char  line[1024];
       unsigned int linelen=1024;
       CountSources = 0;

       while (GetNextLineFile(&aro,line,linelen) )
      {
       state.Clear();
       state<<_U(line);
       if ( state.Length() >=6 )
        {
         Sources->InsertItems(1,&state,0);
         //Output->AppendText(state);
         ++CountSources;
        }
      }

    StopParsingFile (&aro);
    } else
    {
      state<<wxT("Failed Parsing Sources File..!");
      Output->AppendText(state);
    }

    fprintf(stderr,"Source read Done \n");
}

void  WebSniperFrame::PlaySound(wxString sndname)
{

  if ( !SoundOn->IsChecked() ) { return; }

  wxString fullcmd;  fullcmd.Clear();
  int linux_sound;

  #if defined(__WXMSW__)
        linux_sound=0;
  #elif defined(__UNIX__)
        linux_sound=1;
  #endif

  if ( linux_sound == 1 )
  {
   fullcmd<<wxT("aplay Sounds/");
   fullcmd<<sndname;
   wxShell(fullcmd);
  } else
  {
   fullcmd<<wxT("Sounds/");
   fullcmd<<sndname;
   wxSound newsound(fullcmd);
   if ( newsound.IsOk() )
    {
      if ( newsound.Play(wxSOUND_ASYNC) )
      {
        // SOUND OK!
      } else
      { /*wxMessageBox(wxT("Sound error , playing"), wxT("Sound error , playing"));*/ }
    }else
    { /*wxMessageBox(wxT("Sound error , loading"), wxT("Sound error , loading"));*/ }
  }
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

    wxURL http(sitename);

    if (http.GetError() == wxURL_NOERR)
  {


    wxInputStream* in = http.GetInputStream();

    wxFileOutputStream outFile(filename);
    outFile.Write(*in);

    state<<wxT("- Site `") ,  state<<sitename , state<<wxT("` opened!\n");

    Output->AppendText(state);
    PlaySound(wxT("load.wav"));

    delete in;
  } else
  {
    state.Printf(wxT("Site could not be opened!\n")) , Output->AppendText(state);
  }
}




void WebSniperFrame::OnSearchButtonClick(wxCommandEvent& event)
{
    wxString state;
    state.Clear() , state.Printf(wxT("Starting a new search on the internet..!\n"));
    Output->AppendText(state);

    InputParser search_criteria;

    wxString sterm = SearchTerms->GetValue();
    unsigned int sterm_count = search_criteria.SeperateWordsCC(sterm.mb_str(wxConvUTF8));
    char term_memory[1024];
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

    wxString rawfile;
    wxString cleanrawfile;
    char rawfile_s[256];
    char cleanrawfile_s[256];
    unsigned int total_keywords_present;


    ProgressBar->SetRange(CountSources-1);
    for ( int i=0; i<CountSources; i++ )
    {
      state.Clear();
      state << wxT(" `") , state << Sources->GetString(i) , state << wxT("` ");

      rawfile.Clear();
      rawfile<<wxT("raw/_rawfile");
      rawfile<<i;

      cleanrawfile << rawfile;
      cleanrawfile << wxT(".clean");
      DownloadSite(Sources->GetString(i),rawfile);

      state << wxT(" stored to ");
      state << rawfile;
      state << wxT(" \n");
      //Output->AppendText(state);

      strcpy(rawfile_s, (const char*) rawfile.mb_str(wxConvUTF8) );
      strcpy(cleanrawfile_s, (const char*) cleanrawfile.mb_str(wxConvUTF8) );

      fprintf(stderr,"Reading rawfile %s !\n",rawfile_s);
      HTMLAnalyzer site((unsigned char *) rawfile_s,(unsigned char *) cleanrawfile_s);
      fprintf(stderr,"Cleaning HTML Tags !\n");
      site.CleanHTMLTags();

      total_keywords_present=0;
      unsigned int cur_occurances=0;
      unsigned int occurances[sterm_count+1];
      fprintf(stderr,"Checking out occurances on each of the %u terms of search supplied !\n",sterm_count);
      for (unsigned int z=0; z<sterm_count; z++)
      {
          search_criteria.GetWord(z,term_memory,term_memory_length);
          fprintf(stderr,"Checking out %s ",term_memory);
          cur_occurances = site.WordOccurances(( unsigned char *) term_memory ,  search_criteria.GetWordLength(z));
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
           for (unsigned int z=0; z<sterm_count; z++)
           {
             if ( occurances[z] > 0 )
             {
              state << occurances[z];
              state << wxT(" x ");
              search_criteria.GetWord(z,term_memory,term_memory_length);
              state << _U(term_memory);
              if ( z == sterm_count ) state << wxT(" "); else
                                      state << wxT(" , ");
             }
           }

           state << wxT(" ) \n");
           Output->AppendText(state);

      }
    ProgressBar->SetValue(i);
    }

    Refresh();
}

void WebSniperFrame::OnMailButtonClick(wxCommandEvent& event)
{

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
