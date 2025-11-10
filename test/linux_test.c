#define CGI_LINUX_IMPLEMENTATION_ACTIVE
#include "cgi.h"
#include "unistd.h"
#include "time.h"
#include "stdio.h"
int main(){

    clock_t now = clock();
    printf("%li",now);
    

    CGIWindow* window = CGICreateWindow("mywindow","mywindow",0,0,500,500,CGIMakeColor(255,23,12));

    CGIShowWindow(window);

    CGIPoint pnt;
    pnt.x=0;
    pnt.y=0;

    CGIColor_t color = *(CGIColor_t*)CGIQueryWindow(CGI_query_window_base_color,window);

    while(CGIIsWindowOpen(window)){
        CGIRefreshWindow(window);
        CGIBufferClear(window,color);
        
        // pnt = (CGIPoint*)CGIQueryWindow(CGI_query_system_cursor_position,window);

        for(int i=pnt.y;i<pnt.y+100;i++){
            for(int j=pnt.x;j<pnt.x+100;j++){
                CGISetPixel(window,j,i,CGIMakeColor(23,45,67));
            }
        }

        pnt.x++;
        pnt.y++;

        // sleep(0);
        
    }

    CGIWindowCleanup(window);
}