#define STB_IMAGE_IMPLEMENTATION
#define STB_IMAGE_WRITE_IMPLEMENTATION

#include "../include/cgi.h"
#include "stb_image_write.h"
int main(){

    CGIWindow* window = CGICreateWindow("window","window",0,0,500,500,CGIMakeColor(100,200,122));

    CGIShowWindow(window);

    while(CGIIsWindowOpen(window)){
        CGIRefreshWindow(window,CGI_window_refresh_mode_rapid);


        CGIClearBufferRegion(window,0,0,200,200,CGIMakeColor(0,0,0));

        CGIRefreshBuffer(window);


        if(CGIIsWindowKeyDown(window,CGI_KEYCODE_ENTER)){
            CGIWindowSurface surface = CGIGetWindowSurfaceRegion(window,-200,0,300,300);
            

            stbi_write_png("window_surface.png",surface.width,surface.height,surface.channels,surface.buffer,surface.width*surface.channels);
            
            CGIFreeWindowSurface(surface);
        }

    }

    CGICloseWindow(window);
    CGIWindowCleanup(window);
    

}