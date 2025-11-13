#include "cgi.h"
#include "stdio.h"
int main(){ 

    CGI* cgi = CGIStart();

    CGIWindow* window = CGICreateWindow("mywindow","mywindow",0,0,500,500,CGIMakeColor(23,45,122));

    CGIShowWindow(window);

    CGIBool logic = CGI_false;
    logic =CGIPerformCommand(CGI_command_window_resizable_logic,&logic,&window);



    printf("%d",logic);

    while(CGIIsWindowOpen(window)){
        CGIRefreshWindow(window);
        
    }

}