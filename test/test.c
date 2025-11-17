#define CGI_IMPLEMENTATION_MODE_VERBOSE
#include "cgi.h"
#include "stdio.h"
int main(){

    CGI* cgi = CGIStart();

    CGIWindow* window =  CGICreateWindow("mywindow","mywindow",0,0,500,500,CGIMakeColor(23,45,67));

    CGIShowWindow(window);

    CGIPoint p = {0,0};

    while(CGIIsWindowOpen(window)){

        CGIRefreshWindow(window);

        if(CGIIsKeyPressed(window,CGI_input_key_w)){
            p.y--;
        }

        
        if(CGIIsKeyPressed(window,CGI_input_key_s)){
            p.y++;
        }

        
        if(CGIIsKeyPressed(window,CGI_input_key_a)){
            p.x--;
        }

        
        if(CGIIsKeyPressed(window,CGI_input_key_d)){
            p.x++;
        }


        // CGIPerformCommand(CGI_command_window_set_window_pos,&p,&window);
        
        


    }

}