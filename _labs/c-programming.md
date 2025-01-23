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

## Getting Started
Use the GitHub Classroom link posted in the Learning Suite for the lab to accept the assignment. SSH into your Raspberry Pi using VSCode and clone the repository in your home directory. **This lab should be done in VSCode.**

## Overview

For the rest of the labs this semester, we will be focusing on building a strong foundation of programming using the C programming language. C is an old, yet very important language that is still actively used in development today. The creators of C, [Dennis Ritchie](https://computerhistory.org/profile/dennis-ritchie/) and [Ken Thompson](https://computerhistory.org/profile/ken-thompson/), are also the creators of the Unix operating system, the predecessor to the Linux system we are using in this class. A good understanding of how C and Unix work will provide good insight into how computers work in general.

The philosophy of this lab is that the best way to learn something is to jump in and do it. The following sections will guide you through creating, compiling, and running a C program and teach you basic C syntax and tools. Read each section carefully and make sure to complete each step. 

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

Above is a simple [Hello World](https://en.wikipedia.org/wiki/%22Hello,_World!%22_program) program written in C. Its purpose is to provide an example of the most fundamental functions of a language and how a program written in C interfaces with the computer. 

In your lab repository, create a new file called `simple.c`. This can be done using the File Explorer tab in VS Code, or from the terminal using the `nano` command (or a text editor of your choice. You can also create the file without a text editor using `touch` - google it for details). Inside the `simple.c` file, copy and paste the code above, then save and close the file.

We'll break down this code line by line, but first we need to understand how a C program is born.

## The Compilation Process

C is a [compiled language](https://www.geeksforgeeks.org/difference-between-compiled-and-interpreted-language/). This means that the entirety of the code you write in C must go through a special process of being converted into a [binary executable](https://en.wikipedia.org/wiki/Executable) before it can be read and executed by the processor of your computer.

The process of taking all the code you have written in C and translating it into binary is called **compilation**. This process is performed by a program called a **compiler**. In this class, we will use the `gcc` compiler (short for GNU Compiler Collection). 

Compilers do more than simply convert a higher level language to machine language. Many compilers analyze the code being passed in and optimize it so that it can be executed in the fewest amount of instructions possible.

The compilation process has four major steps, which we will break down and execute on `simple.c` below.

### Step 1: Pre-processing

Also called pre-compilation, this step runs a pre-compiler which handles special statements in your code called **directives**. Directives aid in deciding which parts of a written program in C are included during the compilation process. You can normally spot these because they start with a `#` sign.

The `#include` directive looks at a file that exists in the operating system and includes it (essentially copying and pasting it) into your code. For example, `stdio.h` is a file that contains code that deals with the **st**andar**d** **i**nput and **o**utput that can be used in your program (i.e. reading from your keyboard and writing to a terminal). This is where the definition of the `printf()` function resides. Without including `stdio.h`, the "Hello World" code would not know what the `printf()` function was or where it was defined and thus would not be able to execute correctly.

Other interesting compiler directives are `#define`, `#if`/`#else`/`#endif`. You will need to look these up and know what they mean for the lab quiz.

Generate a precompiled version of your `simple.c` code by running the following terminal command:
```bash
gcc -S simple.c > simple_preprocessed.txt
```
Look through the generated text file. Notice that a lot of extra text has been added, but at the last few lines of the text file you will find your original code. 

### Step 2: Compiling

The second stage of the compilation process is called compiling. In this step, the compiler takes your code and converts it into [assembly](https://en.wikipedia.org/wiki/Assembly_language), the specific types of instructions that your specific computer processor knows how to execute. As you will see later in the class, every assembly command can be directly mapped to binary which is then read directly by the computer hardware in your processor.

Many modern compilers will also adjust your code to optimize it.  For example, if your code includes a command to multiply an integer by a multiple of 2 (ex: `newInt = 3 * 4`), your compiler may instead use a binary shift operation (`newInt = 3 << 2`  in C or `shr %rdx, 2` in assembly).

Because each computer model uses different hardware, there will be differences in the assembly generated to be run on different machines. The binary exectuable that is compiled on your doorbell will likely work on other students' doorbells, but not on your laptop.

Compile your C code into assembly by running the following terminal command: 
```bash
gcc -S simple.c
```
This will generate an assembly file called `simple.s`. Open this file and look around. Google one of the commands that you see to try and figure out what it means. 

### Step 3: Assembling

In this next stage of the compilation process, the assembly code generated in the last step will be transformed into binary, the pure language of computers.  There is typically a one-to-one direct mapping from assembly to binary.

Run the following terminal command to generate the binary version of your code: 
```bash
gcc -c simple.c
```

Next, to actually read this code, run the following:
```bash
hexdump simple.o > simple_assembled.txt
```
Look through the text file you just generated. Note that it's not just 0s and 1s. The actual file is 0s and 1s, but we are reading it as hex so that its a little bit more interpretable. You should see two main sections. What do you think these mean? 

### Step 4: Linking

In the final stage of the compilation process, a program called the linker will find all the external references in your assembled code and combine any related .o files together.  For example, our "Hello World" code from earlier includes `stdio.h`.  While the "h-file" (which includes a description of printf, but not its implementation) has been directly included in the pre-processing step, the actual implementation of printf been compiled and assembled elsewhere. The linker will combine your code with the stdio code into a final executable that can be run.j

The linked code is the final product that can be run. The default behavior of running `gcc` with no arguments will run through all of the above steps one after the other.  However, if you still want to see the binary of the final linked product, you can run `hexdump` on your executable binary.

Compile your code into a binary executable program by running the following command:

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
gcc simple.c -o simple
```

This with create a new executable file named `simple` instead of `a.out`, which you can run by typing `./simple`. 



## Breaking down the C code

Now that we understand the compilation process, let's talk about what each part of `simple.c` is doing.  It's included below again for convenience.

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


### Comments
When writing a program, especially more complex ones, leaving comments in the code is essential to increase its readability. There are two types of comments: single and multi-line.

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

Although the *where* and *how* you use comments in your code are of little importance to the execution the program, they are imperative in making your code easier to understand and be reused by yourself and others in the future.

### main() Function
Every program written in C must have a `main()` function. This is the function that will be run upon executing the program. All other functions that might need to be read in the execution of the program must be called in one form or another from the `main()` function.

To declare a function in C, you must write its **return type** (more on that below), its name, and then any parameters it accepts in parentheses:

```c
int main()
```

In the declaration above, we assert that the return type for the `main()` function is an `int` and that we expect no parameters to be passed into it `()`.

To maintain the scope of a function, everything written between curly braces `{}` is considered to be a part of the function declaration that came before it.

### printf()
This function, which comes from `stdio.h` as we learned earlier, allows us to write values to the terminal screen from our program. In this example

```c
printf("Hello, World!\n");
```

prints out `Hello, World!` and then moves the cursor to the next line.

### Return Values
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

A function doesn't need a return value can be assigned the type `void`:

```c
void printHelloWorld() 
{
    printf("Hello World!");
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


## Data Types
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

You can combine the integer data types with the keyword `unsigned` to make an unsigned datatype of the same size.  For example: `unsigned int myInt`.
When the datatypes `char`, `short`, `int`, and `long` are used the processor will interpret the bits used to store the data as two's complement numbers - meaning they can represent both postitive and negative values with a range determined by the size of the data type. The unsigned versions of these types are interpreted as standard non-negative unsigned numbers.

It is also important to note that the size of each of the above types is dependent on the processor on which you will be running the code. For example an `int` on a 16 bit processor will be 16 bits, while on a 32 bit processor an `int` is likely 32 bits, and on a 64-bit processor an `int` is also typically also 32 bits. This affects the range of possible values that can be stored. Understanding these details is important, ignoring them has led to catastrophies in the past ([including rockets that explode shortly after launch](https://en.wikipedia.org/wiki/Ariane_flight_V88)).

Every processor is different, so it is important to pay attention to these details. It is the programmers responsability to understand what these data types mean and how they work on your system.

### Casting
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

This is an example of an explicit cast. Implicit casting can also occur, for example when you compare an int and an unsigned using `>`. These are other important details to pay attention to.

### stdint.h
Standardized data types that explicitly define a specific number of bits (regardless of processor) are defined in the `stdint.h` library. This contains specialized data types such as `uint8_t` and others that have specialized characteristics for specific needs.

For example, if you need to use a data **t**ype that stores an **int**eger that is **u**nsigned (can never be negative) and has  **8** bit length, you would `#include <stdint.h>` and use the `uint8_t` type. This can be useful because the all of the bit patterns possibly contained in the 8 bit value are used to represent numbers above 0 (ex. 0-255). In a normal int, **about but not quite** half of those bit paterns map to negative numbers (thus decreasing the maximum number that can be represented). You should be learning more about this in the lecture portion of class! Additionally, using these types explicitly defines the number of bits as opposed to relying on the compiler and processor specifics of your system.

To understand more about the types of data types that exist in `stdint.h`, you can check out the [documentation for this file](https://man7.org/linux/man-pages/man0/stdint.h.0p.html).

### printf() arguments
As you have seen in our simple C program, we can use the `printf()` function to send text out to the terminal from our program.

```c
printf("Hello, World!\n");
```

However, you may have noticed in other examples the use of strange characters such as `%d` or `%s` that show up in the strings that we are trying to print:

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


## Lab Challenge
To finish this lab, create a new file called `data.c`. This program should do the following. For each requirement, place a comment next to or above it so we know you have completed the required step:
1. Print out the hex equivalent of the `unsigned int`: 3735928559
2. Create a function that takes in a `uint8_t` as a parameter and prints the `char` equivalent. Use it at least 3 times in your `main()`.
3. Use the `printf()` function at least once that has multiple formatting specifiers/placeholders.
4. Use at least 5 different format specifier types in 5 different `printf()` statements.
5. Use some format specifiers/placeholders in `printf()` in unexpected ways (i.e. pass in a `char` and format it with %d, or something similar). Your program must compile. In a comment next to or above this statement, explain the behavior and why you think it works that way.

Complile your code into an executable called `data`. 


## Lab Submission
- Pass off with a TA by showing them the source code and program execution of `data.c`.
  
- Take the Pass-Off Quiz on Learning Suite.

- Follow the instructions in the README file in the repository to write your own README for this lab. Include your name, section, semester, and lab title. A good README should answer the following questions:
  - What is the purpose of this project and its code/files?
  - What is the structure/organization of the project files?
  - How do you build and run the code in this project?

- Add, commit, and push all of your updated files (and your README) as explained under **Committing and Pushing Files** on the [Lab Setup]({{ site.baseurl }}/lab-setup) page. Remember that while these instructions give general information, you need to add and commit all of the files you have modified or created in this lab. 


## Explore More!

- [Understanding `man` pages](https://kitras.io/linux/man-pages/)
- [`printf()` Cheatsheet](https://alvinalexander.com/programming/printf-format-cheat-sheet/)
- [C data types](https://en.wikipedia.org/wiki/C_data_types)
- [Casting in C](https://www.tutorialspoint.com/cprogramming/c_type_casting.htm)
- [GCC Basics](https://www.cs.binghamton.edu/~rhainin1/walkthroughs/gccbasics.html)
- [Compiled v. Interpreted Languages](https://www.geeksforgeeks.org/difference-between-compiled-and-interpreted-language/)
