---
title: Threaded Client
number: 10
layout: lab
---

## Objectives

- Deal with technical debt
- Learn to multitask in C
- Create a POSIX thread with arguments

## Introduction

One of the last fundamentals to know about programming in general is how to make your program do multiple things at once. Until this point, we have written **single-threaded programs**. This means that our programs have only followed a single path of execution while it runs (i.e., the program can only focus on one task at a time). However, many of the problems that we solve in the real-world are much more complex and may require us to interact with many facets of the problem simultaneously. In the lab for this class, creating a useful smart doorbell is definitely one of those multifaceted problems. Consider the following:

- How can I upload my photo to the server while viewing it?

- How can I actively be monitoring the status of my upload while also drawing to my screen in a loop?

While considering the previous questions, you may have noticed that with your current skill set, these problems seem difficult to solve without a complex algorithm or multiple programs running at once. By using threads, we can sidestep a lot of this headache by grouping different tasks/functions into distinct threads and call them from our main thread.

### Creating a Thread

Threads in Linux programming are known as [POSIX](https://en.wikipedia.org/wiki/POSIX) threads (or pthreads for short). `pthread_t` is the data type used to uniquely identify a thread:

```c
#include <pthread.h>

pthread_t my_thread;
```

This object is used inside a range of different functions found inside the `pthread.h` library. The most important function for this lab is `pthread_create()`, which allows us to spawn a new thread and provide the function that it will run. Consider the simple threading program below:

```c
#include <stdio.h>
#include <unistd.h>   // Library that includes sleep()
#include <pthread.h>  // Library that includes pthreading types and functions

void *print_msg(void *unused)
{
    while(1)
    {
        printf("I'm in a thread!\n");
        sleep(2);     // Sleeps for 2 seconds
    }

    return NULL;
}

int main()
{
    pthread_t my_thread;
    pthread_create(&my_thread, NULL, print_msg, NULL);

    while(1)
    {
        printf("I'm in main!\n");
        sleep(1);
    }

    return 0;
}
```

To compile using the `pthread.h` library, you will need to add `-pthread` to your normal `gcc` compile command.

Let's take a closer look at what is going on in the `pthread_create()` function. The arguments that are expected in the function are as follows:

1. The address of your thread struct. This is how your program will know how to identify your thread in the program.

2. This argument expects a special `pthread_attr_t` struct pointer. Since we are covering the basics of threading in this class, this field is ignored by putting a `NULL` in its place.

3. A pointer to the function we are calling. Function pointers can be provided by giving the name of the function. Make sure you only provide the name, and don't *call* the function (eg. `print_msg` instead of `print_msg()`). If you like, you can put a `&` in front of the function name to make it more obvious it's a function pointer (eg. `&print_msg`), but this is not required by the compiler.

4. A pointer you provide to this argument will be passed as an argument to the function you are running in the new thread. This is a great way to provide information to the new thread. Notice that for this example it is `NULL` since our function doesn't require any special arguments. The next section has more information about how to pass arguments into a thread.

You'll notice that both our `main()` function and our `print_msg()` function have `while(1)` loops that will run indefinitely. Because threading allows us to run different functions simultaneously, we will see output that looks something like this:

```bash
I'm in a thread!
I'm in main!
I'm in main!
I'm in a thread!
I'm in main!
I'm in main!
I'm in a thread!
I'm in main!
I'm in main!
I'm in a thread!
I'm in main!
I'm in main!
...
```

### Passing in Arguments to a Thread

While the previous example may have been interesting in the sense that we've learned to use threads, it is pretty lackluster in the sense that we can't get any information into or out of the thread. To do this, we will need to understand working with `void *` and allocating memory.

#### void *: The Generic Pointer

A `void *` is the generic pointer in C. It is used to store a memory address, but does not indicate the type of data stored at the address. This means that any type of pointer can be cast to or from a `void *` object. It is up to you, as the programmer, to know what you passed into the function and cast it to the correct pointer type. This datatype is very useful when using threading functions because it allows us to pass in the address of *any* type of data to the function we are spawning. The threading function also *returns* a `void *` data type, allowing it to pass back a pointer to any type of data. This accounts for why our `print_msg()` function in the previous example program's return type was a `void *`.

#### Example

Consider the following program:

```c
#include <stdio.h>
#include <unistd.h>  // Library that includes sleep()
#include <stdlib.h>  // Library that includes malloc()
#include <pthread.h> // Library that includes pthreading types and functions

void *print_num(void *arg)
{
    while(1)
    {
        // Notice that we have to cast the void * back to an int * and then we use the * to get the value inside
        printf("The number is:\t%d\n", *((int *) arg));
        sleep(2);     // Sleeps for 2 seconds
    }

    return NULL;
}

int main()
{
    int *i = (int *)malloc(sizeof(int));    // Create an int pointer and give it enough space to hold an int.
    *i = 0;                                  // Set the value inside the pointer to 0

    pthread_t my_thread;

    // Notice that we have to cast i to a void *
    pthread_create(&my_thread, NULL, print_num, (void *) i); 

    while(1)
    {
        *i = (*i) + 1;                      // Increment the value inside of i
        printf("I'm in main!\n");
        sleep(1);
    }

    return 0;
}
```

Let's take a closer look at this code example. The `pthread_create()` function is spawning a new thread to run the `print_num` function. We want to pass an integer to this function. Since we always use a `void *` to pass data to a function run by `pthread_create()`, we need to allocate memory for an integer and provide it's address as the last argument to `pthread_create()`.

You'll notice some things about how we've done this:

- We have allocated the space for the integer using `malloc`. While we could have simply declared an integer (`int i;`), this would create the integer on the stack. In general, it's bad practice to pass addresses on the stack to other threads as they may continue to execute after stack variable goes out of scope. Another alternative to `malloc` is to declare the integer as a global variable.

- To set the value of the integer to zero, we needed to refer to the pointer's contents: `*i = 0;`

- When we passed it into `pthread_create()`, we needed to cast it from a `int *` to a `void *`.

Now that we have provided the address of our integer to `pthread_create()`, let's take a look at how it is received by the `print_num()` function:

- The address is provided as an argument to the function with type `void *`.

- To use `arg` we need to do two things:

    1. Cast `arg` back into the type that we expect (in this case an `int *`).

    2. Deference it with a `*` to get the integer value pointed to by this address (the `*` in front of the cast statement).

### Return Values from Threads

To get a return value from a thread in C, you would need to use the `pthread_join()` function. For the purposes of this lab, we will not be using `pthread_join()`, but you can read more about it in the **Explore More!** section. `pthread_join()` is **NOT** recommended for this lab.

## Procedure

### Requirements

- In order to use the threading library, you need to include the library in source code and your compilation.
  - Add `#include <pthread.h>` and `#include <unistd.h>` to your headers in `main.c`.
  - Add `-pthread` to the `CFLAGS` variable in your Makefile.
  - Before you go any farther, try `make` on your project to make sure everything is still working.

- Write a function called `send_image`. This function should take care of connecting to the server, sending the image, receiving the response, and closing the socket. You need to figure out the correct signature for this function.

- Run your code using this newly created function (not using threads) to make sure that everything is still working as expected. Notice that sending data to the server is slow and your menu gets blocked while you are waiting to send the data.

- Instead of calling `send_image` directly, use a thread to call the function instead. Now that calling `send_image` is no longer blocking, you might need to refactor your code to not call `free` right after the thread is started. **Warning: you should not call any display functions in a thread.** The display functions are not thread-safe, meaning that if multiple threads call the same function, weird things can happen. You should only call the display functions in your main loop.

- Create a status bar the size of your highlight bar at the bottom of the screen with a blue background:

  - When your image is being sent in a thread, the text of the bar should say "Sending..." with the bar as a blue background.

  - When your image is sent successfully (i.e. the thread reached the end successfully), change the color of the status bar to green and show "Sent!" as the text. *How are you going to know when the thread is done sending? A global variable that helps you to maintain what state you are in might be helpful.*

  - The status bar should should stay green with the sent message for 2 seconds. After the 2 seconds hide the status bar. *How do you know when it has been 2 seconds? Consider editing your `send_image`.*

- Ensure that your menu is still responsive and you can navigate the menu while the status bar is on screen (while the image is being sent and for the two seconds after).

Here is a demo showing the different features of the lab:

<div class="row">
    <div class="mx-auto">
        <video height=500 controls>
            <source src="{% link assets/threaded-client/demo.mp4 %}" type="video/mp4">
            Your browser does not support the video tag.
        </video>
    </div>
</div>

### Pass Off & Submission

- Your program must compile without warnings or errors. Compile your program with the `-Werror` flag to ensure that it doesn't.
- Pass off to a TA by demonstrating your doorbell running your program that fulfills all of the requirements outlined above.
- Take the Pass off Quiz on Learning Suite.
- Follow the instructions to update your `README.md` file with the new features of this lab.

## Explore More

- [`sleep()` man page](https://man7.org/linux/man-pages/man3/sleep.3.html)
- [`pthread_create()` man page](https://man7.org/linux/man-pages/man3/pthread_create.3.html)
- [`pthread_join()` man page](https://man7.org/linux/man-pages/man3/pthread_join.3.html)
- [pthreads in C](https://www.geeksforgeeks.org/multithreading-in-c/#)
- [Code for the server part of this lab](https://github.com/byu-ecen-224-classroom/client_server)
