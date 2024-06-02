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

    vector<string> names, ports;
    Dslr::getDslrList(names, ports);
    for (size_t i = 0; i < names.size(); i++)
        cout << names.at(i) << " - " << ports.at(i) << endl;

    Dslr cam;
    cout << "Camera name: " << cam.getName() << endl;
    cout << "Camera port: " << cam.getPort() << endl;
    //cam.captureToFile("moop4.jpg");
    char *data;
    unsigned long size = cam.captureRaw(data);
    cout << size << endl;
    
/*
    Printer cp740("CP740");
    cout << cp740.getInfo() << endl;
    cout << (cp740.isAcceptingJobs() ? "Ready" : "Not ready") << endl;
    cout << (cp740.isPrinting() ? "Is printing" : "Is not printing") << endl;

    cp740.printFile("moop.jpg");
    sleep(1);
    cp740.cancelPrint();
*/
    return 0;
 }