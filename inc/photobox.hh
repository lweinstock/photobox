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

class DynamicBitmap : public wxPanel
{
public:
    DynamicBitmap(wxWindow* parent, wxWindowID id, 
        const wxPoint &pos = wxDefaultPosition, 
        const wxSize &size = wxDefaultSize)
        : wxPanel(parent, id, pos, size) {};

    void SetBitmap(const wxBitmap &bmp) { m_bitmap = bmp; }
    wxBitmap GetBitmap() const { return m_bitmap; }

    void PaintEvent(wxPaintEvent &ev);
    void PaintNow();

private:
    wxBitmap m_bitmap;

    void Render(wxDC &dc);
};

/*
 *  Main Window of the App
 */

class MainFrame : public wxFrame
{
public:
    MainFrame(const wxString &title, const wxPoint &pos, const wxSize& size);
    ~MainFrame() {};

    void OnTimerVideo(wxTimerEvent &ev);

private:
    DynamicBitmap *m_view;
    wxBitmap m_bmp;
    wxTimer m_timer;
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
    std::shared_ptr<Dslr> GetDslr();
    std::shared_ptr<Printer> GetPrinter();
    std::shared_ptr<Webcam> GetWebcam();

private:
    std::shared_ptr<Dslr> m_dslr;
    std::shared_ptr<Printer> m_printer;
    std::shared_ptr<Webcam> m_webcam;

    MainFrame* m_mainFrame;
};

wxIMPLEMENT_APP(PhotoBox);

#endif