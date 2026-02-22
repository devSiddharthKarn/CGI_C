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

#ifndef CGI_FONT_RASTER_H
#define CGI_FONT_RASTER_H

#pragma once

#include "cgi.h"

CGI_BEGIN_DECLS

/// @deprecated Use CGIWriteTextWithPen with CGIPen instead. This function uses a fixed 8x8 bitmap font as old-school style implementation.
/// @brief Write text to the specified window at given position with scaling and color
/// @param window Pointer to the CGIWindow instance
/// @param text The text string to be rendered
/// @param x_pos The x position to start rendering the text
/// @param y_pos The y position to start rendering the text
/// @param x_spacing The horizontal spacing between characters
/// @param y_spacing The vertical spacing between lines
/// @param h_scale The horizontal scaling factor for the text
/// @param v_scale The vertical scaling factor for the text
/// @param offset The character offset to start rendering from
/// @param line_wrap Enable or disable line wrapping
/// @param color The color to render the text in
CGIBool CGIWriteText(CGIWindow *window, char *text, unsigned int x_pos, unsigned int y_pos, unsigned int x_spacing, unsigned int y_spacing, unsigned int h_scale, unsigned int v_scale, unsigned int offset, CGIBool line_wrap, CGIColor_t color);

typedef struct CGIPen CGIPen;


/// @brief Generate a CGIPen with the specified TrueType font file and font height
/// @param TTF_Filename Path to the TrueType font file
/// @param fontHeight Desired font height in pixels
/// @return Pointer to the created CGIPen, or NULL on failure
CGIPen *CGIGeneratePen(const char *TTF_Filename, float fontHeight);


/// @brief Write text to the specified window using a CGIPen for font rendering
/// @param window CGIWindow* window as target
/// @param pen CGIPen* pen containing font info
/// @param text The text string to be rendered
/// @param x_pos The x position to start rendering the text
/// @param y_pos The y position to start rendering the text
/// @param x_spacing The horizontal spacing between characters
/// @param y_spacing The vertical spacing between lines
/// @param h_scale The horizontal scaling factor for the text
/// @param v_scale The vertical scaling factor for the text
/// @param offset offset to start rendering from
/// @param line_wrap CGIBool to enable or disable line wrapping
/// @param color The color to render the text in
/// @return CGIBool indicating success or failure
CGIBool CGIWriteTextWithPen(CGIWindow *window, CGIPen *pen, const char *text, unsigned int x_pos, unsigned int y_pos, unsigned int x_spacing, unsigned int y_spacing, float h_scale, float v_scale, unsigned int offset, CGIBool line_wrap, CGIColor_t color);



/// @brief function to destroy the pen created by CGIGeneratePen
/// @param pen CGIPen* pen to destroy
/// @return void
void CGIDestroyPen(CGIPen* pen);

CGI_END_DECLS
#endif
