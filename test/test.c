#include "../include/cgi.h"
#include "stdio.h"
int main(){

    CGI* cgi = CGIStart();

    CGIWindow* window = CGICreateWindow("mywindow","mywindow",0,0,600,600,CGIMakeColor(123,45,122));

    CGIShowWindow(window);

    while(CGIIsWindowOpen(window)){

        CGIRefreshWindow(window);

        if(CGIIsWindowScrolledY(window)){
            printf("scrolled\n");
        }

        if(CGIIsWindowResized(window)){
            printf("resized\n");
        }

    }

}