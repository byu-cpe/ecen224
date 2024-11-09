---
title: "Bomb Lab: Mastering x86-64 assembly and GDB"
short_title: Bomb Lab
icon: fa-duotone fa-bomb
number: 1
layout: lab
---

## Section 1: Getting Started

**<span style="color: red;">Warning: Please do not attempt to start this lab before the date listed on the LearningSuite schedule. If you do, your bomb may be accidentally deleted when the start date passes. If you are trying to work ahead, talk to one of the TAs first.</span>**

**<span style="color: red;">Additionally - this Lab can be very difficult if you don't understand what's going on. Make sure you read all the documentation here before trying to do the lab!</span>**

### Storyline Introduction

The nefarious Dr. Evil has planted a slew of “binary bombs” on the ECEn Department’s Digital Lab machines! Each of these bombs is a program that consists of a sequence of phases. We managed to recover the `main.c` file that he used, but we couldn't recover the code contained in the phases. We do know however that each phase expects you to type a particular string on `stdin`. If you type the correct string, that phase will be **defused** and the bomb will proceed to the next phase. Otherwise, the bomb **explodes** by printing "`BOOM!!!`" and then terminating. The bomb is defused when every phase has been defused.

There are too many bombs for us to deal with, so we are giving each student a bomb to defuse. Your mission, which you have no choice but to accept, is to defuse your bomb before the due date. Good luck, and welcome to the bomb squad!

### First Things First

1. **<span style="color: gold;">You must complete this lab on one of the Digital Lab computers.</span>** The bomb files are specifically compiled for the architecture used on the digital lab machines, and they will *not* run elsewhere. Because all your files are stored in a virtual J Drive connected back to CAEDM, it doesn't matter which computer you log into. You can either:

    - Go physically into the lab and use one of the digital lab machines (make sure that another class is not in session)
    - SSH into a Digital Lab machine from your own machine and run GDB (See **[Appendix A](#appendix-a-sshing-into-the-digital-lab-machines)** for SSH instruction).

2. **<span style="color: gold;">Grading for this Lab:</span>** This lab is worth 70 points. Each time a phase is diffused, your score increases. The phases are weighted according to the following table:

    | Phase          | Point Value |
    | :---:          | :---------: |
    | 1              | 10          |
    | 2              | 10          |
    | 3              | 10          |
    | 4              | 10          |
    | 5              | 15          |
    | 6              | 15          |
    | Total          | 70          |

    Be careful! Each time that the bomb explodes, you will lose 1/2 a point (up to a max of 20 points). If you have exploded your bomb several times, and you don't want to have that penalty, you can download another bomb and try again. The highest score will be selected. Keep in mind that the bombs are randomized, so its better to not let it blow up in the first place!

    You can see your grade by looking at the [Bomb Lab Scoreboard](http://ecen224.byu.edu:15213/scoreboard) which is also found on **Learning Suite**.

3. **<span style="color: gold;">Some resources you'll need:</span>** These two are absolutely must-haves when it comes to GDB and understanding assembly. The x86 Reference Sheet is also extremely helpful if you are studying for Midterm 2!

    - **[GDB Quick Reference]({% link assets/GDB Quick Reference.pdf %})** > A list of all the GDB commands you'll need (see [Section 4: Using GDB](#section-4-using-gdb) as well). The most important commands are highlighted, but depending on how you go about diffusing the bomb, you may want the other ones as well.
    - **[x86-64 Reference Sheet](https://web.stanford.edu/class/cs107/resources/x86-64-reference.pdf)** > This sheet contains a very succint guide to all the basic facts of understanding x86 Assembly. Use this for understanding assembly commands, register names and jobs, and how to do math with registers.

    There are also some additional resources listed in **[Appendix B](#appendix-b-additional-resources)** that you may find helpful.

## Section 2: Download and Unpack The Bomb

 Please make sure you are on the digital lab for this step! Even if you are going to SSH into the digital lab machines to run GDB,  **<span style="color: gold;">you have to have the bomb stored on your J-drive for a digital lab machine to run it.</span>** If you download it onto your own computer, you'll have to use the `scp` command (something we won't cover here) to copy it to your J-drive, or just download a new one on a digital lab machine.

 To obtain your bomb, go to the [Bomb Lab Download page](http://ecen224.byu.edu:15213/) here or on **Learning Suite**. Follow the instructions on the web page (fill out a bomb request form, and the server will deliver a custom bomb to you in the form of a `tar` file named `bombK.tar`, where `K` is the unique number of your bomb.)

 Save the `bombK.tar` file to a directory in your J-drive in which you plan to do your work. Then, open a terminal, navigate to the tar file, and give the command: `tar -xvf bombK.tar`. This will create a directory called `./bombK` with the following files:

    - `README`: Identifies the bomb and its owners.
    - `bomb`: The executable binary bomb.
    - `bomb.c`: The main file we recovered - the bomb’s main routine and a friendly greeting from Dr. Evil.

 While you are unpacking things, we recommend that you also look at **[Appendix C](#appendix-c-using-objdump)** for some additional things you can do to get some basic information out of your bomb before you start diffusing it.

 *As mentioned in the grading section above, you can request more than one bomb at any time. Please do not flood the server with requests. Only your highest bomb will be scored.*

 *If the server is not working, wait 15 minutes and try again (it resets on a period of 15 mins.). If its still down, contact Dr. Lundrigan and he will fix it.*

## Section 3: Defuse Your Bomb

This is the main task of the lab.

**<span style="color: gold;">You must complete this lab on one of the Digital Lab computers.</span>** In fact, there is a rumor that Dr. Evil really is evil, and the bomb will always blow up if run elsewhere. We have identified several other tamper-proofing devices also built into the bomb.

To defuse your bomb, you must simply run it and correctly give the six required passwords or "phases". you can run the bomb with the command

    ./bomb

because the bomb is just a regular executeable file. However, since you do not know any of the passwords, and they are very hard to guess, we strongly advise you instead use a debugger tool (GDB) to disassemble your bomb and step through the code.

Although phases get progressively harder to defuse, the expertise you gain as you move from phase to phase should offset this difficulty. However, the last phase will challenge even the best students, so please don’t wait until the last minute to start.

**<span style="color: gold;">You can also run passwords through your bomb using a text file.</span>** The bomb ignores blank input lines. For example, if you ran your bomb outside of GDB using

    ./bomb psol.txt

it would look for `psol.txt` and read it line by line until it reaches EOF (end of file), and then ask you for any remaining inputs through `stdin`. We aren't sure why Dr. Evil added this feature, but it allows you to not retype the solutions to phases you have already defused.

To avoid accidentally detonating the bomb, you will need to learn how to step through the assembly code at the instruction level and how to set breakpoints. You will also need to learn how to inspect both the registers and the memory states. One of the nice side-effects of doing the lab is that you will get very good at using a debugger, a crucial skill that will pay big dividends the rest of your career.

**<span style="color: gold;">We do make one request: *please do not use brute force!*</span>** You could conceivably write a program that will try every possible key to find the right one. But this is a bad idea for several reasons:

- You lose 1/2 point (up to a max of 20 points) every time you guess incorrectly and the bomb explodes.
- Every time you guess wrong, a message is sent to the bomblab server. You could very quickly saturate the network with these messages, and cause the system administrators to revoke your computer access.
- We haven’t told you how long the strings are, nor have we told you what characters are in them. Even if you made the (incorrect) assumptions that they all are less than 80 characters long and only contain letters, then you would have 26<sup>80</sup> guesses for each phase. This will take a very long time to run, and you will not get the answer before the assignment is due.

When you have completed the assignment (fully or partially) submit the answers to your bomb in the assignment on LearningSuite.

## Section 4: Using GDB

GDB, or GNU Debugger, is a command-line tool available on virtually every platform, designed for debugging programs in languages like C and C++. It allows you to trace through a program line by line, examine memory and register contents, and inspect both source code and assembly code. For our purposes GDB is essential because it will allow you to reverse-engineer the binary `bomb` file without access to the original source code. By setting breakpoints, monitoring memory changes, and examining assembly instructions, you will systematically analyze the program’s behavior, identify the required inputs, and progress through each “bomb” phase safely.

While we won't go diving into how to do the assembly analysis, we will provide some references and tutorials for the basic usage of GDB. As you read through this, try to focus on learning what options you have in GDB for analyzing code, so that when you start get into your own bomb, you can use the tools below to take intentional steps towards debugging.

### Set up your workspace

Whenever you run GDB, you should make sure that you do it in a terminal that has enough space to show you the code and the registers all at once. While working on the bomb lab, it is often helpful to have your screen divided in half as follows:

**<span style="color: red;">Insert image of well divided screen</span>**

This setup provides a nice balance between having space to see what's happening in GDB, and having another window to read documentation, etc.

It is also strongly recommended to have second tab open in your terminal (in case you need to run other commands outside of GDB) and to keep pen and paper on hand for taking notes on the code and writing down important values.

### Opening GDB

to open GDB, simply type:

    gdb bomb 

while you are in your bomb folder. From here, your terminal will spit out a few lines, and the end result will be that your cursor ends up looking like this:

    (gdb)

Now you have GDB open, but it still doesn't look any more helpful than it did before. Let's change the layout to be more useful by typing

    (gdb) layout asm
    (gdb) layout regs

This will add the assembly, and then the registers to our window so that we can see the program being debugged. At this point, your terminal looks something like this:

**<span style="color: red;">Insert image of terminal</span>**

Now that you have GDB set up, you can start debugging the program. The next few sections will tell you how to do several things in GDB that you will use to debug the lab.

### Running & stopping GDB

*Note: Before running your own bomb, you should read the other sections, which teach you how to protect yourself against explosions.Running the code without understanding how to stop it will likely result in lost points.*

If you want to run the bomb, all you have to do is type

    (gdb) run

From here, you should your assembly and register panes jump into action! You will also notice that your output pane begins acting **both** as your input to the program **and** as your input to the debugger. You can tell when you are talking to the program or the debugger by looking to see if the active line has a `(gdb)` at the beginning. **If it does not**, then you are entering input to the bomb. Be careful not to accidentally give debugger commands as your input!

If you want to **stop** the bomb, all you have to do is type

    (gdb) kill

which will then ask (to which you respond `y`)

    Are you sure you want to kill the running instance (y or n)?

It will then say `killed inferior 1() process` which means the program is no longer runnning. **This, in combination with breakpoints, is how you prevent the bomb from exploding.** If you are panicked and you cannot figure out how to get the program to stop, you can also type `quit` to quit GDB entirely, or just close the active terminal session. As long as the code in the function that explodes the bomb does not run, you will not lose any points.

*Important Notice! Everytime you quit GDB (by closing terminal or typing `quit`, all your breakpoints will be removed.)*

#### Helpful Tip: using a text file to pass phases quickly

While running the bomb in GDB, you can give command line arguments just as you would in the regular command line. As mentioned above in [Section 3](#section-3-defuse-your-bomb), you can give the bomb a command line argument that will allow you to put in several passwords at once. To do the same thing in GDB, just add that argument to the end of `run`:

    (gdb) run psol.txt

This will run the debugger with all the solutions that you have currently gathered, which will save you lots of time in the long run. We recommend that you keep a seperate terminal window open in part so that you can write to that text file without having to close GDB.

### Stepping through code

Once the code is running, you will need to step through the code at various intervals in order to make sense of what's going on. Here are the descriptions of the basic movement commands you can use in GDB:

- `continue` - Also known as 'c'. Runs code until the next break point is hit. Be careful with this command; using it carelessly can easily lead to you stepping too far and exploding your bomb.
- `nexti <count>` - Also known as `ni`; *Next Instruction*. Runs *count* instructions at once. default is 1. Note also that this instruction only counts instructions contained within the current scope (i.e. a `call func` command will be considered one instruction, and all of `func` will be run at once.)

### Setting breakpoints

*Important Notice! Everytime you quit GDB (by closing terminal or typing `quit`, all your breakpoints will be removed.)*

### Expressions in GDB

## Appendices

### Appendix A: SSHing into the Digital Lab Machines

#### Method 1

 You must first SSH into `ssh.et.byu.edu` with your CAEDM username and password. From there, you can SSH into a Digital Lab computer, `digital-##.ee.byu.edu`, where ## can be a number 01 to 60 (you can choose any number, and two people can use the same number). Again, use your **CAEDM username and password** to log into those machines. For example, run the following commands

 ```bash
 ssh username@ssh.et.byu.edu
 # Once SSH'd into the CAEDM computer
 ssh username@digital-##.ee.byu.edu
 ```

 Replace `username` with your CAEDM username and `##` with the number of the Digital Lab computer you want to log into. 

#### Method 2

 You can use the CAEDM VPN to appear to be on campus and ssh into a machine. Here are the quick facts:

 ```bash
 Servername: vpn.et.byu.edu
 Protocol: IKEv2
 Authentication: EAP-MSCHAPv2
 ```

 If you don't already know how to set up a VPN, go to [the CAEDM VPN page](https://caedm.et.byu.edu/wiki/index.php/VPN) and scroll to the bottom, where you will find a table of link to setting up the VPN depending on your operating system. After you have gotten the VPN to connect succesfully, your computer will appear to be coming from inside the CAEDM network. This means that you can SSH in directly with `ssh username@digital-##.ee.byu.edu`, where `username` is your CAEDM username, and `##` is any number between 01 and 60.

### Appendix B: Additional Resources

- [Jump Instructions](https://stackoverflow.com/questions/9617877/assembly-jg-jnle-jl-jnge-after-cmp)

- [Article on reading assembly](https://www.timdbg.com/posts/fakers-guide-to-assembly/)

- [Compiler Explorer](https://godbolt.org) -> Compare C code with its corresponding assembly code.

Looking for a particular tool? How about documentation? Don’t forget, the commands `apropos`, `man`, and `info` are your friends. In particular, `man ascii` might come in useful. `info gas` will give you more than you ever wanted to know about the GNU Assembler. Also, the web may also be a treasure trove of information. If you get stumped, reach out to the TAs, and then the instructor.

### Appendix C: Using Objdump

- `objdump -t`

    This will print out the bomb’s symbol table. The symbol table includes the names of all functions and global variables in the bomb, the names of all the functions the bomb calls, and their addresses. You may learn something by looking at the function names!

- `objdump -d`

    Use this to disassemble all of the code in the bomb. You can also just look at individual functions. Reading the assembler code can tell you how the bomb works.

    Although `objdump -d` gives you a lot of information, it doesn’t tell you the whole story. Calls to system-level functions are displayed in a cryptic form. For example, a call to `sscanf` might appear as:

    ```asm
    8048c36: e8 99 fc ff ff call 80488d4 <_init+0x1a0>
    ```

    To determine that the call was to `sscanf`, you would need to disassemble within gdb.

- `strings`

    This utility will display the printable strings in your bomb.