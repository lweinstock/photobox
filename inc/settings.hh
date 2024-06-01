#ifndef PB_SETTINGS_HH
#define PB_SETTINGS_HH

#include <wx/wxprec.h>
#ifndef WX_PRECOMP
    #include <wx/wx.h>
#endif

class SettingsFrame : public wxFrame
{
public:
    SettingsFrame(const wxString &title, const wxPoint &pos, const wxSize &sz);
    ~SettingsFrame() {};

    void OnButtonListCams(wxCommandEvent &ev);
    void OnButtonListDSLRs(wxCommandEvent &ev);
    void OnButtonListPrinters(wxCommandEvent &ev);

private:
    wxChoice *m_choiceCam;
    wxChoice *m_choiceDslr;
    wxChoice *m_choicePrinter;

};

#endif