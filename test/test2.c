#include "../include/cgi.h"
#include "../include/cgi_font.h"

int main(){

    CGIWindow* window = CGICreateWindow("window","window",0,0,500,500,CGIMakeColor(0,0,0));

    CGIPen* pen =CGIGeneratePen("arial.ttf",48.0f);

    CGIShowWindow(window);

    while(CGIIsWindowOpen(window)){

        CGIRefreshWindow(window,CGI_window_refresh_mode_rapid);

        CGIWriteTextWithPen(window,pen,"Hello CGI   This is Siddharth's CGI library and most welcome",0,-100,0,0,1,2,1,CGI_true,CGIMakeColor(255,255,255));

        CGIRefreshBuffer(window);
    }

}