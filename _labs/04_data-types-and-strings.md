---
title: Data Types & Strings
number: 4
layout: lab
---

## Objectives

- Become familiar with the data types included in `stdint.h`
- Become familiar with the C implementation of strings
- Gain some practice with basic debugging techniques.

## Introduction

A common mistake C programmers make is picking inccorect data types:

- types that are inconsistant across different architechtures and compilers,
- types that are too small or cannot hold the correct values,
- types that don't reflect their use case or purpose.

As a C programmer, you must weigh all these factors and more. This lab will expose you to some of the nuances of the native C data types and those included in the `stdint.h` library. Additionally, you will practice working with strings in C.

## Procedure

### Part 1: Important Components of C

In the C Programming lab, you worked with a few of C's _native_ data types, those that are implicitly defined in C without `#include`-ing any other libraries. Some things you need to consider and operations you should know are listed below.

#### Inconsistant Sizes

It is important to remember that the native integer types in C (`short`, `int`, `long`) do not have a defined length. This peculiarity can have disasterous consequences. For example, if you need a variable that expects values ranging from zero to two-million, an `int` would be just fine on a 64-bit computer (like most modern computers), but on a 32-bit computer `int` has a maximum of only around 32,000, causing bugs that are difficult to trace.

There are several methods to check the width or size of your variables. First, we can use the built-in C function `sizeof()`. This will tell you the number of **bytes** used by a type. `sizeof()` accepts a variable or a data type:

```c
int n = 2283;
printf("%d\n", sizeof(n));      // Prints the size of n in bytes
printf("%d\n", sizeof(char));   // Prints the size of a char
```

Because the sizes and signedness of the all the data types defined in `stdint.h` are defined and constant, it is **good practice** to use the integer values in `stdint.h` instead of the native C integers.  

```c
unsigned int x = 40; // Acceptable, but unreliable.
uint32_t y = 40;     // Always 32 bits.
```

#### Overflow and Underflow

All data types have a maximum and minimum value based on their width. What happens when we try to set a variable beyond these maximum or minimums?

If a variable is at the maximum value it can store, adding one will cause the carried bit to be **truncated**, meaning it is "lost", dropped by the processor. This truncation results in **Overflow** errors. In similar manner, when you drop below the minimum value, you cause an **Underflow** error. _Note: overflow and underflow can appear slightly differently if the type is signed or unsigned._

Overflow Example:

```c
// Because an uint8_t has a maximum value of 255 (and adding 1 will overflow back to 0), 
// x will always be less than 500.  Thus, this for-loop will never end.
for (uint8_t x = 0; x < 500; x++) {
    ...
}
```

Underflow Example:

```c
// Imagine a sensor that detects the number of frogs entering and leaving a small pond.
uint32_t numFrogsInPond = 0;
// a bird flying by is mistaken for a frog leaving the pond:
numfrogsInPond--; // numFrogsInPond = 0 - 1 = 4,294,967,295; underflow has occurred.
```

While using the maximum size (64 bit) would fix most overflow errors, in most cases, memory space is limited in your program. For example, With a resolution of `3840 x 2160` and 3 bytes per pixel (one `uint8_t` for each `R` `G` `B`), the approximate (uncompressed) size of a 4K image is about 24.9 MB. Using `uint64_t` instead would take up a little over 199 MB, meaning that most cameras could only take a few dozen pictures before running out of space.

Choosing your data types intentionally can be a helpful debugging tool as well. If you select data types that properly match the range of expected values, searching for overflow errors can find logical errors elsewhere in your program.

#### Order of Operations in C

The order of operations in C are shown in the following [table](https://www.tutorialspoint.com/cprogramming/c_operators_precedence.htm) (highest/first priority at the top, lowest/last priority at the bottom):

| Operation(s) | Operator | Associativity |
| -------- | -------- | ------------- |
| Postfix (function calls, array/pointer/struct access, and postfix inc/dec) | () [] -> . ++ -- | Left-to-right |
| Unary (negative, logical/binary not, prefix inc/dec, casting, address reference, sizeof) | - ! ~ ++ -- (type)* & sizeof   | Right-to-left |
| Multiplicative | * / %                                | Left-to-right |
| Additive       | + -                                  | Left-to-right |
| Shift          | << >>                                | Left-to-right |
| Relational     | < <= > >=                            | Left-to-right |
| Equality       | == !=                                | Left-to-right |
| Bitwise AND    | &                                    | Left-to-right |
| Bitwise XOR    | ^                                    | Left-to-right |
| Bitwise OR     | \|                                   | Left-to-right |
| Logical AND    | &&                                   | Left-to-right |
| Logical OR     | \|\|                                 | Left-to-right |
| Conditional    | ?:                                   | Right-to-left |
| Assignment     | = += -= *= /= %=>>= <<= &= ^= \|=    | Right-to-left |
| Comma          | ,                                    | Left-to-right |

#### Literals in C

A **literal** is any hardcoded values included in your code. These are often numbers, but can be strings, characters, or booleans as well.  

```c
double a = 125 / 3.14;                  // 125 & 3.14
uint16_t b = 0x1234;                    // 0x1234
if ( check_small_enough(a) == true )    // true
    printf("Yipee!\n");                 // "Yipee!"
```

By default, integer literals are treated as `int`s. Decimal literals are treated as `double`s. Adding a suffix to some literals allows to specify what type it should be considered. For example, `125` is an an `int` by default, but `125U` will be treated as an unsigned int. Similarly, `3.14` is assumed to be a `double` by default, but `3.14f` will be treated as a `float`.  

Integer literals can be specified in either decimal, hexadecimal (prefixed by `0x`), and binary (prefixed by `0b`).

```c
50;      // all three of these have the decimal value 50.
0x32;
0b00110010;
```

##### Sign Extension

Whenever you cast a signed integer type to another signed integer type with a larger _width_ (e.g. `char` to `int`), the data is **sign extended** to the new length. This property of 2's Complement numbers means if you set the extra bits in the casted value to match the **Most Significant Bit** (MSB) of the old value, the casted number retains its decimal value. This is best explained through visual example:

```c
int16_t a = 0xF123;     // Binary Value:                 1111000100100011 | 16 bits
int32_t b = a;          // New    Value: 11111111111111111111000100100011 | 32 bits
                        // Both retain the same value (0xF123: -3058) but have different widths
```

Because unsigned integer types are not in 2's Complement, casting is much simpler; the data is always **zero extended**:

```c
uint16_t c = 0xF123U;   // Binary Value:                 1111000100100011 | 16 bits
int32_t d = c;          // New    Value: 00000000000000001111000100100011 | 32 bits
                        // Both retain the same value (0xF123: 61731) but have different widths
```

##### Truncation

**Truncation** is the opposite effect of Sign Extension - casting from a larger integer type to a smaller type (either signed or unsigned). When values are truncated, any of the extra bits from the original value are "chopped off" or thrown away by the processor. Anytime that your variable stores a value larger than the maximum of the type it is truncated to, you will lose data:

```c
int16_t a = 0xF1F3;     // Binary: 1111000111110011 Decimal: -3597 (0xF123)
int8_t b = a;           // Binary:         11110011 Decimal: -13   (0xF3)
```

It is also possible to cast between the floating point types `double` and `float`. When this happens, the number of bits devoted to the exponent and mantissa fields changes. When converting from `float` to `double`, the number of possible values that can be represented increases - meaning that precision is increased. When going the other way though, precision is lost - meaning we may have to round the value due to the fact that the new type can't represent all of the same values. When this happens, the value of the `double` is converted to the closest possible number that can be represented by a `float`.

##### Integer Promotion

Most of the examples above are implicit casts that occur because we are assigning one type to a new variable of a different type. **It is important to remember that implicit casting also occurs anytime you use two different types in an expression or operation**.

Compilers try to protect against casting and overflow errors using **Integer Promotion**. Anytime an integer type _smaller_ than `int` is used in an expression it is "upgraded" to either an `int` or `unsigned int`, depending on the size needed, and then the computation is carried out.

Additionally, when **types are mixed in an operation**, the type of individual values are often cast to the larger (or more expressive) type _before the operation is performed_. As explained above, this is primarily due to the fact that the hardware calculation must deal with data of the same type.

The following rules are used to describe how this casting will occur (in order of priority):

1. If one operand is a `double`, then the other operand (integer or floating point) **will always** be cast to `double`.
2. If one operand is a `float` and the other operand **is an integer**, then the integer will be cast to `float`.
3. If **both** operands are integers, then both undergo **integer promotion** as described above, then:
  a. If both operands are either signed or unsigned, then the smaller type is converted to the larger one.
  b. If the unsigned operand is larger (or if both are the same size), then the signed operand is converted to the unsigned type.
  c. If the unsigned operand is smaller and the signed type can represent all possible values of the unsigned type, then the unsigned operand is converted to the signed type.
  d. If the unsigned operand is smaller and the signed type can't represent all possible values of the unsigned, then both operands are converted to an unsigned of the same length as the signed operand.

More details on this process are available [here](https://en.cppreference.com/w/c/language/conversion).

In all of these cases, after the operation is performed, if the result of the calculation is assigned to a variable of another type, then an additional cast is carried out before storing the result.

#### Characters and Strings

You have probably noticed by now that C does not include a string data type. Instead, individual letters are represented with the `char` data type, and "strings" can be made with arrays of characters. Each decimal value from  0 to 127 is mapped to a specific character per the ASCII (American Standard Code for Information Interchange) standard. For example, the character `A` is represented by the decimal value 65. This is different from the character `a`, which is represented by the decimal value 97. You can find this standard by simply googling "ASCII Table" or by using a website like [ascii-code.com](https://www.ascii-code.com/).

To help you with these manipulations, the `string.h` library includes various string manipulation functions.  Some of these include:

- `strlen()`, which returns the length of a string (not including the null terminator)
- `sprintf()`, which works with the same formatting specifiers as `printf()`, but instead of printing to the terminal, it "prints" to an array or memory location.
- `strcpy()`, which allows you to copy characters from one string into another.
- `strncat()`, which allows you to concatenate the first `n` bytes of string A to the end of string B.
- `strncmp()`, which allows you to compare `n` bytes of string A and B.

Remember that all strings must end with a **null terminator**, `\0`, which tells the computer where the string ends. Missing a null terminator can cause many different kinds of bugs where extra memory is read, or the program gives a segmentation fault.

##### Using Functions with Arrays and Strings

When a function returns, the memory associated with it is released and can be used for other things. As with all variables, this means that an array declared locally in a function "disappears" after the function returns. This causes big problems if somewhere else in the code is still trying to access that memory. Arrays (and strings) that are needed after a function returns **should not** be declared or defined within that function.

Instead, pass a pointer to the array as a parameter function that uses it. Since an array's **base**, or name, is basically a pointer under the hood, passing arrays around like this means changes made to the array by a function will persist, even after the function is done.

```c
// Bad function; the array data will be lost once the function returns
char * makeAString_bad() {
    char localStr[] = "I made a string!"; 
    return localStr;
}
```

```c
// Note that "stringToFill" must be large enough to fit the string to prevent unexpected behavior.
void makeAString_good(char* stringToFill) {
    stringToFill = "I made a string!";
}
```

### Part 2: Debugging

#### Intentional and Incremental Programming

Intentional and incremental programming are two important mindsets to have while diving into software development. Intentional programming is a mindset that prioritizes writing code that is **clear, concise, and easy to understand**. In other words, the emphasis is on writing code that accurately reflects your intentions rather than trying whatever and seeing if it sticks. Approaching a solution in code intentionally will inevitably be a lot more productive and easier to maintain and extend in the future.

Incremental programming, on the other hand, involves **dividing the solution you are trying to create into small, manageable chunks** that can be developed and tested individually. This allows for a more flexible, iterative approach to software development. This means that changes can be made easily and quickly, without having to completely overhaul the entire solution when something crashes or changes.

Handling large amounts of data in C is no small task (as you will see in the next lab). It requires attention to detail and a good understanding of the code you are writing and what it does. As the projects you do become more complex and involved, it is important to remember that **the best line of defense against buggy code is a good offense: intentional and incremental programming.**

#### Trace debugging

Trace debugging is a technique used to identify the root cause of a problem in a program by logging the values of variables and other information at specific points in time. This trace data can provide insight into how your program is actually behaving.

One form of trace debugging is performing manual checks on data by adding `printf()` statements to a program. Printing out the value of a buggy variable every time it gets modified can help you track the changes and note where the algorithm goes wrong.

```c
// Trace debug print statement (the original string value)
printf("Original Str:\t%s\n", original_str);    
// Some function that doesn't work or may be buggy
modify_string(original_str); 
// Trace debug print statement (the modified string value)
printf("Modified Str:\t%s\n", original_str);    

```

You'll be surprised to see how effective logging or printing variable values can go to ensure a smooth development experience. Trace debugging helps identify and fix bugs, improve the performance of your program, and improve the overall reliability of your software.

#### Using a Logging Library

Using regular `printf` statements to trace debug has some problems. If you have a lot of print statements in your code, it can be difficult to keep track of them all, and when you are done debugging, you have to go back and remove each one.

Logging libraries give you control over when the log messages are written to the console, allowing you to filter out messages based on their severity or other criteria. They also contain many helpful features for tracing messages back to a line of code. One such library is [log.c](https://github.com/rxi/log.c). This library provides a simple logging interface that you can use to log messages to the console. We have provided the `log.c` and `log.h` files in the lab repository. To use this library, you will need to include the `log.h` header file at the top of your code:

```c
#include "log.h"
```

and link the `log.c` file when you compile your program:

```sh
gcc -Werror main.c log.c
```

To use the logging library, you can call any of the `log_` functions to log messages to the console. For example, you can use the `log_info` function to log an informational message:

```c
log_info("This is an informational message");
```

It will output something like this:

```sh
11:56:31 INFO  main.c:9: This is an informational message
```

You can also use the `log_debug`, `log_warn`, and `log_error` functions to log debug, warning, and error messages, respectively. Any `log_` function takes the same arguments as `printf` (and adds a newline for you!):

```c
int value = 42;
log_info("The value is %d", value);
```

### Part 3: Lab Requirements

In your repository, you will find files called `data.c` and `custom_strings.c`, along with their equivalent `.h` files. These files include various functions that you will either need to write or debug.  Descriptions for each function can be found in the `.h` files.

As you are working on these functions, please respect the following restrictions:

1. Do not change data types of parameters or return values
    unless otherwise stated.
2. If you need to change a data type to a floating-point
    number, then use `double`, not `float`.  The autograder
    uses `doubles`, not `floats`.
3. You can use `printf()` statments for debugging `data.c` and
    `custom_strings.c`, but comment them out or remove them
    before you pass off with a TA.
4. Follow the specific rules in the function descriptions.

Your repository includes a `main.c` for your own use to print and debug the code.  However, for pass off, you will compile the `data.c` and `custom_strings.c` files with the `lab4_passoff.o` file.  This is a binary file that is already compiled and ready to be linked to your code.

To compile and run your program (as defined in `main.c`) you can run the following commands:

```bash
gcc -Werror main.c data.c custom_strings.c -o lab4_main
```

After compiling the program, **check that your program has no warnings or errors**, then run:

```bash
./lab4_main
```

To compile and run the pass off script, you can run:

```bash
gcc -Werror lab4_passoff.o data.c custom_strings.c -o passoff
```

After compiling the program, run:

```bash
./passoff
```

The gcc command above will compile your `.c` files and then link them together with the `lab4_passoff.o` file that contains our test code.

Note that if you want to include a logging library, you will have to include that in the `gcc` commands as well.

#### Tests

Your code will be tested with the following inputs:

**SPRING 2025: You Do Not Have to Implement These Functions:**

##### Calculate Circumfrence
<!-- 
1. Radius: 1,  Output: 6.28
2. Radius: 3.14, Output: 19.729
3. Radius: 0, Output: 0.0
4. Radius: -10, Output: -1.0 -->

##### Calculate Average

<!-- 1. Array: {255, 255, 255, 255, 255}, Output: 255.0
2. Array: {10, 11, 13, 15, 2}, Output: 10.2
3. Array: {100, 91, 127, 23, 8}, Output: 69.8 -->

##### Any Bit Is One

<!-- 1. Input: 0x00000000, Output: 0 (false)
2. Input: 0xDEADBEEF, Output: 1 (true)
3. Input: 0xFFFFFFFF, Output: 1 (true) -->

##### Any Bit is Zero

<!-- 1. Input: 0x00000000, Output: 1 (true)
2. Input: 0xDEADBEEF, Output: 1 (true)
3. Input: 0xFFFFFFFF, Output: 0 (true) -->

##### Extract From Word

<!-- Each of the following words will be tested for each position (0-3):

1. 0xDEADBEEF
2. 0x01234567
3. 0X00FEED08 -->

**SPRING 2025: But You Do Have to Implement These Functions:**

##### Multiply By Base 2

1. Input: 1 (num) and 0 (power), Output: 1
2. Input: 8 and 3, Output: 64
3. Input: 25 and 2, Output: 100

##### Check if Space

1. Input: 3 (val) and 4 (maxBytes), Output: 1
2. Input: -5 and 2, Output: 0
3. Input: 10 and 64, Output: 1

##### Get String Length

1. Input: "ECEN 225 is a really great class!", Output: 33
2. Input: "Hello World!", Output: 12
3. Input: "Escape Characters: \0\n\t\f", Output: 19

##### To Upper

1. Input: "computer", Output: "COMPUTER"
2. Input: "ecen225", Output: "ECEN225"
3. Input: "Aren't frogs amazing?", Output: "AREN'T FROGS AMAZING?"

##### Find Last Char

1. Input: 'd', "Hello World", Output: 10
2. Input: '2', "ECEN 224", Output: 6
3. Input: '.', "data.h", Output: 4
4. Input: '?', "I have no questions", Output: -1

##### Get File Extension

1. Input: "data.c", Output: "c"
2. Input: "tux.bmp", Output: "bmp"
3. Input: "assemblyFile.asm", Output: "asm"

##### Combine Path

1. Inputs: "documents" and "resume.pdf", Output: "documents/resume.pdf"
2. Inputs: "viewer" and "tux.bmp", Output: "viewer/tux.bmp"
3. Inputs: "ecen224" and "README.md", Output: "ecen224/README.md"

## Lab Submission

- Your program must compile without warnings or errors. Compile your program with the `-Werror` flag to ensure that it doesn't.
- Pass off to a TA:
  - Show the output of the passoff executable
  - Show how you implemented your functions in `data.c` and `custom_strings.c`
- Take the Pass off Quiz on Learning Suite.
- Follow the instructions in the `submission.md` file in the repository to update your README file with what you did in this lab.

## Explore More

- [Data Types in C](https://www.tutorialspoint.com/cprogramming/c_data_types.htm)
- [Automatic Conversions in C](https://en.cppreference.com/w/c/language/conversion)
- [C Arrays and Their Storage in Memory](https://www.geeksforgeeks.org/c-arrays/?ref=shm)
- [Functions in String.h](https://cplusplus.com/reference/cstring/) (note that this library is technically for C++, but the C library is included in that language)
