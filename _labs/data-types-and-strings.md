---
title: data-types-and-strings
number: 4
layout: lab
---

## Objectives

- Become familiar with the data types included in `stdint.h`
- Become familiar with c-style strings
- Gain some practice with debugging

## Getting Started

Use the GitHub Classroom link posted in the Learning Suite for the lab to accept the assignment. Next, ssh into your Raspberry Pi using VSCode and clone the repository in your home directory. **This lab should be done in VSCode on your Raspberry Pi.**

## Overview

C is a strongly typed language, which means that the type of all variables, functions, and parameters need to be explicitly stated in code.  In the C Programming lab, you worked with a few of these data types.  In this lab, you will be fixing bugs relating to misuing different data types.  Additionally, you will learn about how strings are implemented in c, and how you can manipulate these strings

### Data Types

We already discussed some of C's data types in the last lab.  Recall that there are several different types of data types natively in C, including `int` and `float`.  Some libraries like `stdint.h` also give you access to other data types, like `uint32_t` or `bool` (yes, bool is not actually a primative data type; it's not built into C by default!).  You can even create your own data types with [typedef](https://en.wikipedia.org/wiki/Typedef), though we won't make you do that in this class. By convention, data types that are not native to C are suffixed with `_t`.

As you learned last week as you read the documentation for `stdint.h`, there are advantages to using different data types. For example, if you are making a program that includes a variable `num_students` in the class, you wouldn't need that variable to be negative.  So, you can use a `uint8_t` to limit the values to positive numbers.  Some things to consider when deciding that type an interger variable include:

- Does the variable ever need to be negative?
- How much memory should the variable take up?
- What are the upper and lower limits of values of the variable?

### Arrays

Arrays are C's method of keeping a list of some sort of data type. Nearly every other data structure in C derieves from an array.

Example is an example of how to declare an array.
```c
int myArray1[] = {0, 1, 2, 3, 4}; // This fills the content of the array with the values you provide.  
                                  // The array will only be as large as the number of elements you include.

int myArray2[5] = {0, 1}; // This fills indexes 0 and 1 with 0 and 1.  Since the size was explicited declared, 
                          // indexes 2-4 will be initalized to 0.

int myArray3[5] = {}; // This array will be fully initialized to 0.

int myArray4[5]; // This is a valid array, but the values are not initialized.
```

Once you have an array, you can access it with the bracket operator.  You'll also learn in the lectures that arrays can be accessed using pointer math.

Iterating over an array is a common practice in C; you will find yourself doing it many times in this class.  Below is an example of printing each value of an array.

```C
#define ARRAY_LEN 5

... 

int myArray[ARRAY_LEN] = {0, 1, 2, 3, 4};

for (int i = 0; i < ARRAY_LEN; i++) {
    printf("%d\n", array[i]);
}

```

You'll also see arrays with multiple dimensions.  You can think of these arrays like a table or grid.

### Characters and Strings

You may have noticed by now that C doesn't include a string data type.  In C, letters are represented with the `char` data type. Each value 0-127 is mapped to a character per the ASCII (American Standard Code for Information Interchange) standard.  For example, the character 'A' is represented by the decimal value 65. You can find this standard by simply googling "ASCII Table" or by using a website like [ascii-code.com](https://www.ascii-code.com/). 

Charaters in C are initalized with single quotes or with their numeric equivalent
```C
// myChar1 and myChar2 will hold the same value
char myChar1 = 'A';
char myChar2 = 65;
```

Strings are arrays of `char`s.  Thus, you could create a string the same way you would an array.  However, it is more common (and far easier) to use double quotes.
```C
// myString1 and myString2 are equivalent
char myString1[] = {'H', 'e', 'l', 'l', 'o', ' ', 'w', 'o', 'r', 'l', 'd'};
char myString2[] = "Hello world"; 
```

All strings in C end with a null terminator, which has the interger value 0. When C library functions examine or maniuplate a string, it will stop reading when it hits a 0.  When you initalize a string with double quotes, C will automatically add the null terminator on the end (assuming there is enough memory allocated).

```C
// The printf statement below will only print "Hello"
char myString[] = {'H', 'e', 'l', 'l', 'o', 0, 'w', 'o', 'r', 'l', 'd'};
printf("%s\n", myString);
```

Since strings are simple arrays of chars, you can iterate over them the same way you would an array.  The code example below replaces every `l` in the string with `L`.

```C
#define STRING_LENGTH 64
...
char hello[STRING_LENGTH] = "Hello world!";
for (int i = 0; i < STRING_LENGTH; i++) {
    if (hello[i] == 0) {
        // Hit the null terminator, so we end the loop
        break;
    } else if (hello[i] == 'l') {
        hello[i] = 'L';
    }
}
// hello is now "HeLLo worLd!"
```

The `string.h` library includes various string manipulation functions.  One of these is `strlen()`, which returns the length of a string up until its null terminator.  You can also combine strings using the `sprintf()` function. `sprintf()` works with the same formatting specifiers as `printf()`, but instead of printing to the terminal, it creates a string.

### Using Functions with Arrays and Strings

When they are declared, arrays are stored in its local function's memory.  When that function ends, the data with the array is will be cleared from memory.  For this reason, functions that manipulate or generate an array usually don't return the array itself.  Instead, these functions require the user to pass in the array as a parameter.  Since arrays are pointers under the hood, changes made in the function will persist once the function is done. 
```C
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
