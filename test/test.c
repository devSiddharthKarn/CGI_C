#include "cgi.h"
#include "stdio.h"
#include "math.h"
int main(){


    CGIWindow* window = CGICreateWindow("mywindow","mywindow",0,0,500,500,CGIMakeColor(23,45,6));

    CGIShowWindow(window);

    while(CGIIsWindowOpen(window)){
        unsigned int width = *(unsigned int*)CGIPerformQuery(CGI_query_window_width_unsigned_int,NULL,window);
        unsigned int height = *(unsigned int*)CGIPerformQuery(CGI_query_window_height_unsigned_int,NULL,window);

        printf("%d, %d\n",width,height);

        CGIRefreshWindow(window);
        CGIRefreshBuffer(window);
    }

    CGIWindowCleanup(window);

}