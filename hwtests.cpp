#include <iostream>

#include <wx/wxprec.h>
#ifndef WX_PRECOMP
    #include <wx/wx.h>
#endif

//#include <opencv2/opencv.hpp>

#include <devices/dslr.hh>
#include <devices/printer.hh>

using namespace std;

int main(int argc, char** argv)
{
/*
    vector<string> names, ports;
    Dslr::getDslrList(names, ports);
    for (size_t i = 0; i < names.size(); i++)
        cout << names.at(i) << " - " << ports.at(i) << endl;

    Dslr cam;
    cout << "Camera name: " << cam.getName() << endl;
    cout << "Camera port: " << cam.getPort() << endl;
    //cam.captureToFile("moop4.jpg");
    vector<char> data;
    unsigned long size = cam.capture(data);
    cout << data.size() << endl;
    for (int i = 0; i < 20; i++)
        printf("0x%04X\n", data[i]);
    
*/
    vector<string> names, interfaces;
    unsigned n = Printer::getPrinterList(names, interfaces);
    for (unsigned i = 0; i < n; i++)
        cout << "Printer " << names.at(i) << " @ " << interfaces.at(i) << endl;


    Printer cp740("CP740");
    cout << cp740.getInfo() << endl;
    cout << (cp740.isAcceptingJobs() ? "Ready" : "Not ready") << endl;
    cout << (cp740.isPrinting() ? "Is printing" : "Is not printing") << endl;
    cp740.reset();
 /*   cp740.cancelAllJobs();

    cp740.printFile("tmp.jpg");
    for (int i = 0; i < 20; i++)
    {
        cout << (cp740.isPrinting() ? "Is printing" : "Is not printing") << endl;
        sleep(1);
    }
*/
    return 0;
 }