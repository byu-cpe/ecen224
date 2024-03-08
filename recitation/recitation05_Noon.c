#include <stdio.h>
#include <stdint.h>

int mydivide(int a, int b, int *p) {
    *p = a % b;
    return a / b;
}

/*
void printbinary(short x) {
    for (int i=15; i>=0; --i) {
        printf("%d", (x >> i) & 1);
    }
    printf("\n");
}*/

void printbinary(short x) {
    for (int i=0; i<16; ++i) {
        printf("%c", (x & 0x8000) ? '1' : '0');
        x <<= 1;
    }
    printf("\n");
}

void printstringln(char *p) {
    while (*p != (char)0) {
        fwrite(p, 1, 1, stdout);
        p++;
    }
    fwrite("\n", 1, 1, stdout);
}

struct thing {
    int ivalue;
    char cvalue;
};
typedef struct thing thg;

int main() {
    unsigned char c = 0x9c;
    unsigned short s = 0x9a7b;
    unsigned int i = 0xbc8fe301;
    unsigned long l = 0xabcdef0123456789;
    uint16_t i16 = 0xA49D;
    size_t t = 25;
    
    /*
    printf("c = %02x sizeof(c)=%ld\n", c, sizeof(c));
    printf("s = %04x sizeof(s)=%ld\n", s, sizeof(s));
    printf("i16 = %04x sizeof(s)=%ld\n", i16, sizeof(i16));
    printf("i = %08x sizeof(i)=%ld\n", i, sizeof(i));
    printf("l = %016lx sizeof(l)=%ld\n", l, sizeof(l));
    printf("t = %016lx sizeof(t)=%ld\n", t, sizeof(t));
    */

    int r;
    int q = mydivide(12, 7, &r);
    printf("q = %d, r = %d\n", q, r); 

    short a = 0x0002;
    short b = 0x0102;
    printbinary(a);
    printbinary(b);
    printbinary(a | b);
    printbinary(a & b);
    printbinary(a << 2);
    a <<= 2;
    printbinary(a);

    printstringln("Hello, world!");

    char *pchar = 0;
    //*pchar = 'Q'; // Cause seg fault by writing to null pointer.
    printf("pchar = %lx\n", (size_t)pchar);
    pchar++;
    printf("pchar = %lx\n", (size_t)pchar);

    int *pint = 0;
    printf("pint = %lx\n", (size_t)pint);
    pint++;
    printf("pint = %lx\n", (size_t)pint);

    struct thing th;
    th.ivalue = 42;
    th.cvalue = 'B';

    thg *pt;
    pt = &th;
    printf("%d\n", pt->ivalue);
}