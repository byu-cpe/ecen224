---
title: "Bomb Lab: Mastering x86-64 assembly and a Debugger"
short_title: Bomb Lab
icon: fa-duotone fa-bomb
number: 1
layout: lab
---

## Getting Started
 You must complete this lab on one of the Digital Lab computers. You can either go physically into the lab and use one of the computers, or you can SSH into one. If you are off campus however, you can not SSH into them directly. Below are two ways that you use to SSH into one of the lab machines from your home computer. Method 1 is faster, but Method 2 is much more reliable.

 Because all your files are stored in a virtual J Drive connected back to CAEDM, it doesn't matter which computer you log into. From this SSH session, you can work on and solve your bomb.

### Download Instructions
 Go to the Bomb Lab assignment in the LearningSuite schedule. There you will find links to the assignment and the scoreboard. It is highly recommended to download your bomb by going to a lab machine in person, as the SSH tutorial does not cover how to copy files through SSH and you will need to do that on your own if you download the bomb on your own machine.


### Method 1
 You must first SSH into `ssh.et.byu.edu` with your CAEDM username and password. From there, you can SSH into a Digital Lab computer, `digital-##.ee.byu.edu`, where ## can be a number 01 to 60 (you can choose any number, and two people can use the same number). Again, use your **CAEDM username and password** to log into those machines. For example, run the following commands

 ```bash
 ssh username@ssh.et.byu.edu
 # Once SSH'd into the CAEDM computer
 ssh username@digital-##.ee.byu.edu
 ```

 Replace `username` with your CAEDM username and `##` with the number of the Digital Lab computer you want to log into. 

### Method 2
 You can use the CAEDM VPN to appear to be on campus and ssh into a machine. Here are the quick facts:
 
 ```bash
 Servername: vpn.et.byu.edu
 Protocol: IKEv2
 Authentication: EAP-MSCHAPv2
 ```
 
 If you don't already know how to set up a VPN, go to [the CAEDM VPN page](https://caedm.et.byu.edu/wiki/index.php/VPN) and scroll to the bottom, where you will find a table of link to setting up the VPN depending on your operating system. After you have gotten the VPN to connect succesfully, your computer will appear to be coming from inside the CAEDM network. This means that you can SSH in directly with `ssh username@digital-##.ee.byu.edu`, where `username` is your CAEDM username, and `##` is any number between 01 and 60.

## Introduction
The nefarious Dr. Evil has planted a slew of “binary bombs” on the ECEn Department’s Digital Lab machines. A binary bomb is a program that consists of a sequence of phases. Each phase expects you to type a particular string on `stdin`. If you type the correct string, then the phase is *defused* and the bomb proceeds to the next phase. Otherwise, the bomb *explodes* by printing "`BOOM!!!`" and then terminating. The bomb is defused when every phase has been defused.

There are too many bombs for us to deal with, so we are giving each student a bomb to defuse. Your mission, which you have no choice but to accept, is to defuse your bomb before the due date. Good luck, and welcome to the bomb squad!

## Step 1: Get Your Bomb
To obtain your bomb, follow the instructions on the web page for this lab. (You point your Web browser at a particular server, fill out a bomb request form, and the server will deliver a custom bomb to you in the form of a `tar` file named `bombK.tar`, where `K` is the unique number of your bomb.)

Save the `bombK.tar` file to a (protected) directory in which you plan to do your work. Then give the
command: `tar -xvf bombK.tar`. This will create a directory called `./bombK` with the following files:
- `README`: Identifies the bomb and its owners.
- `bomb`: The executable binary bomb.
- `bomb.c`: Source file with the bomb’s main routine and a friendly greeting from Dr. Evil.

If for some reason you request multiple bombs, this is not a problem. Choose one bomb to work on and delete the rest.

## Step 2: Defuse Your Bomb
Your job for this lab is to defuse your bomb.

You must do the assignment on one of the Digital Lab machines. In fact, there is a rumor that Dr. Evil really is evil, and the bomb will always blow up if run elsewhere. There are several other tamper-proofing devices built into the bomb as well, or so we hear.

You can use many tools to help you defuse your bomb. Please look at the **hints** section for some tips and ideas. The best way is to use your favorite debugger to step through the disassembled binary.

Each time your bomb explodes it notifies the bomblab server, and you lose 1/2 point (up to a max of 20 points) in the final score for the lab. So there are consequences to exploding the bomb. You must be careful!

The first four phases are worth 10 points each. Phases 5 and 6 are a little more difficult, so they are worth 15 points each. So the maximum score you can get is 70 points.

Although phases get progressively harder to defuse, the expertise you gain as you move from phase to phase should offset this difficulty. However, the last phase will challenge even the best students, so please don’t wait until the last minute to start.

The bomb ignores blank input lines. If you run your bomb with a command line argument, for example,
```
linux> ./bomb psol.txt
```
then it will read the input lines from `psol.txt` until it reaches EOF (end of file), and then switch over
to `stdin`. In a moment of weakness, Dr. Evil added this feature so you don’t have to keep retyping the solutions to phases you have already defused.

To avoid accidentally detonating the bomb, you will need to learn how to single-step through the assembly code and how to set breakpoints. You will also need to learn how to inspect both the registers and the memory states. One of the nice side-effects of doing the lab is that you will get very good at using a
debugger. This is a crucial skill that will pay big dividends the rest of your career.

## Scoring and Grading
The bomb will notify your instructor automatically about your progress as you work on it. You can keep track of how you are doing by looking at the class scoreboard. (A link to the scoreboard is given on the LearningSuite assignment). Every time the bomb explodes, your overall score will drop slightly. If you feel that explosions have dropped your grade too far, you can download another bomb. Your final grade will be the highest grade of all the bombs associated to your student ID.

When you have completed the assignment (fully or partially) submit the answers to your bomb in the assignment on LearningSuite.

## Hints *(Please read this!)*
There are many ways of defusing your bomb. You can examine it in great detail without ever running the program, and figure out exactly what it does. This is a useful technique, but it not always easy to do. You can also run it under a debugger, watch what it does step by step, and use this information to defuse it. This is probably the fastest way of defusing it.

We do make one request: *please do not use brute force!* You could conceivably write a program that will try every possible key to find the right one. But this is a bad idea for several reasons:
- You lose 1/2 point (up to a max of 20 points) every time you guess incorrectly and the bomb explodes.
- Every time you guess wrong, a message is sent to the bomblab server. You could very quickly saturate the network with these messages, and cause the system administrators to revoke your computer access.
- We haven’t told you how long the strings are, nor have we told you what characters are in them. Even if you made the (incorrect) assumptions that they all are less than 80 characters long and only contain letters, then you would have 26<sup>80</sup> guesses for each phase. This will take a very long time to run, and you will not get the answer before the assignment is due.

There are many tools which are designed to help you figure out both how programs work, and what is wrong when they don’t work. Here is a list of some of the tools you may find useful in analyzing your bomb, and hints on how to use them.

- `gdb`

    The GNU debugger, this is a command-line debugger tool available on virtually every platform. You can trace through a program line by line, examine memory and registers, look at both the source code and assembly code (we are not giving you the source code for most of your bomb), set breakpoints, set memory watch points, and write scripts.
    
    The CS:APP web site:

    [http://csapp.cs.cmu.edu/public/students.html](http://csapp.cs.cmu.edu/public/students.html)

    has a very handy single-page `gdb` summary that you can print out and use as a reference.

    Here are some other tips for using `gdb`:
    - To keep the bomb from blowing up every time you type in a wrong input, you’ll want to learn how to set breakpoints.
    - For online documentation, type “`help`” at the `gdb` command prompt, or type “`man gdb`”, or “`info gdb`” at a Unix prompt. Some people also like to run `gdb` under `gdb-mode` in `emacs`.
  
- `objdump -t`

    This will print out the bomb’s symbol table. The symbol table includes the names of all functions and global variables in the bomb, the names of all the functions the bomb calls, and their addresses. You may learn something by looking at the function names!

- `objdump -d`

    Use this to disassemble all of the code in the bomb. You can also just look at individual functions. Reading the assembler code can tell you how the bomb works.
    
    Although `objdump -d` gives you a lot of information, it doesn’t tell you the whole story. Calls to system-level functions are displayed in a cryptic form. For example, a call to `sscanf` might appear as:
    ```
    8048c36: e8 99 fc ff ff call 80488d4 <_init+0x1a0>
    ```
    To determine that the call was to `sscanf`, you would need to disassemble within gdb.
    
- `strings`

    This utility will display the printable strings in your bomb.

Looking for a particular tool? How about documentation? Don’t forget, the commands `apropos`, `man`, and `info` are your friends. In particular, `man ascii` might come in useful. `info gas` will give you more than you ever wanted to know about the GNU Assembler. Also, the web may also be a treasure trove of information. If you get stumped, please consult the TAs or your instructor.

## Resources

- [GDB Quick Reference]({% link assets/GDB Quick Reference.pdf %})

- [x86-64 Reference Sheet](https://web.stanford.edu/class/cs107/resources/x86-64-reference.pdf)

- [Jump Instructions](https://stackoverflow.com/questions/9617877/assembly-jg-jnle-jl-jnge-after-cmp)

- [Article on reading assembly](https://www.timdbg.com/posts/fakers-guide-to-assembly/)

- [Compiler Explorer](https://godbolt.org). Compare C code with its corresponding assembly code.
