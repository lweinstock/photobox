#ifndef PB_DSLR_HH
#define PB_DSLR_HH

#include <gphoto2/gphoto2.h>
#include <wx/image.h>

#include <exception.hh>

/*
 *  Brief exception class
 */

class DslrException : public PhotoboxException
{
public:
    DslrException(std::string err_msg, int err_no = 0) 
        : PhotoboxException(err_msg, err_no) {};
    virtual ~DslrException() noexcept {};
};

/*
 *  C++ wrapper of libgphoto2 for handling a simple DSLR
 */

class Dslr
{
public:
    // Initialize first camera found
    Dslr();

    // Initialize camera at specified port
    Dslr(std::string port);
    
    ~Dslr();

    void captureToFile(std::string name);
    unsigned long captureRaw(const char *raw_data);

    std::string getName() const { return m_name; }
    std::string getPort() const { return m_port; }

    // Get a list of all connected and supported cameras
    static unsigned getDslrList(std::vector<std::string> &name_list,
        std::vector<std::string> &port_list);

private:
    std::string getCameraName(std::string port);
    static void checkAndThrow(int ret, std::string err_msg);

    static GPContext* s_ctx;
    Camera* m_cam;
    std::string m_name, m_port;
};

#endif