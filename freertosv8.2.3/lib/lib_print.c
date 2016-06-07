/***************************************************************/
/*  @filename:           printk.c									     */
/*  @Description:       format the wanted string to a bufer				     */
/*  @usage:               printk(*fmt); 								     */
/*  @register the console first:  register_console(your_write_console)  	     */
/*  @type:    int (*your_write_console)(char)					            */
/*  @define a global buffer called :  char printk_buf[NUMER] somewhere      */
/***************************************************************/


#include "lib_print.h"
#include "bsp_uart.h"
#include "portmacro.h"

/*The buffer for printk to format strings...*/
//char printk_buf[256];


/********************************************************************/
/****       register printk buttom routine                         **/
/********************************************************************/
void write_console(char c)
{
	unsigned int ulNum = 0;
	CKUartWrite((unsigned char *)&c, 1UL, (unsigned int *)&ulNum, UART_BLOCKING);
}



/*printk function will format the output string here*/
/*DO BE careful about the overflow check!          */
/*I do not complete the check yet :)    LH            */

/**
 * strnlen - Find the length of a length-limited string
 * @s: The string to be sized
 * @count: The maximum number of bytes to search
 */
static ck_size_t ck_strnlen(const char * s, ck_size_t count)
{
        const char *sc;

        for (sc = s; count-- && *sc != '\0'; ++sc)
                /* nothing */;
        return sc - s;
}



static int ck_skip_atoi(const char **s)
{
        int i=0;

        while (ck_isdigit(**s))
                i = i*10 + *((*s)++) - '0';
        return i;
}



static char *ck_number (char *buf, char *end, long num,\
	       	int base, int size,int precision, int type);



/*
* Storage alignment properties
*/

#define  CK_AUPBND         (sizeof (CK_NATIVE_INT) - 1)
#define  CK_ADNBND         (sizeof (CK_NATIVE_INT) - 1)

/*
* Variable argument list macro definitions
*/

#define ck_bnd(X, bnd)    (((sizeof (X)) + (bnd)) & (~(bnd)))
#define ck_va_arg(ap, T)   (*(T *)(((ap) += (ck_bnd (T, CK_AUPBND))) - (ck_bnd (T,CK_ADNBND))))
#define ck_va_end(ap)      (void) 0
#define ck_va_start(ap, A) (void) ((ap) = (((char *) &(A)) + (ck_bnd (A,CK_AUPBND))))
      

int printk(const char *fmt, ...)
{
        ck_va_list args;
        unsigned int printed_len = 0;
        char *p;
        static char printk_buf[256];
        p = printk_buf;

        /* Emit the output into the temporary buffer */
        ck_va_start(args, fmt);
        printed_len = ck_vsnprintf(printk_buf, sizeof(printk_buf), fmt, args);
        ck_va_end(args);
		

        portENTER_CRITICAL();
        while(printed_len != 0){
        	write_console(*p++);
        	printed_len --;
        }
        portEXIT_CRITICAL();
	
		return 0;

}


/**
* ck_vsnprintf - Format a string and place it in a buffer
* @buf: The buffer to place the result into
* @size: The size of the buffer, including the trailing null space
* @fmt: The format string to use
* @args: Arguments for the format string
*
* Call this function if you are already dealing with a va_list.
* You probably want snprintf instead.
 */
int ck_vsnprintf(char *buf, ck_size_t size, const char *fmt, ck_va_list args)
{
        int len;
        unsigned long num;
        int i, base;
        char *str, *end, c;
        const char *s;

        int flags;              /* flags to number() */

        int field_width;        /* width of output field */
        int precision;          /* min. # of digits for integers; max
                                   number of chars for from string */
        int qualifier;          /* 'h', 'l', or 'L' for integer fields */
                                /* 'z' support added 23/7/1999 S.H.    */
                                /* 'z' changed to 'Z' --davidm 1/25/99 */
        
	str = buf;
        end = buf + size - 1;
        
        if (end < buf - 1) { 
                end = ((void *) -1);
                size = end - buf + 1;
        }
        
        for (; *fmt ; ++fmt) {
                if (*fmt != '%') { 
                        if (str <= end)
                                *str = *fmt;
                        ++str;
                        continue;
                }
                
                /* process flags */
                flags = 0;
                repeat: 
                        ++fmt;          /* this also skips first '%' */
                        switch (*fmt) {
                                case '-': flags |= LEFT; goto repeat;
                                case '+': flags |= PLUS; goto repeat;
                                case ' ': flags |= SPACE; goto repeat;
                                case '#': flags |= SPECIAL; goto repeat;
                                case '0': flags |= ZEROPAD; goto repeat;
                        }

                /* get field width */
                field_width = -1;
                if (ck_isdigit(*fmt))
                        field_width = ck_skip_atoi(&fmt);
                else if (*fmt == '*') {
                        ++fmt;
                        /* it's the next argument */
                        field_width = ck_va_arg(args, int);
                        if (field_width < 0) {
                                field_width = -field_width;
                                flags |= LEFT;
                        }
                }

                /* get the precision */
                precision = -1;
                if (*fmt == '.') {
                        ++fmt;
                        if (ck_isdigit(*fmt))
                                precision = ck_skip_atoi(&fmt);
                        else if (*fmt == '*') {
                                ++fmt;
                                /* it's the next argument */
                                precision = ck_va_arg(args, int);
                        }
                        if (precision < 0)
                                precision = 0;
                }

                /* get the conversion qualifier */
                qualifier = -1;
                if (*fmt == 'h' || *fmt == 'l' || *fmt == 'L' || *fmt =='Z') {
                        qualifier = *fmt;
                        ++fmt;
                        if (qualifier == 'l' && *fmt == 'l') {
                                qualifier = 'L';
                                ++fmt;
                        }
                }

                /* default base */
                base = 10;

                switch (*fmt) {
                        case 'c':
                                if (!(flags & LEFT)) {
                                        while (--field_width > 0) {
                                                if (str <= end)
                                                        *str = ' ';
                                                ++str;
                                        }
                                }
                                c = (unsigned char) ck_va_arg(args, int);
                                if (str <= end)
                                        *str = c;
                                ++str;
                                while (--field_width > 0) {
                                        if (str <= end)
                                                *str = ' ';
                                        ++str;
                                }
                                continue;

                        case 's':
                                s = ck_va_arg(args, char *);
                                if (!s)
                                        s = "<NULL>";

                                len = ck_strnlen(s, precision);

                                if (!(flags & LEFT)) {
                                        while (len < field_width--) {
                                                if (str <= end)
                                                        *str = ' ';
                                                ++str;
                                        }
                                }
                                for (i = 0; i < len; ++i) {
                                        if (str <= end)
                                                *str = *s;
                                        ++str; ++s;
                                }
                                while (len < field_width--) {
                                        if (str <= end)
                                                *str = ' ';
                                        ++str;
                                }
                                continue;

                        case 'p':
                                if (field_width == -1) {
                                        field_width = 2*sizeof(void *);
                                        flags |= ZEROPAD;
                                }
                                str = ck_number(str, end,
                                                (unsigned long) ck_va_arg(args, void *),
                                                16, field_width, precision, flags);
                                continue;


                        case 'n':
                                /* FIXME:
                                * What does C99 say about the overflow case here? */
                                if (qualifier == 'l') {
                                        long * ip = ck_va_arg(args, long *);
                                        *ip = (str - buf);
                                } else if (qualifier == 'Z') {
                                        ck_size_t * ip = ck_va_arg(args, ck_size_t *);
                                        *ip = (str - buf);
                                } else {
                                        int * ip = ck_va_arg(args, int *);
                                        *ip = (str - buf);
                                }
                                continue;

                        case '%':
                                if (str <= end)
                                        *str = '%';
                                ++str;
                                continue;

                                /* integer number formats - set up the flags and "break" */
                        case 'o':
                                base = 8;
                                break;

                        case 'X':
                                flags |= LARGE;
                        case 'x':
                                base = 16;
                                break;

                        case 'd':
                        case 'i':
                                flags |= SIGN;
                        case 'u':
                                break;

                        default:
                                if (str <= end)
                                        *str = '%';
                                ++str;
                                if (*fmt) {
                                        if (str <= end)
                                                *str = *fmt;
                                        ++str;
                                } else {
                                        --fmt;
                                }
                                continue;
                }
                if (qualifier == 'L')
                        num = ck_va_arg(args, long);
                else if (qualifier == 'l') {
                        num = ck_va_arg(args, unsigned long);
                        if (flags & SIGN)
                                num = (signed long) num;
                } else if (qualifier == 'Z') {
                        num = ck_va_arg(args, ck_size_t);
                } else if (qualifier == 'h') {
                        num = (unsigned short) ck_va_arg(args, int);
                        if (flags & SIGN)
                                num = (signed short) num;
                } else {
                        num = ck_va_arg(args, unsigned int);
                        if (flags & SIGN)
                                num = (signed int) num;
                }
                str = ck_number(str, end, num, base,
                                field_width, precision, flags);
        }
        if (str <= end)
                *str = '\0';
        else if (size > 0)
                /* don't write out a null byte if the buf size is zero */
                *end = '\0';
        /* the trailing null byte doesn't count towards the total
        * ++str;
        */
        return str-buf;
}

static char *
ck_number (char *buf, char *end, long num, int base, int size,
	int precision, int type)
{
  char c, sign, tmp[66];
  const char *digits;
  const char small_digits[] = "0123456789abcdefghijklmnopqrstuvwxyz";
  const char large_digits[] = "0123456789ABCDEFGHIJKLMNOPQRSTUVWXYZ";
  int i;

  digits = (type & LARGE) ? large_digits : small_digits;
  if (type & LEFT)
    type &= ~ZEROPAD;
  if (base < 2 || base > 36)
    return 0;
  c = (type & ZEROPAD) ? '0' : ' ';
  sign = 0;
  if (type & SIGN)
    {
      if (num < 0)
	{
	  sign = '-';
	  num = -num;
	  size--;
	}
      else if (type & PLUS)
	{
	  sign = '+';
	  size--;
	}
      else if (type & SPACE)
	{
	  sign = ' ';
	  size--;
	}
    }
  if (type & SPECIAL)
    {
      if (base == 16)
	size -= 2;
      else if (base == 8)
	size--;
    }
  i = 0;
  if (num == 0)
    tmp[i++] = '0';
  else
    while (num != 0)
      tmp[i++] = digits[ck_do_div (num, base)];
  if (i > precision)
    precision = i;
  size -= precision;
  if (!(type & (ZEROPAD + LEFT)))
    {
      while (size-- > 0)
	{
	  if (buf <= end)
	    *buf = ' ';
	  ++buf;
	}
    }
  if (sign)
    {
      if (buf <= end)
	*buf = sign;
      ++buf;
    }
  if (type & SPECIAL)
    {
      if (base == 8)
	{
	  if (buf <= end)
	    *buf = '0';
	  ++buf;
	}
      else if (base == 16)
	{
	  if (buf <= end)
	    *buf = '0';
	  ++buf;
	  if (buf <= end)
	    *buf = digits[33];
	  ++buf;
	}
    }
  if (!(type & LEFT))
    {
      while (size-- > 0)
	{
	  if (buf <= end)
	    *buf = c;
	  ++buf;
	}
    }
  while (i < precision--)
    {
      if (buf <= end)
	*buf = '0';
      ++buf;
    }
  while (i-- > 0)
    {
      if (buf <= end)
	*buf = tmp[i];
      ++buf;
    }
  while (size-- > 0)
    {
      if (buf <= end)
	*buf = ' ';
      ++buf;
    }
  return buf;
}
