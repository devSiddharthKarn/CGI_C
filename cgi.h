#include "src/typedefs.h"



typedef struct CGIWindow CGIWindow;

CGIWindow *CGICreateWindow(char *classname, char *window_name, unsigned int x_pos, unsigned int y_pos, unsigned int width, unsigned int height, CGIColor_t color);

CGIBool CGIWindowCleanup(CGIWindow *window);

CGIBool CGIShowWindow(CGIWindow *window);

void CGISetPixel(CGIWindow *window, int x_pos, int y_pos, CGIColor_t color);

const void *CGIQueryWindow(CGIQuery query, CGIWindow *window);

CGIBool CGIIsWindowOpen(const CGIWindow *window);

CGIColor_t CGIMakeColor(unsigned char r, unsigned char g, unsigned char b);

CGIBool CGIRefreshWindow(CGIWindow *window);

CGIBool CGIRefreshBuffer(CGIWindow *window);

CGIBool CGIClearBuffer(CGIWindow *window, CGIColor_t color);

CGIBool CGIIsKeyPressed(CGIWindow*  window,CGIInputKey key);

CGIBool CGIIsWindowFocused( CGIWindow* window);

