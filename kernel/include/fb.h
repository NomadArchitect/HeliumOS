#ifndef HELIUM_FB_H
#define HELIUM_FB_H

#include <stdbool.h>
#include <stdarg.h>
#include <stdint.h>
#include <stddef.h>
#include <pack.h>

#define PSF_MAGIC (0x864ab572)

typedef struct 
{
    uint32_t magic        ;
    uint32_t version      ;
    uint32_t headersize   ;
    uint32_t flags        ;
    uint32_t numglyph     ;
    uint32_t bytesperglyph;
    uint32_t height       ;
    uint32_t width        ;
} pack psf_font;

// namespace fb

/**
 * \brief Loads the fonts and initializes the frambuffer for writing text.
 */
void fb_init();

void fb_flush(uint32_t scanline, uint32_t sl_count);

/**
 * \brief Sets the color of the pixel at (\a x, \a y) to \a c
 * \param c: Color to set in the format specified by \a bootboot.fb_type.
 * \param x
 * \param y
 */
void fb_pxs(uint32_t c, uint32_t x, uint32_t y, uint8_t* buf);

/**
 * \brief Returns the color of the pixel at (\a x, \a y).
 * \param x
 * \param y
 * \return The color of the pixel in the format specifed by \a bootboot.fb_type .
 */
uint32_t fb_pxg(uint32_t x, uint32_t y);

/**
 * \brief Scrolls the text up \a n lines.
 * \param n
 */
void fb_scroll(uint32_t n, bool flush);

/**
 * \brief Writes the ASCII character \a c at (\a x, \a y).
 * \param c
 * \param x
 * \param y
 */
void fb_putc(char c, uint32_t x, uint32_t y, bool flush);

/**
 * \brief Writes the ASCII character \a c at the position of the cursor.
 * \param c
 */
void fb_wrc(char c, bool flush);

/**
 * \brief Writes the null terminated string \a s to the framebuffer. Unlike fb_wr, 
 *        this one does not support printf-like functionality.
 * \param s
 */
void fb_wrs(char const* s, bool flush);

/**
 * \brief Writes \a len characters from \a s, ignoring any null termination
 * \param s
 * \param len
 */
void fb_wrm(char const* s, size_t len, bool flush);

/**
 * \brief Writes the null terminated string \a s to the framebuffer with support to printf-like % commands.
 * \param s
 * \param ...
 */
void fb_wr(char const* s, bool flush, ...);

/**
 * \brief Writes a null terminated string \a s to the framebuffer with support printf-like % commands.
 * \param s
 * \param args: A va_list of optional arguments for % commands
 */
void vfb_wr(char const* s, bool flush, va_list args);

#endif