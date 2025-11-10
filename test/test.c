#define CGI_WINDOWS_IMPLEMENTATION_ACTIVE
#include "cgi.h"
#include "Windows.h"
int main(){

    CGIWindow*  window = CGICreateWindow("mywindow","mywindow",0,0,500,500,CGIMakeColor(10,200,167));

    CGIPoint point;
    point.x=0;
    point.y=0;

    CGIColor_t color = *(CGIColor_t*)CGIQueryWindow(CGI_query_window_base_color,window);


    HDC hdc = (HDC)CGIQueryWindow(CGI_query_window_internal_win32_HDC,window);

    CGIShowWindow(window);

    while(CGIIsWindowOpen(window)){
        CGIRefreshWindow(window);
        // CGIBufferClear(window,color);

        // TextOutA(hdc,0,0,"hello world",11);
        // TextOut(hdc,10,10,"Siddharth",9);

        for(int i=point.y;i<point.y+100;i++){
            for(int j=point.x;j<point.x+200;j++){
                CGISetPixel(window,j,i,CGIMakeColor(23,45,89));
            }
        }

        if(CGIIsKeyPressed(window,CGI_input_key_a)){
            point.x--;
        }

        if(CGIIsKeyPressed(window,CGI_input_key_s)){
            point.y++;
        }

        if(CGIIsKeyPressed(window,CGI_input_key_d)){
            point.x++;
        }

        if(CGIIsKeyPressed(window,CGI_input_key_w)){
            point.y--;
        }
    }

}