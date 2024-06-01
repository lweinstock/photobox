#ifndef PB_WEBCAM_HH
#define PB_WEBCAM_HH

#include <opencv2/opencv.hpp>

#include <exception.hh>

/*
 *  Brief exception class
 */

class WebcamException : public PhotoboxException
{
public:
    WebcamException(std::string err_msg, int err_no = 0) 
        : PhotoboxException(err_msg, err_no) {};
    virtual ~WebcamException() noexcept {};
};

/*
 *  C++ wrapper for openCV video capture (maybe go for libcam..?)
 */

class Webcam
{
public:
    Webcam(std::string file_name);
    ~Webcam();

    cv::Mat getFrame(float scale = 1.0, bool flip = true);
    int getWidth() const { return m_width; }
    int getHeight() const { return m_height; }

    static unsigned getWebcamList(std::vector<std::string> &devices);

private:
    cv::VideoCapture m_cap;

    int m_width, m_height;
};

#endif