#include <settings.hh>

#include <devices/webcam.hh>
#include <devices/dslr.hh>
#include <devices/printer.hh>

using namespace std;

SettingsFrame::SettingsFrame(const wxString &title, const wxPoint &pos, 
    const wxSize &sz) : wxFrame(NULL, wxID_ANY, title, pos, sz)
{
    wxBoxSizer *bSizerHorz = new wxBoxSizer(wxHORIZONTAL);
    this->SetSizer(bSizerHorz);

    // Settings frame has three parts: webcam, printer, and DSLR settings
    wxSize size(160, 40);

    // Left part: webcam settings
    wxStaticBox *sbCam = new wxStaticBox(this, wxID_ANY, "Webcam");
    wxStaticBoxSizer *sbSzCam = new wxStaticBoxSizer(sbCam, wxVERTICAL);
    bSizerHorz->Add(sbSzCam, 1, wxALL | wxEXPAND, 5);

    wxGridSizer *gSzCam = new wxGridSizer(2, 2, 5, 5);
    sbSzCam->Add(gSzCam, wxALL | wxEXPAND, 5);

    wxStaticText *txtCamPort = new wxStaticText(this, wxID_ANY, "Device:");
    m_choiceCam = new wxChoice(this, wxID_ANY, wxDefaultPosition, size);
    wxButton *btnCamFind = new wxButton(this, wxID_ANY, "Find devices...",
        wxDefaultPosition, size);
    wxButton *btnCamApply = new wxButton(this, wxID_ANY, "Apply", 
        wxDefaultPosition, size);
    gSzCam->Add(txtCamPort, 1, wxALL | wxALIGN_CENTER, 5);
    gSzCam->Add(m_choiceCam, 1, wxALL | wxALIGN_CENTER, 5);
    gSzCam->Add(btnCamFind, 1, wxALL | wxALIGN_CENTER, 5);
    gSzCam->Add(btnCamApply, 1, wxALL | wxALIGN_CENTER, 5);

    // Mid part: DSLR settings
    wxStaticBox *sbDslr = new wxStaticBox(this, wxID_ANY, "DSLR");
    wxStaticBoxSizer *sbSzDslr = new wxStaticBoxSizer(sbDslr, wxVERTICAL);
    bSizerHorz->Add(sbSzDslr, 1, wxALL | wxEXPAND, 5);

    wxGridSizer *gSzDslr = new wxGridSizer(2, 2, 5, 5);
    sbSzDslr->Add(gSzDslr, wxALL | wxEXPAND, 5);

    wxStaticText *txtDslrPort = new wxStaticText(this, wxID_ANY, "Device:");
    m_choiceDslr = new wxChoice(this, wxID_ANY, wxDefaultPosition, size);
    wxButton *btnDslrFind = new wxButton(this, wxID_ANY, "Find devices...",
        wxDefaultPosition, size);
    wxButton *btnDslrApply = new wxButton(this, wxID_ANY, "Apply", 
        wxDefaultPosition, size);
    gSzDslr->Add(txtDslrPort, 1, wxALL | wxALIGN_CENTER, 5);
    gSzDslr->Add(m_choiceDslr, 1, wxALL | wxALIGN_CENTER, 5);
    gSzDslr->Add(btnDslrFind, 1, wxALL | wxALIGN_CENTER, 5);
    gSzDslr->Add(btnDslrApply, 1, wxALL | wxALIGN_CENTER, 5);

    // Right part: Printer settings
    wxStaticBox *sbPrinter = new wxStaticBox(this, wxID_ANY, "Printer");
    wxStaticBoxSizer *sbSzPrinter = new wxStaticBoxSizer(sbPrinter, wxVERTICAL);
    bSizerHorz->Add(sbSzPrinter, 1, wxALL | wxEXPAND, 5);

    wxGridSizer *gSzPrinter = new wxGridSizer(2, 2, 5, 5);
    sbSzPrinter->Add(gSzPrinter, wxALL | wxEXPAND, 5);

    wxStaticText *txtPrinterPort = new wxStaticText(this, wxID_ANY, "Device:");
    m_choicePrinter = new wxChoice(this, wxID_ANY, wxDefaultPosition, size);
    wxButton *btnPrinterFind = new wxButton(this, wxID_ANY, "Find devices...",
        wxDefaultPosition, size);
    wxButton *btnPrinterApply = new wxButton(this, wxID_ANY, "Apply", 
        wxDefaultPosition, size);
    gSzPrinter->Add(txtPrinterPort, 1, wxALL | wxALIGN_CENTER, 5);
    gSzPrinter->Add(m_choicePrinter, 1, wxALL | wxALIGN_CENTER, 5);
    gSzPrinter->Add(btnPrinterFind, 1, wxALL | wxALIGN_CENTER, 5);
    gSzPrinter->Add(btnPrinterApply, 1, wxALL | wxALIGN_CENTER, 5);

    this->Layout();

    // Bindings
    btnCamFind->Bind(wxEVT_BUTTON, &SettingsFrame::OnButtonListCams, this);
    btnDslrFind->Bind(wxEVT_BUTTON, &SettingsFrame::OnButtonListDSLRs, this);
    btnPrinterFind->Bind(wxEVT_BUTTON, &SettingsFrame::OnButtonListPrinters, this);

    return;
}

void SettingsFrame::OnButtonListCams(wxCommandEvent &ev)
{
    vector<string> devices;
    int n = Webcam::getWebcamList(devices);
    wxMessageBox(wxString::Format("Found %i webcams!", n), "Find webcams...",
        wxOK);

    if (n == 0) return;
    
    for (auto dev : devices)
        m_choiceCam->Append(dev);

    return;
}

void SettingsFrame::OnButtonListDSLRs(wxCommandEvent &ev)
{
    vector<string> names, ports;
    int n = Dslr::getDslrList(names, ports);
    wxMessageBox(wxString::Format("Found %i printers!", n), "Find DSLRs...",
        wxOK);

    if (n == 0) return;
    
    for (auto name : names)
        m_choiceDslr->Append(name);

    return;
}

void SettingsFrame::OnButtonListPrinters(wxCommandEvent &ev)
{
    vector<string> names, interfaces;
    int n = Printer::getPrinterList(names, interfaces);
    wxMessageBox(wxString::Format("Found %i printers!", n), "Find printers...",
        wxOK);

    if (n == 0) return;
    
    for (auto name : names)
        m_choicePrinter->Append(name);

    return;
}

