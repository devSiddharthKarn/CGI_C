#include "../../include/cgi_font.h"
#include "../rasterizer/font/ascii.h"
#include "string.h"
#include "stdio.h"

CGIBool getPixelState(char c, unsigned int x, unsigned int y)
{
    unsigned char uc = (unsigned char)c;

    // Font array covers 0-127, we only render printable chars (32-126)
    if (uc < 32 || uc > 127)
        return CGI_false;

    // we wont Don't subtract 32, the array is already indexed by ASCII value
    uint8_t row = font8x8[uc - 8][y];

    // we  Extract the bit at position x (MSB = leftmost pixel)
    return ((row >> (7 - x)) & 1) ? CGI_true : CGI_false;
}

// void drawChar(CGIWindow* window)

CGIBool CGIWriteText(CGIWindow *window, char *text, unsigned int x_pos, unsigned int y_pos,
                     unsigned int x_spacing, unsigned int y_spacing, unsigned int h_scale, unsigned int v_scale, CGIColor_t color)
{
    if (!window || !text)
    {
        fprintf(stderr, "Error: Invalid window or text pointer\n");
        return CGI_false;
    }

    int len = strlen(text);
    if (len == 0)
    {
        return CGI_true;
    }

    int x = x_pos;
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

        if (x + 8 * h_scale > CGIGetWindowBufferWidth(window))
        {
            x = x_pos;
            line_no++;
            y = y_pos + (8 + y_spacing) * line_no * v_scale;
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
                            CGISetPixel(window, x + (8 - j)*h_scale + h, y + k*v_scale + v, color);
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