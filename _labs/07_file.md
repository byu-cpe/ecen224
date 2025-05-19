---
title: File
number: 7
layout: lab
---

## Objectives

- Read in files into a program
- Create the necessary logic to handle an interface
- Filter out files by pattern

## Introduction

Learning to interact with the peripherals of the Pi Z2W is what allows us to take a simple single-board computer and turn it into a system with a more directed purpose. You may have noticed by looking at the future lab names of this class that the culmination of this project is to make a smart doorbell system. To that end, a HAT with an LCD screen and some buttons has been provided. Its purpose is to provide a way for a user of your smart doorbell to directly interact with the computer that is powering it.

In the last lab you were able to draw practically anything to the LCD screen by utilizing the provided vendor libraries. You were also able to correlate these drawing events with button presses. In this lab you will take this practice and put it towards a more practical use: creating a photo and log viewer for your doorbell.

The photo and log viewer of this lab consists of several components: scanning the folder where the intended files live, filtering out any files that the photo and log viewer doesn't care about, and displaying the intended content of each file.

### Scanning the images Folder

In this lab you are expected to scan through a current folder and make a list of strings that indicate the contents of that folder. While this is as trivial as a simple `ls -al` command in the shell, it becomes more challenging when trying to achieve the same objective in C. Implement the following directions from the sections below to create an array of strings that contain all of the files of interest inside the `viewer` folder. Your scanning function should filter out anything that doesn't end in `.bmp` or `.log`.

#### DIR Pointers

Before you can read in the contents of a directory in C, you will need a variable that holds some information about the directory that you are reading. For this, you can use a `DIR` pointer. The `DIR` pointer represents a _directory stream_, or an ordered list of all the entries in a directory.

This object will allow us to look at the location of where our directory lives and then use this a pointer to iterate through files in a directory and (for our purposes) retrieve those file names. To make this pointer:

```c
DIR *dp;    // Creates a directory pointer dp
```

#### Folder operations

With your newly allocated `DIR` pointer, you can use the following functions that will help us interface with the directory in our code.

- `opendir()` and `closedir()`

    In order to open a directory while using the `DIR` pointer, you can do the following:

    ```c
    DIR *dp;
    dp = opendir("my-folder");

    if (dp == NULL) {
        // Folder does not exist
    }

    // Use dp to read the contents of the folder
    ```

    This opens the `my-folder` directory into code and puts the `dp` pointer at the top of this directory. When you are done iterating through the directory, you will need to close it by using the following call to `closedir()`:

    ```c
    closedir(dp);
    ```

#### Parsing through the Folder

`DIR` points to the location of a file in a directory, but does not give any information about what is at that location. To access some basic information about the file such as its name and size, you need an object that corresponds to the file data. This object is known as the `struct dirent` pointer and is the return value of `readdir()`. When you call `readdir()` on a `DIR` pointer, it will return a struct that contains the information for the file at that location in the form of a `dirent` struct pointer, and then moves the `DIR` pointer to the next entry in the directory stream. You can use this built-in iteration functionality to iterate through the files in a directory. Once you have finished iterating through all of the files in a `DIR` pointer, `readdir()` will return `NULL`:

- `readdir()` example:

    ```c
    struct dirent * entry;    // Current folder object

    while ((entry = readdir(dp)) != NULL) {
        // Do something with the directory...
    }
    ```

- `d_name`
    The `d_name` value inside of the `dirent` struct holds the file name of the file you are looking at. To see what the name of the current file is from a `readdir()` operation, do the following:

    ```c
    entry = readdir(dp);                      // Get the current file object
    printf("Item name: %s\n", entry->d_name); // Print out name
    ```

#### Filtering Files

While reading in your files using the folder operations, you may want to filter out some folders or files that are not of interest to you such as the `.` and `..` files that exist in every directory. To do this, you can check the name of the filename with the `strncmp()`. If the files match a certain value (i.e. the last 3 characters of the name are `bmp` or `log`), you can add them to a list, else you can ignore it. More reading on how to use `strncmp()` is found in the **Explore More** section below.

### Reading Files

In this lab, you will be expected to read both `.bmp` and `.log` files from the `viewer` folder. Then you must show them on the LCD screen of your Pi Z2W. In order for your program to input these files in code so you can interact with them, you will need to become familiar with some basic C constructs such as `FILE` streams and their associated functions. The information in the following section should help you learn about reading files into C and writing them to a different destination (i.e. in this case, to your LCD screen).

#### FILE Pointers

Interacting with a file will require you to make a `FILE` stream pointer. This pointer represents the current position of where a read or a write action is inside of a file. For example, if you open a file to read for the first time, the pointer will be at the beginning of the document. As you read words by advancing the pointer position with functions such as `fscanf()` or `fread()`, the pointer value will increment. For this lab, you will use `FILE` pointers only in the context of the functions needed for the file operations described below. To declare a FILE pointer:

```c
FILE *fp;    // This declares a file pointer that is read to be used in file operations
```

#### File Operations

There are a few essential file operations that exist in the `stdio.h` library. The following is a selection of functions that will help you with to input files into a location in your code.

- `fopen()` and `fclose()`

    `fopen()` takes in a `FILE` stream pointer, a file path, and some special strings to indicate the mode as arguments for opening a file. For example:

    ```c
    FILE *fp;    // File pointer for interacting with file.
    fp = fopen("fancy_output.log", "r");
    ```

    This code will create a `FILE` stream pointer called `fp`. You then use `fopen` to set the location of `fp` to the beginning of the `fancy_output.log` file in the current directory with the `r`ead mode (look at the **Explore More!** section for different file modes).

    Once you have done everything that you want to with the file, you must be responsible and release the pointer back to the system. This is done by closing the file using `fclose()`. To do this, simply do the following:

    ```c
    fclose(fp);    // Closes the file that was attached to the fp pointer
    ```

- `fread()`

    Once you have a file open, how do you use the `FILE` stream pointer to get data? This is where functions like `fread` come into play. This function reads data from a file pointer. Specifically, it takes a buffer to read data into, the size of the data, the length of the data, and the file pointer.

    ```c
    char data[256];            // Creates a string that can hold up to 256 characters 
    fread(data, 1, 256, fp);   // Reads up to 256 bytes (or characters) from the file
    ```

## Procedure

### Program Development

Now that you have your `test.c` file from the last lab with basic menu features, you will begin developing your `main.c` program that actually runs your doorbell. Remember, use `make` or `make main.c` to create your `main` executable. **All the development for the rest of the semester will be on `main.c` and its helper files.**

### Technical Debt

Technical debt is the idea that when you code something for the first time, it is normally not the most polished code. When you need to go back and improve the functionality of your code, there will be some refactoring needed.

Technical debt often happens when different features are so intertwined with each other that to change, add, or remove a feature requires much of the code to be rewritten. There are a few things you can do to help yourself stay out of technical debt over the next few weeks:

- Be intentional about the choices and names you use for variables etc.
- Make features independant of the program; in other words, each new feature should not rely on all the other features to work. That way, if something must be removed, it doesn't impact the rest of the development. One example is coding features into functions that can be added/moved, instead of writing all the code directly into `main.c`.

### Requirements

In this lab you should accomplish the following:

- Use the menu functionality you developed in the last lab to list all of the files in the `viewer` folder. You can copy as much of the code from `test.c` as you would like into `main.c`.

- When the right button is pressed, the selected file should be drawn to the screen for two seconds. The menu should then be redrawn.

- You only need to show up to 8 items on the display. You can ignore the rest of the items if there are more than 8.

To accomplish these tasks, you will need to:

- Copy over the `draw_menu` function from the previous lab
  - File names should be drawn in either 8pt or 12pt font.
- Implement the `get_entries` function:
  - Filter out any file that does not end in `.bmp` or `.log`. (Hint: Sounds like you need to **get** the **file extension**).
  - Populate the `entries` array with the the `.bmp` and `.log` files.
  - Return the number of files read in.
  - If you find more than 8 files, you need only read in the first 8.
- Implement the `draw_file` function
  - If the file is a `.bmp` image, display the corresponding image for 2 seconds and then go back to the menu view with the highlight bar over the selected item.
  - If the file is a `.log` file, display the contents of the file for 2 seconds and then go back to the menu view with the highlight bar over the selected item.  You should only show the first 100 characters of the file.

Here is a demo showing the different features of the lab:

<div class="row">
    <div class="mx-auto">
        <video height=500 controls>
            <source src="{% link assets/file/demo.mp4 %}" type="video/mp4">
            Your browser does not support the video tag.
        </video>
    </div>
</div>

## Pass Off & Submission

- Your program must compile without warnings or errors. Your `Makefile` has the `-Werror` flag to ensure that it doesn't.
- Pass off to a TA by demonstrating your doorbell running your program that fulfills all of the requirements outlined above.
- **Make sure to upload your changes back to your GitHub repository. Follow the instructions on the Github Setup page.**
- Take the Pass off Quiz on Learning Suite.
- Follow the instructions to update your `README.md` file with the new features of this lab.

## Explore More

- [List Files in Folder in C](https://stackoverflow.com/questions/4204666/how-to-list-files-in-a-directory-in-a-c-program)
- [Opening and Closing a File in C Tutorial](https://www.programiz.com/c-programming/c-file-input-output#:~:text=()%20returns%20NULL.-,rb,Open%20for%20writing.)
- [`fscanf()` Tutorial](https://www.tutorialspoint.com/c_standard_library/c_function_fscanf.htm)
- [Comparing strings in C](https://www.tutorialspoint.com/c_standard_library/c_function_strncmp.htm)
