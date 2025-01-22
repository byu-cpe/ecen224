---
title: Camera
number: 8
layout: lab
---

## Objectives

- Take a picture with the camera
- Handle raw data in a buffer
- Write a `.bmp` file
- Update viewer interface

## Getting Started

Use the GitHub Classroom link posted in the Learning Suite for the lab to accept the assignment. Next, ssh into your Raspberry Pi using VSCode and clone the repository in your home directory. **This lab should be done in VSCode on your Raspberry Pi. Make sure the lab is top level folder of your VSCode editor.**

## Overview

In the past labs, much time has been spent making sure the files of the camera are easily accessible and represented on the LCD screen of your Pi Z2W kits. In this lab you will be expected to integrate a camera module to your design and take pictures. 

In the [Getting Started Lab]({% link _labs/getting-started.md %}), you should have already physically installed your camera, using the **longer** of the two ribbon cables in the correct orientation.

### Creating Large Buffers
The skills that you need to complete this lab are very closely related to the skills you needed for the last lab. You are expected to capture a very large buffer of information that comes from a camera and store it in a buffer (`uint8_t *`) in C. 

Normally, if we knew the size of these buffers, we would be able to create a regular array in C:

```c
uint8_t my_new_buf[256];    // Create a buffer of a known size
```

However, this becomes difficult when our buffers become very large. For example, the buffers that you dealt with in the **Image** lab had millions of values in it! This is much too large for variables that are declared on the stack. Instead, you will need to use `malloc()` in conjunction with an unbounded array:

```c
uint8_t *my_new_buf = malloc(sizeof(uint8_t) * SIZEOFYOURBUFFER);

// Whenever we are done with a malloc'd buffer in C, don't forget to call the following line:
free(my_new_buf);
```

Malloc takes one argument, the number of bytes to allocate for the buffer. It is common to provide the size of the data type the buffer will be (in this case, `uint8_t` so `sizeof(uint8_t)`) and the number of elements you want. This creates your buffer in the *heap* where there is much more space for larger variables like this. However, whenever we declare something in C and manage its memory, we must remember to call `free()` on the object once we are done. If this is not done, then there will be memory leaks inside of your program which could potentially grind your system to a halt.

### Writing to a File
You'll notice in your lab files for this lab, you are provided with a `camera.h` and a partially filled in `camera.c`.

In this lab you will be expected to capture image data from a camera, save it to a file, and then show it on the screen of your new smart doorbell. In order to write information to files in C, you will be expected to know one more function than you already do: `fwrite()`.

Much like reading a files as you did in the last lab, to write, you will need to open a file using `fopen()`. You may be thinking, how can I open a file when it doesn't exist yet? With `fopen()` you can name the file you want to create and what kind of mode you want to create it in by using `fopen()`. For example, to create a new file named "banana.txt", I would do the following:

```c
FILE *fp;
fp = fopen("banana.txt", "w");
```

The second argument in `fopen()` is mode we want to open the file in. This allows the operating system to know what type of content will be written to the file. In our example above, we want to write ASCII text to the file (as implied with the `.txt` extension), so we use the `w` mode. Other special modes for `fopen()` can be seen in the following table below.

| Mode | Description                                                                                                                                                                                                                                                                                                                                                                                                     |
| ---- | --------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------- |
| `r`  | Open text file for reading.  The stream is positioned at the beginning of the file.                                                                                                                                                                                                                                                                                                                             |
| `r+` | Open for reading and writing.  The stream is positioned at the beginning of the file.                                                                                                                                                                                                                                                                                                                           |
| `w`  | Truncate file to zero length or create text file for writing.  The stream is positioned at the beginning of the file.                                                                                                                                                                                                                                                                                           |
| `w+` | Open for reading and writing.  The file is created if it does not exist, otherwise it is truncated.  The stream is positioned at the beginning of the file.                                                                                                                                                                                                                                                     |
| `a`  | Open for appending (writing at end of file).  The file is created if it does not exist.  The stream is positioned at the end of the file.                                                                                                                                                                                                                                                                       |
| `a+` | Open for reading and appending (writing at end of file). The file is created if it does not exist.  Output is always appended to the end of the file.  POSIX is silent on what the initial read position is when using this mode. For glibc, the initial file position for reading is at the beginning of the file, but for Android/BSD/MacOS, the initial file position for reading is at the end of the file. |

It is considered good practice to add a `b` at the end of the letter if you intend to write to a file in a binary format (i.e. `.bmp` files) to indicate your intent, however this extra `b` is not enforced on many modern systems like Linux.

After the file is opened, you can then use the `fwrite()` command to write to this opened file. Instructions on how to use `fwrite()` can be found in the corresponding link in the **Explore More!** section at the bottom of the page. Don't forget to use `fclose()` on the file pointer when you are done writing the file!

## Requirements

- First verify your camera is working. To do that, run the following command. If this command succeeds, that means you have correctly connected the camera. If the command fails, such as a camera not found error, then you need to fix the connection.

```
libcamera-still -n --immediate -e bmp --width 128 --height 128 -o camera-test.bmp
```
  
- Copy the three functions you implemented (`get_entries`, `draw_menu`, and `draw_file`) and your `main` function from last lab into your main.c for this lab.

- Copy your `remove_color_channel` and `or_filter` functions into `image.c` from your Image Lab code. You will be using this code later on.

- We have changed the contents of the viewer folder slightly. Make sure your code works as expected. If it doesn't fix it. You should be able to scroll through the list and display the contents of a file by pressing the right button.

- Assign the center button to do the following:
    - Clear the screen and write a message telling the person you are taking a picture. You can write what ever you want (e.g., "Say Cheese!").
  
    - Take a picture with the Pi Z2W's camera using the `camera_capture_data` function found in `camera.c`.
  
    - Save the raw data received from the capture to a `.bmp` file. You will have to implement the `camera_save_to_file` function found in `camera.c`. The file must be named `doorbell.bmp` and saved in the `viewer` folder.

    - Update the contents of your menu. It should now include the newly saved file.

    - Display the contents of the picture using the newly added `display_draw_image_data` function. However, before you can use that function, you must convert the raw `uint8_t *` buffer into a `Bitmap` struct. This is because `camera_campture_data` returns the bytes of the BMP image file, which includes the BMP header. `display_draw_image_data` is expecting raw pixels values without a header. To process the header and pixel values, call `create_bmp` located in image.c. This function will parse the header and set the `pxl_data` struct field to just the raw pixel data. A diagram of this process can be seen [here]({% link assets/camera/data_flow.png %}).

    - While the contents of the picture is showing, you must handle the following:
      - If the right button of the d-pad is pressed, filter out the red color of the image.
      - If the left button of the d-pad is pressed, filter out the blue color of the image.
      - If the up button of the d-pad is pressed, filter out the green color of the image.
      - If the down button of the d-pad is pressed, or-filter the image.
      - If the center button of the d-pad is pressed, exit from the submenu by clearing the picture and displaying the menu.

- Your program must not have any memory leaks. This means for every `malloc`, you need to have a corresponding `free` call. `create_bmp` allocates memory for the `Bitmap` struct using `malloc`. You must free that space by calling `destroy_bmp` when you are done with the struct.

- All other functionality should stay the same.

Here is a demo showing the different features of the lab:

<div class="row">
    <div class="mx-auto">
        <video height=500 controls>
            <source src="{% link assets/camera/demo.mp4 %}" type="video/mp4">
            Your browser does not support the video tag.
        </video>
    </div>
</div>

## Submission

- To pass off to a TA, demonstrate your doorbell running your program that fulfills all of the requirements outlined above. You must also show the TAs where you are calling `free` and `destroy_bmp`.

- Take the Pass off Quiz on Learning Suite.

- Follow the instructions in the README file in the repository to write your own README for this lab. Include your name, section, semester, and lab title. A good README should answer the following questions:
  - What is the purpose of this project and its code/files?
  - What is the structure/organization of the project files?
  - How do you build and run the code in this project?

- Add and Commit all of your updated files (and your README) as explained under **Committing and Pushing Files** on the [Lab Setup]({{ site.baseurl }}/lab-setup) page. Remember that while these instructions give general information, you need to add and commit all of the files you have modified or created in this lab. 

## Explore More!

- [`fwrite()` in C](https://www.tutorialspoint.com/c_standard_library/c_function_fwrite.htm)

- [Memory Allocation in C](https://www.geeksforgeeks.org/dynamic-memory-allocation-in-c-using-malloc-calloc-free-and-realloc/)

- [Connect Camera to Raspberry Pi](https://www.arducam.com/raspberry-pi-camera-pinout/)
