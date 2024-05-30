#include <photobox.hh>

MainFrame::MainFrame(const wxString& title, const wxPoint& pos, 
    const wxSize& size) : wxFrame(NULL, wxID_ANY, title, pos, size)
{
    return;
}

bool PhotoBox::OnInit()
{
    m_mainFrame = new MainFrame("Hello World", wxPoint(50, 50), wxSize(450, 340));
    
    //m_mainFrame->ShowFullScreen(true); -> For final app!

    m_mainFrame->Maximize(true);
    m_mainFrame->Show();
    return true;
}

bool PhotoBox::OnExceptionInMainLoop()
{
    try
    {
        throw;
    }
    catch (PhotoboxException &ex)
    {
        wxMessageBox(ex.what(), "Exception in Main Loop", wxOK);
    }

    return true;
}
