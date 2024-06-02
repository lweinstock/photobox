#include <photobox.hh>

IMPLEMENT_APP(PhotoBox)

using namespace std;

bool PhotoBox::OnInit()
{
    // Initialise hardware
    m_webcam = std::make_shared<Webcam>("/dev/video4");
    m_dslr = std::make_shared<Dslr>();

    m_viewFrame = new ViewFrame("View Finder", wxDefaultPosition, 
        wxSize(450, 600));
    m_viewFrame->Maximize(true);    // Alt.: ShowFullScreen(true);
    m_viewFrame->Show();

/*
    m_settingsFrame = new SettingsFrame("Settings", wxDefaultPosition, 
        wxSize(450, 600));
    m_settingsFrame->Show();
*/

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

bool PhotoBox::SetDslr(std::string &port)
{    
    m_dslr.reset( new Dslr(port) );
    return false;
}

bool PhotoBox::SetPrinter(std::string &name)
{
    m_printer.reset( new Printer(name) ); 
    return false;
}

bool PhotoBox::SetWebcam(std::string &dev)
{
    m_webcam.reset( new Webcam(dev) );
    return true;
}

shared_ptr<Dslr> PhotoBox::GetDslr() 
{ 
    if (!m_dslr)
        throw PhotoboxException("DSLR not initialised");
    return m_dslr; 
}

shared_ptr<Printer> PhotoBox::GetPrinter() 
{ 
    if (!m_printer)
        throw PhotoboxException("Printer not initialised");
    return m_printer; 
}

shared_ptr<Webcam> PhotoBox::GetWebcam() 
{ 
    if (!m_webcam)
        throw PhotoboxException("Webcam not initialised");
    return m_webcam; 
}

