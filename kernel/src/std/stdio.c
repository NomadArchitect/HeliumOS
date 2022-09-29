#include <stdbool.h>
#include <stddef.h>
#include <string.h>
#include <stdio.h>
#include <ctype.h>
#include <utils.h>

#include <debug.h>

#define PRINTF_BUF_SIZE (68) // This buffer is enough if we are printing anything other than a string

FILE* stdout;
FILE* stderr;

static FILE __stdout;
static FILE __stderr;

void __init_stdio()
{
    stdout = &__stdout;
    stderr = &__stderr;

    __stdout = dbg_output_file();
    __stderr = dbg_output_file();
}

int tpf(char const* template, ...)
{
    /* Implementation copied from printf() */
    va_list va;
    va_start(va, template);
    int ret = vprintf(template, va);
    va_end(va);
    return ret;
}


int printf(const char* template, ...)
{
    // TODO: If this function changes, tpf() should also be updated to be similar
    va_list va;
    va_start(va, template);
    int ret = vprintf(template, va);
    va_end(va);
    return ret;
}

int vprintf(char const* template, va_list va)
{
    return vfprintf(stdout, template, va);
}


int fprintf(FILE* stream, char const* template, ...)
{
    va_list va;
    va_start(va, template);
    int ret = vfprintf(stream, template, va);
    va_end(va);
    return ret;
}

int vfprintf(FILE* stream, char const* template, va_list va)
{
    va_list va2;
    va_copy(va2, va);
    size_t len = vsnprintf(0, 0, template, va2);
    va_end(va2);

    char buf[len + 1];

    vsnprintf(buf, len + 1, template, va);
    int ret = stream->write_string(stream, buf);
    return ret ? ret : len;
}

int snprintf(char* s, size_t size, char const* template, ...)
{
    va_list va;
    va_start(va, template);
    int ret = vsnprintf(s, size, template, va);
    va_end(va);
    return ret;
}


static inline char* __print_num(char* buf, int base, bool sign, bool pref, bool usign, uint64_t v)
{
    char* result = 0;

    if(usign)
        result = utos(v, base, buf + PRINTF_BUF_SIZE - 1);
    else
        result = ntos(v, base, buf + PRINTF_BUF_SIZE - 1);

    buf[PRINTF_BUF_SIZE - 1] = 0;
    if(pref)
    {
        result -= 2;
        result[0] = '0';
        switch(base)
        {
#define PN_PFX_BASE(n, c) case n: result[1] = c; break
            PN_PFX_BASE(10, 'd');
            PN_PFX_BASE(2 , 'b');
            PN_PFX_BASE(8 , 'c');
            PN_PFX_BASE(16, 'x');
#undef PN_PFX_BASE
            default:
                result[1] = 'u';
                break;
        }
    }
    if(sign && isdigit(result[1]) && base == 10)
        *--result = '+';
    return result;
}

#define print_num(buf, base, sign, pref, lmode, usign, va) \
    ({ \
        uint64_t v;\
        if(lmode)  \
            v = va_arg(va, uint64_t); \
        else \
            v = va_arg(va, uint32_t); \
        __print_num(buf, base, sign, pref, usign, v); \
    })


int vsnprintf(char* s, size_t size, char const* template, va_list va)
{
    size_t total_chars = 0;
    while(*template)
    {
        char  buf[PRINTF_BUF_SIZE];
        char const* to_print = buf;
        int max = INT32_MAX;
        int min = 0;
        bool pad0 = false;
        if(*template == '%') // do something fancy
        {
            ++template;

            bool lmode = false;
            bool sign = false;
            bool pref = false;
            // read flags if found
            while(*template == '#' || *template == '+' || *template == '0')
            {
                if(*template == '#' && !pref)
                    pref = true;
                else if(*template == '+' && !sign)
                    sign = true;
                else if(*template == '0' && !pad0)
                    pad0 = true;
                else // some flag was duplicated
                    break; // break without incrementing template, subsequent code will deal with it
                ++template;
            }
            // read min if found
            if(*template == '*')
            {
                min = va_arg(va, int);
                ++template;
            }
            else
                min = stou(template, &template, 10);
            // read max if found
            if(*template == '.')
            {
                if(*++template == '*')
                {
                    max = va_arg(va, int);
                    ++template;
                }
                else
                    max = stou(template, &template, 10);
            }
            // read type
            if(*template == 'l')
            {
                lmode = true;
                ++template;
            }
            // read conv
            switch(*template++)
            {
                // number types
                case 'd':
                case 'i':
                    to_print = print_num(buf, 10, sign, pref, lmode, false, va);
                    break;
                case 'u':
                    to_print = print_num(buf, 10, sign, pref, lmode, true , va);
                    break;
                case 'b':
                    to_print = print_num(buf, 2 , sign, pref, lmode, true , va);
                    break;
                case 'o':
                    to_print = print_num(buf, 8 , sign, pref, lmode, true , va);
                    break;
                case 'x':
                    to_print = print_num(buf, 16, sign, pref, lmode, true , va);
                    break;
                case 'p':
                    to_print = print_num(buf, 16, sign, pref, true , true , va);
                    break;
                case 'z':
                {
                    char* tail = buf + PRINTF_BUF_SIZE - 1;
                    size_t unit_order = 0;
                    size_t denom = 1;
                    size_t num;
                    if(lmode)
                        num = va_arg(va, uint64_t);
                    else
                        num = va_arg(va, uint32_t);
                    *tail = 0;
                    --tail;
                    while(unit_order < UNITS_COUNT)
                    {
                        if(num / denom % 1024)
                        {
                            *tail = g_units_sign[unit_order];
                            tail = utos(num / denom % 1024, 10, tail) - 1;
                        }
                        denom *= 1024;
                        ++unit_order;
                    }
                    to_print = tail + 1;
                    break;
                }
                // character types
                case 'c':
                    buf[0] = va_arg(va, int);
                    buf[1] = 0;
                    break;
                case 's':
                    to_print = va_arg(va, char const*);
                    break;
                case 'e':
                    to_print = "Not implemented";
                    max = INT32_MAX;
                    min = 0;
                    break;
                case '%':
                    buf[0] = '%';
                    buf[1] = 0;
                    break;
                default:
                    buf[0] = '?';
                    buf[1] = 0;
                    break;
            }
        }
        else
        {
            buf[0] = *template++;
            buf[1] = 0;
        }
        // After this line, template is supposed to be pointing at the next character
        int printed = 0; // printed in this loop;
        size_t tplen;
        if(max == INT32_MAX)
            tplen = strlen(to_print);
        else
            tplen = max;
        min -= tplen;

        // print leading spaces/0s
        while(min > 1)
        {
            ++total_chars;
            if(total_chars < size && s)
                *s++ = pad0 ? '0' : ' ';
            --min;
        }
        // print *to_print
        while(to_print && *to_print && printed < max)
        {
            ++total_chars;
            ++printed;
            if(total_chars < size && s)
                *s++ = *to_print;
            ++to_print;
        }
    }
    if(s)
        *s = 0;
    return total_chars;
}


int fputc(int c, FILE* stream)
{
    if(stream->write_chr(stream, c))
        return EOF;
    return c;
}

int putchar(int c)
{
    return fputc(c, stdout);
}

int fputs(char const* s, FILE* stream)
{
    return stream->write_string(stream, s) ? EOF : strlen(s);
}

int puts(char const* s)
{
    return fputs(s, stdout);
}
