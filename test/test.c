#include "../include/cgi.h"

int main(){

    CGI* cgi = CGIStart();

    CGIWindow* window = CGICreateWindow("window","mywindow",0,0,600,600,CGIMakeColor(10,30,40));

    CGIShowWindow(window);

    // int index =0;

    while(CGIIsWindowOpen(window)){
        CGIRefreshWindow(window);
        // CGIClearBuffer(window,CGIMakeColor(100,20,30));
        CGIRefreshBuffer(window);


        CGIPoint p = CGIGetWindowCursorPosition(window);

        
        if(CGIIsKeyPressed(window,CGI_input_key_ctrl)){
            CGISetPixel(window,p.x,p.y,CGIMakeColor(100,100,255));
            
        }
    }


    CGIWindowCleanup(window);
    CGIEnd(cgi);
    
    return 0;
}