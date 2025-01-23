---
title: Data Types & Strings
number: 4
layout: lab
---

## Objectives

- Become familiar with the data types included in `stdint.h`
- Become familiar with the C implementation of strings
- Gain some practice with basic debugging techniques.

## Getting Started

Use the GitHub Classroom link posted in the Learning Suite for the lab to accept the assignment. Next, ssh into your Raspberry Pi using VSCode and clone the repository in your home directory. **This lab should be done in VSCode on your Raspberry Pi.**

## Overview

C is a [strongly typed language](https://en.wikipedia.org/wiki/Strong_and_weak_typing), meaning that the _type_ of all variables, functions, and parameters need to be explicitly stated in code. On one hand, this means that your code will be more verbose.  On the other hand, this give you great control over your variables' sizes and inputs.

Since C is strongly typed, it is essential that you understand why different data types are used.  In C, it is easy to pick types that are inconsistant accross different computers, types that are too small, or types that don't reflect your purpose. As a C programmer, you must weigh these factors and more.

In this lab, you will gain more exposure to the nuances of the data types native to C and included in the `stdint.h` library.  Additionally, you will practice manipulating strings.  By the end of this lab, you will have written and/or debugged several functions relating to C's data types and strings.

## Data Types

In the C Programming lab, you worked with a few of C's native data types, meaning the data types that are in C without `#include`-ing any other libraries.  However, as you have also learned, these data types also have some nuances you need to consider when programing. Below are some of the considerations and operations you need to know.

### Inconsistant sizes

We already discussed some of C's data types in the last lab.  Recall that there are several different data types native to C, including `int` and `float`.  It is important to note that the native intergers in C (namely `short`, `int`, and `long`) do not have a defined length; C allows each computer manfacturer to decide how many bits an `int` will take in memory.

This peculariarity has had disasterous consequences. Say for example, you expect that your `int` will have values ranging from zero to two-million.  On a 64-bit computer (like most modern computers), using an `int` will be just fine.  However, on a 32-bit or 16-bit computer, you will likely find that your variable is cut off around 32,000. 

To be sure that you know what size your variables will take up, you can use another library that defines additional datatypes.  For example, the `stdlib.h` library adds `size_t`, which represents the maximum unsigned interger allowed on your system, or `stdbool.h`, which adds `bool.h` (bool is not actually a primative data type, meaning it's not built into C by default!). You can even create your own data types with [typedef](https://en.wikipedia.org/wiki/Typedef), though we won't cover that extensively in this class. By convention, data types that are not native to C are suffixed with `_t` to show they are a type.

One of the most common libraries you'll see is `stdint.h`, which as you learned last week, includes defintions for several intergers.  Because the sizes and signedness of the those intergers are defined and constant, it is **good practice** to use the interger values in `stdint.h` instead of the native C intergers.  

```C
unsigned int x = 40; // Acceptable, but unreliable
uint32_t y = 40; // You know that y will always have 32 bytes.
```

### Overflow and Underflow

We've established by this point that different types have a maximum and minimum value.  What happens when we try to set a variable beyond these maximum or minimums?

The answer is called **Overflow** and **Underflow** for exceeding the maximum and minimum respectively.  The examples below cover overflow, but underflow works exactly the same (in the opposite direction).

See, when you're doing math conventionally, if you run out of digits, you would simply add another digit. However, computers have limited bits for representing numbers.  If it is at the maximum value, adding one will simply cause the carried over number to be truncated.
```C
// Conventional Math
9999 + 1 = 10000 
// 9999 has 4 digits and contains the maximum value for a 4 digit number.  When we add 1, we need to add a 5th digit, 1

// Limited digit math
0b11111111 + 1 = 0 
// 0b11111111 = 255 is the maximum value for a uint8_t. Adding 1 will cause overflow, and the expression evaluates to 0.
```

Overflow and underflow have very real impacts on code written in C:

```C
// Because an unsigned char has a maximum value of 255, x will always be less than 500.  This for loop will never end.
// In this scenario, it would be better to use a larger data type.
for (unint8_t x = 0; x < 500; x++) {
    ...
}
```
```C
// Imagine a sensor that detects the number of frogs entering and leaving a small pond.
uint32_t numFrogsInPond = 0;
// A frog jumps in the pond, so we go up by one.
numFrogsInPond++; // numFrogsInPond = 1
// The sensor sees the frog leave, but it glitched and double counted the frog leaving.
numfrogsInPond -= 2 // numFrogsInPond = 1-2 = 4,294,967,295
// 4.29 billion frogs is simply too many frogs in a small pond.  Your frog pond management system breaks and everyone is sad.
```

Seeing these errors with overflow and underflow, you may be inclined to simply use the largest data type whenever possible.  However, you should realize that if you use large data types when you don't need to, you will likely run out of memory.

For example, there are currently 124 students in ECEN 225 this semester.  If we were to write a program that counts the number of students in the class, a uint8_t would suffice just fine.  You just need to think about what the possible ranges of your variable will be.

### Casting

Sometimes, you will have a variable that is the incorrect type for the operation you want to accomplish.  In this case, we can *cast* your variable, or temporarily change its datatype.

For example, lets say you wanted to divide two intergers to get a decimal.  In C, dividing an interger by an interger will result in an interger.  However, if you explictly say you want a float, you'll get a float.

To explicitly cast a variable, use the notation `([desired type])` in front of your variable:
```C
5 / 2; // This will evaluate to 2
(float) 5 / 2; // This will evaluate to 2.5
```
Casting can also go in reverse:
```C
// Imagine you have a program where you want to print out a message for each dollar in your bank account
// For loops conventionally only work with ints, so you can cast your balance to an int

double balanceInTheBank = 2.53;
int32_t balanceAsInt = (int32_t) balanceInTheBank;
for (int32_t i = 0; i < balanceAsInt; i++) {
    printf("Another dollar!\n");
}
```

Casting also works between different interger types.  It is fairly common to cast small intergers up to larger intergets, unsigned to signed, etc.  Be warned, however, that the ranging still applies.  If you try to cast a large interger down to a smaller size, you can overflow.
```C
uint32_t largeNumber = 300;
uint8_t  smallNumber = (uint8_t) largeNumber; // Small number will overflow since 300 is beyond a uint8_t's range
```

Interger casting under the hood is fairly simple.  When going between an unsigned in and a signed int of the same size, no bits chamge.  The binary is simply interpreted differently.  When casting a small signed interger to a higher size, the most significant bit (MSB) is copied into the new bytes.  Unsigned ints will simply get zeros in the new bytes.  When casting from a large int to a small int, the value will be truncated (that's what causes the overflow).

```C
// Casting a small signed to a larger signed
int8_t smallSigned = 0x80; // 1000 0000 = -128
int16_t largerSigned = (int16_t) smallSigned; // The sign extended, 0xFF80 = 1111 1111 1000 0000 = -128

// Casting a signed to an unsigned
uint8_t smallUnsigned = (uint8_t) smallSigned; // 1000 0000 = 128.  smallUnsigned and smallSigned have the same binary value, they are just interpreted differently

// Casting from a large signed to a small signed
largeSigned = 0x0180; // 0000 0001 1000 0000 = 384
smallSigned = (int8_t) largeSigned; // The binary is truncated from 0x0180 -> 0x80.  smallSigned now equals -128.
```

Casting also happens *implicitly*, meaning that your code can take liberties to cast a variable automatically.  This actually happens a lot between different interger types.  For example, the `%d` format specifier for `printf()` actually looks for a 32-bit interger.  If you pass an 8 or 16 bit interger in, your code will bump up the size automatically.

Implicit casting can be more problematic when you are using intergers and floats, or unsigned and signed intergers.  In these scenarios:
1. Intergers in an expression with a `double` or a `float` will be cast to `double` or `float`.
2. Signed intergers in an expression with unsigned intergers will be cast to unsigned
```C
uint8_t myUnsignedInt = 10;
int8_t  mySignedInt   = -5;
double   myDouble      = 2.5;

// This is 10 / 2.5.  The 10 will be implicity cast to 10.0, so this expression evaluates to 4.
myUnsignedInt / myDouble; 

// This is -5 - 10.  Intuitivly, we would say this is equal to -15.  However, the signed int is cast instead. 
// This actually evaluates to 250 - 10 = 240.
mySignedInt - myUnsignedInt; 
```

To fix implicit casting, you can explicitly cast the expression to your desired type.  However, if you ever find yourself with an issue with implicit casting, it may be worth asking yourself if you are actually using the correct data type.

### Summary
When declaring a variable, remember to consider what its purpose will be.  Ask yourself:

- Does the variable ever need to be negative?
- How much memory should the variable take up?
- What are the upper and lower limits of values of the variable?
- Do I need a decimal number, or will an interger suffice?

The above is not an exhuastive list, but if you can answer those questions, you will likely land on a good data type to use.

### Arrays

Arrays are C's method of keeping lists. These lists can be of any data type, but any array can only be defined for a single data type. Nearly every other data structure in C derives from an array.

Here is an example of how to declare an array.

```c
#define ARRAY_LEN 5
// Syntax:
// <type> <name>[<opt. size>] = {<opt. initializers>};
int myArray1[] = {0, 1, 2, 3, 4}; // If no size is specified, the array will be as
                                  // large as the number of elements you include.

int myArray2[ARRAY_LEN] = {0, 1};   // Since the size was explicited declared, indexes 0 and 1 
                                    // get 0 and 1, repspecively, and indexes 2-4 will be initalized to 0.

int myArray3[ARRAY_LEN] = {};       // This array will be fully initialized to 0.

int myArray4[ARRAY_LEN];            // This is a valid array, but the values are not initialized.
```
Note that once an array is declared, its size is unchangable. Be careful when using unbounded (no size given) or uninitialized (no values given) arrays. Both can cause serious errors if used incorrectly!

Once you have an array, you can access it with the bracket operator:

```c
int myArray1[ARRAY_LEN] = {0, 1, 2, 3, 4};

int object = myArray[2];

printf("%d\n",object); // prints out the number "2"
```

You'll also learn in the lectures that arrays can be accessed using pointers or math operations.

Iterating over an array is a common practice in C; you will find yourself doing it many times in this class.  Below is an example of printing each value of an array.

```c
int myArray[ARRAY_LEN] = {0, 1, 2, 3, 4};

for (int i = 0; i < ARRAY_LEN; i++) {
    printf("%d\n", array[i]);
}

```

You'll also see arrays with multiple dimensions.  You can think of these arrays tables or a grids. They can have many axes.

```c
char sentences[10][50]; // an array of 10 arrays of 50 chars; in other words,
                        // 10 strings of max length 50.

uint8_t seats_in_a_building[8][10][50]; // an array representing 8 floors, with
                                        // 10 rooms on each floor, and 50 seats in each room.

uint8_t video_pixels[30*60][1280][720][3]; // an array representing 30 seconds of a 60 fps, 720p video (1280x720),
                                           // with each pixel getting 3 values for red, green, and blue.
```

While higher dimensional arrays are rare and harder to understand intuitively, they are useful becuase arrays are always stored in **contiguous** memory, meaning that the elements must go in order. To us, a 2D is represented as a table of values.  To a computer, the array is just one long list.  This grouping allows maximum packing efficiency when storing huge quantities of data.  You'll get more experience with large arrays in the next lab.

### Characters and Strings

You have probably noticed by now that C does not include a string data type.  Instead, individual letters are represented with the `char` data type. Each decimal value from  0 to 127 is mapped to a specific character per the ASCII (American Standard Code for Information Interchange) standard.  For example, the character `A` is represented by the decimal value 65. This is different from the character `a`, which is represented by the decimal value 97. You can find this standard by simply googling "ASCII Table" or by using a website like [ascii-code.com](https://www.ascii-code.com/). 

> Note that some systems also have mappings for interger values 128-255.  However, C doesn't actually specify if a `char` is equivalent to a `uint8_t` or a `int8_t`. Each computer will vary whether a char has range -128 to 127 or 0 to 255.  If you aren't using `char`s for being characters, you're better off using one of the `stdint.h` values.

Charaters in C can be initalized with single quotes or with their numeric equivalent:

```c
// All of the chars will hold the same value.
char myChar1 = 'A';
char myChar2 = 65;          // 'A' in decimal
char myChar3 = 0b01000001;  // 'A' in binary
char myChar4 = 0x41;        // 'A' in hexedecimal
```

A string is simply an array of `char`s. Thus, you could create a string the same way you would an array. However, it is more common (and far easier) to use double quotes.

```c
// myString1 and myString2 are equivalent
char myString1[] = {'H', 'e', 'l', 'l', 'o', ' ', 'w', 'o', 'r', 'l', 'd', 0};
char myString2[] = "Hello world"; 
```
Notice that when declaring the array with an initializer list, there is an extra `0` added to the end. All strings in C must end with a **null terminator**, which has the integer value 0. This tells the computer when the string is finished and no more bytes should be read. When you initalize a string with double quotes, C will automatically add the null terminator on the end (assuming there is enough memory allocated for the whole string to fit).

```c
// The printf statement below will only print "Hello"
char myString[] = {'H', 'e', 'l', 'l', 'o', 0, 'w', 'o', 'r', 'l', 'd'};
printf("%s\n", myString);
```

Since strings are just arrays, you can iterate over them and manipulate them in the same ways. The example below replaces every `l` in the string with `L`.

```c
#define MAX_STR_LEN 64

char hello[MAX_STR_LEN] = "Hello world!";

printf("%s\n", hello); // prints "Hello world!"

for (int i = 0; i < MAX_STR_LEN; i++) {
    if (hello[i] == 0) {
        // if we hit the null terminator, end the loop
        break;
    } else if (hello[i] == 'l') {
        hello[i] = 'L';
    }
}

printf("%s\n", hello); // prints "HeLLo worLd!"
```

To help you with these manipulations, the `string.h` library includes various string manipulation functions.  some of these include:

- `strlen()`, which returns the length of a string (not including the null terminator)
- `sprintf()`, which works with the same formatting specifiers as `printf()`, but instead of printing to the terminal, it "prints" to an array or memory location.
- `strcpy()`, which allows you to copy characters from one string into another.
- `strncat()`, which allows you to concatenate the first `n` bytes of string A to the end of string B.
- `strncmp()`, which allows you to compare `n` bytes of string A and B.

### Using Functions with Arrays and Strings

When arrays are first declared, they are stored in memory owned by the function that created the array (e.g. `main`. You will learn more about this later in the class). When that function ends, the array will be cleared from memory. Becuase the lifespan of an array is often very short, functions that manipulate or generate arrays usually don't return the array itself. Instead, these functions require the user to pass around pointers to the array as a parameter. Since an array's base is basically a pointer under the hood, passing arrays around like this means changes made to the array by a function will persist, even after the function is done.

```c
// Bad function; the array data will be lost once the function returns
char* makeAString_bad() {
    return "I made a string!";
}

// Good function; the caller will declare an empty array and the function will populate it.  
// Note though that the stringToFill will have to be large enough to fit the string or weird things will happen.
void makeAString_good(char* stringToFill) {
    stringToFill = "I made a string!";
}
```
### Debugging Help

You are likely already familar debugging.  As a programmer at any level, you will need to develop the identify problems in your code and fix them.  Below we have some advice on different programming and debugging methods.

#### Intentional and Incremental Programming
Intentional and incremental programming are two important mindsets to have while diving into software development. Intentional programming is a mindset that prioritizes writing code that is **clear, concise, and easy to understand**. In other words, the emphasis is on writing code that accurately reflects the your intentions rather than trying whatever and seeing if it sticks. Approaching a solution in code intentionally will inevitably be a lot more productive and easier to maintain and extend in the future. 

Incremental programming, on the other hand, involves **dividing the solution you are trying to create into small, manageable chunks** that can be developed and tested individually. This allows for a more flexible, iterative approach to software development. This means that changes can be made easily and quickly, without having to completely overhaul the entire solution when something crashes. 

Handling large amounts of data in C is no small task (as you will see in the next lab). It requires attention to detail and a good understanding of the code you are writing and what it does. As the projects in this and other programming-centric classes become more complex and involved, it is important to remember that **the best line of defense against buggy code is a good offense: intentional and incremental programming.**

#### Trace debugging
Trace debugging is a technique used to identify the root cause of a problem in a program by logging the values of variables and other information at specific points in time. This trace data can provide insight into how your program is actually behaving.

One form of trace debugging is performing manual checks on data by adding `printf()` statements to a program. Printing out the value of a buggy variable every time it gets modified can help you track the changes and note where the algorithm goes wrong.

```c
void modify_string(char * in_str){
    // Implementation of function here
    //      ...     
}

...

printf("Original Str:\t%s\n", original_str);    // <--- Trace debug print statement (sees the original string value)

modify_string(original_str); 

printf("Modified Str:\t%s\n", original_str);    // <--- Trace debug print statement (sees the modified string value)

```

And that's really all there is to this method! You'll be surprised to see how effectively simply logging/printing variable values as you go can ensure a smoother development experience. By using trace debugging, you can quickly identify and fix bugs, improve the performance of your program, and improve the overall reliability of your software.

#### Using a Logging Library
However, using just regular `printf` statements has some problems. For example, if you have a lot of print statements in your code, it can be difficult to keep track of them all. And when you are done debugging, you have to go back and remove all the print statements you added.

Rather than putting print statements everywhere, you can use a logging library to help you debug your code. Using a logging library can make it easier to log messages to the console and can provide additional features such as logging levels, timestamps, and the file location/line number where the log message occurred. Logging libraries also give you control over when the log messages are written to the console, allowing you to filter out messages based on their severity or other criteria. One such library is [log.c](https://github.com/rxi/log.c). This library provides a simple logging interface that you can use to log messages to the console. We have provided the `log.c` and `log.h` files in the lab repository. To use this library, you will need to include the `log.h` header file to the top of your code:

```c
#include "log.h"
```

 and link the `log.c` file when you compile your program:
 
 ```bash
 gcc main.c log.c
 ```

To use the logging library, you can call the `log_*` functions to log messages to the console. For example, you can use the `log_info` function to log an informational message:

```c
log_info("This is an informational message");
```

It will output something like this:

```
11:56:31 INFO  main.c:9: This is an informational message
```

You can also use the `log_debug`, `log_warn`, and `log_error` functions to log debug, warning, and error messages, respectively. `log_*` takes the same arguments as `printf` (except it adds a newline for you!), so you can use format specifiers to include variables in the log message:

```c
int value = 42;
log_info("The value is %d", value);
```

## Requirements

In your repository, you will find files called `data.c` and `custom_strings.c`, along with their equivalent `.h` files.  These files include various functions that you will either need to write or debug.  Descriptions for each function can be found in the `.h` files.

Your repository includes a `main.c` for your own use to print and debug the code.  However, for pass off, you will compile the `data.c` and `custom_strings.c` files with the `lab4_passoff.o` file.  This is a binary file that is already compiled and ready to be linked to your code.

To run the pass off script, you can run:
```bash
gcc lab4_passoff.o data.c custom_strings.c -o passoff
./passoff
``` 

### Tests
Your code will be tested with the following inputs:

#### Calculate Circumfrence
1. Radius: 1,  Output: 6.28
2. Radius: 3.14, Output: 19.729
3. Radius: 0, Output: 0.0
4. Radius: -10, Output: -1.0

#### Calculate Average
1. Array: {255, 255, 255, 255, 255}, Output: 255.0
2. Array: {10, 11, 13, 15, 2}, Output: 10.2
3. Array: {100, 91, 127, 23, 8}, Output: 69.8

#### Any Bit Is One
1. Input: 0x00000000, Output: 0 (false)
2. Input: 0xDEADBEEF, Output: 1 (true)
3. Input: 0xFFFFFFFF, Output: 1 (true)

#### Any Bit is Zero
1. Input: 0x00000000, Output: 1 (true)
2. Input: 0xDEADBEEF, Output: 1 (true)
3. Input: 0xFFFFFFFF, Output: 0 (true)

#### Extract From Word
Each of the following words will be tested for each position (0-3):
1. 0xDEADBEEF
2. 0x01234567
3. 0X00FEED08

#### Multiply By Base 2
1. Input: 1 (num) and 0 (power), Output: 1
2. Input: 8 and 3, Output: 64
3. Input: 25 and 2, Output: 100

#### Check if Space
1. Input: 3 (val) and 4 (maxBytes), Output: 1
2. Input: -5 and 2, Output: 0
3. Input: 10 and 64, Output: 1

#### Get String Length
1. Input: "ECEN 225 is a really great class!", Output: 33
2. Input: "Hello World!", Output: 12
3. Input: "Escape Characters: \0\n\t\f", Output: 19

#### To Upper
1. Input: "computer", Output: "COMPUTER"
2. Input: "ecen225", Output: "ECEN225"
3. Input: "Aren't frogs amazing?", Output: "AREN'T FROGS AMAZING?"

#### Find Last Char
1. Input: 'd', "Hello World", Output: 10
2. Input: '2', "ECEN 224", Output: 6
3. Input: '.', "data.h", Output: 4
4. Input: '?', "I have no questions", Output: -1

#### Get File Extension
1. Input: "data.c", Output: "c"
2. Input: "tux.bmp", Output: "bmp"
3. Input: "assemblyFile.asm", Output: "asm"

#### Combine Path
1. Inputs: "documents" and "resume.pdf", Output: "documents/resume.pdf"
2. Inputs: "viewer" and "tux.bmp", Output: "viewer/tux.bmp"
3. Inputs: "ecen224" and "README.md", Output: "ecen224/README.md"

## Submission

- Complete the Learning Suite pass off quiz.

- To successfully submit your lab, you will need to follow the instructions in the [Lab Setup]({{ site.baseurl }}/lab-setup) page, especially the **Committing and Pushing Files** section.

- To pass off to a TA
    - Show the output of the passoff executable
    - Show how you implemented your functions in `data.c` and `custom_strings.c`


## Explore More!
- [Functions in String.h](https://cplusplus.com/reference/cstring/) (note that this library is technically for C++, but the C library is included in that language)
