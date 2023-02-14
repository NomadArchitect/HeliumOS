#include <debug.h>
#include <asm/io.h>

int dbg_write_chr(FILE *f, char c)
{
  as_outb(DEBUG_CONSOLE, c);
  return 0;
}

int dbg_write_string(FILE *f, char const *str)
{
  while(str && *str)
  {
    dbg_write_chr(f, *str);
    ++str;
  }
  return 0;
}

FILE dbg_output_file()
{
  FILE f;
  f.write_chr = dbg_write_chr;
  f.write_string = dbg_write_string;
  return f;
}
