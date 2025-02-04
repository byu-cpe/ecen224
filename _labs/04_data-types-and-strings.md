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

Since C is strongly typed, it is essential that you understand the use cases for each type. A common mistake C programmers make is picking types that are either:

- inconsistant across different architechtures and compilers,
- too small or cannot hold the correct values,
- or types that don't reflect your purpose.

As a C programmer, you must weigh all these factors and more. This lab will expose you to some of the nuances of the native C data types and those included in the `stdint.h` library. Additionally, you will practice manipulating strings. By the end of this lab, you will have written and/or debugged several functions relating to C's data types and strings.

## Data Types

In the C Programming lab, you worked with a few of C's _native_ data types, those that are implicitly defined in C without `#include`-ing any other libraries. Some things you need to consider and operations you should know are listed below.

### Inconsistant Sizes

We already discussed some of C's data types in the last lab.  Recall that there are several different data types native to C, including integers and floating point numbers.  It is important to note that the native integers in C (namely `short`, `int`, and `long`) do not have a defined length; C leaves it up to the computer manfacturer (and/or the writers of the compiler for that processor) to decide how many bits an `int` will take in memory.

This peculiarity can have disasterous consequences. For example, if you need a variable that expects values ranging from zero to two-million, an `int` will be just fine on a 64-bit computer (like most modern computers). However, the same (working) program compiled on a 32-bit computer will have `int` variables with a maximum of only around 32,000, causing overflow errors and nearly untraceable bugs.

There are several methods to check the size of data that your variables will take up. First, we can use the built-in C function `sizeof()`. This will tell you the number of **bytes** that a data type holds (to get the total bits, multiply the returned integer of bytes by 8). We can either pass `sizeof` a variable, or we can pass it the name of a data type directly:

```c
int n = 2283;
printf("%d\n", sizeof(n));      // Prints the size of n in bytes
printf("%d\n", sizeof(char)*8); // Prints the number of bits in a char
```

Another way to be sure that you know what size your variables will take up is to use a library that defines additional datatypes.  For example, the `stdlib.h` library adds `size_t`, which represents the maximum unsigned integer allowed on your system, or `stdbool.h`, which adds `bool`s (bool is not actually a primative data type, meaning it's not built into C by default!). You can even create your own data types with [typedef](https://en.wikipedia.org/wiki/Typedef), though we won't cover that extensively in this class. By convention, data types that are not native to C are suffixed with `_t` to show they are a type.

One of the most common libraries you'll see is `stdint.h` which, as you learned last week, includes definitions for several integers (both signed and unsigned) with an explicit number of bits.  Because the sizes and signedness of the those integers are defined and constant, it is **good practice** to use the integer values in `stdint.h` instead of the native C integers.  


```c
unsigned int x = 40; // Acceptable, but unreliable. How big is this?
uint32_t y = 40;     // You know that y will always have 32 bits.
```

### Overflow and Underflow

All data types, native or not, have a maximum and minimum value. What happens when we try to set a variable beyond these maximum or minimums?

**Overflow** occurs when you exceed the maximum, and **Underflow** when you drop below the minimum. The examples below cover overflow, but underflow works exactly the same (in the opposite direction).

When doing math on paper, if you run out of digits, you would simply add another digit. However, computers have limited bits for representing numbers.  If a variable is at the maximum value, adding one will simply cause the carried over number to be truncated.

```c
// Conventional Math
9999 + 0001 = 10000 
// 9999 has 4 digits and contains the maximum value for a 4 digit number.  When we add 1, we need to add a 5th digit, 1
// Limited digit math
0b11111111 + 0b00000001 = 0b00000000 
// 0b11111111 = 255 is the maximum value for a uint8_t. Adding 1 will cause overflow, and the expression evaluates to 0.
```

Overflow and underflow have very real impacts on code written in C:

Overflow Example:
```c
// Because an unsigned char has a maximum value of 255 (and adding 1 will overflow back to 0), 
// x will always be less than 500.  Thus, this for-loop will never end.
// In this scenario, it would be better to use a larger data type.
for (uint8_t x = 0; x < 500; x++) {
    ...
}
```
Underflow Example:
```c
// Imagine a sensor that detects the number of frogs entering and leaving a small pond.
uint32_t numFrogsInPond = 0;
// A frog jumps in the pond, so we go up by one.
numFrogsInPond++; // numFrogsInPond = 1
// Next, the sensor sees the frog leave. There is a glitch, the frog is counted as leaving twice.
numfrogsInPond -= 2 // numFrogsInPond = 1-2 = 4,294,967,295
// 4.29 billion frogs is simply too many frogs in a small pond. 
// Your frog pond management system has broken. Everyone is sad.
```

Seeing these errors with overflow and underflow, you may be inclined to simply use the largest data type whenever possible.  However, in some cases memory space is severely limited or can negativelly impact the performance of your program.
For example, each color value of an 8-bit color image exactly fits within a uint8_t  (or one byte). With a resolution of 3840 x 2160 and 3 color values per pixel, the approximate (uncompressed) size of a 4K image is about 24.8 MB. 
If you were to instead store each color value within a ``uint64_t``, the exact same image would take up 199 MB - thats almost a GB for every five images! All that extra space is just storing zeros with no useful information.

Similar concepts are important when sending data between two computers or any two systems. For underwater communication (say between two underwater robots), we have to use sound to communicate and the amount of data we can send is very limited (like 32 bytes per second). 
In this case, only four ``double`` sized variables could be sent per packet, but 32 ``uint8_t`` variables could be sent. 

In many contexts, you can determine a limit on the range of values that are possible and select a datatype based upon this. For example, on a typical semester we often have somewhere around 100 students in ECEN 225.  If we were to write a program that counts the number of students in the class, a ``uint8_t`` would suffice just fine.  However, if in a future year 256 students were to register, than we would have problems. 
It is important to think about these details when determining the datatype for any variable you select when programing in C. 

### Casting, Order of Operations, and Constants

Sometimes, you will have a variable that is the incorrect type for the operation you want to accomplish.  In this case, we can *cast* your variable, or convert it to another datatype. However, understanding the details of how this casting occurs is essential - as its not always intuitive.

#### Explicit vs Implicit Casting 
Casting can be done *explicitly* - meaning when you, the programmer, write code saying you want a cast to occur. An example of casting an integer to a float is shown below:

```c
float a;
uint8_t b = 10;
a = (float) b;
```

Casting also happens *implicitly*, meaning that the compiler will take the liberty of converting your variables for you if you combine multiple types in an expression. If you assign one variable to that of another type, for example, the compiler will cast for you. So we actually don't need the ``(float)`` in the above example, because the compiler knows that you are assigning a ``uint8_t`` to a ``float``. So the code below is equivalent to the code shown above and a cast to ``float`` is inserted by the compiler when ``b`` is assigned to ``a``:

```c
float a;
uint8_t b = 10;
a = b;
```

Implicit casts also happen when expressions mix variables of different types. This happens because the processor hardware circuit, the part of the computer that actually carries out the computations, requires everything to be the same type before it can complete the calculation. Thus, before dividing or comparing two numbers (for example), they are converted to the same type before the calculation is carried out. Understanding the details of how this happens is pretty important.

For example, lets say you wanted to divide two integers to get a decimal.  In the hardware that implements the division, floating point division is different from integer division (and they can't be mixed). Integer division will take two integers, divide them, and then truncate (or drop) any decimal portion in the result. This makes sense because integer data types can't represent decimal information anyway. Floating point division takes two floating point numbers and results in another floating point number - all of which are represented using either a ``float`` or a ``double`` (depending on the datatypes of the variables) and thus decimal portions are maintained. 

If you want the decimal part of the result to be maintained, we need to ensure we are using floating point division. To do this you can explictly cast one of the ints to a ``float`` (or a ``double``) before the calculation happens, then the other ``int`` will be implicitly cast by the compiler before the division occurs and the result will also be a floating point number.

The example below demonstrates how this works:
```c
int32_t a = 5;
int32_t b = 2;
float c, d;
c = a / b; // This will evaluate to 2.0.
d = (float) a / b; // This will evaluate to 2.5.
```

In the example above ``c`` evaluates to 2.0 because both ``a`` (value 5) and ``b`` (value 2) are integers and thus integer division is the operation that is carried out. Integer division happens before the assignment (and results in 2) which is then cast to a float before being assigned to ``c``.

Alternativelly, ``d`` evaluates to 2.5 because you, the programmer, explicitly cast ``a`` to be of type ``float`` and thus the compiler will implicitly cast ``b`` to be of type ``float`` so that floating point division can take place. In this case casting is not needed in the assignment (because the result is already a float).


#### Order of Operations in C
Notice that in the example above (where ``c`` became 2.0), that the result was cast to a floating point even though the decimal points were truncated. This happens because of the order of operations. 

The order of operations in C are shown in the following [table](https://www.tutorialspoint.com/cprogramming/c_operators_precedence.htm) (highest/first priority at the top, lowest/last priority at the bottom):


| Category | Operator | Associativity |
| -------- | -------- | ------------- |
| Postfix (function calls, array/pointer/struct access, and postfix inc/dec) | () [] -> . ++ -- | Left-to-right |
| Unary (negative, logical/binary not, prefix inc/dec, casting, address reference, sizeof) | - ! ~ ++ -- (type)* & sizeof	| Right-to-left |
| Multiplicative	| * / %	| Left-to-right |
| Additive	| + -	| Left-to-right |
| Shift	| << >>	| Left-to-right |
| Relational	| < <= > >=	| Left-to-right |
| Equality	| == !=	| Left-to-right |
| Bitwise AND	| &	| Left-to-right |
| Bitwise XOR	| ^	| Left-to-right |
| Bitwise OR	| \|	| Left-to-right |
| Logical AND	| &&	| Left-to-right |
| Logical OR	| \|\|	| Left-to-right |
| Conditional	| ?:	| Right-to-left |
| Assignment	| = += -= *= /= %=>>= <<= &= ^= \|=	| Right-to-left | 
| Comma	| ,	| Left-to-right | 

Notice that explicit casting happens before division, which happens before assignment. In the example with ``c`` above, there is no explicit cast, so integer division is the highest priority. Then the result of the integer division is cast to a float when the assignment happens.

#### Literals in C
We also need to talk about literals in C. Literals are hardcoded values you include in your code. These are often numbers, but can be strings, characters, or booleans as well.  

To demonstrate what we mean, ``125``, ``3.14``, ``0x1234``, ``"Yipee!\n"`` and ``true`` are each examples of literals in the code below.
```c
double a = 125 / 3.14;
uint16_t b = 0x1234;
if ( check_small_enough(a) == true )
    printf("Yipee!\n");
```

By default, whole number literals are treated as having type ``int``. Numerical literals with a decimal point are treated as having type ``double``. If you want to specify a different type for a literal, you can sometimes do this by adding a suffix. 
For example, ``125`` by default will be treated as an ``int``, but ``125U`` will be treated as an unsigned. Similarly, ``3.14`` will be treated as a ``double`` by default, but ``3.14f`` will be treated as having type ``float``.  

Integer literals can be specified in either decimal or hexadecimal (using the prefix ``0x``). While not natively in C, the GCC compiler also allows binary literals via the prefix ``0b``. As with integer literals, by default (and as long as the value fits) the compiler will treat the literal as having type ``int``. Adding a suffix of ``u`` or ``U`` will specify that it should be treated as unsigned. 

While all of the following have a value of 5, the type varies depending on the suffix:
```c
5;      // This will be treated as a (signed) int
5U;     // This will be treated as an unsigned int 
0x5;    // This will be treated as a (signed) int
0x5U;   // This will be treated as an unsigned int
0b101;  // This will be treated as a (signed) int
0b101U; // This will be treated as an unsigned int
```

#### Casting Between Sizes of the Same Type

It is fairly common to need to cast between datatypes that are of the same type but different sizes. 

When casting from a smaller signed integer type to a larger signed integer type, the data bits are **sign** **extended**. Similarly, when casting from a smaller unsigned integer type to a larger unsigned integer type, the data bits are **zero extended**. This difference is so that, in both cases, the value represented will remain the same. 
Making a variable larger only increases the range of possible values that can be stored and thus _casting up_ in size  should always result in the same value being represented.

```c
uint16_t a = 0xF123U; // Decimal value of 61731
uint32_t b = a; // An implicit cast zero extends a to 0x0000F123 (still 61731)
int16_t c = 0xF123; // Decimal value of -3805
int32_t d = c; // An implicit cast sign extends c to 0xFFFFF123 (still -3805)
```

When casting from a larger integer type to a smaller integer of the same type (either signed or unsigned), there is a good chance that the smaller type will not be able to hold the original value. When we cast this way the higher order bits of the value are _truncated_. This means all of the higher order bits are thrown away and the new smaller result simply stores the least significant bits corresponding to the new size. When this happens the value may change drastically if the original value is too high to be represented in the new type.

```c
uint16_t a = 0xF123U; // Decimal value of 61731
uint8_t b = a; // An implicit cast truncates a to 0x23 (35)
int16_t c = 0xF1F3; // Decimal value of -3597
int8_t d = c; // An implicit cast truncates c to 0xF3 (-13)
```

It is also possible to cast between the floating point types ``double`` and ``float``. When this happens, the number of bits devoted to the exponent and mantissa fields changes. When converting from ``float`` to ``double``, the number of possible values that can be represented increases - meaning that precision is increased. When going the other way though, precision is lost - meaning we may have to round the value due to the fact that the new type can't represent all of the same values. When this happens, the value of the ``double`` is converted to the closest possible number that can be represented by a ``float``. 

#### Casting Between Types

What actually happens when you cast between types depends on the two types. 

When casting between signed and unsigned integer types (of the same size), the cast doesn't change the bits stored in memory at all - the compiler just reinterprets them as coming from the new type. 
This is done because the overlapping range of values that can be represented by both signed (two's compliment) ints and unsigned ints are represented by the same bit patterns in both representations. 
In short, if the original value lies between zero and the maximum positive value representable with a signed int - then no conversion is needed. 

However, if the original value of the variable does not fall within the valid range of the new type, the value will change. 

For example, when casting from a signed int to an unsigned int if the number is negative (and thus has a 1 in the sign bit position), the new value will be a very large positive number. 
```c
int8_t a = 0xF3; // Value is -13
uint8_t b = a; // Bits are the same (1111 0011), but new value is 243
```

Similarly, when casting from an unsigned to a signed int, large values will become negative numbers. 
```c
uint8_t a = 0xFFu; // Value is 255
int8_t b = a; // Bits are the same (1111 1111), but new value is -1
```

When casting from an integer to a floating point type (or vice-versa), however, the entire bit pattern and representation changes. As such, the actual bits in the processor or memory that store the result must be calculated accordingly.

When casting from an integer to floating point, the result will be the closest representable value possible for the new type. Additionally, when going from a floating point number to an integer, the decimal portion is dropped and only the whole number portion is maintained.  

#### Implicit Casting (and Integer Promotion) in Expressions 

Most of the examples above are implicit casts that occur because we are assigning one type to a new variable of a different type. This is very common occurance when working with multiple types. **However, it is important to remember that implicit casting also occurs in expressions and operations**. 
This includes calculations (such as involving ``+``, ``-``, ``/``, ``*``, ``%``, ``&``, ``^``, ``|``, ``~``, ``>>``, ``<<``) as well as in comparisons (such as involving ``<``, ``>``, ``>=``, ``<=``, ``==``, ``!=``, ``?:``).

When any of the above operations are carried out on integer types that are smaller than an ``int`` (including ``char``, ``short``, ``int8_t``, ``uint8_t``, ``int16_t``, and ``uint16_t``), these types are "upgraded" to an ``int`` if an ``int`` can store the full range of original values, otherwise they are "upgraded" to an ``unsigned int`` before the calculation is carried out. This is called **integer promotion** and designed to help avoid overflow/underflow and preserve the sign/value of the result. 

Additionally, when **types are mixed in an operation**, the type of individual values are often cast to the larger (or more expressive) type _before the operation is performed_.  As explained above, this is primarily due to the fact that the hardware calculation must deal with data of the same type. 

The following rules are used to describe how this casting will occur (in order of priority):
- If one operand is a ``double``, then the other operand (integer or floating point) will be cast to ``double``.
- If one operand is a ``float`` and the other operand is an integer, then the integer will be cast to ``float``.
- If both operands are integers, then both undergo _integer promotion_ as described above, then:
    - If both operands are either signed or unsigned, then the smaller type is converted to the larger one.
    - If the unsigned operand is larger (or if both are the same size), then the signed operand is converted to the unsigned type.
    - If the unsigned operand is smaller and the signed type can represent all possible values of the unsigned type, then the unsigned operand is converted to the signed type.
    - If the unsigned operand is smaller and the signed type can't represent all possible values of the unsigned, then both operands are converted to an unsigned of the same length as the signed operand. 

More details on this process are available [here](https://en.cppreference.com/w/c/language/conversion).

In all of these cases, after the operation is performed, if the result of the calculation is assigned to a variable of another type, then an additional cast is carried out before storing the result.

Lets go through a few more examples to demonstrate how this works.

```c
uint8_t a = 10;
uint8_t b = 255;
uint32_t c = a + b; // Both values are promoted to ints and the result evaluates to 265
uint8_t d = a + b; // This will evaluate to 265, then be cast to a uint8_t w/ value 9
```

In the above example, ``a`` and ``b`` are promoted to type ``int`` before the calculation happens via integer promotion. This promotion allows them to avoid overflow when adding 10 to 255. They are then cast back to either 32 or 8 bits depending on the type of the variable they are being assigned to. When casting to ``uint8_t`` truncation occurs and we recieve a final value of 9.

Now take a look at a more complicated example. 
```c
    int32_t a_int = -1;
    float a_float = -1;
    uint32_t b = 0;
    int32_t c = 1;
    int32_t z_int = a_int / (b - c); // Evaluates to 1
    float z_float = a_float / (b - c); // Evaluates to -0.00000...
```
This is surprising, because we would expect calculations to arrive at the same result!

Lets step through this. ``b`` is of type ``uint32_t`` and ``c`` is of type ``int32_t``. The parenthesis around ``b-c`` give it first priority according the order of operations defined above. Both operands are the size as an ``int``, so integer promotion is not needed. However, ``b`` and ``c`` _are_ of different types. Since both variables are of the same size, ``c`` is cast to match the type of ``b`` or ``unint32_t`` and then we can carry out the subtraction. We then evaluate ``0 - 1`` which overflows up the maximum value for a ``uint32_t`` and the result is also of type ``uint32_t``. When we go to carry out division with ``a_int``, we now need to cast ``a_int`` to a ``uint32_t`` for the same reasons as we casted when doing subtraction. This also wraps to the maximum value for a ``uint32_t``, so the division results in a value of 1. 

On the other hand, when we go to carry out division with ``a_float``, we now have an operand of type ``float`` in the expression. As such, the compiler will implicitly cast our subtraction result (the maximum value for a ``uint32_t``) to a ``float`` before doing division. However because ``a_float`` was defined as a ``float`` and does not need to be cast, we end up dividing -1 by a very large number resulting in a negative number very close to zero. 

#### Summary

As you can see, there is a lot that goes into understanding how types and casting works. However, it is really important to understand these details. If you aren't careful alot of things can go wrong - things like your [rocket exploding](https://en.wikipedia.org/wiki/Ariane_flight_V88) or your self-driving car crashing.

When declaring a variable, remember to consider what its purpose will be.  Ask yourself:

- Does the variable ever need to be negative?
- How much memory should the variable take up?
- What are the upper and lower limits of values of the variable?
- Do I need a decimal (or floating point) number, or will an integer suffice?
- Will I need to combine this variable with other types in expressions in the future?
- If I need to mix types in an expression, how will this affect things?

The above is not an exhuastive list, but if you can answer those questions, you will likely land on a good data type to use.

### Arrays

Arrays are C's method of keeping lists. These lists can be of any data type, but any array can only be defined for a single data type. Nearly every other data structure in C derives from an array.

Here is an example of how to declare an array.

```c
#define ARRAY_LEN 5
// Syntax:
// <type> <name>[<opt. size>] = {<opt. initializers>};
int myArray1[] = {0, 1, 2, 3, 4};   // If no size is specified, the array will be as
                                    // large as the number of elements you include.

int myArray2[ARRAY_LEN] = {0, 1};   // Since the size was explicited declared, indexes 0 and 1 
                                    // get 0 and 1, repspecively, and indexes 2-4 will be initalized to 0.

int myArray3[ARRAY_LEN] = {};       // This array will be fully initialized to 0.

int myArray4[ARRAY_LEN];            // This is a valid array, but the values are not initialized.
```

Note that once an array is declared, its size is unchangable. Be careful when using unbounded (no size given) or uninitialized (no values given) arrays. Both can cause serious errors if used incorrectly!

Once you have declared an array, you can access it with the bracket operator:

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

You'll also see arrays with multiple dimensions.  You can think of these arrays as tables or grids. They can have many axes.

```c
char sentences[10][50]; // an array of 10 arrays of 50 chars; in other words,
                        // 10 strings of max length 50.

uint8_t seats_in_a_building[8][10][50]; // an array representing 8 floors, with
                                        // 10 rooms on each floor, and 50 seats in each room.

uint8_t video_pixels[30*60][1280][720][3]; // an array representing 30 seconds of a 60 fps, 720p video (1280x720),
                                           // with each pixel getting 3 values for red, green, and blue.
```

While higher dimensional arrays are a bit harder to understand intuitively, they are useful because arrays are always stored in **contiguous** memory, meaning that the elements are stored one after the other in a long line in the memory of your computer. To us, a 2D array is represented as a table of values.  To a computer, the array is just one long list.  This grouping allows maximum packing efficiency when storing huge quantities of data. You'll get more experience with large arrays in the next lab.

### Characters and Strings

You have probably noticed by now that C does not include a string data type. Instead, individual letters are represented with the `char` data type. Each decimal value from  0 to 127 is mapped to a specific character per the ASCII (American Standard Code for Information Interchange) standard. For example, the character `A` is represented by the decimal value 65. This is different from the character `a`, which is represented by the decimal value 97. You can find this standard by simply googling "ASCII Table" or by using a website like [ascii-code.com](https://www.ascii-code.com/).

_Note: Some systems also have mappings for integer values 128-255 (called the UNICODE standard).  However, C doesn't actually specify if a `char` is equivalent to a `uint8_t` or a `int8_t`. Each computer will vary whether a char has range -128 to 127 or 0 to 255.  If you aren't using `char`s for being characters, you're better off using one of the `stdint.h` values._

Characters in C can be initalized with single quotes or with their numeric equivalent:

```c
// All of the chars will hold the same value.
char myChar1 = 'A';
char myChar2 = 65;          // 'A' , but declared in decimal
char myChar3 = 0b01000001;  // 'A' , but declared in binary
char myChar4 = 0x41;        // 'A' , but declared in hexedecimal
```

A string is simply an array of `char`s. Thus, you could create a string the same way you would an array. However, it is more common (and far easier) to use double quotes.

```c
// myString1 and myString2 are equivalent
char myString1[] = {'H', 'e', 'l', 'l', 'o', ' ', 'w', 'o', 'r', 'l', 'd', 0};
char myString2[] = "Hello world"; 
```

Notice that when declaring the array with an initializer list, there is an extra `0` added to the end. This is the **null terminator**, which has the integer value 0 and tells the computer the string is finished and no more bytes should be read. When you initalize a string with double quotes, C will automatically add the null terminator on the end (assuming there is enough memory allocated for the whole string to fit). **All strings must have a null terminator!**

```c
// The printf statement below will only print "Hello"
char myString[] = {'H', 'e', 'l', 'l', 'o', 0, 'w', 'o', 'r', 'l', 'd'};
printf("%s\n", myString);
```

Because strings are just arrays, you can iterate over them and manipulate them in the same ways. The example below replaces every `l` in the string with `L`.

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

To help you with these manipulations, the `string.h` library includes various string manipulation functions.  Some of these include:

- `strlen()`, which returns the length of a string (not including the null terminator)
- `sprintf()`, which works with the same formatting specifiers as `printf()`, but instead of printing to the terminal, it "prints" to an array or memory location.
- `strcpy()`, which allows you to copy characters from one string into another.
- `strncat()`, which allows you to concatenate the first `n` bytes of string A to the end of string B.
- `strncmp()`, which allows you to compare `n` bytes of string A and B.

#### Using Functions with Arrays and Strings

As with all variables, arrays are stored in memory. If a variable (or an array) is declared and created within a function, it is stored in the memory belonging to that function. When a function returns, the memory associated with it is released and can be used for other things. This causes big problems if somewhere else in the code is still trying to access that memory. To account for this problem, arrays (and strings) that are needed after a function returns should not be declared or defined within that function. 

Instead, these functions require the user to pass around pointers to the array as a parameter. Since an array's base is basically a pointer under the hood, passing arrays around like this means changes made to the array by a function will persist, even after the function is done.

```c
// Bad function; the array data will be lost once the function returns
char* makeAString_bad() {
    return "I made a string!";
}

// Good function; the caller will declare an empty array and then pass a pointer to it.
// The function below will then populate or modify the string as needed.  
// Note though that the stringToFill will have to be large enough to fit the string or weird and bad things will happen.
void makeAString_good(char* stringToFill) {
    stringToFill = "I made a string!";
}
```

## Debugging

As a programmer at any level, you will need to identify problems in your code and fix them. Below is some advice on different programming and debugging methods.

### Intentional and Incremental Programming

Intentional and incremental programming are two important mindsets to have while diving into software development. Intentional programming is a mindset that prioritizes writing code that is **clear, concise, and easy to understand**. In other words, the emphasis is on writing code that accurately reflects your intentions rather than trying whatever and seeing if it sticks. Approaching a solution in code intentionally will inevitably be a lot more productive and easier to maintain and extend in the future.

Incremental programming, on the other hand, involves **dividing the solution you are trying to create into small, manageable chunks** that can be developed and tested individually. This allows for a more flexible, iterative approach to software development. This means that changes can be made easily and quickly, without having to completely overhaul the entire solution when something crashes or changes.

Handling large amounts of data in C is no small task (as you will see in the next lab). It requires attention to detail and a good understanding of the code you are writing and what it does. As the projects you do become more complex and involved, it is important to remember that **the best line of defense against buggy code is a good offense: intentional and incremental programming.**

### Trace debugging

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

You'll be surprised to see how effective logging or printing variable values can go to ensure a smooth development experience. Trace debugging helps identify and fix bugs, improve the performance of your program, and improve the overall reliability of your software.

### Using a Logging Library

Using regular `printf` statements to trace debug has some problems. If you have a lot of print statements in your code, it can be difficult to keep track of them all, and when you are done debugging, you have to go back and remove each one.

To make this easier, you can use a logging library to help you debug your code. Logging libraries give you control over when the log messages are written to the console, allowing you to filter out messages based on their severity or other criteria. They also contain many helpful features for tracing messages back to the code. One such library is [log.c](https://github.com/rxi/log.c). This library provides a simple logging interface that you can use to log messages to the console. We have provided the `log.c` and `log.h` files in the lab repository. To use this library, you will need to include the `log.h` header file at the top of your code:

```c
#include "log.h"
```

and link the `log.c` file when you compile your program:

```bash
gcc main.c log.c
```

To use the logging library, you can call any of the `log_` functions to log messages to the console. For example, you can use the `log_info` function to log an informational message:

```c
log_info("This is an informational message");
```

It will output something like this:

```c
11:56:31 INFO  main.c:9: This is an informational message
```

You can also use the `log_debug`, `log_warn`, and `log_error` functions to log debug, warning, and error messages, respectively. Any `log_` function takes the same arguments as `printf` (and adds a newline for you!), so you can use format specifiers to include variables in the log message:

```c
int value = 42;
log_info("The value is %d", value);
```

## Lab Requirements and Instructions

In your repository, you will find files called `data.c` and `custom_strings.c`, along with their equivalent `.h` files.  These files include various functions that you will either need to write or debug.  Descriptions for each function can be found in the `.h` files.

Your repository includes a `main.c` for your own use to print and debug the code.  However, for pass off, you will compile the `data.c` and `custom_strings.c` files with the `lab4_passoff.o` file.  This is a binary file that is already compiled and ready to be linked to your code.

To compile and run your program (as defined in `main.c`) you can run the following commands:

```bash
gcc main.c data.c custom_strings.c -o lab4_main
./lab4_main
```

To compile and run the pass off script, you can run:

```bash
gcc lab4_passoff.o data.c custom_strings.c -o passoff
./passoff
```
The gcc command above will compile your `.c` files and then link them together with the `lab4_passoff.o` file that contains our test code. 

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

- Pass off to a TA:
  - Show the output of the passoff executable
  - Show how you implemented your functions in `data.c` and `custom_strings.c`

- Complete the Learning Suite pass off quiz.

- To successfully submit your lab, you will need to follow the instructions in the [Lab Setup]({{ site.baseurl }}/lab-setup) page, especially the **Committing and Pushing Files** section.



## Explore More

- [Data Types in C](https://www.tutorialspoint.com/cprogramming/c_data_types.htm)
- [Automatic Conversions in C](https://en.cppreference.com/w/c/language/conversion)
- [C Arrays and Their Storage in Memory](https://www.geeksforgeeks.org/c-arrays/?ref=shm)
- [Functions in String.h](https://cplusplus.com/reference/cstring/) (note that this library is technically for C++, but the C library is included in that language)
