#include "cgi.h"

struct ball{
    float x_pos;
    float y_pos;
    int width;
    int height;
    float velocity;
    float acceleration;
    CGIColor_t color;
};

int main(){

    CGI* cgi = CGIStart();

    CGIWindow* window = CGICreateWindow("mywindow","mywindow",0,0,500,500,CGIMakeColor(100,23,123));

    struct ball ball;
    ball.x_pos=0;
    ball.y_pos=0;
    ball.acceleration=0.002;
    ball.velocity=0;
    ball.width=40;
    ball.height = 40;
    ball.color = CGIMakeColor(0,0,0);

    CGIShowWindow(window);

    CGIBool bounce = CGI_false;

    while(CGIIsWindowOpen(window)){
        CGIRefreshWindow(window);
        CGIClearBuffer(window,CGIMakeColor(100,23,45));
        
        
        for(int i=ball.y_pos;i<ball.y_pos+ball.height;i++){
            for(int j=ball.x_pos;j<ball.x_pos+ball.width;j++){
                CGISetPixel(window,j,i,ball.color);
            }
        }
        
        
        CGIRefreshBuffer(window);

        
        if(ball.y_pos+ball.height>=500 ){
            ball.acceleration=-ball.velocity;
            ball.velocity=0;
            // ball.y_pos+=ball.velocity;
            // bounce = CGI_true;
        }else{
            
        }
        ball.velocity+=ball.acceleration;
        ball.y_pos+=ball.velocity;
    }



}