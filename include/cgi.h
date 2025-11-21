#include "typedefs.h"

#ifndef CGI_LIB_H
#define CGI_LIB_H
#endif

#pragma once

#ifdef __cplusplus
#define CGI_BEGIN_DECLS \
    extern "C"          \
    {
#define CGI_END_DECLS }
#else
#define CGI_BEGIN_DECLS
#define CGI_END_DECLS
#endif

typedef struct CGI CGI;

typedef struct CGIWindow CGIWindow;

CGI_BEGIN_DECLS

CGI *CGIStart();

CGIBool CGIUpdate(CGI *cgi);

CGIBool CGIEnd(CGI *cgi);

CGIWindow *CGICreateWindow(char *classname, char *window_name, unsigned int x_pos, unsigned int y_pos, unsigned int width, unsigned int height, CGIColor_t color);

CGIBool CGIWindowCleanup(CGIWindow *window);

CGIBool CGIShowWindow(CGIWindow *window);

void CGISetPixel(CGIWindow *window, int x_pos, int y_pos, CGIColor_t color);

const void *CGIPerformQuery(CGIQuery query, CGI *cgi, CGIWindow *window);

CGIBool CGIIsWindowOpen(const CGIWindow *window);

CGIBool CGICloseWindow(CGIWindow *window);

CGIColor_t CGIMakeColor(unsigned char r, unsigned char g, unsigned char b);

CGIBool CGIRefreshWindow(CGIWindow *window);

CGIBool CGIRefreshBuffer(CGIWindow *window);

CGIBool CGIClearBuffer(CGIWindow *window, CGIColor_t color);

CGIBool CGIPerformCommand(CGICommand command, const void *args, const void *acceptor);

CGIBool CGIIsKeyPressed(CGIWindow *window, CGIInputKey key);

CGIBool CGIIsWindowFocused(CGIWindow *window);

CGIBool CGIIsWindowResized(CGIWindow *window);

CGIPoint CGIGetWindowPosition(CGIWindow *window);

unsigned int CGIGetWindowHeight(CGIWindow *window);

unsigned int CGIGetWindowwidth(CGIWindow *window);

unsigned int CGIGetWindowBufferHeight(CGIWindow *window);

unsigned int CGIGetWindowBufferWidth(CGIWindow *window);

CGIColor_t CGIGetWindowBaseColor(CGIWindow *window);

CGIPoint CGIGetWindowCursorPosition(CGIWindow *window);

float CGIGetWindowScrollDeltaX(CGIWindow *window);

float CGIGetWindowScrollDeltaY(CGIWindow *window);

CGIBool CGIIsWindowScrolledX(CGIWindow *window);

CGIBool CGIIsWindowScrolledY(CGIWindow *window);

CGI_END_DECLS;
