#include "../include/cgi.h"

int main(){

    CGI* cgi = CGIStart();

    CGIWindow * window = CGICreateWindow("mywindow","window",50,100,300,400,CGIMakeColor(100,200,22));

    CGIShowWindow(window);

    while(CGIIsWindowOpen(window)){
        if(CGIIsKeyPressed(window,CGI_input_key_shift) && CGIIsMouseButtonPressed(cgi,CGI_input_key_mouse_l)){
            CGIPoint p = CGIGetWindowCursorPosition(window);

            CGISetPixel(window,p.x,p.y,CGIMakeColor(1,233,1));
        }

        CGIRefreshWindow(window);
        CGIRefreshBuffer(window);
        CGIUpdate(cgi);
    }

    CGICloseWindow(window);
    CGIEnd(cgi);
}