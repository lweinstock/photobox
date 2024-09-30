#include <photobox.hh>
#include <view.hh>

#include <wx/image.h>
#include <wx/bitmap.h>
#include <wx/mstream.h>

using namespace std;

void DynamicBitmap::PaintEvent(wxPaintEvent &ev)
{
    wxPaintDC dc(this);
    this->Render(dc);
    return;
}

void DynamicBitmap::PaintNow()
{
    wxClientDC dc(this);
    this->Render(dc);
    return;
}

void DynamicBitmap::Render(wxDC &dc)
{
    if ( !m_bitmap.IsOk() )
        return;
    int xcenter = (this->GetSize().GetWidth() - m_bitmap.GetWidth())/2;
    int ycenter = (this->GetSize().GetHeight() - m_bitmap.GetHeight())/2;
    wxPoint center(xcenter, ycenter);
    dc.DrawBitmap(m_bitmap, center);
    return;
}

wxImage cvMatToWxImage(cv::Mat &mat)
{
    cv::Mat tmp;

    // Convert to RGB (openCV usually uses BGR...)
    if (mat.channels() == 1)
        cv::cvtColor(mat, tmp, cv::COLOR_GRAY2RGB);
    else if (mat.channels() == 4)
        cv::cvtColor(mat, tmp, cv::COLOR_BGRA2RGB);
    else
        cv::cvtColor(mat, tmp, cv::COLOR_BGR2RGB);

    long imsize = tmp.rows * tmp.cols * tmp.channels();
    wxImage ret(tmp.cols, tmp.rows, (unsigned char*)malloc(imsize), false);
    unsigned char* s = tmp.data;
    unsigned char* d = ret.GetData();
    for (long i = 0; i < imsize; i++)
        d[i] = s[i];

    return ret;
}

/*
 *      M A I N   F R A M E
 */

ViewFrame::ViewFrame(const wxString& title, const wxPoint& pos, 
    const wxSize& size) 
    : wxFrame(NULL, wxID_ANY, title, pos, size), m_view(NULL), m_bmp(), 
    m_timer_vid(this, VID_TIMER_ID), m_timer_lpr(this, LPT_TIMER_ID), 
    m_state(VIEW_FINDER)
{
    wxBoxSizer *bSizer = new wxBoxSizer(wxHORIZONTAL);
    this->SetSizer(bSizer);
    
    m_view = new DynamicBitmap(this, wxID_ANY);
    m_view->SetBackgroundColour(*wxBLACK);
    bSizer->Add(m_view, 1, wxALL | wxEXPAND, 5);

    wxBoxSizer *bSzVert = new wxBoxSizer(wxVERTICAL);
    bSizer->Add(bSzVert, 0, wxALL | wxEXPAND, 5);

    m_btnTakePicture = new wxButton(this, wxID_ANY, "Take picture");
    wxButton *btnPrint = new wxButton(this, wxID_ANY, "Print");
    wxButton *btnUpload = new wxButton(this, wxID_ANY, "Upload");
    bSzVert->Add(m_btnTakePicture, 2, wxALL | wxEXPAND, 5);
    bSzVert->Add(btnPrint, 1, wxALL | wxEXPAND, 5);
    bSzVert->Add(btnUpload, 1, wxALL | wxEXPAND, 5);

    this->Layout();

    m_timer_vid.Start(100);

    // Bindings
    this->Bind(wxEVT_TIMER, &ViewFrame::OnTimerVideo, this, VID_TIMER_ID);
    this->Bind(wxEVT_TIMER, &ViewFrame::OnTimerPrinter, this, LPT_TIMER_ID);
    m_view->Bind(wxEVT_PAINT, &DynamicBitmap::PaintEvent, m_view);
    m_btnTakePicture->Bind(wxEVT_BUTTON, &ViewFrame::OnTakePicture, this);
    btnPrint->Bind(wxEVT_BUTTON, &ViewFrame::OnPrint, this);
    btnUpload->Bind(wxEVT_BUTTON, &ViewFrame::OnUpload, this);

    // Add JPG capabilities to wxImage
    wxImage::AddHandler(new wxJPEGHandler());

    return;
}

void ViewFrame::OnTakePicture(wxCommandEvent &ev)
{
    if (m_state == VIEW_FINDER)
    {
        // Wait 5 seconds, update GUI every 20ms
        wxStopWatch sw;
        m_btnTakePicture->Disable();
        while ( sw.Time() < 5000 ) 
        {
            if (sw.Time() > 4000) 
                m_btnTakePicture->SetLabel("1...");
            else if (sw.Time() > 3000) 
                m_btnTakePicture->SetLabel("2...");
            else if (sw.Time() > 2000) 
                m_btnTakePicture->SetLabel("3...");
            else if (sw.Time() > 1000) 
                m_btnTakePicture->SetLabel("4...");
            else if (sw.Time() > 0) 
                m_btnTakePicture->SetLabel("5...");

            usleep(20e3);
            wxYield();
        }

        // Take picture with DSLR (try 5 times max.)
        auto dslr = wxGetApp().GetDslr();
        for (int i = 0; i < 5; i++) 
        {
            try
            {
                //dslr->captureToFile("tmp.jpg");
                //m_bmp.LoadFile("tmp.jpg", wxBITMAP_TYPE_JPEG);
                vector<char> data = dslr->capture();
                wxMemoryInputStream dataStream(data.data(), data.size());
                m_img.LoadFile(dataStream, wxBITMAP_TYPE_JPEG);
                m_bmp = m_img.Mirror();
                break;
            }
            catch(PhotoboxException &ex)
            {
                cout << "Retrying..." << endl;
                continue;
            }
        }

        // Update button and state
        m_btnTakePicture->Enable();
        m_btnTakePicture->SetLabel("Cancel");
        m_state = SHOW_PHOTO;
    } 
    else if (m_state == SHOW_PHOTO)
    {
        m_btnTakePicture->SetLabel("Take picture");
        m_state = VIEW_FINDER;
    }

    return;
}

void ViewFrame::OnPrint(wxCommandEvent &ev)
{
    auto printer = wxGetApp().GetPrinter();

    if ( printer->isPrinting() )    // Dont start a new job
        return;

    m_bmp.SaveFile("tmp.jpg", wxBITMAP_TYPE_JPEG);
    printer->printFile("tmp.jpg");
    m_timer_lpr.StartOnce(5000);    // Check printing status in 5s
    return;
}

void ViewFrame::OnUpload(wxCommandEvent &ev)
{
    return;
}

void ViewFrame::OnTimerVideo(wxTimerEvent &ev)
{
    if (m_state == VIEW_FINDER)
    {
        auto webcam = wxGetApp().GetWebcam();
        // Get image from webcam
        cv::Mat mat = webcam->getFrame(/*scale*/);
        m_bmp = cvMatToWxImage(mat);
    }
    // Calculate scaling fractor from height ratio
    float scale = static_cast<float>(m_view->GetSize().GetHeight()) 
                / static_cast<float>(m_bmp.GetSize().GetHeight());
    // Resize image to always fill the screen
    wxBitmap::Rescale(m_bmp, scale*m_bmp.GetSize());
    // Update dynamic bitmap and refresh
    m_view->SetBitmap(m_bmp);
    m_view->Refresh();
    return;
}

void ViewFrame::OnTimerPrinter(wxTimerEvent &ev)
{
    auto printer = wxGetApp().GetPrinter();

    if ( printer->isPrinting() )    // Everything is fine
        return;

    // Something is amiss!
    printer->cancelAllJobs();
    printer->reset();
    wxMessageDialog dia(this, "Bitte pruefen Sie, ob ausreichend Papier und "
        "Toner vorhanden sind. Starten Sie den Druckvorgang danach erneut.",
        "Der Drucker konnte den Druckvorgang nicht starten!", wxOK);
    dia.ShowModal();
    
    return;
}
