#ifndef PB_PRINTER_HH
#define PB_PRINTER_HH

#include <cups/cups.h>
#include <string>
#include <vector>

#include <exception.hh>

/*
 *  Brief exception class
 */

class PrinterException : public PhotoboxException
{
public:
    PrinterException(std::string err_msg, int err_no = 0) 
        : PhotoboxException(err_msg, err_no) {};
    virtual ~PrinterException() noexcept {};
};

/*
 *  Simple C++ wrapper class for CUPS printer
 */

class Printer {
public:
    Printer(std::string name);
    ~Printer();

    void printFile(std::string fname);
    void cancelPrint();

    std::string getInfo() const;
    std::string getStateDesc() const;
    bool isAcceptingJobs() const;
    bool isPrinting() const;
    bool isStopped() const;

    // Get list of all supported and connected printers
    static int getPrinterList(std::vector<std::string> &names,
        std::vector<std::string> &interfaces);

private:
    static void checkAndThrow(int ret, std::string msg);
    int getState() const;

    cups_dest_t *m_dest, *m_dests;
    int m_num_dests, m_jobid;
};

#endif