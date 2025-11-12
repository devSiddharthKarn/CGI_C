#include "cgi.h"
#include "stdio.h"
#include "stdlib.h"

#define TRAIL_LENGTH 100

typedef struct {
    CGIPoint pos;
    CGIColor_t color;
} TrailPoint;

int main() {
    CGI* cgi = CGIStart();

    CGIWindow* window = CGICreateWindow("mywindow","Rainbow Trail",10,50,800,600,CGIMakeColor(0,0,0));
    CGIShowWindow(window);

    TrailPoint trail[TRAIL_LENGTH];
    for(int i=0;i<TRAIL_LENGTH;i++){
        trail[i].pos.x = 0;
        trail[i].pos.y = 0;
        trail[i].color = CGIMakeColor(0,0,0);
    }

    CGIPoint cursor = *(CGIPoint*)CGIPerformQuery(CGI_query_window_cursor_position,cgi,window);

    while(CGIIsWindowOpen(window)) {
        CGIRefreshWindow(window);
        CGIClearBuffer(window,CGIMakeColor(0,0,0));

        // Shift trail
        for(int i=TRAIL_LENGTH-1;i>0;i--){
            trail[i] = trail[i-1];
        }
        trail[0].pos = cursor;
        trail[0].color = CGIMakeColor(rand()%256, rand()%256, rand()%256); // random color

        // Draw trail
        for(int i=0;i<TRAIL_LENGTH;i++){
            for(int y=trail[i].pos.y; y<trail[i].pos.y+20; y++){
                for(int x=trail[i].pos.x; x<trail[i].pos.x+20; x++){
                    CGISetPixel(window, x, y, trail[i].color);
                }
            }
        }

        CGIRefreshBuffer(window);
        cursor = *(CGIPoint*)CGIPerformQuery(CGI_query_window_cursor_position,cgi,window);
        CGIUpdate(cgi);
    }
}
