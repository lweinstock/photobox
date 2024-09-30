#include <devices/printer.hh>
#include <cups/ipp.h>
#include <cups/ppd.h>

#include <iostream>

using namespace std;

Printer::Printer(string name) 
    : m_name(name), m_dest(nullptr), m_dests(nullptr), m_num_dests(0), m_jobid(0)
{
    this->updateDest();
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

void Printer::cancelAllJobs()
{
    int ret = cupsCancelJob(m_dest->name, CUPS_JOBID_ALL);
    checkAndThrow(ret, "Failed to cancel all print jobs");
    return;
}

std::string Printer::getInfo()
{
    return string( cupsGetOption("printer-make-and-model", m_dest->num_options, 
        m_dest->options) );
}

std::string Printer::getStateDesc() 
{
    return string( cupsGetOption("printer-state-reasons", m_dest->num_options, 
            m_dest->options) );
}

bool Printer::isAcceptingJobs()
{
    this->updateDest();
    string ret( cupsGetOption("printer-is-accepting-jobs", m_dest->num_options, 
        m_dest->options) );

    if (ret == "true")
        return true;
    return false;
}

bool Printer::isIdle()
{
    int state = this->getState();
    
    if (state == 3)
        return true;
    return false;
}

bool Printer::isPrinting()
{
    int state = this->getState();
    
    if (state == 4)
        return true;
    return false;
}

bool Printer::isStopped()
{
    int state = this->getState();
    
    if (state == 5)
        return true;
    return false;
}

void Printer::reset()
{
    string command = "cupsenable " + m_name;
    int ret = system(command.c_str());
    if (ret != 0)
        throw PrinterException("Failed to reset printer " + m_name);
    return;
}

unsigned Printer::getPrinterList(vector<string> &names, vector<string> &interfaces)
{
    cups_dest_t *dests;
    int n = cupsGetDests(&dests);
    for (int i = 0; i < n; i++)
    {
        names.emplace_back(dests[i].name);
        if (dests[i].instance)
            interfaces.emplace_back(dests[i].name);
        else
            interfaces.push_back("");
    }
    
    cupsFreeDests(n, dests);
    return static_cast<unsigned>(n);
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

void Printer::updateDest()
{
    if (m_dests)
        cupsFreeDests(m_num_dests, m_dests);
    
    m_num_dests = cupsGetDests(&m_dests);
    checkAndThrow(m_num_dests, "Failed to get list of printers");
    m_dest = cupsGetDest(m_name.c_str(), NULL, m_num_dests, m_dests);

    if (m_dest == NULL) 
        throw PrinterException("Failed to find printer " + m_name);

    return; 
}


int Printer::getState()
{   
    this->updateDest();
    const char *ret = cupsGetOption("printer-state", m_dest->num_options, 
        m_dest->options);
    return atoi(ret);
}