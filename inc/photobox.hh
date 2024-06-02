#ifndef PHOTOBOX_APP_HH
#define PHOTOBOX_APP_HH 

#include <wx/wxprec.h>
#ifndef WX_PRECOMP
    #include <wx/wx.h>
#endif

#include <memory>   // Smart pointers

#include <devices/dslr.hh>
#include <devices/printer.hh>
#include <devices/webcam.hh>

#include <settings.hh>
#include <view.hh>

/*
 *  The actual app!
 */

class PhotoBox: public wxApp
{
public:
    virtual bool OnInit();

    virtual void OnUnhandledException() override { throw; }
    virtual bool OnExceptionInMainLoop() override;

    // Initialise hardware
    bool SetDslr(std::string &port);
    bool SetPrinter(std::string &name);
    bool SetWebcam(std::string &dev);

    // Hardware can be accessed via 'auto ptr = wxGetApp().GetDslr/Printer/...'
    std::shared_ptr<Dslr> GetDslr();
    std::shared_ptr<Printer> GetPrinter();
    std::shared_ptr<Webcam> GetWebcam();

private:
    std::shared_ptr<Dslr> m_dslr;
    std::shared_ptr<Printer> m_printer;
    std::shared_ptr<Webcam> m_webcam;

    ViewFrame *m_viewFrame;
    SettingsFrame *m_settingsFrame;
};

DECLARE_APP(PhotoBox)

#endif