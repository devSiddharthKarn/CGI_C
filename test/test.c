#include "../include/cgi.h"
// #include "unistd.h"
#include "stdio.h"
int main(){

    CGI* cgi = CGIStart();

    CGIWindow* window = CGICreateWindow("mywindow","window",0,0,500,500,CGIMakeColor(100,233,111));

    CGIShowWindow(window);

    while(CGIIsWindowOpen(window)){
        CGIUpdate(cgi);
        CGIRefreshWindow(window);
        CGIRefreshBuffer(window);
        
        if(CGIIsMouseButtonPressed(cgi,CGI_input_key_mouse_l)){
            printf("left\n");
        }
        
        if(CGIIsMouseButtonPressed(cgi,CGI_input_key_mouse_r)){
            printf("right\n");
        }
    }

}