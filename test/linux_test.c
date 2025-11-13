#include "cgi.h"
#include "stdio.h"
int main(){ 

    CGI* cgi = CGIStart();

    CGIWindow* window = CGICreateWindow("mywindow","mywindow",0,0,500,500,CGIMakeColor(23,45,122));

    CGIBool logic = CGI_true;
    logic =CGIPerformCommand(CGI_command_window_set_window_show_status,&logic,&window);

    printf("%d",logic);

    while(CGIIsWindowOpen(window)){
        CGIRefreshWindow(window);
        
    }

}