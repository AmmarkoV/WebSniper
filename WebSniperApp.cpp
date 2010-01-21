/***************************************************************
 * Name:      WebSniperApp.cpp
 * Purpose:   Code for Application Class
 * Author:    Ammar Qammaz (ammarkov@gmail.com)
 * Created:   2010-01-06
 * Copyright: Ammar Qammaz (http://62.103.22.50)
 * License:
 **************************************************************/

#include "WebSniperApp.h"

//(*AppHeaders
#include "WebSniperMain.h"
#include <wx/image.h>
//*)

IMPLEMENT_APP(WebSniperApp);

bool WebSniperApp::OnInit()
{
    //(*AppInitialize
    bool wxsOK = true;
    wxInitAllImageHandlers();
    if ( wxsOK )
    {
    	WebSniperFrame* Frame = new WebSniperFrame(0);
    	Frame->Show();
    	SetTopWindow(Frame);
    }
    //*)
    return wxsOK;

}
