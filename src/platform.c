#ifdef _WIN32
#include "win32.c"
#elif __linux__
#include "linux.c"
#else
#error "Unsupported platform by CGI"
#endif