#include <devices/printer.hh>

#include <iostream>

using namespace std;

Printer::Printer(string name) 
    : m_dest(nullptr), m_dests(nullptr), m_num_dests(0), m_jobid(0)
{
    m_num_dests = cupsGetDests(&m_dests);
    checkAndThrow(m_num_dests, "Failed to get list of printers");
    m_dest = cupsGetDest(name.c_str(), NULL, m_num_dests, m_dests);
    if (m_dest == NULL) 
    {
        cerr << "Failed to get printer '" << name << "'" << endl;
        abort();
    }
    return;
}

Printer::~Printer()
{
    cupsFreeDests(m_num_dests, m_dests);
    return;
}

void Printer::printFile(string fname)
{
    m_jobid = cupsPrintFile(m_dest->name, fname.c_str(), fname.c_str(), 
        m_dest->num_options, m_dest->options);
    checkAndThrow(m_jobid, "Failed printing file " + fname);
    return;
}

void Printer::cancelPrint()
{
    int ret = cupsCancelJob(m_dest->name, m_jobid);
    checkAndThrow(ret, "Failed to cancel print job");
    return;
}

std::string Printer::getInfo() const
{
    return string( cupsGetOption("printer-make-and-model", m_dest->num_options, 
        m_dest->options) );
}

std::string Printer::getStateDesc() const 
{
    return string( cupsGetOption("printer-state-reasons", m_dest->num_options, 
            m_dest->options) );
}

bool Printer::isAcceptingJobs() const
{
    string ret( cupsGetOption("printer-is-accepting-jobs", m_dest->num_options, 
        m_dest->options) );

    if (ret == "true")
        return true;
    return false;
}

bool Printer::isPrinting() const
{
    int state = this->getState();
    
    if (state == 4)
        return true;
    return false;
}

bool Printer::isStopped() const
{
    int state = this->getState();
    
    if (state == 5)
        return true;
    return false;
}

void getPrinterList(vector<string> names, vector<string> interfaces)
{
    // TODO
    return;
}

/*
 *      P R I V A T E   M E T H O D S
 */

void Printer::checkAndThrow(int ret, string msg)
{
    if (ret == 0) {
        string err_string(cupsLastErrorString());
        throw PrinterException(msg + ": " + err_string, cupsLastError());
    }
    return;
}

int Printer::getState() const
{   
    const char *ret = cupsGetOption("printer-state", m_dest->num_options, 
        m_dest->options);
    return atoi(ret);
}