#define CGI_IMPLEMENTATION_MODE_VERBOSE

#include "../include/cgi.h"

int main(){

    CGIWindow* window = CGICreateWindow("abc","my custom window",0,0,500,500,CGIMakeColor(0,233,0));

    CGIShowWindow(window);

    CGIPoint p{0,0};


    

    while(CGIIsWindowOpen(window)){
        CGIRefreshWindow(window,CGI_window_refresh_mode_rapid);



        // CGIClearBuffer(window,CGIMakeColor(0,233,0));
        CGIClearBufferRegion(window,0,0,500,500,CGIMakeColor(0,233,0));

        for(int i=p.y;i<50+p.y;i++){
            for(int j=p.x;j<30+p.x;j++){

                CGISetPixel(window,j,i,CGIMakeColor(0,0,0));
            }
        }

        // CGIRefreshBuffer(window);

        CGIRefreshBufferRegion(window,0,0,500,500);


        if(CGIIsWindowKeyDown(window,CGI_KEYCODE_W)){
            p.y--;
        }

        if(CGIIsWindowKeyDown(window,CGI_KEYCODE_S)){
            p.y++;
        }

        if(CGIIsWindowKeyDown(window,CGI_KEYCODE_A)){
            p.x--;
        }

        if(CGIIsWindowKeyDown(window,CGI_KEYCODE_D)){
            p.x++;
        }
    }

}