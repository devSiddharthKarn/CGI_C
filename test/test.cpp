#include "cgi.h"

int main(){


    CGIWindow* window = CGICreateWindow("mywindowclass","mywindow",0,0,100,100,CGIMakeColor(24,45,123));

    CGIShowWindow(window);


    CGIPoint point={0,0};

    while(CGIIsWindowOpen(window)){
        CGIRefreshWindow(window);


        for(int i=point.y;i<point.y+point.y;i++){
            for(int j = point.x;j<point.x+point.x;j++){
                CGISetPixel(window,j,i,CGIMakeColor(123,23,144));
            }
        }

    }

}