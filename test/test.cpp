#include "cgi.h"
#include "cstdio"
#include "unistd.h"
int main(){

    CGI* cgi = CGIStart();

    CGIWindow*window = CGICreateWindow("mywindow","mywindow",0,0,500,500,CGIMakeColor(233,12,34));

    CGIBool logic;

    float a ;

    CGIShowWindow(window);

    while(CGIIsWindowOpen(window)){
        CGIRefreshWindow(window);
        CGIUpdate(cgi);

        logic = *(CGIBool*)CGIPerformQuery(CGI_query_window_is_scrolled_y,cgi,window);

        a = *(float*)CGIPerformQuery(CGI_query_window_scroll_delta_y,cgi,window);

        printf("%f\n",a);

        sleep(1);
    }

}