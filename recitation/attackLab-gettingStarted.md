---
title: AttackLab Getting Started
layout: walkthrough
permalink: /:path/:basename
---
You must complete this lab on the CAEDM server, `ssh.et.byu.edu` because it has the right configuration to allow stack execution exploits. Conveniently, this server is available through SSH from on or off campus. We recommend completing the task in the lab due to the availabilty of TAs but access from other locations is available.

In [LearningSuite](https://learningsuite.byu.edu/.2RLw/cid-wk9m3Zc7ATmQ/pages/id-pDAT), click on the "Attack Programming Assignment" to download your target. It will be a `.tar` file with a name like `targetK.tar` where `K` is a number. You must be in the lab or on the campus network for the download and scoreboard links to work.

Copy your target to your home directory. This directory is shared between the lab computers and the CAEDM server. So, if you download using a lab computer you can just drop the file where you want it.

### Copying the file from a personal computer

If you are working from your own computer, use [scp](https://www.man7.org/linux/man-pages/man1/scp.1.html) to transfer files. For example, to copy targetK from the current local directory to the server use this command:

```
scp targetK.tar username@ssh.et.byu.edu:~
```

Substitute your CADEM username for `username` and the actual name of your target .tar file for `targetK.tar`. The ~ tilde character at the end of the path indicates to copy into your home directory on the server. It will prompt you for your password.

In similar manner, you can use `scp` to copy other files both ways between computers.

### Connect to the CADEM server

Now, make an SSH connection to your server

```
ssh username@ssh.et.byu.edu
```

Now, you are ready to begin the lab.

## Unpacking the package
On the server use the following command:

```
tar -xvf targetK.tar
```
(Substitute the actual name of your `.tar` file.)

This will result in a folder called `targetK` containing six files:

* README.txt: A file describing the contents of the directory.
* ctarget: The target executable program for Phases 1-3.
* rtarget: The target executable program for Phases 4-5.
* hex2raw: A utility to generate attack strings from hexadecimal source
* cookie.txt: The number of your specific cookie to be used in Phases 2 and up.
* farm.c: Source code to the "gadget farm" for uses in Phases 4 and 5.

## Finding values for Phase 1
To solve Phase 1 you need to know the size of your buffer and the location of the `touch1` function. You can find both of these values using objdump.

```
objdump -d ctarget > ctarget.txt
```
This will disassemble `ctarget` and put the result in `ctarget.txt`. In the text file it produced, look for the `getbuf` function. The first instruction will subtract some value from register `rsp`. This is to make room for local variables. Since the only local variable is the buffer, then the amount subtracted from `rsp` is the size of your buffer.

Now, in `ctarget.txt` look for the `touch1` function. The disassembly will show the address of that function.

### Finding the values using gdb
You can find these same two values using `gdb` with the following commands:

```
gdb ctarget
disassemble getbuf
disassemble touch1
```

The first disassemble will give you the code for `getbuf` from which you can deduce the size of the buffer by looking at how much is subtracted from `rsp`.

The second disassemble will give you the address of touch1.

### Hint: Jumping to addresses
All jumps in x86 machine code are relative. That is, the address in the `jmp` command is added to the current `rip` instruction pointer. A negative number will jump to a lower address and a positive number will jump to a higher address. This is done so that code can be *relocated* without having to change all of the jump commands.

Because of this, when you follow the tips in *Appendix B* to generate machine code, it's hard to a `jmp` that will go to an exact address. To do so, you need to know the address of where your code will land and then take the difference between the two addresses. An easy alternative is to `push` the address you want on the stack and then use `ret` to go to that address.

```
pushq $0xabcdefgh
retq
```

### Solving the Phases
Follow the instructions to understand the challenges of the other Phases. For some of them, you'll need to run the target under the `gdb` debugger and set breakpoints to find the values and addresses you need.

