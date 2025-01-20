---
title: C Programming
number: 3
layout: lab
---

## Objectives
- Understand basic C syntax
- Know the difference between the standard data types
- Experiment with `printf` a bit and different data types
- Compilation of a program by using `gcc`

## Getting Started
Use the GitHub Classroom link posted in the Learning Suite for the lab to accept the assignment. Next, ssh into your Raspberry Pi using VSCode and clone the repository in your home directory. **This lab should be done in VSCode.**

## Overview

For the rest of the labs this semester, we will be focusing on building a strong foundation of programming using the C programming language. C is an old, yet very important language that is still actively used in development today. The creators of C, [Dennis Ritchie](https://computerhistory.org/profile/dennis-ritchie/) and [Ken Thompson](https://computerhistory.org/profile/ken-thompson/) are also the creators of the Unix operating system, the predecessor to the Linux system we are using in this class. A good understanding of how C and Unix work will provide good insight into how most computers work in general.

The philosophy of this lab is that the best way to learn something is to jump in and do it. Most of this page will be reference for all the problems and programming challenges in the lab README.md.

### A simple C Program

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

Above is a simple [Hello World](https://en.wikipedia.org/wiki/%22Hello,_World!%22_program) program written in C. Its purpose is to provide an example of the most fundamental functions of a language and how a program written in it interfaces with the computer. We'll break down this code line by line, but first we need to understand how a c program is born.

## The Compilation Process

C is a [compiled language](https://www.geeksforgeeks.org/difference-between-compiled-and-interpreted-language/). This means that the entirety of the code you write in C must go through a special process of being converted into a [binary executable](https://en.wikipedia.org/wiki/Executable) before it can be read and executed by the processor of your computer.

The process of taking all the code you have written in C and translating it into binary is called **compilation**. This process is performed by a program called a **compiler**. In this class, we will use the `gcc` compiler (short for GNU Compiler Collection). 

Although it may seem compilers only perform the simple task of converting a higher level language to machine language, the truth is that they are much more robust and advanced than that. Many compilers analyze the code being passed in and optimize it so that it can be executed in the fewest amount of instructions possible.

The compliation process has four major steps, which we will break down below.

### Step 1: Pre-processing

Also called pre-compliation, this step invovles taking special statements in your code

Before compiling your code, a pre-compiler is run which handles special statements called **directives**. Directives aid in deciding which parts of a written program in C are included during the compilation process. You can normally spot these because they start with a `#` sign.

The `#include` directive looks at a file that exists in the operating system and includes it (essentially copying and pasting it) into your code. For example, `stdio.h` is a file that contains code that deals with the **st**andar**d** **i**nput and **o**utput that can be used in your program (i.e. reading from your keyboard and writing to a terminal). This is where the definition of the `printf()` function resides. Without including `stdio.h`, the "hello world" code would not know what the `printf()` function was or where it was defined and thus would not be able to execute correctly.

Other interesting compiler directives are `#define`, `#if`/`#else`/`#endif`. You will need to look these up and know what they mean for the lab questions.

To see the precompiled version of your C code, you can run the following terminal command:
```bash
gcc -S program.c > processed_program.txt
```

### Step 2: Compiling

Confusing as it may be, the second stage of the compilation process is also called compiling. In this step, the compiler takes your code and converts it into [https://en.wikipedia.org/wiki/Assembly_language](assembly), a set of instructions easily read by your computer's hardware.  As you will see later in the class, every assembly command can be directly mapped to binary, making it very easy for a computer to understand.

Many modern compilers will also adjust your code to optimize it.  For example, if your code includes a command to multiply an interger by a multiple of 2 (ex: `newInt = 3 * 4`), your compiler may instead use a binary shift operation (`newInt = 3 << 2`  in C or `shr %rdx, 2` in assembly).

Because each computer model uses different hardware, there will be slight differences in how a computer compiles a code on different machines. The binary exectuable that is compiled on your doorbell will likely work on other students' doorbells, but not on your lap computer.

To see the compiled version of your C code, you can run the following terminal command:
```bash
gcc -S program.c
```

### Step 3: Assembling

In this next stage of the compiliation process, the assembly code generated in the last step will be transformed into binary, the pure language of 

### A simple C Program

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

Above is a simple [Hello World](https://en.wikipedia.org/wiki/%22Hello,_World!%22_program) program written in C. Its purpose is to provide an example of the most fundamental functions of a language and how a program written in it interfaces with the computer. Let's take a look at it line by line and break down some essential components.

#### Comments
When writing a program, especially more complex ones, leaving comments in the code is essential to increase its readability. There are two types of comments: the single and multi-line comments.

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

Although the *where* and *how* you use comments in your code are of little importance to the execution the program, they are imperative in making your code easier to understand and be reused by yourself and different people in the future.

#### Compiler Directives
C is a [compiled language](https://www.geeksforgeeks.org/difference-between-compiled-and-interpreted-language/). This means that the entirety of the code you write in C must go through a special process of being converted into a [binary executable](https://en.wikipedia.org/wiki/Executable) before it can be read and executed by the processor of your computer.

The process of taking all the code you have written in C and translating it into binary is called **compilation**. This process is performed by a program called a **compiler**. In this class, we will use the `gcc` compiler (short for GNU Compiler Collection). 

Although it may seem compilers only perform the simple task of converting a higher level language to machine language, the truth is that they are much more robust and advanced than that. Many compilers analyze the code being passed in and optimize it so that it can be executed in the fewest amount of instructions possible.

Before compiling your code, a pre-compiler is run which handles special statements called **directives**. Directives aid in deciding which parts of a written program in C are included during the compilation process. You can normally spot these because they start with a `#` sign.

The `#include` directive looks at a file that exists in the operating system and includes it (essentially copying and pasting it) into your code. For example, `stdio.h` is a file that contains code that deals with the **st**andar**d** **i**nput and **o**utput that can be used in your program (i.e. reading from your keyboard and writing to a terminal). This is where the definition of the `printf()` function resides. Without including `stdio.h`, the "hello world" code would not know what the `printf()` function was or where it was defined and thus would not be able to execute correctly.

Other interesting compiler directives are `#define`, `#if`/`#else`/`#endif`. You will need to look these up and know what they mean for the lab questions.

#### main() Function
Every program written in C must have a `main()` function. This is the function that will be run upon executing the program. All other functions that might need to be read in the execution of the program must be called in one form or another from the `main()` function.

To declare a function in C, you must write its **return type** (more on that below), its name, and then any parameters it accepts in parentheses:

```c
int main()
```

In the declaration above, we assert that the return type for the `main()` function is an `int` and that we expect no parameters to be passed into it `()`.

To maintain the scope of a function, everything written between curly braces `{}` is considered to be a part of the function declaration that came before it.

#### printf()
This function, which comes from `stdio.h` as we learned earlier, allows us to write values to the terminal screen from our program. In this example

```c
printf("Hello, World!\n");
```

prints out `Hello, World!` and then moves the cursor to the next line.

#### Return Values
Functions that completes execution in a C program have a single (optional) return value that is passed up to the function that calls it. For example if I wanted to write a quick function that took the sum of two numbers, I could write it like so:

```c
int sum(int a, int b)
{
    return a + b;
}
```

If I call this function in my main function:

```c
int main()
{
    int result = sum(2, 3);
    printf("%d\n", result);
}
```

I can expect that the program will print out `5` because the **return value** that the `sum()` provided was an `int` which was the sum  of `a + b`.

If I wanted to have my value return the value as a `float`, I would rewrite the signature of my function as:

```c
float sum(float a, float b)
{
    return a + b;
}
```

If the `main()` function is the outermost function that we can have in a C program, why does it have a return value? Well, this return value is often used to indicate if the program finished correctly or crashed in a specific way. Take the following example:

```c
// This is example code, it will not run because some of these values have not been defined.

int main()
{
    if(err_type == "Crash")
    {
        return -1;
    }
    else if(err_type == "Insufficient data")
    {
        return -2;
    }
    else
    {
        // This assumes no errors
        return 0;
    }
}
```

If the program runs into specific errors, it will return different values to the outermost function. But since the `main()` function is the outermost function, it is the shell that receives the error code. In Bash or ZSH after running a program, you can check the return value of its main function by typing in `echo $?`.

#### Compiling with GCC
Now that we understand what each line of our simple C program does, it is time to run it. In your lab repository, create a new file called `simple.c` in your lab directory. Inside that file, copy and paste the code at the top of the **A Simple C Program** section and save and close the file.

To compile this code, we will use `gcc` to turn it into a binary executable program:

```bash
gcc simple.c
```

This will create a new file in our directory called `a.out` which we can run by:

```bash
./a.out
```

If you followed these steps correctly, your code should output the following to the terminal:

```
Hello, World!

```

If you are not content with all of your binary executables being named `a.out` every time you compile your C project, you can use the `-o` flag on the `gcc` program to name the output file:

```bash
gcc simple.c -o myprogram
```

This with create a new file named `myprogram` instead of `a.out`.

### Data Types
Now that we have dissected our first C program, it is time to dive a little deeper into some details of the details. 

In order to successfully create a meaningful program in any language, you need to know how to correctly store and portray information. Unlike some modern languages, C is a **strongly-typed** language. This means that every time we declare a new variable, we need to specify what **type** it is. The native data types (i.e. no `#include`ing libraries are necessary) in C are the following:

| type     | Description                                                                                               |
| -------- | --------------------------------------------------------------------------------------------------------- |
| `char`   | Stores an integer (or a letter, which are the same thing)                                                 |
| `short`  | Stores an integer                                                                                         |
| `int`    | Stores an integer                                                                                         |
| `long`   | Stores an integer                                                                                         |
| `float`  | One way to store a real number with a floating decimal point (i.e. you can put it anywhere in the number) |
| `double` | Larger version of a float                                                                                 |

It would be impractical to go over every data type that exists in C and explain its function in a lab setting. It will be your responsibility to understand what these data types mean and how they can be used as they come up. Google is your friend.

#### Casting
Sometimes it will be necessary to take the result of one number and represent it in a different type of variable. The process of the translating from one data type to another is known as **casting** and will be a very useful tool in this and other labs.

For example, let's say I have a variable that was stored as an `int` and another variable that is a `float`. :

```c
int num = 7;
float num_f = 0;
```

If I want to create a new variable where the `7` in `num` is treated as a floating point number, (i.e. `7.0`), I can cast it by doing the following.

```c
num_f = (float) num;
```

#### stdint.h
Another treasure trove of information on data types exists in the `stdint.h` library. This contains specialized data types such as `uint8_t` and others that have specialized characteristics for specific needs.

For example, if you need to use a data **t**ype that stores an **int**eger that is **u**nsigned (can never be negative) and has  **8** bit length, you would `#include <stdint.h>` and use the `uint8_t` type. This can be useful because the all of the bit patterns possibly contained in the 8 bit value are used to represent numbers above 0 (ex. 0-255). In a normal int, **about but not quite** half of those bit paterns map to negative numbers (thus decreasing the maximum number that can be represented). You should be learning more about this in the lecture portion of class!

To understand more about the types of data types that exist in `stdint.h`, you can check out the [documentation for this file](https://man7.org/linux/man-pages/man0/stdint.h.0p.html).

### printf
As you have seen in our simple C program, we can use the `printf()` function to send text out to the terminal from our program.

```c
printf("Hello, World!\n");
```

However, you may have noticed in other examples the use of strange characters such as `%d` or `%s` that pop in the strings that we are trying to print:

```c
int grade = 87;
printf("Final grade:\t%d", grade);  // This should print out "Final grade:    87"
```

Characters with a `%` followed by a letter represent a placeholder/format specifier for certain types of data which are additionally passed into our `printf()` message as arguments. For example, `%d` means that the value of the passed in argument should be interpreted as **d**ecimal number when it is printed to the screen. In our example above, the `%d` would be replaced with the variable `grade`.

Multiple values can also be passed into the `printf()` statement, as shown below:

```c
int num1 = 0;
int num2 = 1;
int num3 = 2;
printf("First num:\t%d\nSecond num:\t%d\nThird num:\t%d\n", num1, num2, num3);

// The code above should print out:
// First num:    0
// Second num:   1
// Third num:    2
```

The following table is a useful cheat sheet and will give you an idea of the different types of formatting specifiers that can be used in the `printf()` statement:

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

## Lab Submission

- Answer the questions and complete the challenges found in the `README.md`.

- The `README.md` walks you through creating a new file called `data.c` with some requirements. Show the source code and program execution of `data.c` to the TA.

- To successfully submit your lab, you will need to follow the instructions in the [Lab Setup]({{ site.baseurl }}/lab-setup) page, especially the **Committing and Pushing Files** section.

## Explore More!

- [Understanding `man` pages](https://kitras.io/linux/man-pages/)
- [`printf()` Cheatsheet](https://alvinalexander.com/programming/printf-format-cheat-sheet/)
- [C data types](https://en.wikipedia.org/wiki/C_data_types)
- [Casting in C](https://www.tutorialspoint.com/cprogramming/c_type_casting.htm)
- [GCC Basics](https://www.cs.binghamton.edu/~rhainin1/walkthroughs/gccbasics.html)
- [Compiled v. Interpreted Languages](https://www.geeksforgeeks.org/difference-between-compiled-and-interpreted-language/)
