#include <photobox.hh>
#include <view.hh>

#include <wx/image.h>

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
    m_timer(this, wxID_ANY), m_state(VIEW_FINDER)
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

    m_timer.Start(60);

    // Bindings
    this->Bind(wxEVT_TIMER, &ViewFrame::OnTimerVideo, this);
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

        // Take picture with DSLR
        auto dslr = wxGetApp().GetDslr();
        dslr->captureToFile("tmp.jpg");
        m_bmp.LoadFile("tmp.jpg", wxBITMAP_TYPE_JPEG);

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
        // Calculate scaling fractor from height ratio
        float scale = static_cast<float>(m_view->GetSize().GetHeight()) 
                    / static_cast<float>(webcam->getHeight());
        // Get image from webcam
        cv::Mat mat = webcam->getFrame(scale);
        m_bmp = cvMatToWxImage(mat);
    }

    // Update dynamic bitmap and refresh
    m_view->SetBitmap(m_bmp);
    m_view->Refresh();
    return;
}
