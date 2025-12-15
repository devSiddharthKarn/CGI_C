#define STB_IMAGE_IMPLEMENTATION
#define STB_IMAGE_WRITE_IMPLEMENTATION

#include "../include/cgi.h"
#include "stb_image.h"
#include "stb_image_write.h"

void DrawRect(CGIWindow* window, unsigned int x_pos, unsigned int y_pos, unsigned int width, unsigned int height, CGIColor_t color){
    CGIClearBufferRegion(window,x_pos,y_pos,width,height,color);
    return;
}


int main(){

    CGIWindow* window = CGICreateWindow("TestClass","CGI Window Test",100,100,800,600,CGIMakeColor(20,20,30));

    CGIShowWindow(window);

    CGIPoint pos={
        .x=0,
        .y=0
    };
    
    CGIWindowSurface surface;

    while(CGIIsWindowOpen(window)){
        CGIRefreshWindow(window,CGI_window_refresh_mode_rapid);

        DrawRect(window,100,100,200,150,CGIMakeColor(200,50,50));

        if(CGIIsWindowKeyDown(window,CGI_KEYCODE_ESCAPE)){
            break;
        }

        if(CGIIsWindowKeyDown(window,CGI_KEYCODE_ENTER)){
            printf("Window Surface - Width: %u, Height: %u\n",surface.width,surface.height);

            surface = CGIGetWindowSurface(window);

            stbi_write_png("window_surface.png",surface.width,surface.height,surface.channels,surface.buffer,surface.width*surface.channels);
            printf("Saved window surface to window_surface.png\n");


        }
        DrawRect(window,pos.x,pos.y,50,50,CGIMakeColor(0,255,0));

        if(CGIIsWindowKeyDown(window,CGI_KEYCODE_W)){
            pos.y-=5;
        }

        if(CGIIsWindowKeyDown(window,CGI_KEYCODE_S)){
            pos.y+=5;
        }

        if(CGIIsWindowKeyDown(window,CGI_KEYCODE_A)){
            pos.x-=5;
        }
        
        if(CGIIsWindowKeyDown(window,CGI_KEYCODE_D)){
            pos.x+=5;
        }

        CGIRefreshBuffer(window);
    }
    
    CGIFreeWindowSurface(surface);
    CGICloseWindow(window);
    CGIWindowCleanup(window);

}