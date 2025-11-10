#include "cgi.h"

int main(){


    CGIWindow* window = CGICreateWindow("mywindowclass","mywindow",0,0,500,500,CGIMakeColor(24,45,123));

    CGIShowWindow(window);


    CGIPoint point={0,0};

    CGIColor_t color = *(CGIColor_t*)CGIQueryWindow(CGI_query_window_base_color,window);

    while(CGIIsWindowOpen(window)){
        CGIRefreshWindow(window);
        CGIBufferClear(window,color);

        for(int i=point.y;i<point.y+point.y+100;i++){
            for(int j = point.x;j<point.x+point.x+100;j++){
                CGISetPixel(window,j,i,CGIMakeColor(123,23,144));
            }
        }

        if(CGIIsKeyPressed(window,CGI_input_key_w)){
            point.y--;
        }


        if(CGIIsKeyPressed(window,CGI_input_key_s)){
            point.y++;
        }

        if(CGIIsKeyPressed(window,CGI_input_key_a)){
            point.x--;
        }

        if(CGIIsKeyPressed(window,CGI_input_key_d)){
            point.x++;
        }
    }

}