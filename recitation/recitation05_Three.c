// Compile: gcc stuff.c
// Compile for 32-bit: gcc -m32 stuff.c
// Compile to assembly: gcc -S stuff.c
#include <stdio.h>
#include <stdint.h>

int mydivide(int a, int b, int* pr) {
    *pr = a % b;
    return a / b;
}

int16_t mydivide16(int16_t a, int16_t b, int16_t* pr)
{
    *pr = a % b;
    return a / b;
}

void setbit(int bit, int* px) {
    *px |= (1 << bit);
}

void resetbit(int bit, int* px) {
    *px &= ((1 << bit) ^ -1);
}

void printbinary(int x) {
    for (int i=31; i>=0; --i) {
        printf("%d", (x >> i) & 1);
    }
    printf("\n");
}

int main() {
    unsigned char c = 0x9c;
    unsigned short s = 0x9a7b;
    unsigned int i = 0xbc8fe301;
    uint64_t l = 0xabcdef0123456789;
    uint16_t i16 = 0xA49D;
    size_t t = 25;
    
    printf("c = %02x sizeof(c)=%zd\n", c, sizeof(c));
    printf("s = %04x sizeof(s)=%zd\n", s, sizeof(s));
    printf("i16 = %04x sizeof(s)=%zd\n", i16, sizeof(i16));
    printf("i = %08x sizeof(i)=%zd\n", i, sizeof(i));
    printf("l = sizeof(l)=%zd\n", sizeof(l));
    printf("t = %016zx sizeof(t)=%zd\n", t, sizeof(t));

    int r;
    int d = mydivide(12, 5, &r);
    printf("12 / 5 = %d remainder %d\n", d, r);
    int16_t r16;
    int16_t d16 = mydivide16(12, 5, &r16);
    printf("12 / 5 = %d remainder %d\n", d16, r16);

    char* pc = 0;
    int* pint = 0;
    printf("pc = %8zx pint = %8zx\n", (size_t)pc, (size_t)pint);
    ++pc;
    ++pint;
    printf("pc = %8zx pint = %8zx\n", (size_t)pc, (size_t)pint);

    int a[5];
    a[0] = 10;
    a[1] = 11;
    a[2] = 12;
    a[3] = 13;
    a[4] = 14;

    for (int i=0; i<5; ++i) {
        printf("a[%d]=%d\n", i, i[a]); // Translates to *(i+a)
    }

    printf("3[a]=%d\n", 3[a]); // Translates to *(3+a);

    printf("\n");
    int x = 0x0800;
    printbinary(x);
    setbit(3, &x);
    printbinary(x);
    x <<= 1;
    printbinary(x);
    resetbit(12, &x);
    printbinary(x);
}
