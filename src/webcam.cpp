#include <devices/webcam.hh>

#include <iostream>

using namespace std;

Webcam::Webcam(string file_name) : m_cap(), m_width(0), m_height(0)
{
    if ( !m_cap.open(file_name, cv::CAP_V4L) )
        throw PhotoboxException("Failed to open " + file_name);

    m_width = m_cap.get(cv::CAP_PROP_FRAME_WIDTH);
    m_height = m_cap.get(cv::CAP_PROP_FRAME_HEIGHT);

    return;
}

Webcam::~Webcam()
{
    if( m_cap.isOpened() )
        m_cap.release();
    return;
}

cv::Mat Webcam::getFrame(float scale, bool flip)
{
    if ( !m_cap.isOpened() )
        throw WebcamException("Camera is not opened");
    
    cv::Mat frame;
    m_cap.read(frame);

    if ( frame.empty() )
        throw WebcamException("Grabbed empty frame");

    if (scale != 1.0)
        cv::resize(frame, frame, cv::Size(), scale, scale);

    if (flip)   // Flip image around y-axis
        cv::flip(frame, frame, 1);

    return frame;
}

unsigned Webcam::getWebcamList(std::vector<std::string> &devices)
{
    // Checks the first 20 (arbitrary choice) devices files '/dev/video*'
    cv::VideoCapture tmp;
    for (int i = 0; i < 20; i++)
    {
        string dev = "/dev/video" + to_string(i);
        if ( tmp.open(dev.c_str(), cv::CAP_V4L) )
        {
            devices.push_back(dev);
            tmp.release();
        }
    }

    return devices.size();
}


