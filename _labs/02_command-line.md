---
title: Command Line
number: 2
layout: lab
---

## Objectives

- Become familiar with the Linux environment, commands, and some applications.
- Be able to navigate a directory tree from the command line.
- Use a text editor to create and edit text files.

## Getting Started
Use the GitHub Classroom link posted in the Learning Suite for the lab to accept the assignment. Next, `ssh` into your Raspberry Pi and clone the repository into your Raspberry Pi's home directory. This lab must be done on your Raspberry Pi. As a reminder, to `ssh` into your Rapsberry Pi, plug it in, and run this command on the terminal of your lab computer:

```bash
ssh [your-netid]@doorbell-[your-netid].local
```

Fill in your NetID in the command. To clone the repository, follow the instructions on the [Lab Setup]({% link _pages/lab_setup.md %}) page.

**This lab should be done in the terminal, not VSCode.**

## Overview

In this lab you will learn some basic Linux features and skills that are useful throughout a technical career. The Linux environment is used commonly in the engineering, scientific and research communities. One of the reasons for this stems from the openness of Linux and the many applications that run on it. The source code to most everything that runs on Linux, and the [kernel](http://www.kernel.org/) itself, is available on the Web. If you don’t like something or want to improve it, nothing is stopping you from getting the source code and changing it. You are encouraged to learn and explore beyond what is presented here on your own. We can only touch on a few Linux features. Discovering the flexibility that is available to the creative mind is left to you. 

### Explore Common Linux Commands
Work through **Modules 1, 2, 3 (skip the printing commands), and 4** of the [Linux Survival tutorial](http://linuxsurvival.com/). It has a simulated Linux terminal for practicing what you learn in the modules.

For a brief list of commonly used commands, see the [Linux Command Summary]({{site.baseurl}}/resources#linux_commands_summary/).

### Keyboard Shortcuts
Even though using the command line to do things in Linux may seem like a lot of typing, there are shortcuts in place that can tremendously reduce the number of keystrokes. With a little practice, the shortcuts become a natural part of doing work on the command line and can even be more efficient than pointing, clicking, and navigating menus in a graphical interface. With a graphical interface, a user will often need to move their hand back and forth between the keyboard and the mouse. This motion is inefficient, even though most of us get used to it. With command line operation, our fingers never need to leave the keyboard and reach for the mouse.

#### Command History
When you type a command on the keyboard, there is a good chance you will want to execute it again, exactly the same way, or with a small variation. **Using the up and down arrows on the keyboard allows you to navigate through a history of commands you have typed on the command line**. When you hit the up arrow key the first time, the last command you executed will appear on the command line. If you hit up arrow again, you will see the next to the last and so forth. Typing down arrow will show more recent commands. After you have selected a previous command with the arrow keys, hitting the `Enter` key will execute the command.

#### Command Editing
Once you have selected a prior command with the up and down arrow keys, you may want to slightly change it before you execute it again. **The left and right arrow keys allow you to move the cursor to a location where you want to change the command.** Use the backspace key to remove characters and then type new text at that location. When satisfied with the modified command, hit `Enter` to execute it. This is useful, for example, when you want to execute the same command on a different filename. Just hit up arrow to get the previous command, then backspace over the old file name and type the new one.

#### Command Completion
Another helpful shortcut is called tab completion. If the tab key is pressed while a partial command or file name is typed, the shell will complete the rest of the text based on what has already been typed. Open a terminal window to your home directory (you can do this by typing `Ctrl`+`Alt`+`T` and then `ssh`ing into your Raspberry Pi) and type the following and then hit `Tab`.

```bash
ls /d
```

After hitting `Tab` you should see that “/d” was extended to “/dev/”. Now type `Enter` to execute the command. When navigating deep directory hierarchies, tab completion can assist in typing what could otherwise be very long path names. For example, tab can be pressed multiple times to keep extending the path name after typing a few characters, enough to make the name unique.

Command names can also be tab completed. In a terminal window, type the following and then hit `Tab`.

```bash
mkd
```

Pressing the tab key should complete the command to mkdir. Press `Enter` to start the web browser. If you press `Tab` and you don’t get a response, you have mistyped the leading characters or there are other options that are ambiguous. Typing `Tab` again a second time will list the remaining options that begin with the characters you have typed. For example type in 

```bash
mk
```

Now hit `Tab` a couple of times and you should see something like this:
```
mkdir        mk_modmap    mksh         mktemp       mkvinfo      mkvpropedit  
mkfifo       mknod        mksh-static  mkvextract   mkvmerge
```

You can `Tab` through the possible commands and hit `Enter` to select the highlighted command. 

### Set and Read Environment Variables
The Linux environment holds configuration variables used by application programs. These variables are set in the environment by a user or by the system as needed. Knowing how to set environment variables and view them is important, since some Linux application programs may depend on you setting these up properly before running them. For example, the shell inside your terminal uses environment variables to indicate the location of where all the commands are stored. Now let’s see how to set an environment variable. The following example will set the variable MYLAB to a value of 02.

```bash
export MYLAB=02
```

After typing the command in the terminal, you can view the value that was last set by typing:

```bash
echo $MYLAB
```
Notice that you have to put a `$` in front of the variable name in order to refer to it. 

You can also see your variable among all the others by typing:

```bash
env
```

There are some **system environment variables** that are defined for you when you open a terminal window, like `$SHELL`, `$USER`, and `$HOME`. You can read some more about environment variables [here](https://www.cherryservers.com/blog/how-to-set-list-and-manage-linux-environment-variables).

### Get Familiar with a Command Line Text Editor
Several text editors are installed on your system, for example, `code`, `gedit`, `nano`, `vim`, and `emacs`. Some applications can only be run from the command line, while others can be launched graphically by clicking on its icon in the application screen. Choose an editor, and use it to create a new file. **Hint: If you are doing this lab SSHed into your Pi Z2W, `code` and `gedit` will not work because they require a GUI. `emacs` can also be terminal based (and is a great option - google the debate between `vim` and `emacs`) - but will need to be installed before you can use it.**

Open a new file in the cloned lab directory using `nano` and create a note to yourself. Experiment with deleting text, copying, and pasting. Save the file. Quit out of `nano` and type `ls -l` to see details about the text file you created.

### Shell Challenge

To round out your shell learning experience, you are **required** to complete the shell challenge. In the lab repo, uncompress the `challenge.tar.xz` package; you will want to Google on how to do this. Notice that the file has two layers - the .tar and the .xz. In order to view the contents of the folder you must remove both (this can be done in one command). Inside the resulting directory, complete the various levels of the challenge to finish the lab. 

In order to complete this lab, you will need to use Google. If you don’t know how to do something, Google will help you find the answer. You will likely want to search things like:

- How to uncompress a tar.xz file

- How to make a script executable in Linux

**To ensure this is graded successfully, make sure you commit the decompressed files!**

## Lab Submission

- Answer the questions in the `README.md`. (This is one OUTSIDE of the challenge folder)

- Pass off to a TA by showing the correct output of your `tree` command. Make sure to show hidden folders and files.

- To successfully submit your lab, you will need to follow the instructions in the [Lab Setup]({{ site.baseurl }}/lab-setup) page, especially the **Committing and Pushing Files** section.

## Explore More!

- [The Linux Command Line for Beginners (tutorial)](http://ubuntu.com/tutorials/command-line-for-beginners)
- [The Linux Command Line (book online)](http://linuxcommand.org/tlcl.php)
- [Shell (bash) Built-in Commands - cd, pwd, echo, etc.](https://www.gnu.org/software/bash/manual/html_node/Shell-Builtin-Commands.html)
- [GNU Coreutils - cp, mv, rm, mkdir, chown, etc.](https://www.gnu.org/software/coreutils/manual/html_node/index.html)
- [Oh My Zsh + PowerLevel10k](https://dev.to/abdfnx/oh-my-zsh-powerlevel10k-cool-terminal-1no0)
