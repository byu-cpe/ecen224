---
title: Display
number: 6
layout: lab
---

## Objectives

- Become familiar with the LCD and button shield for the Pi Z2W.
- Learn to use a "vendor" library to control different hardware accessories.
- Associate graphical events with specific inputs.
- Get experience using a Makefile

## Getting Started

Use the GitHub Classroom link posted in the Learning Suite for the lab to accept the assignment. Next, ssh into your Raspberry Pi using VSCode and clone the repository in your home directory. **This lab should be done in VSCode on your Raspberry Pi. Make sure the lab is top level folder of your VSCode editor.**

## Overview

<figure class="image mx-auto" style="max-width: 750px">
  <img src="{% link assets/display/lcd-button-shield.png %}" alt="Units of the course.">
  <figcaption style="text-align: center;">The Waveshare 1.44" LCD and button module. This is known as a Hardware Attached on Top module, or a HAT for short.</figcaption>
</figure>

Single board computers (SBCs) like the Pi Z2W are remarkable partly for their ability to add on hardware in the form of HATs (Hardware Attached on Top; sometimes called "shields" in the [Arduino](https://www.arduino.cc/) community). These useful devices use the **GPIO pins**, one of the most attractive features of an SBC, to extend the feature set dramatically.

GPIO (General Purpose Input/Output) pins are the main method of connecting new peripherals to an SBC. A **peripheral** is any device that is connected to a computer to enhance its functionality (i.e a mouse, keyboard, monitor, printer, etc). The Pi Z2W GPIO pins allow for many peripherals, such as sensors, motors, etc., to be connected all at once. This makes it a great choice in systems that interface with specialized or custom hardware.

In this lab we will use the Waveshare 1.44" HAT, which uses all 40 of the GPIO pins on the Pi Z2W. In return, the HAT provides a small Liquid Crystal Display (LCD) screen, a directional button pad (d-pad), and a set of action keys.

### BCM2835 Library

#### Installation

In order to interface with the GPIO of the Pi Z2W, we need to install a library.

1. First make sure that you are in your home directory on the Pi Z2W:

    ```bash
    cd ~
    ```

2. Then use `wget` to download the compressed archive with the library from the developer's website:

    ```bash
    wget http://www.airspayce.com/mikem/bcm2835/bcm2835-1.71.tar.gz
    ```

3. Once the file has downloaded, uncompress the archive using the `tar` utility:

    ```bash
    tar xvfz bcm2835-1.71.tar.gz
    ```

4. Go into the uncompressed directory:

    ```bash
    cd bcm2835-1.71
    ```

5. Run the `configure` script. This will provide operating system information to the library so it knows how to configure itself to work specifically on the Pi Z2W architecture:

    ```bash
    ./configure
    ```

6. Once the `configure` script has prepared the necessary modifications, run the `make` command to compile library's source files (i.e all its `.c` files)

    ```bash
    make
    ```

7. Finally, move the compiled binaries to the folder in the operating system where your `gcc` compiler looks for system libraries:

    ```bash
    sudo make install
    ```

The library is now installed and accessible to the operating system! Your terminal is still in the library directory, so be sure to navigate back to your Lab 6 project directory before continuing. 

#### Compiling into a Project

Now that the `bcm2835` library is installed, we can use it in any C program that we like! This comes especially in handy for our new LCD and button HAT. However, since this is an installed library and not a default one, we have to let `gcc` know that we are trying to include it in the compilation process. This is done by adding the `-l bcm2835` flag to our normal `gcc` compilation command. The `-l` lets `gcc` know we are including a custom system library, while the `bcm2835` part is just the name of the library itself.

In this lab you won't actually run `gcc` yourself, so you don't need to worry about including this flag. We will compile our project using Make, which will handle the `gcc` calls for us, including adding the `-l bcm2835` flag. We'll talk more about Make later.

### Drawing to the Screen

In this lab you will be responsible for writing a `main.c` file that will draw shapes and images to the LCD screen. The library responsible for this is found in the `lib/display.h` library file in the lab repository. This is a wrapper file that interacts with the `bcm2835` library we installed. There are many functions that can accomplish various techniques such as drawing shapes or writing text. Become familiar with the `display.h` and read the corresponding comments.

Before the LCD can be used, you will need to call the `display_init()` function once in your code at the beginning of `main`.

#### Orientation and Dimensions

When drawing on the screen, it is important to have a good mental model of what the coordinate system of the screen is like. For this particular LCD module, we have set up the axes like so:

<figure class="image mx-auto" style="max-width: 750px">
  <img src="{% link assets/display/axes.png %}" alt="Units of the course.">
</figure>

The height and width of the screen are `#define`d in the `display.h` file as `DISPLAY_WIDTH` and `DISPLAY_HEIGHT`. These values can be useful if you are trying to define coordinates for shapes relative to those points.

#### Colors

Most of the `display` functions take in a color parameter to give color to the shapes you are drawing or the text that you are writing. These colors are `#define`d in the `lib/colors.h` file.

Example:

```c
display_draw_rectangle(0, 5, 128, 15, BYU_ORANGE, true, 1);
```

where `BYU_ORANGE` is `#define`d in `colors.h`.

#### Fonts

Part of the `display` library allows you to draw strings on the screen. One of the parameters for drawing the string to the screen is selecting a font. These fonts are all located in the `fonts` folder and are accessible through the `fonts.h` library. Each font represents different font sizes. To use the fonts in the `display_draw_string()` function, you will need to pass the address of the font you desire:

```c
display_draw_string(10, 10, "Hello, World!", &Font8, WHITE, BLACK);
```

The following fonts are available:

- Font8
- Font12
- Font16
- Font20
- Font24

### Interacting with Buttons

To read the state of the d-pad you will be using the functions found defined in the `buttons.h` library interface. When a button is actively being pressed, the function to read it will return a `0`, else if it unpressed, it will return a `1`.

Example:

```c
if (button_up() == 0) {
    // Do something upon detecting button press

    while (button_up() == 0) {
        // Do something while the button is pressed
        delay_ms(1);
    }
}
else {
    // Do something while the button is not pressed
}
```

Before the buttons can be used, you will need to call the `buttons_init()` function once in your code at the beginning. *If you don't call `buttons_init()` before you start using the buttons, weird things can happen!* You're code might work, but then when you reboot or rerun your code, it might not!
Note that you must call `display_init()` before you call `buttons_init()` or you will get a segmentation fault.

### Device delay

You will see in your `main.c` function that your code will loop infinitely. This means that anything inside the `while(true)` loop will repeat over and over until the program is terminated by the user through the shell. Running a `while(true)` loop without any sort of control can cause system resources to be eaten up and cause your program to be run inefficiently. For this, we have provided the `delay_ms()` inside the `device.h` library. This will allow you to essentially create a wait time in the execution of your loop. This is handy if you want to draw something to the screen and have it only appear for a certain amount of time before the logic in your program goes on.

### Logging

We also give you `log.h` and `log.c` files that you can find under the `/lib` folder. The functions in these files (`log_info()` or `log_debug()` for example) can be used like printfs to output data to the terminal (or optionally a log file). However, these only print to the terminal if the current **log level** is equal to or lower than the log command.  For example, if the log level is set to `LOG_INFO`, then `log_debug()` messages won't show, but `log_info()` through `log_fatal()` messages will.  You can change the current log level with the `log_set_level()` function. These tools can be used to have printfs that you don't need to remove, but that can be turned on/off when you are debugging.

### Compiling with Make

By this point, you also may be realizing that large projects can become unweildy in compiling.  This lab, for example, contains eleven `.c` files.  To compile it yourself, you would have to enumerate each of them in the command line:

```bash
gcc -o main -l bcm2835 main.c buttons.c device.c display.c lcd.c log.c font8.c font12.c font16.c font20.c font24.c
```

Compiling like this will work, but has a couple problems:

1. You are recompiling each file each time. If you make a single change to your `main.c` file, each other file would also be recompiled. For large projects, this can take a lot of time.
2. Any typo in your command would halt the compilation. Repeatedly typing in this command (or even copy/pasting it) could propogate errors and waste a lot of time.

Early C programmers recognized these issues too. So, just four years after C was released, the Make software was created. Make revolves around a single file called the `Makefile`. The inside of a `Makefile` looks a lot like bash scripts (like the one you made in Lab 2); you can create "rules" that perform a set of command line operations and generate files for you. So, instead of the long `gcc` command that you used up above, you can run one simple command:

```bash
make
```

And everything will be done for you.

Using `Make` speeds up the compiling process by performing intermediate steps in the compilation process. Consider the example from above where you only updated `main.c`; to recompile, instead of recompiling every single `.c` file, you can create a `.o` file for each source file. Recall that `.o` files are compiled and assembled, but not yet linked together. Make generates a `.o` for each source file on its first run. For subsequent recompiles, Make is smart enough to only recreate `.o` files if their corresponding `.c` file has been updated. IT then links the new `.o` to all the old, unchanged ones to generate the executable.

**In this lab going forward, to compile your code, you should use `make` instead of `gcc`.**  The Makefiles will be provided for this class, so you don't need to entirely understand how they work right now. However, you will see them in future classes, so it may be worth taking a look at them.

### Executing

Compiling with the provided Makefile generates an executable called `main`. You run this executable the same way you run any other, using `./main`, with one exception. Accessing the HAT hardware requires special permissions, so you will need to run the exeutable with `sudo` (e.g., `sudo ./main`), otherwise you will likely see a `segmentation fault`.

## Requirements

You will demonstrate your understanding of the `display` and `buttons` libraries and how to use them by accomplishing the tasks listed below:

1. Implement each of the following functions:

    a. **clearScreen**: Clears the screen to white

    b. **drawHelloWorld**: Draws "hello world" on the screen 10 times in different colors

    c. **drawChars**: Draw 10 chars of various values, sizes, colors, and locations onto the screen

    d. **drawStars**: Draw at least 20 stars onto the screen

    e. **drawFlag**: Uses at least 5 functions from `display.h` to draw any flag you wish.  3 of the `display_draw_###` functions must be unique.

    Make and test each of these functions *before* you create the menu. You can test functions by calling them in your while(true) loop in main(). 

2. Implement a menu functionality using `drawMenu` and `main`.  Your menu should have the following functionality:

    a. The menu draws the strings contained in the "entries" array. Use an 8 or 12 point font.

    b. When no button is pressed, nothing happens.

    c. One entry is selected at a time, which is represented by being highlighted in a different color.

    d. Pressing the up or down button will change the selected entry.

    e. Pressing the right button on the selected entry will run the function with the associated name.  For example, the pressed the right button when the entry "Flag" is selected will cause the `drawFlag` to run.

    f. After a right button press, the code should wait 2 seconds, then redraw the menu.

    g. The selection should "wrap" from top to bottom. In other words, if you press the down button while you have the bottom entry selected, the selection will move to the top entry, and if you press the top entry when the bottom

Here is a demo of the completed lab:

<div class="row">
    <div class="mx-auto">
        <video height=500 controls>
            <source src="{% link assets/display/lab6demo.mp4 %}" type="video/mp4">
            Your browser does not support the video tag.
        </video>
    </div>
</div>

## Submission

- Pass off with a TA, by demonstrating your doorbell running your program that fulfills all of the requirements outlined above. Your code should compile without warnings or errors.

- Take the Pass off Quiz on Learning Suite.

- Follow the instructions in the README file in the repository to write your own README for this lab. Include your name, section, semester, and lab title. A good README should answer the following questions:
  - What is the purpose of this project and its code/files?
  - What is the structure/organization of the project files?
  - How do you build and run the code in this project?

- Add and Commit all of your updated files (and your README) as explained under **Committing and Pushing Files** on the [Lab Setup]({{ site.baseurl }}/lab-setup) page. Remember that while these instructions give general information, you need to add and commit all of the files you have modified or created in this lab.

## Explore More

- [Waveshare 1.44" LCD HAT](https://www.waveshare.com/1.44inch-lcd-hat.htm)
- [Raspberry Pi GPIO Breakdown](https://pinout.xyz)
- [Different types of Pi HATs](https://www.pishop.us/product-category/raspberry-pi/raspberry-pi-hats/)
- [Retro Handheld Console with LCD HAT](https://pihw.wordpress.com/guides/mini-retro-pi-setup/)
- [Crazy Pi GPIO Usage](https://sss.readthedocs.io/en/latest/Hardware/Final%20product/)
- Makefile Tutorials: [here]({{ site.baseurl }}/recitation/make-walkthrough) and [here](https://makefiletutorial.com/)
