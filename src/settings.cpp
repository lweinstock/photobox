#include <settings.hh>

#include <devices/webcam.hh>
#include <devices/dslr.hh>
#include <devices/printer.hh>

using namespace std;

SettingsFrame::SettingsFrame(const wxString &title, const wxPoint &pos, 
    const wxSize &sz) : wxFrame(NULL, wxID_ANY, title, pos, sz)
{
    wxBoxSizer *bSizer = new wxBoxSizer(wxVERTICAL);
    this->SetSizer(bSizer);
    wxSize size(160, 40);

    // Settings frame has three parts: webcam, printer, and DSLR settings


    // Upper part: webcam settings
    wxStaticBox *sbCam = new wxStaticBox(this, wxID_ANY, "Webcam");
    wxStaticBoxSizer *sbSzCam = new wxStaticBoxSizer(sbCam, wxVERTICAL);
    bSizer->Add(sbSzCam, 1, wxALL | wxEXPAND, 10);

    wxGridSizer *gSzCam = new wxGridSizer(2, 2, 5, 5);
    sbSzCam->Add(gSzCam, wxALL | wxEXPAND, 5);

    wxStaticText *txtCamPort = new wxStaticText(this, wxID_ANY, "Device:");
    m_choiceCam = new wxChoice(this, wxID_ANY, wxDefaultPosition, size);
    wxButton *btnCamFind = new wxButton(this, wxID_ANY, "Find devices...",
        wxDefaultPosition, size);
    wxButton *btnCamSet = new wxButton(this, wxID_ANY, "Set", 
        wxDefaultPosition, size);
    gSzCam->Add(txtCamPort, 1, wxALL | wxALIGN_CENTER, 5);
    gSzCam->Add(m_choiceCam, 1, wxALL | wxALIGN_CENTER, 5);
    gSzCam->Add(btnCamFind, 1, wxALL | wxALIGN_CENTER, 5);
    gSzCam->Add(btnCamSet, 1, wxALL | wxALIGN_CENTER, 5);


    // Mid part: DSLR settings
    wxStaticBox *sbDslr = new wxStaticBox(this, wxID_ANY, "DSLR");
    wxStaticBoxSizer *sbSzDslr = new wxStaticBoxSizer(sbDslr, wxVERTICAL);
    bSizer->Add(sbSzDslr, 1, wxALL | wxEXPAND, 10);

    wxGridSizer *gSzDslr = new wxGridSizer(2, 2, 5, 5);
    sbSzDslr->Add(gSzDslr, wxALL | wxEXPAND, 5);

    wxStaticText *txtDslrPort = new wxStaticText(this, wxID_ANY, "Device:");
    m_choiceDslr = new wxChoice(this, wxID_ANY, wxDefaultPosition, size);
    wxButton *btnDslrFind = new wxButton(this, wxID_ANY, "Find devices...",
        wxDefaultPosition, size);
    wxButton *btnDslrSet = new wxButton(this, wxID_ANY, "Set", 
        wxDefaultPosition, size);
    gSzDslr->Add(txtDslrPort, 1, wxALL | wxALIGN_CENTER, 5);
    gSzDslr->Add(m_choiceDslr, 1, wxALL | wxALIGN_CENTER, 5);
    gSzDslr->Add(btnDslrFind, 1, wxALL | wxALIGN_CENTER, 5);
    gSzDslr->Add(btnDslrSet, 1, wxALL | wxALIGN_CENTER, 5);


    // Lower part: Printer settings
    wxStaticBox *sbPrinter = new wxStaticBox(this, wxID_ANY, "Printer");
    wxStaticBoxSizer *sbSzPrinter = new wxStaticBoxSizer(sbPrinter, wxVERTICAL);
    bSizer->Add(sbSzPrinter, 1, wxALL | wxEXPAND, 10);

    wxGridSizer *gSzPrinter = new wxGridSizer(2, 2, 5, 5);
    sbSzPrinter->Add(gSzPrinter, wxALL | wxEXPAND, 5);

    wxStaticText *txtPrinterPort = new wxStaticText(this, wxID_ANY, "Device:");
    m_choicePrinter = new wxChoice(this, wxID_ANY, wxDefaultPosition, size);
    wxButton *btnPrinterFind = new wxButton(this, wxID_ANY, "Find devices...",
        wxDefaultPosition, size);
    wxButton *btnPrinterSet = new wxButton(this, wxID_ANY, "Set", 
        wxDefaultPosition, size);
    gSzPrinter->Add(txtPrinterPort, 1, wxALL | wxALIGN_CENTER, 5);
    gSzPrinter->Add(m_choicePrinter, 1, wxALL | wxALIGN_CENTER, 5);
    gSzPrinter->Add(btnPrinterFind, 1, wxALL | wxALIGN_CENTER, 5);
    gSzPrinter->Add(btnPrinterSet, 1, wxALL | wxALIGN_CENTER, 5);

    this->Layout();

    // Bindings
    btnCamFind->Bind(wxEVT_BUTTON, &SettingsFrame::OnButtonListCams, this);
    btnCamSet->Bind(wxEVT_BUTTON, &SettingsFrame::OnButtonSetCam, this);
    btnDslrFind->Bind(wxEVT_BUTTON, &SettingsFrame::OnButtonListDSLRs, this);
    btnPrinterFind->Bind(wxEVT_BUTTON, &SettingsFrame::OnButtonListPrinters, this);

    return;
}

void SettingsFrame::OnButtonListCams(wxCommandEvent &ev)
{
    vector<string> devices;
    int n = Webcam::getWebcamList(devices);
    wxMessageBox(wxString::Format("Found %i webcams!", n), "", wxOK);

    if (n == 0) return;
    
    for (auto dev : devices)
        m_choiceCam->Append(dev);

    return;
}

void SettingsFrame::OnButtonSetCam(wxCommandEvent &ev)
{
    wxString dev = m_choiceCam->GetString( m_choiceCam->GetSelection() );
    return;
}


void SettingsFrame::OnButtonListDSLRs(wxCommandEvent &ev)
{
    vector<string> names, ports;
    int n = Dslr::getDslrList(names, ports);
    wxMessageBox(wxString::Format("Found %i DSLRs!", n), "", wxOK);

    if (n == 0) return;
    
    for (auto name : names)
        m_choiceDslr->Append(name);

    return;
}

void SettingsFrame::OnButtonListPrinters(wxCommandEvent &ev)
{
    vector<string> names, interfaces;
    int n = Printer::getPrinterList(names, interfaces);
    wxMessageBox(wxString::Format("Found %i printers!", n), "", wxOK);

    if (n == 0) return;
    
    for (auto name : names)
        m_choicePrinter->Append(name);

    return;
}

