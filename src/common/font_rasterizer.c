
#define STB_TRUETYPE_IMPLEMENTATION

#include "../../external/stb_truetype.h"
#include "../rasterizer/font/ascii.h"

#include "../../include/cgi.h"
#include "../../include/cgi_font.h"
#include "stdlib.h"
#include "string.h"
#include "stdio.h"


///@deprecated Use CGIWriteTextWithPen with CGIPen instead.
CGIBool getPixelState(char c, unsigned int x, unsigned int y)
{
    unsigned char uc = (unsigned char)c;

    if (uc < 32 || uc > 127)
        return CGI_false;

    uint8_t row = font8x8[uc - 8][y];

    // we  Extract the bit at position x (MSB = leftmost pixel)
    return ((row >> (7 - x)) & 1) ? CGI_true : CGI_false;
}

// void drawChar(CGIWindow* window)

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
CGIBool CGIWriteText(CGIWindow *window, char *text, unsigned int x_pos, unsigned int y_pos,
                     unsigned int x_spacing, unsigned int y_spacing, unsigned int h_scale, unsigned int v_scale,
                     unsigned int offset, CGIBool line_wrap, CGIColor_t color)
{
    if (!window)
    {
        fprintf(stderr, "Error: Invalid window \n");
        return CGI_false;
    }

    if (!text)
        return CGI_true;

    int len = strlen(text);
    if (len == 0)
    {
        return CGI_true;
    }

    int x = x_pos + (8 + x_spacing) * h_scale * offset; // Start position
    int y = y_pos;

    int line_no = 0;

    for (unsigned int i = 0; i < len; i++)
    {
        char c = text[i];

        if (c < 32 || c > 127)
        {
            x += ((8 + x_spacing) * h_scale); // Still advance cursor
            continue;
        }

        if (line_wrap)
        {
            if (x + 8 * h_scale > CGIGetWindowBufferWidth(window))
            {
                x = x_pos;
                line_no++;
                y = y_pos + (8 + y_spacing) * line_no * v_scale;
            }
        }

        for (int j = 0; j < 8; j++) // x within character
        {
            for (int k = 0; k < 8; k++) // y within character
            {
                if (getPixelState(c, j, k))
                {
                    // printf("PIXEL AT %d %d\n",x+j,y+k);
                    for (int h = 0; h < h_scale; h++)
                    {
                        for (int v = 0; v < v_scale; v++)
                        {
                            CGISetPixel(window, x + (8 - j) * h_scale + h, y + k * v_scale + v, color);
                        }
                    }

                    // CGIRefreshBuffer(window);
                    // Sleep(2000);
                }
            }
        }
        x += (7 + x_spacing) * h_scale;
    }
    return CGI_true;
}

// void CGIDebugPrintChar(char c)
// {
//     unsigned char uc = (unsigned char)c;

//     if (uc < 32 || uc > 127)
//     {
//         printf("Character '%c' (0x%02X) is out of range\n", c, uc);
//         return;
//     }

//     printf("\nCharacter '%c' (0x%02X) bitmap:\n", c, uc);

//     for (int k = 0; k < 8; k++)
//     {
//         uint8_t row = font8x8[uc][k];
//         printf("Row %d (0x%02X): ", k, row);

//         for (int j = 0; j < 8; j++)
//         {
//             if ((row >> (7 - j)) & 1)
//             {
//                 printf("█");
//             }
//             else
//             {
//                 printf("·");
//             }
//         }
//         printf("\n");
//     }
// }

// // Test function to verify font rendering
// void CGITestFontRendering(CGIWindow *window)
// {
//     printf("\n=== Testing Font Rendering ===\n");

//     // Test individual characters
//     CGIDebugPrintChar('A');
//     CGIDebugPrintChar('0');
//     CGIDebugPrintChar(' ');

//     // Test actual rendering
//     CGIColor_t white = {255, 255, 255};
//     CGIColor_t red = {255, 0, 0};
//     CGIColor_t green = {0, 255, 0};

//     CGIWriteText(window, "Hello World!", 10, 10, 2, white);
//     CGIWriteText(window, "Testing 123", 10, 30, 1, red);
//     CGIWriteText(window, "ABCDEFGHIJKLMNOPQRSTUVWXYZ", 10, 50, 0, green);

//     printf("=== Font Test Complete ===\n\n");
// }

struct CGIPen
{
    stbtt_fontinfo fontInfo;
    float font_height;
    float scale;
    unsigned char *bitmap;
    unsigned char *fontBuffer;
};

unsigned char *CGI_Helper_Read_TTF_File(const char *filename)
{
    FILE *fp = fopen(filename, "rb");
    if (!fp)
    {
        return NULL;
    }

    fseek(fp, 0, SEEK_END);
    size_t f_size = ftell(fp);
    rewind(fp);

    unsigned char *buffer = (unsigned char *)malloc(f_size);
    if (!buffer)
    {
        fprintf(stderr, "Memory allocation failed for TTF file: %s\n", filename);
        fclose(fp);
        return NULL;
        // exit(EXIT_FAILURE);
    }

    size_t read_count = fread(buffer, sizeof(unsigned char), f_size, fp);
    fclose(fp);

    if (read_count != f_size)
    {
        fprintf(stderr, "Failed to read entire TTF file: %s\n", filename);
        free(buffer);
        return NULL;
        // exit(EXIT_FAILURE);
    }

    return buffer;
}

CGIPen *CGIGeneratePen(const char *TTF_Filename, float fontHeight)
{
    size_t fileBufferSize = 0;
    unsigned char *fileBuffer = CGI_Helper_Read_TTF_File(TTF_Filename);

    if (fileBuffer == NULL)
    {
        fprintf(stderr,"error reading file:%s", TTF_Filename);
        return NULL;
    }

    CGIPen *pen = (CGIPen *)malloc(sizeof(CGIPen));

    if (!pen)
    {
        fprintf(stderr, "error allocating memory for CGIPen");
        free(fileBuffer);
        return NULL;
    }

    int result = stbtt_InitFont(&pen->fontInfo, fileBuffer, 0);

    if (!result)
    {
        fprintf(stderr,"error init font from file:%s \n",TTF_Filename);
        free(fileBuffer);
        free(pen);
        return NULL;
    }

    pen->font_height = fontHeight;
    pen->scale = stbtt_ScaleForPixelHeight(&pen->fontInfo, pen->font_height);
    pen->fontBuffer = fileBuffer;

    return pen;
}
void CGI_Helper_blit_glyph(CGIWindow *window, int x_pos, int y_pos, int width, int height, unsigned char *glyph, CGIColor_t color)
{
    int window_width = CGIGetWindowBufferWidth(window);
    int window_height = CGIGetWindowBufferHeight(window);

    for (int i = 0; i < height; i++)
    {
        int fb_y = y_pos + i;
        
        if (fb_y < 0 || fb_y >= window_height)
            continue;

        for (int j = 0; j < width; j++)
        {
            int fb_x = x_pos + j;
            
            if (fb_x < 0 || fb_x >= window_width)
                continue;

            float alpha = (float)glyph[i * width + j] / 255.0f;

            CGIColor_t pixelColor = CGIGetPixelColor(window, fb_x, fb_y);

            CGIColor_t output_color;
            output_color.r = (unsigned char)((1 - alpha) * pixelColor.r + alpha * color.r);
            output_color.g = (unsigned char)((1 - alpha) * pixelColor.g + alpha * color.g);
            output_color.b = (unsigned char)((1 - alpha) * pixelColor.b + alpha * color.b);

            CGISetPixel(window, fb_x, fb_y, output_color);
        }
    }
}

CGIBool CGIWriteTextWithPen(
    CGIWindow *window,
    CGIPen *pen,
    const char *text,
    unsigned int x_pos,
    unsigned int y_pos,
    unsigned int x_spacing,
    unsigned int y_spacing,
    float h_scale,       // horizontal scaling factor
    float v_scale,       // vertical scaling factor
    unsigned int offset, // starting horizontal offset
    CGIBool line_wrap,   // wrap text if reaching window width
    CGIColor_t color)
{
    if (!window || !pen || !text)
        return CGI_false;

    int len = strlen(text);
    if (len == 0)
        return CGI_true;

    int window_width = CGIGetWindowBufferWidth(window); 
    int window_height = CGIGetWindowBufferHeight(window);

    // Calculate baseline offset (ascent)
    int ascent, descent, lineGap;
    stbtt_GetFontVMetrics(&pen->fontInfo, &ascent, &descent, &lineGap);
    int baseline_offset = (int)(ascent * pen->scale * v_scale);

    // Calculate offset: width occupied by N average characters (including spacing)
    // Use 'n' as average character for width calculation
    int avg_advance = 0, avg_lsb = 0;
    stbtt_GetCodepointHMetrics(&pen->fontInfo, 'n', &avg_advance, &avg_lsb);
    int offset_pixels = (int)(offset * (avg_advance * pen->scale * h_scale + x_spacing));

    int cursor_x = x_pos + offset_pixels;
    int cursor_y = y_pos + baseline_offset;  // Start at baseline, not top

    for (int i = 0; i < len; i++)
    {
        char ch = text[i];

        // get metrics for cursor advance 
        int advance, leftSideBearing;
        stbtt_GetCodepointHMetrics(&pen->fontInfo, ch, &advance, &leftSideBearing);

        // Calculate the actual width this character will occupy
        int char_width = (int)(advance * pen->scale * h_scale);
        
        // Line wrapping: check BEFORE rendering
        if (line_wrap && (cursor_x + char_width) > window_width)
        {
            cursor_x = x_pos; 
            cursor_y += (int)(pen->font_height * v_scale + y_spacing); 
        }

        // Clip vertically if baseline exceeds window height
        if (cursor_y - baseline_offset >= window_height)
        {
            break; // stop rendering
        }

        // Get bitmap for current glyph
        int width = 0, height = 0, x_off = 0, y_off = 0;
        unsigned char *glyph = stbtt_GetCodepointBitmap(
            &pen->fontInfo,
            pen->scale * h_scale, // scale in pixels (horizontal)
            pen->scale * v_scale, // scale in pixels (vertical)
            ch,
            &width,
            &height,
            &x_off,
            &y_off);

        if (glyph)
        {
            // Blit the glyph at cursor position plus offsets
            // The offsets position the glyph correctly relative to the baseline
            CGI_Helper_blit_glyph(window, cursor_x + x_off, cursor_y + y_off, width, height, glyph, color);

            // Free glyph bitmap after blitting
            stbtt_FreeBitmap(glyph, NULL);
        }

        // Advance cursor for all characters (including spaces)
        if (i < len - 1)
        {
            int kern = stbtt_GetCodepointKernAdvance(&pen->fontInfo, ch, text[i + 1]);
            cursor_x += (int)((advance + kern) * pen->scale * h_scale) + x_spacing;
        }
        else
        {
            cursor_x += (int)(advance * pen->scale * h_scale) + x_spacing;
        }
    }

    return CGI_true;
}
void CGIDestroyPen(CGIPen* pen){
    if(!pen) return;

    if(pen->bitmap){
        free(pen->bitmap);
    }

    if(pen->fontBuffer){
        free(pen->fontBuffer);
    }

    free(pen);
}
