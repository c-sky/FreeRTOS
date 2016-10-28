#ifndef CK_PRINTK_H
#define CK_PRINTK_H


typedef char *ck_va_list;
typedef unsigned int CK_NATIVE_INT;
typedef unsigned int ck_size_t;



#define ZEROPAD 1               /* pad with zero */
#define SIGN    2               /* unsigned/signed long */
#define PLUS    4               /* show plus */
#define SPACE   8               /* space if plus */
#define LEFT    16              /* left justified */
#define SPECIAL 32              /* 0x */
#define LARGE   64              /* use 'ABCDEF' instead of 'abcdef' */
#define ck_isdigit(c) (c >= '0' && c <= '9')
#define ck_do_div(n,base) \
    ({ \
        long __res; \
        __res = ((unsigned long) n) % (unsigned) base; \
        n = ((unsigned long) n) / (unsigned) base; \
        __res; \
   })


/* printk.c */
int printk(const char *fmt, ...);
int ck_vsnprintf(char *buf, ck_size_t size, const char *fmt, ck_va_list args);
//extern char printk_buf[256];
#endif
