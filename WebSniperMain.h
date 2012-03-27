/***************************************************************
 * Name:      WebSniperMain.h
 * Purpose:   Defines Application Frame
 * Author:    Ammar Qammaz (ammarkov@gmail.com)
 * Created:   2010-01-06
 * Copyright: Ammar Qammaz (http://62.103.22.50)
 * License:
 **************************************************************/

#ifndef WEBSNIPERMAIN_H
#define WEBSNIPERMAIN_H

//(*Headers(WebSniperFrame)
#include <wx/gauge.h>
#include <wx/checkbox.h>
#include <wx/button.h>
#include <wx/menu.h>
#include <wx/statusbr.h>
#include <wx/frame.h>
#include <wx/stattext.h>
#include <wx/textctrl.h>
#include <wx/listbox.h>
//*)

class WebSniperFrame: public wxFrame
{
    public:

        WebSniperFrame(wxWindow* parent,wxWindowID id = -1);
        virtual ~WebSniperFrame();
        void LoadSources();
        void MessageCstr(char * msg);
        void PlaySound(wxString sndname);
        void MyCopyFile(wxString from,wxString to);
        void ReplaceDownloadedWithDiff(wxString oldfile,wxString newfile);
        void CreateHomeFolderWithFiles();
    private:
        //(*Handlers(WebSniperFrame)
        void OnQuit(wxCommandEvent& event);
        void OnAbout(wxCommandEvent& event);
        void OnSearchButtonClick(wxCommandEvent& event);
        void OnMailButtonClick(wxCommandEvent& event);
        void OnAddWebsiteClick(wxCommandEvent& event);
        void OnRemWebsiteClick(wxCommandEvent& event);
        void OnSeeButtonClick(wxCommandEvent& event);
        void OnStopClick(wxCommandEvent& event);
        //*)
        void DownloadSite(wxString sitename,wxString filename);

        //(*Identifiers(WebSniperFrame)
        static const long ID_LISTBOX1;
        static const long ID_TEXTCTRL1;
        static const long ID_BUTTON1;
        static const long ID_BUTTON2;
        static const long ID_TEXTCTRL2;
        static const long ID_TEXTCTRL3;
        static const long ID_BUTTON3;
        static const long ID_GAUGE1;
        static const long ID_STATICTEXT2;
        static const long ID_BUTTON4;
        static const long ID_STATICTEXT1;
        static const long ID_STATICTEXT3;
        static const long ID_CHECKBOX1;
        static const long ID_CHECKBOX2;
        static const long ID_CHECKBOX3;
        static const long ID_BUTTON5;
        static const long ID_BUTTON6;
        static const long idMenuQuit;
        static const long idMenuAbout;
        static const long ID_STATUSBAR1;
        //*)

        //(*Declarations(WebSniperFrame)
        wxCheckBox* ChangesOnly;
        wxTextCtrl* Output;
        wxStatusBar* StatusBar1;
        wxButton* RemWebsite;
        wxButton* Stop;
        wxStaticText* StaticText1;
        wxButton* SeeButton;
        wxStaticText* StaticText3;
        wxButton* MailButton;
        wxCheckBox* DownloadEnabled;
        wxButton* SearchButton;
        wxTextCtrl* Website;
        wxStaticText* StaticText2;
        wxGauge* ProgressBar;
        wxTextCtrl* SearchTerms;
        wxCheckBox* SoundOn;
        wxListBox* Sources;
        wxButton* AddWebsite;
        //*)

        unsigned int CountSources;
        unsigned int StopEnabled;
        DECLARE_EVENT_TABLE()
};

#endif // WEBSNIPERMAIN_H
