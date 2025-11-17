#include "cgi.h"

int main(){

    CGI* cgi = CGIStart();

    CGIWindow* window = CGICreateWindow("mywindow","mywindow",0,0,500,500,CGIMakeColor(23,45,67));

    CGIShowWindow(window);

    while(CGIIsWindowOpen(window)){
        CGIRefreshWindow(window);
        CGIUpdate(cgi);
        CGIRefreshBuffer(window);
    }

    CGIWindowCleanup(window);
    CGIEnd(cgi);

}