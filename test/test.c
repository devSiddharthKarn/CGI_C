#include "../include//cgi.h"
#include "../include/cgi_font.h"
#include "stdio.h"
#include "stdlib.h"

int main(int argc, char * argv[]){
    if((argc<=1)){
        fprintf(stderr,"please make an input file\n");
        return -1;
    }

    FILE* fp = fopen(argv[1],"r");

    if(fp==NULL){
        fprintf(stderr,"no such file found\n");
        return -1;
    }

    fseek(fp,0,SEEK_END);
    size_t size = ftell(fp);
    rewind(fp);

    char * buffer = (char *)malloc(sizeof(char)*size);

    fread(buffer,1,size,fp);

    fclose(fp);

    CGI* cgi = CGIStart();

    CGIWindow * window = CGICreateWindow("iwndow","window",40,40,600,600,CGIMakeColor(233,12,233));

    CGIColor_t bg = CGIGetWindowBaseColor(window);

    CGIShowWindow(window);

    float y=0;

    while(CGIIsWindowOpen(window)){
        CGIUpdate(cgi);
        CGIRefreshWindow(window);
        CGIClearBuffer(window,bg);

        CGIWriteText(window,buffer,10,y,1,1,5,5,0,CGI_true,CGIMakeColor(0,0,0));

        if(CGIIsWindowScrolledY(window)){
            y+=CGIGetWindowScrollDeltaY(window)*100;
        }

        CGIRefreshBuffer(window);
    }


    CGICloseWindow(window);

    CGIWindowCleanup(window);

    CGIEnd(cgi);

    free(buffer);

    return 0;

}