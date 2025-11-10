#include "cgi.h"
#include "Windows.h"
int main(){

    CGIWindow* window = CGICreateWindow("mywindow","mywindow",0,0,500,500,CGIMakeColor(244,123,12));

    CGIShowWindow(window);

    // CGIQueryWindow()

    HDC hdc = (HDC)CGIQueryWindow(CGI_query_window_internal_win32_HDC,window);

    while(CGIIsWindowOpen(window)){
        CGIRefreshWindow(window);

    }

}