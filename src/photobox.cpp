#include <photobox.hh>

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

/*
 *      M A I N   F R A M E
 */

MainFrame::MainFrame(const wxString& title, const wxPoint& pos, 
    const wxSize& size) 
    : wxFrame(NULL, wxID_ANY, title, pos, size), m_view(NULL), m_bmp(), 
    m_timer(this, wxID_ANY)
{
    wxBoxSizer *bSizer = new wxBoxSizer(wxVERTICAL);
    this->SetSizer(bSizer);
    
    m_view = new DynamicBitmap(this, wxID_ANY);
    m_view->SetBackgroundColour(*wxBLACK);
    bSizer->Add(m_view, 1, wxALL | wxEXPAND, 5);
    this->Layout();

    m_timer.Start(60);

    // Bindings
    this->Bind(wxEVT_TIMER, &MainFrame::OnTimerVideo, this);
    m_view->Bind(wxEVT_PAINT, &DynamicBitmap::PaintEvent, m_view);

    return;
}

void MainFrame::OnTimerVideo(wxTimerEvent &ev)
{
    auto webcam = wxGetApp().GetWebcam();
    // Calculate scaling fractor from height ratio
    float scale = static_cast<float>(m_view->GetSize().GetHeight()) 
                / static_cast<float>(webcam->getHeight());
    cv::Mat mat = webcam->getFrame(scale);
    m_bmp = cvMatToWxImage(mat);

    // Update webcam view
    m_view->SetBitmap(m_bmp);
    m_view->Refresh();
    return;
}

/*
 *      T H E   A P P
 */

bool PhotoBox::OnInit()
{
    // Initialise hardware
    m_webcam = std::make_shared<Webcam>("/dev/video4");

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
