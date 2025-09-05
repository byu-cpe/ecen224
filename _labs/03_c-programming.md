---
title: C Programming
number: 3
layout: lab
---

## Objectives

- Compile a C program using `gcc`
- Understand basic C syntax
- Know the difference between the standard data types
- Experiment with `printf` and data types

## Introduction

The goal of this semester is for you to build a strong foundation in C programming. C is a very important language that is still actively used today. The creators of C, [Dennis Ritchie](https://computerhistory.org/profile/dennis-ritchie/) and [Ken Thompson](https://computerhistory.org/profile/ken-thompson/), are also the creators of Unix, the predecessor to the Linux system we are using in this class. A good understanding of how C and Unix work will provide large insight into how computers work in general.

```c
// This is a single line comment.

/* 
This is a multi-line comment.
Any text that is between the slash-star and
the star-slash will be ignored.
*/

#include <stdio.h>

int main()
{
    printf("Hello, World!\n");
    return 0;
}

```

Above is a simple [Hello World](https://en.wikipedia.org/wiki/%22Hello,_World!%22_program) program written in C. The "Hello World" program is commonly used to demonstrate the basics of a programming language.

In the folder for this lab (`starter-labs/03-c-programming/`), create a new file called `simple.c`. This can be done using the File Explorer tab in VS Code. Inside the `simple.c` file, copy and paste the code above, then save and close the file. We will use this program to explore the basics of creating, compiling, and running a C program. Read each section carefully and make sure to complete each step.

## Procedure

### Part 1: Exploring The Compilation Process

C is a [compiled language](https://www.geeksforgeeks.org/difference-between-compiled-and-interpreted-language/), meaning  that the code you write must be **compiled** into a [binary executable](https://en.wikipedia.org/wiki/Executable) before it can be executed. This compilation process is done by a program known as the **compiler**. In this class, we will use the `gcc` compiler (short for GNU Compiler Collection).

Compilers do more than simply convert a higher level language to machine language. Many compilers analyze the code being passed in and optimize it to be executed in the fewest amount of instructions possible. Let's examine the steps compilers take to build an executable version of your program.

**Follow along with the steps below to see how your `simple.c` program is compiled into an executable binary. This is a requirement for the lab.**

#### Step 1: Pre-processing

This step runs a pre-compiler program which handles special statements in your code called **directives**. Directives, which are denoted by the `#` sign, help decide which parts of a written program in C are included during the compilation process.

One of the most common directives is the `#include` directive, which "includes" an already existing file into your code. In our `simple.c` program, we have included the `<stdio.h>` library, which has several functions to assist with the "standard" inputs and outputs in your program (i.e. reading from your keyboard and writing to a terminal). This is also where the definition of the `printf()` function resides. Without including `stdio.h`, `simple.c` would not know what `printf()` was or where it was defined and thus would not be able to compile.

Some other interesting compiler directives are `#define`, `#if`/`#else`/`#endif`. Take a minute to look up what they mean for your lab quiz.

Generate a precompiled version of your `simple.c` code by running the following terminal command:

```bash
gcc -E simple.c > simple_preprocessed.txt
```

Look through the generated text file. While a lot has been added, the last few lines of the text file are your original code.

#### Step 2: Compiling

In this step, the compiler translates your pre-processed code into [assembly language](https://en.wikipedia.org/wiki/Assembly_language). Assembly is made up of **instructions** that can be executed by your computer processor. For example, all the instructions you will study in ECEn 224 use the [x86 instruction set](https://en.wikipedia.org/wiki/X86_instruction_listings). All C programs can be compiled into some set of these instructions.

Many modern compilers will also make optimizing adjustments to your code. For example, if your code includes a command to multiply an integer by four (ex: `newInt = 3 * 4;`), your compiler may instead use a binary shift operation (`newInt = 3 << 2;`  in C or `shr %rdx, 2` in assembly). These adjustments reduce the number of instructions needed, increasing the speed and efficiency of your program.

Compile your C code into assembly by running the following terminal command:

```bash
gcc -S simple.c
```

Because different processors and hardware have different instruction sets, compiled assembly code can usually only be run on the machine that compiled it. For example, the binary `simple.c` exectuable you just compiled on your doorbell uses the [ARM instruction set](https://en.wikipedia.org/wiki/ARM_architecture_family). The resulting executable would likely work on another student's doorbell, but it would not work on your `x86` lab machine.

Open the `simple.s` file you just generated and look around. Google one of the commands that you see to try and figure out what it means.

#### Step 3: Assembling

The next step is to convert the assembly code into machine code, which consists of binary that the processor can execute. This involves translating each assembly instruction into its corresponding binary **opcode** and arguments and organizing them into larger segments such as the `code` and `data` segments. These segments can then be processed by the operating system.

Run the following terminal command to generate the binary version of your code:

```bash
gcc -c simple.c
```

Next, to actually read this code, run the following:

```bash
hexdump simple.o > simple_assembled.txt
```

Look through the text file you just generated. While the actual file we generated is 0s and 1s, we used the `hexdump` command to read it as hex and make it slightly more interpretable. You should see two main sections. What do you think these mean?

#### Step 4: Linking

In the final stage of the compilation process, a program called the **linker** will find all the external references in your assembled code and combine any related `.o` files together. Let's return to the example of `#include <stdio.h>` in our "Hello World" program. While we know that the `.h` file associated with `<stdio.h>` was included in our pre-processing step, the actual implementation of functions like `printf` were compiled and assembled elsewhere. The final step to using `<stdio.h>` is for the linker to combine your `simple.o` code with the `stdio.o` code located elsewhere and create an executable that has all the references it needs to be run.

The output of the linking step is the final product. If you want to see the binary of the final linked product, you can run `hexdump` on your executable binary.

Compile your code into a binary executable program by running the following command:

```bash
gcc simple.c
```

Whenever you run `gcc` without any arugments, it runs through all four of the steps automatically. This will create a new file in our directory called `a.out`. We can run our created program by typing:

```bash
./a.out
```

If you followed these steps correctly, your code should output the following to the terminal:

```bash
Hello, World!

```

The default name for all compiled programs is `a.out`. If you wish to change that, you can use the `-o` flag on the `gcc` program to name the output file:

```bash
gcc simple.c -o simple
```

This with create a new executable file named `simple` instead of `a.out`, which you can run by typing `./simple`.

### Part 2: Examining C code

Now that we understand the compilation process, let's examine the syntax of `simple.c`.

```c
// This is a single line comment.

/* 
This is a multi-line comment.
Any text that is between the slash-star and
the star-slash will be ignored.
*/

#include <stdio.h>

int main()
{
    printf("Hello, World!\n");
    return 0;
}

```

#### Comments

There are two types of comments: single and multi-line.

```c
// This is a single line comment.
```

Any text written after the `//` symbols wil be ignored during the compilation process of the program. These comments are most helpful when trying to provide clarity to the purpose of a line code or even to demarcate sections of code.

If you want to take up several lines to describe a complicated concept or simply put some detailed notes in your program, you can use the multi-line comment.

```c
/*
This is a multi-line comment.
Any text that is between the slash-star and
the star-slash will be ignored.
*/
```

Anything between the `/*` and `*/` will also be ignored during the compilation process of the program. One example for good use of multi-line comments is explaining the purpose of a function and it parameters right before its declaration.

*Where* and *how* you use comments determines how well your code can be understood and reused by yourself and others in the future. Good comments tell readers how functions work, what their purpose is, and how programs should be used.

#### The main() Function

**All C programs must have a `main()` function.** The `main()` function serves as a starting point for your program whenever it is executed. All your code and calls to other functions must be contained within the `main()` function.

To declare a function in C, you must write its **return type** (more on that below), its name, and then any parameters it accepts in parentheses:

In the declaration above, we declare the return type for `main()` as an `int`. We also expect no parameters to be passed into it, so the parentheses are empty `()`.

Everything written between the curly braces `{}` is considered to be part of the function's **scope**, meaning that all variables and instructions only exist while that function is active.

#### printf()

This function, which comes from `stdio.h` as we learned earlier, is **called by** `main()` and allows us to write values to the terminal screen from our program. In this example

```c
printf("Hello, World!\n");
```

prints out `Hello, World!`; the `\n` is an **escape character** which moves the cursor to the next line.

#### Return Values

Whenever a function finishes executing, it has the option to return a single value back to the function that called it. The type of value it returns is given by the return type. If a function should not return a value, its return type is `void`. For example, a function adds two numbers might look like this:

```c
int sum(int a, int b)
{
    return a + b;
}
```

To use `sum()`, I call this function in my main function:

```c
int main()
{
    int result = sum(2, 3);
    printf("%d\n", result);
}
```

I can expect that this program will print out `5` because the **return value** of `sum()` was an `int` whose value is the sum of my two parameters, `a` (2) and `b` (3).

If `main()` is the the starting point of a program, why does it have a return value? This return value is often used to indicate if the program finished correctly or crashed in a specific way. Take the following example:

```c
#include <stdio.h>

int main()
{
    int result = printf("Hello There!\n");

    if(result <= 0)
    {
        return -1; // If printf fails.
    }
    else
    {
        return 0; // A 0 generally denotes no errors.
    }
}
```

The shell that executed the program receives this error code. In Bash or ZSH after running a program, you can check the return value of its main function by typing in `echo $?`.

#### Data Types

In order to create a meaningful program in any language, you need to know how to correctly store information. C is a **strongly-typed** language, meaning that we must specify the **type** of every new variable we create is. C has the following **native** (built-in) data types:

| type     | Size (Bytes) | Description                                                 |
| -------- | ------------ | ----------------------------------------------------------- |
| `char`   | 1            | Stores an integer                                           |
| `short`  | 2            | Stores an integer (has a greater range than `char`)         |
| `int`    | 4            | Stores an integer (has a greater range than `short`)        |
| `long`   | 8            | Stores an integer (has a greater range than `int`)          |
| `float`  | 4            | Stores decimal numbers                                      |
| `double` | 8            | Stores decimal numbers (has greater precision than `float`) |

Each of these data types has two key components: the type of value that is stored (integer, decimal, etc.), and the **size**, which determines the range of numbers that type can store. Every data type in C has a predetermined **width**, or number of bits that it has to store information. A binary number with `N` bits can store numbers from `0` to `(2^N)-1`. Computers don't usually do operations down to a single bit, so we usually talk about the width of a data type in units of bytes instead of bits.

It is also important to note that the size of each of the above types is dependent on the processor on which you will be running the code. For example an `int` on a 16 bit processor will be 16 bits, while on a 32 bit processor an `int` is usually 32 bits, and on a 64-bit processor an `int` is also typically also 32 bits. It is the programmer's responsibility to understand what these data types mean and how they work on your system. Ignoring these important details has led to catastrophies in the past ([including rockets that explode shortly after launch](https://en.wikipedia.org/wiki/Ariane_flight_V88)).

Even though variables are stored binary numbers, **we dont have to interpret the bits as regular binary**. For example, negative numbers are stored by saving the most significant bit to represent the sign instead of part of the value. This is called **Two's Complement**, and it changes the range of a data type to be `-2^(N-1)` to `2^(N-1)-1`. This is the default storage scheme for all data types that store integers. For decimal or **floating point** numbers, the default scheme is the **IEEE Floating Point** format.

You can control the scheme that a data type uses to maximize its efficiency. For example, the keyword `unsigned` makes the compiler assume the bits to be regular binary instead of Two's Complement, preventing us from storing negative numbers but doubling the range of positive numbers we can store.

#### Casting

Becuase all the data types have different widths and use different schemes, we often need to take the value of one data type and represent it as a different type. The process of the translating from one data type to another is known as **casting** and will be a very useful tool in this and other labs whenever you need to perform an operation with two variables of different types.

For example, given an `int` and a `float`:

```c
int num = 7;
float num_f = 0;
```

If I want the *float* `num_f` to have the same value (`7`) as the *int* `num`, I must first cast `num` to a float by adding `(float)` before it's name:

```c
num_f = (float) num;
```

This this type of casting is called an **explicit cast** because you put the desired type in parentheses. There is another kind of casting called **implicit casting** that can occur automatically when you do operations against mis-matched data types.

**You must be careful** when casting that the value you are casting can be stored in both types. For example, if you have the number `400` stored in a `short` (16 bits) and you try to cast it to a `char` (8 bits), the resulting value will appear as `-112` becuase the number `400` is too large for a `char` type. This type of error is especially dangerous with implicit casts, since you may not know which type is being converted. Implicit casting can be prevented by explicitly casting variables yourself to ensure this doesn't happen.

### Part 3: stdint and printf

#### stdint.h

Standardized data types that explicitly define the number of bits they use (regardless of processor) are defined in the `stdint.h` library. This contains specialized data types such as `uint8_t`, `uint16_t`, etc. whose widths are *always* indicated in their name. These data types are useful for adding clarity about what values you are storing and avoiding casting errors.

Example: If you needed a data type for positive integers less than 256, you could `#include <stdint.h>` and use the `uint8_t` type (`u` for unsigned, `int`, `8` for 8 bits, and `_t` for type). Because this type explicitly defines the number of bits it uses, it is very clear what values this variable expects to receive.

To understand more about the types of data types that exist in `stdint.h`, you can check out [the documentation for stdint](https://man7.org/linux/man-pages/man0/stdint.h.0p.html).

#### printf() arguments

In addition to printing text

```c
printf("Hello, World!\n");
```

`printf` is capable of printing variables, using **print specifiers** such `%d` or `%s`:

```c
int grade = 87;
printf("Final grade: %d", grade);  // This prints out "Final grade: 87"
```

For each specifier you use, you must also give a value:

```c
int num1 = 0;
int num2 = 1;
int num3 = 2;
printf("First num:\t%d\nSecond num:\t%d\nThird num:\t%d\n", num1, num2, num3);
// matching pairs:  ^1               ^2              ^3     ^1    ^2    ^3

// Output:
// First num:   0
// Second num:  1
// Third num:   2
```

The following table is a useful cheat sheet for the different types of formatting specifiers that can be used:

| Symbol | Description                        |
| ------ | ---------------------------------- |
| `%c`   | character                          |
| `%d`   | decimal (integer) number (base 10) |
| `%e`   | exponential floating-point number  |
| `%f`   | floating-point number              |
| `%i`   | integer (base 10)                  |
| `%o`   | octal number (base 8)              |
| `%s`   | a string of characters             |
| `%u`   | unsigned decimal (integer) number  |
| `%x`   | number in hexadecimal (base 16)    |
| `%%`   | print a percent sign               |
| `\%`   | print a percent sign               |

In addition to the format specifiers listed above, there are several **escape characters** which tell `printf()` to do things you can't normally type:

| Symbol | Description                                                                                                                                        |
| ------ | -------------------------------------------------------------------------------------------------------------------------------------------------- |
| `\n`   | newline (moves to the next line)                                                                                                                   |
| `\r`   | carriage Return (moves cursor to the beginning of the same line)                                                                                   |
| `\t`   | tab (adds a horizontal tab space)                                                                                                                  |
| `\b`   | backspace (removes the previous character)                                                                                                         |
| `\\`   | backslash (\) literal                                                                                                                              |
| `\'`   | single quote (') literal; avoids confusing the compiler                                                                                            |
| `\"`   | double quote (") literal; avoids confusing the compiler                                                                                            |
| `\?`   | question mark (?), helps avoid [trigraph](https://en.wikipedia.org/wiki/Digraphs_and_trigraphs_(programming)#C) issues. (You can just use `?` too) |
| `\0`   | null character, used to terminate strings                                                                                                          |

### Part 4: Lab Challenge

To finish this lab, create a new file called `data.c`. This program should do the following. For each requirement, place a comment next to or above it so we know you have completed the required step:

1. Print out the hex equivalent of the `unsigned int`: 3735928559
2. Create a function that takes in a `uint8_t` as a parameter and prints the `char` equivalent. Use it at least 3 times in your `main()`.
3. Use the `printf()` function at least once that has multiple formatting specifiers/placeholders.
4. Use at least 5 different format specifier types in 5 different `printf()` statements.
5. Use some format specifiers/placeholders in `printf()` in unexpected ways (i.e. pass in a `char` and format it with %d, or something similar). Your program must compile. In a comment next to or above this statement, explain the behavior and why you think it works that way.

Complile your code into an executable called `data`.

## Lab Submission

- Your program must compile without warnings or errors. Compile your program with the `-Werror` flag to ensure that it doesn't.

- Pass off with a TA by showing them the following:
  - The five files you generated in Part 1: `simple_preprocessed.txt`, `simple.s`, `simple.o`, `simple_assembled.txt`, and `a.out` (or `simple` if you renamed it).
  
  - The source code and program execution of `data.c`. Show the TA how each of the required components functions, and why the output appears the way it does (what casting occurred, etc.).

- Take the Pass off Quiz on Learning Suite.

- Follow the instructions in the `submission.md` file in the repository to update your README file with what you did in this lab.

## Explore More

- [Understanding `man` pages](https://kitras.io/linux/man-pages/)
- [`printf()` Cheatsheet](https://alvinalexander.com/programming/printf-format-cheat-sheet/)
- [C data types](https://en.wikipedia.org/wiki/C_data_types)
- [Casting in C](https://www.tutorialspoint.com/cprogramming/c_type_casting.htm)
- [GCC Basics](https://www.cs.binghamton.edu/~rhainin1/walkthroughs/gccbasics.html)
- [Compiled v. Interpreted Languages](https://www.geeksforgeeks.org/difference-between-compiled-and-interpreted-language/)
