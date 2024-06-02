#ifndef PB_VIEW_HH
#define PB_VIEW_HH

#include <wx/wx.h>

#include <opencv2/opencv.hpp>

/*
 *  Dynamic bitmap for image preview (aka view finder) and view of final photo
 */

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

// Convert opencv Mat to wxImage (bgr -> rgb)
wxImage cvMatToWxImage(cv::Mat &mat);


/*
 *  Main Window of the App
 */

class ViewFrame : public wxFrame
{
public:
    ViewFrame(const wxString &title, const wxPoint &pos, const wxSize& size);
    ~ViewFrame() {};

    void OnTakePicture(wxCommandEvent &ev);
    void OnPrint(wxCommandEvent &ev);
    void OnUpload(wxCommandEvent &ev);
    void OnTimerVideo(wxTimerEvent &ev);

private:
    DynamicBitmap *m_view;
    wxImage m_img;
    wxBitmap m_bmp;
    wxTimer m_timer;
    wxButton *m_btnTakePicture;

    enum state : unsigned {VIEW_FINDER = 0, SHOW_PHOTO} m_state;
};

#endif