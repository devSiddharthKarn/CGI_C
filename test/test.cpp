#include "include/cgi.h"
#include "stdio.h"
int main(){

    CGI* cgi = CGIStart();

    CGIWindow* window = CGICreateWindow("mywindow","mywindow",10,50,600,600,CGIMakeColor(123,255,123));

    CGIShowWindow(window);

    CGIPoint p = *(CGIPoint*)CGIPerformQuery(CGI_query_system_cursor_position,cgi,window);

    CGIPoint point;

    while(CGIIsWindowOpen(window)){
        CGIRefreshWindow(window);
        CGIClearBuffer(window,CGIMakeColor(123,255,124));
        
        point = *(CGIPoint*)CGIPerformQuery(CGI_query_window_position,cgi,window);

        printf("%d,%d\n",point.x,point.y);
        
        for(int i=p.y;i<p.y+100;i++){
            for(int j=p.x;j<p.x+100;j++){
                CGISetPixel(window,j,i,CGIMakeColor(255,0,0));
            }
        }
        
        CGIRefreshBuffer(window);
        p = *(CGIPoint*)CGIPerformQuery(CGI_query_system_cursor_position,cgi,window);
        CGIUpdate(cgi);
    }

}