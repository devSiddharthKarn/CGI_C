#include "../include/cgi.h"
#include "../include/cgi_font.h"

#include "string.h"

#ifdef _WIN32
#include "Windows.h"
#define SLEEP(seconds) Sleep(seconds*1000);
#elif __linux__
#include "unistd.h"
#define SLEEP(seconds) sleep(seconds);
#endif


// int count_char(int n){
//     int count=0;
//     while(n!=0){
//         count++;
//         n/=10;
//     }

//     return count;
// }

// char* intToChar(int n){
//     int mem = count_char(n);
//     char ret[mem];
// }

int main(){

    CGI* cgi = CGIStart();

    CGIWindow * window = CGICreateWindow("window","window",50,50,500,500,CGIMakeColor(123,222,12));

    CGIShowWindow(window);

    int i =0;


    char *texts[]={"Siddharth","Seema","Yaron","Ram","Shyam","hari"};
    

    while(CGIIsWindowOpen(window)){
        CGIUpdate(cgi);
        CGIRefreshWindow(window);
        CGIClearBuffer(window,CGIMakeColor(123,222,12));

        CGIWriteText(window,texts[i],40,40,1,1,3,3,CGIMakeColor(10,33,111));

        i+=1;
        if(i>5){
            i=0;
        }

        // _sleep(1000);
        CGIRefreshBuffer(window);

        SLEEP(1);


    }

}