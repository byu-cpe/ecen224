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

In the C Programming lab, you worked with a few of C's native data types, meaning the data types that are in C without `#include`-ing any other libraries.  However, as you have also learned, these data types also have some nuances you need to consider when programing.

In this lab, you will gain more exposure to the nuances of the data types native to C and included in the `stdint.h` library.  Additionally, you will practice manipulating strings.  By the end of this lab, you will have written and/or debugged several functions relating to C's data types and strings.

### Data Types

We already discussed some of C's data types in the last lab.  Recall that there are several different data types native to C, including `int` and `float`.  It is important to note that the native intergers in C (namely `short`, `int`, and `long`) do not have a defined length; C allows each computer manfacturer to decide how many bits an `int` will take in memory.

This peculariarity has had disasterous consequences. Say for example, you expect that your `int` will have values ranging from zero to two-million.  On a 64-bit computer (like most modern computers), using an `int` will be just fine.  However, on a 32-bit or 16-bit computer, you will likely find that your variable is cut off around 32,000.  At that point, your variable will **overflow**.

**Overflow** is when you increment (or decrement) a variable beyond its maximum (or minimum) limit.  This can cause some pernitious bugs.  Consider the examples below:

```C
// Because an unsigned char has a maximum value of 255, i will always be less than 500.  This for loop will never end.
for (unsigned char i = 0; i < 500; i++) {
    ...
}
```
```C
// Underflow example; trying to subtract 1 from an unsigned int = 0 will not go to -1

```

// TODO: Casting (I and E) and examples

Some libraries like `stdint.h` give you access to additional data types, like `uint32_t` or `bool` (yes, bool is not actually a primative data type, meaning it's not built into C by default!).  You can even create your own data types with [typedef](https://en.wikipedia.org/wiki/Typedef), though we won't cover that in this class. By convention, data types that are not native to C are suffixed with `_t` to show they are a type.

As you learned last week as you read the documentation for `stdint.h`, there are advantages to using different data types. For example, if you were making a program that included a variable `num_students`, you probably would never need that variable to be negative. So, by using a `uint8_t` instead of a `char`, you get an extra bit to use (the sign bit), which doubles the maximum useful number you can hold. Some things to consider when deciding that type an interger variable include:

- Does the variable ever need to be negative?
- How much memory should the variable take up?
- What are the upper and lower limits of values of the variable?

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

While higher dimensional arrays are rare and harder to understand intuitively, they are useful becuase arrays are always stored in **contiguous** memory, meaning that the elements must go in order without any gaps or spacing in between. This grouping allows maximum packing efficiency when storing huge quantities of data.

### Characters and Strings

You have probably noticed by now that C does not include a string data type.  Instead, individual letters are represented with the `char` data type. Each decimal value from  0 to 127 is mapped to a specific character per the ASCII (American Standard Code for Information Interchange) standard.  For example, the character `A` is represented by the decimal value 65. This is different from the character `a`, which is represented by the decimal value 97. You can find this standard by simply googling "ASCII Table" or by using a website like [ascii-code.com](https://www.ascii-code.com/). 

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
- `memcpy()`, which allows you to copy bytes from location A to location B.
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

In your repository, you will find files called `data.c` and `custom_strings.c`, along with their equivalent `.h` files.  These files include buggy code that you need to fix.

Additionally, you will write the following string manipulation functions:
1. `getFileExtension`: a function that takes the name of a file and returns its extension.  For example, passing in `rickroll.gif` would return `gif`.
2. `combinePath`: a function that takes a folder name and a file name and combines them together.  For example, passing in the folder `foo` and the file `bar.txt` would return `foo/bar.txt`.

Write these functions well; you will copy them into future labs.

## Submission

- Complete the Learning Suite pass off quiz.

- To successfully submit your lab, you will need to follow the instructions in the [Lab Setup]({{ site.baseurl }}/lab-setup) page, especially the **Committing and Pushing Files** section.

- To pass off to a TA:
    - Show the output of the functions you fixed using the test cases in main.c.
    - Show the output of the `getFileExtension` function using three different example file names.
    - Show the output of the `combinePath` function using three different folder and file names.


## Explore More!
- [Functions in String.h](https://cplusplus.com/reference/cstring/) (note that this library is technically for C++, but the C library is included in that language)
