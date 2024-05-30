#ifndef PB_EXCEPTION_HH
#define PB_EXCEPTION_HH

#include <exception>
#include <string>

/*
 *  Basic exception class
 */

class PhotoboxException : public std::exception
{
public:
    PhotoboxException(std::string err_msg, int err_no = 0)
        : std::exception(), m_err_msg(err_msg), m_err_no(err_no) {};
    virtual ~PhotoboxException() noexcept {};
    virtual const char* what() const noexcept override 
        { return m_err_msg.c_str(); }
    int errorNo() noexcept { return m_err_no; }

private:
    std::string m_err_msg;
    int m_err_no;
};

#endif