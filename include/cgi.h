/*
 * Copyright 2025 Siddharth Karn
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *     http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */

/// @file cgi.h
/// @brief Main CGI library header file
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

// /// @brief Opaque structure representing the CGI instance
// typedef struct CGI CGI;

CGI_BEGIN_DECLS
/// @brief Opaque structure representing a CGI window
typedef struct CGIWindow CGIWindow;



/// @brief Create a new CGI window
/// @param classname The class name for the window
/// @param window_name The name/title of the window
/// @param x_pos The x position of the window
/// @param y_pos The y position of the window
/// @param width The width of the window
/// @param height The height of the window
/// @param color The base color of the window
/// @return Pointer to the created CGIWindow instance, or NULL on failure
CGIWindow *CGICreateWindow(char *classname, char *window_name, unsigned int x_pos, unsigned int y_pos, unsigned int width, unsigned int height, CGIColor_t color);

/// @brief Get the current event image from the window
/// @param window CGIWindow* window
/// @return CGIEventImage
CGIEventImage CGIGetWindowEventImage(CGIWindow* window);

/// @brief Get the current surface of the window
/// @param window CGIWindow* window
/// @return CGIWindowSurface(the current window surface as in an allocated memory buffer that needs to be freed after use)
CGIWindowSurface CGIGetWindowSurface(CGIWindow* window);

/// @brief Free the resources associated with a CGIWindowSurface that was previously allocated
/// @param surface CGIWindowSurface surface to be freed
void CGIFreeWindowSurface(CGIWindowSurface surface);

/// @brief Clean up and destroy a CGI window
/// @param window Pointer to the CGIWindow instance to be cleaned up
/// @return CGI_true if successful, otherwise CGI_false
CGIBool CGIWindowCleanup(CGIWindow *window);

/// @brief Show the specified CGI window
/// @param window Pointer to the CGIWindow instance to be shown
/// @return CGI_true if successful, otherwise CGI_false
CGIBool CGIShowWindow(CGIWindow *window);

/// @brief Set a pixel in the window's buffer to a specified color
/// @param window Pointer to the CGIWindow instance
/// @param x_pos The x position of the pixel
/// @param y_pos The y position of the pixel
/// @param color The color to set the pixel to
/// @return void
void CGISetPixel(CGIWindow *window, int x_pos, int y_pos, CGIColor_t color);

/// @brief Refresh a specific region of the window's buffer(no return for less overhead and fast blitting)
/// @param window CGIWindow* pointer to the window whose buffer region is to be refreshed
/// @param x x-position of the region to be refreshed
/// @param y y-position of the region to be refreshed
/// @param width width of the region to be refreshed
/// @param height height of the region to be refreshed
/// @return void
void CGIRefreshBufferRegion(CGIWindow *window, int x, int y, int width, int height);

/// @brief  Perform a query on the CGI or CGIWindow structure
/// @param query name of the query to be performed
/// @param acceptor whom to make it accept
/// @return const void * pointer to the result of the query (need to be casted to appropriate type)
// const void *CGIPerformQuery(CGIQuery query, const void *acceptor);

/// @brief check if the window is open
/// @param window CGIWindow pointer to the window to be checked
/// @return CGI_true if the window is open, otherwise CGI_false
CGIBool CGIIsWindowOpen(const CGIWindow *window);

/// @brief close the specified window
/// @param window Pointer to the CGIWindow instance to be closed
/// @return CGI_true if successful, otherwise CGI_false
CGIBool CGICloseWindow(CGIWindow *window);

/// @brief Create a CGIColor_t from r, g, b values
/// @param r Red channel value (0-255)
/// @param g Green channel value (0-255)
/// @param b Blue channel value (0-255)
/// @return CGIColor_t representing the color
CGIColor_t CGIMakeColor(unsigned char r, unsigned char g, unsigned char b);

/// @brief  Refresh the specified window
/// @param window Pointer to the CGIWindow instance to be refreshed
/// @return CGI_true if successful, otherwise CGI_false
CGIBool CGIRefreshWindow(CGIWindow *window, CGIWindowRefreshMode window_refresh_mode);

/// @brief Refresh the buffer of the specified window
/// @param window Pointer to the CGIWindow instance whose buffer is to be refreshed
/// @return CGI_true if successful, otherwise CGI_false
CGIBool CGIRefreshBuffer(CGIWindow *window);

/// @brief Clear the buffer of the specified window with a given color
/// @param window Pointer to the CGIWindow instance whose buffer is to be cleared
/// @param color The color to clear the buffer with
/// @return CGI_true if successful, otherwise CGI_false
CGIBool CGIClearBuffer(CGIWindow *window, CGIColor_t color);

/// @brief Clear the specific buffer region of the specified window with a given color(has no return for less overhead and fast blitting)
/// @param window CGIWindow* to the window whose buffer region is to be cleared
/// @param x x-position of the region to be cleared
/// @param y y-position of the region to be cleared
/// @param width width of the region to be cleared
/// @param height height of the region to be cleared
/// @param color CGIColor_t color to clear the region with
/// @return void
void CGIClearBufferRegion(CGIWindow *window, int x, int y, int width, int height, CGIColor_t color);

/// @brief  Perform a command on the CGI or CGIWindow structure
/// @param command      The command to be performed
/// @param args         Arguments for the command
/// @param acceptor     The entity to accept the command
/// @return CGI_true if successful, otherwise CGI_false
// CGIBool CGIPerformCommand(CGICommand command, const void *args, const void *acceptor);

/// @brief Check if a specific key is pressed in the given window
/// @param window Pointer to the CGIWindow instance
/// @param key The key to check
/// @return CGI_true if the key is pressed, otherwise CGI_false
// CGIBool CGIIsKeyDown(CGIKeyCode key);

// CGIBool CGIIsKeyUp(CGIKeyCode key);

CGIBool CGIIsWindowKeyDown(CGIWindow* window, CGIKeyCode key);
CGIBool CGIIsWindowKeyUp(CGIWindow* window, CGIKeyCode key);


/// @brief Check if the specified window is focused
/// @param window Pointer to the CGIWindow instance
/// @return CGI_true if the window is focused, otherwise CGI_false
CGIBool CGIIsWindowFocused(CGIWindow *window);

/// @brief Check if the specified window has been resized
/// @param window Pointer to the CGIWindow instance
/// @return CGI_true if the window has been resized, otherwise CGI_false
CGIBool CGIIsWindowResized(CGIWindow *window);

/// @brief Get the name of the specified window
/// @param window Pointer to the CGIWindow instance
/// @return Pointer to the window's name string
char *CGIGetWindowName(CGIWindow *window);

/// @brief Get the position of the specified window
/// @param window Pointer to the CGIWindow instance
/// @return The position of the window as a CGIPoint structure
CGIPoint CGIGetWindowPosition(CGIWindow *window);

/// @brief Get the height of the specified window
/// @param window Pointer to the CGIWindow instance
/// @return The height of the window
unsigned int CGIGetWindowHeight(CGIWindow *window);

/// @brief Get the width of the specified window
/// @param window Pointer to the CGIWindow instance
/// @return The width of the window
unsigned int CGIGetWindowWidth(CGIWindow *window);

/// @brief Get the buffer height of the specified window
/// @param window Pointer to the CGIWindow instance
/// @return The buffer height of the window
unsigned int CGIGetWindowBufferHeight(CGIWindow *window);

/// @brief Get the buffer width of the specified window
/// @param window Pointer to the CGIWindow instance
/// @return The buffer width of the window
unsigned int CGIGetWindowBufferWidth(CGIWindow *window);

/// @brief Get the base color of the specified window
/// @param window Pointer to the CGIWindow instance
/// @return The base color of the window
CGIColor_t CGIGetWindowBaseColor(CGIWindow *window);

/// @brief Get the cursor position of the specified window
/// @param window Pointer to the CGIWindow instance
/// @return The cursor position of the window as a CGIPoint structure
CGIPoint CGIGetWindowCursorPosition(CGIWindow *window);

/// @brief Get the horizontal scroll delta of the specified window
/// @param window Pointer to the CGIWindow instance
/// @return The horizontal scroll delta of the window
float CGIGetWindowScrollDeltaX(CGIWindow *window);

/// @brief Get the vertical scroll delta of the specified window
/// @param window Pointer to the CGIWindow instance
/// @return The vertical scroll delta of the window
float CGIGetWindowScrollDeltaY(CGIWindow *window);

/// @brief Check if the specified window has been scrolled horizontally
/// @param window Pointer to the CGIWindow instance
/// @return CGI_true if the window has been scrolled horizontally, otherwise CGI_false
CGIBool CGIIsWindowScrolledX(CGIWindow *window);

/// @brief Check if the specified window has been scrolled vertically
/// @param window Pointer to the CGIWindow instance
/// @return CGI_true if the window has been scrolled vertically, otherwise CGI_false
CGIBool CGIIsWindowScrolledY(CGIWindow *window);

/// @brief Check if a specific mouse button is pressed
/// @param cgi Pointer to the CGI instance
/// @param key The mouse button key to check
/// @return CGI_true if the specified mouse button is pressed, otherwise CGI_false
// CGIBool CGIIsMouseButtonPressed(CGIWindow* window,CGIInputKey key);

// CGIBool CGIIsCursorKeyDown(CGIKeyCode key);

// CGIBool CGIIsCursorKeyUp(CGIKeyCode key);

CGIBool CGIIsWindowCursorKeyDown(CGIWindow* window,CGIKeyCode key);
CGIBool CGIIsWindowCursorKeyUp(CGIWindow* window,CGIKeyCode key);

/// @brief  Set the title of the specified window
/// @param window Pointer to the CGIWindow instance
/// @param title The new title for the window
/// @return CGI_true if the title was successfully set, otherwise CGI_false
CGIBool CGISetWindowTitle(CGIWindow *window, char *title);

/// @brief Set the position of the specified window
/// @param window Pointer to the CGIWindow instance
/// @param position The new position for the window
/// @return CGI_true if the position was successfully set, otherwise CGI_false
CGIBool CGISetWindowPosition(CGIWindow *window, CGIPoint position);

/// @brief Set the size of the specified window
/// @param window Pointer to the CGIWindow instance
/// @param size The new size for the window
/// @return CGI_true if the size was successfully set, otherwise CGI_false
CGIBool CGISetWindowSize(CGIWindow *window, CGIPoint size);

/// @brief Set the base color of the specified window
/// @param window Pointer to the CGIWindow instance
/// @param base_color The new base color for the window
/// @return CGI_true if the base color was successfully set, otherwise CGI_false
CGIBool CGISetWindowBaseColor(CGIWindow *window, CGIColor_t base_color);

/// @brief Set the show logic of the specified window
/// @param window Pointer to the CGIWindow instance
/// @param logic The new show logic for the window
/// @return CGI_true if the show logic was successfully set, otherwise CGI_false
CGIBool CGISetWindowShowLogic(CGIWindow *window, CGIBool logic);

/// @brief Set the focus logic of the specified window
/// @param window Pointer to the CGIWindow instance
/// @param logic The new focus logic for the window
/// @return CGI_true if the focus logic was successfully set, otherwise CGI_false
CGIBool CGISetWindowFocusLogic(CGIWindow *window, CGIBool logic);

/// @brief Set the resizable logic of the specified window
/// @param window Pointer to the CGIWindow instance
/// @param logic The new resizable logic for the window
/// @return CGI_true if the resizable logic was successfully set, otherwise CGI_false
CGIBool CGISetWindowResizableLogic(CGIWindow *window, CGIBool logic);

/// @brief Set the minimizable logic of the specified window
/// @param window Pointer to the CGIWindow instance
/// @param logic The new minimizable logic for the window
/// @return CGI_true if the minimizable logic was successfully set, otherwise CGI_false
CGIBool CGISetWindowMinimizableLogic(CGIWindow *window, CGIBool logic);

/// @brief Set the maximizable logic of the specified window
/// @param window Pointer to the CGIWindow instance
/// @param logic The new maximizable logic for the window
/// @return CGI_true if the maximizable logic was successfully set, otherwise CGI_false
CGIBool CGISetWindowMaximizableLogic(CGIWindow *window, CGIBool logic);

#ifdef CGI_WINDOWS_IMPLEMENTATION_ACTIVE

#include "Windows.h"

/// @brief Get the Win32 HWND handle of the specified window
/// @param window CGIWindow instance
/// @return HWND handle of the window
HWND CGIGetWindow_Win32_HWND(CGIWindow *window);

/// @brief Get the Win32 HDC handle of the specified window
/// @param window CGIWindow instance
/// @return HDC handle of the window
HDC CGIGetWindow_Win32_HDC(CGIWindow *window);

/// @brief Get the Win32 WNDCLASSA structure of the specified window
/// @param window CGIWindow instance
/// @return WNDCLASSA structure of the window
WNDCLASSA CGIGetWindow_Win32_WNDCLASSA(CGIWindow *window);

/// @brief Get the Win32 PAINTSTRUCT structure of the specified window
/// @param window CGIWindow instance
/// @return PAINTSTRUCT structure of the window
PAINTSTRUCT CGIGetWindow_Win32_PaintStruct(CGIWindow *window);

/// @brief Get the Win32 BITMAPINFO structure of the specified window
/// @param window CGIWindow instance
/// @return BITMAPINFO structure of the window
BITMAPINFO CGIGetWindow_Win32_BITMAPINFO(CGIWindow *window);

/// @brief Get the Win32 MSG structure of the specified window
/// @param window CGIWindow instance
/// @return MSG structure of the window
MSG CGIGetWindow_Win32_MSG(CGIWindow *window);

#endif

#ifdef CGI_LINUX_IMPLEMENTATION_ACTIVE

#include "X11/Xlib.h"

/// @brief Get the Xlib Display pointer of the specified window
/// @param window CGIWindow instance
/// @return Display pointer of the window
Display *CGIGetWindow_Linux_Xlib_Display_pointer(CGIWindow *window);

/// @brief Get the Xlib screen number of the specified window
/// @param window CGIWindow instance
/// @return Screen number of the window
int CGIGetWindow_Linux_Xlib_screen(CGIWindow *window);

/// @brief Get the Xlib Window of the specified window
/// @param window CGIWindow instance
/// @return Window of the window
Window CGIGetWindow_Linux_Xlib_window(CGIWindow *window);

/// @brief Get the Xlib Colormap of the specified window
/// @param window CGIWindow instance
/// @return Colormap of the window
Colormap CGIGetWindow_Linux_Xlib_colormap(CGIWindow *window);

/// @brief Get the Xlib GC (Graphics Context) of the specified window
/// @param window CGIWindow instance
/// @return GC (Graphics Context) of the window
GC CGIGetWindow_Linux_Xlib_GC(CGIWindow *window);

/// @brief Get the Xlib base color of the specified window
/// @param window CGIWindow instance
/// @return XColor base color of the window
XColor CGIGetWindow_Linux_Xlib_base_color(CGIWindow *window);

#endif

CGI_END_DECLS;
