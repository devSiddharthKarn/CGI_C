#include "stdio.h"
#include "cgi.h"
int main(){

    CGI* cgi = CGIStart();

    CGIWindow* window = CGICreateWindow("mywindow","mywindow",0,0,400,500,CGIMakeColor(123,233,12));

    CGIColor_t color = *(CGIColor_t*)CGIPerformQuery(CGI_query_window_base_color_CGIColor_t,cgi,window);

    CGIBool logic=CGI_true;

    CGIPerformCommand(CGI_command_window_set_window_show_status,&logic,&window);

    CGIPoint point={0,0};

    while(CGIIsWindowOpen(window)){
        CGIRefreshWindow(window);
        CGIClearBuffer(window,color);

        for(int i=point.y;i<100+point.y;i++){
            for(int j=point.x;j<100+point.x;j++){
                CGIPoint p={j,i};

                CGIPerformCommand(CGI_command_window_set_pixel,&p,&window);

            }
        }

        CGIRefreshBuffer(window);

        if(CGIIsKeyPressed(window,CGI_input_key_escape)){
            CGICloseWindow(window);
        }
    }

    CGIWindowCleanup(window);
    printf("Program terminated gracefully");

    return 0;

}