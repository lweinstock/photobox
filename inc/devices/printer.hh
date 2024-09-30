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
    void restartPrint();
    void cancelPrint();
    void cancelAllJobs();

    std::string getInfo();
    std::string getStateDesc();
    bool isAcceptingJobs();
    bool isIdle();
    bool isPrinting();
    bool isStopped();
    void reset();

    // Get list of all supported and connected printers
    static unsigned getPrinterList(std::vector<std::string> &names,
        std::vector<std::string> &interfaces);

private:
    void updateDest();  // Update printer information stored in m_dest
    int getState();
    static void checkAndThrow(int ret, std::string msg);

    std::string m_name;
    cups_dest_t *m_dest, *m_dests;
    int m_num_dests, m_jobid;
};

#endif