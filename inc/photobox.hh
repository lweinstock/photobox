#ifndef PHOTOBOX_APP_HH
#define PHOTOBOX_APP_HH 

#include <wx/wxprec.h>
#ifndef WX_PRECOMP
    #include <wx/wx.h>
#endif

#include <memory>   // Smart pointers

#include <devices/dslr.hh>
#include <devices/printer.hh>

/*
 *  Main Window of the App
 */

class MainFrame : public wxFrame
{
public:
    MainFrame(const wxString &title, const wxPoint &pos, const wxSize& size);

private:

};

/*
 *  The actual app!
 */

class PhotoBox: public wxApp
{
public:
    virtual bool OnInit();

    virtual void OnUnhandledException() override { throw; }
    virtual bool OnExceptionInMainLoop() override;

    // Hardware can be accessed via 'auto ptr = wxGetApp().GetDslr/Printer/...'
    std::shared_ptr<Dslr> GetDslr() const { return m_dslr; }
    std::shared_ptr<Printer> GetPrinter() const { return m_printer; }

    // Frames
    MainFrame* GetMainFrame() const { return m_mainFrame; }

private:
    std::shared_ptr<Dslr> m_dslr;
    std::shared_ptr<Printer> m_printer;

    MainFrame* m_mainFrame;
};

wxIMPLEMENT_APP(PhotoBox);

#endif