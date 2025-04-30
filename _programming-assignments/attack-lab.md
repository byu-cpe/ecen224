---
title: "Attack Lab: Understanding Buffer Overflow Bugs"
short_title: Attack Lab
icon: fa-duotone fa-bow-arrow
number: 3
layout: lab
---

## 0. Getting Started

You must complete this lab on the CAEDM server (`ssh.et.byu.edu`) to perform this lab. You can ssh into the server by running the following command:

```bash
ssh <caedm_username>@ssh.et.byu.edu
```

Replace `<caedm_username>` with your actual CAEDM username. Your home directory is mounted between `ssh.et.byu.edu`, the Digital Lab computers, and the Embedded Lab computers, so you can download the target from any of those locations.


### Download Instructions
Go to the `Attack Programming Assignment` entry in *LearningSuite* where you will find a link to download your attack target and one to view the scoreboard.

For more detail, view the [Attack Lab - Getting Started](/ecen224/recitation/attackLab-gettingStarted) script from class.

## 1. Introduction
This assignment involves generating a total of five attacks on two programs having different security vulnerabilities. Outcomes you will gain from this lab include:
- You will learn different ways that attackers can exploit security vulnerabilities when programs do not safeguard themselves well enough against buffer overflows.
- Through this, you will get a better understanding of how to write programs that are more secure, as well as some of the features provided by compilers and operating systems to make programs less vulnerable.
- You will gain a deeper understanding of the stack and parameter-passing mechanisms of x86-64 machine code.
- You will gain a deeper understanding of how x86-64 instructions are encoded.
- You will gain more experience with debugging tools such as `GDB` and `OBJDUMP`

<!-- https://getbootstrap.com/docs/5.3/components/alerts/ -->
<div class="alert alert-warning" role="alert">
    <b>Note:</b> In this lab, you will gain firsthand experience with methods used to exploit security weaknesses in operating systems and network servers. Our purpose is to help you learn about the runtime operation of programs and to understand the nature of these security weaknesses so that you can avoid them when you write system code. We do not condone the use of any form of attack to gain unauthorized access to any system resources.
</div>

You will want to study Sections 3.10.3 and 3.10.4 of the CS:APP3e book as reference material for this lab.

## 2. Logistics

### 2.1 Getting Files
To obtain the your unique file, follow the instructions on the webpage for this lab. (You point your Web browser at a particular server and the server builds your files and returns them to your browser in a `tar` file called `targetK.tar`, where `K` is the unique number of your target programs.)

Save the `targetK`.tar file in a (protected) Linux directory in which you plan to do your work. Then give the command: `tar -xvf targetK.tar`. This will extract a directory `targetK` containing the files described below.

You should only download one set of files. If for some reason you download multiple targets, choose one target to work on and delete the rest.

<div class="alert alert-danger" role="alert">
    <b>Warning:</b> If you expand your <code>targetK.tar</code> on a PC, by using a utility such as Winzip, or letting your browser do the extraction, you’ll risk resetting permission bits on the executable files.
</div>

The files in `targetK` include:
- `README.txt`: A file describing the contents of the directory
- `ctarget`: An executable program vulnerable to *code-injection* attacks
- `rtarget`: An executable program vulnerable to *return-oriented-programming* attacks
- `cookie.txt`: An 8-digit hex code that you will use as a unique identifier in your attacks.
- `farm.c`: The source code of your target’s “gadget farm,” which you will use in generating return-oriented programming attacks.
- `hex2raw`: A utility to generate attack strings.

In the following instructions, we will assume that you have copied the files to a protected local directory, and that you are executing the programs in that local directory.

### 2.2 Important Points
Here is a summary of some important rules regarding valid solutions for this lab. These points will not make much sense when you read this document for the first time. They are presented here as a central reference of rules once you get started.
- You must do the assignment on a machine that is similar to the one that generated your targets.
- Your solutions may not use attacks to circumvent the validation code in the programs. Specifically, any address you incorporate into an attack string for use by a `ret` instruction should be to one of the following destinations:
    - The addresses for functions `touch1`, `touch2`, or `touch3`.
    - The address of your injected code
    - The address of one of your gadgets from the gadget farm.
- You may only construct gadgets from file `rtarget` with addresses ranging between those for functions `start_farm` and `end_farm`

## 3. Target Programs
<!-- The words "CTARGET" and "RTARGET" are drawn in small caps on the PDF. I've rendered them as code snippets. If they need to be small caps, it looks like we would achieve this by placing them in <span style="font-variant:small-caps;">ctarget</span> -->
Both `CTARGET` and `RTARGET` read strings from standard input. They do so with the function `getbuf` defined below:

```c
unsigned getbuf()
{
    char buf[BUFFER_SIZE];
    Gets(buf);
    return 1;
}
```
The function `Gets` is similar to the standard library function `gets`-- it reads a string from standard input (terminated by ‘`\n`’ or end-of-file) and stores it (along with a null terminator) at the specified destination. In this code, you can see that the destination is an array `buf`, declared as having `BUFFER_SIZE` bytes. At the time your targets were generated, `BUFFER_SIZE` was a compile-time constant specific to your version of the programs.

Functions `Gets()` and `gets()` have no way to determine whether their destination buffers are large enough to store the string they read. They simply copy sequences of bytes, possibly overrunning the bounds of the storage allocated at the destinations. 

If the string typed by the user and read by `getbuf` is sufficiently short, it is clear that `getbuf` will return 1, as shown by the following execution examples:

```
unix> ./ctarget
Cookie: 0x1a7dd803
Type string: Keep it short!
No exploit. Getbuf returned 0x1
Normal return
```
Typically an error occurs if you type a long string:
```
unix> ./ctarget
Cookie: 0x1a7dd803
Type string: This is not a very interesting string, but it has the property ...
Ouch!: You caused a segmentation fault!
Better luck next time
```

(Note that the value of the cookie shown will differ from yours.) Program `RTARGET` will have the same behavior. As the error message indicates, overrunning the buffer typically causes the program state to be corrupted, leading to a memory access error. Your task is to be more clever with the strings you feed `CTARGET` and `RTARGET` so that they do more interesting things. These are called *exploit* strings.

Both `CTARGET` and `RTARGET` take several different command line arguments:
- `-h`: Print list of possible command line arguments
- `-q`: Don’t send results to the grading server
- `-i FILE`: Supply input from a file, rather than from standard input

Your exploit strings will typically contain byte values that do not correspond to the ASCII values for printing characters. The program `HEX2RAW` will enable you to generate these raw strings. See [Appendix A](#appendix-a-using-hex2raw) for more information on how to use `HEX2RAW`.

**Important points:**
- Your exploit string must not contain byte value `0x0a` at any intermediate position, since this is the ASCII code for newline (‘`\n`’). When `Gets` encounters this byte, it will assume you intended to terminate the string.
- `HEX2RAW` expects two-digit hex values separated by one or more white spaces. So if you want to create a byte with a hex value of 0, you need to write it as `00`. To create the word `0xdeadbeef` you should pass “`ef be ad de`” to `HEX2RAW` (note the reversal required for little-endian byte ordering).

When you have correctly solved one of the levels, your target program will automatically send a notification to the grading server. For example:
```
unix> ./hex2raw < ctarget.l2.txt | ./ctarget
Cookie: 0x1a7dd803
Type string:Touch2!: You called touch2(0x1a7dd803)
Valid solution for level 2 with target ctarget
PASSED: Sent exploit string to server to be validated.
NICE JOB!
```
The server will test your exploit string to make sure it really works, and it will update the Attacklab scoreboard page indicating that your userid (listed by your target number for anonymity) has completed this phase.

You can view the scoreboard by pointing your Web browser at the link given on the class Web page for this class. Unlike the Bomb Lab, there is no penalty for making mistakes in this lab. Feel free to fire away at `CTARGET` and `RTARGET` with any strings you like.

<!-- IMPORTANT NOTE: You can work on your solution on any Linux machine, but in order to submit your solution, you will need to be running on one of the ECEn Department’s spice machines. -->

[Figure 1](#figure-1-summary-of-attack-lab-phases) summarizes the five phases of the lab. As can be seen, the first three involve code-injection (CI) attacks on CTARGET, while the last two involve return-oriented-programming (ROP) attacks on `RTARGET`.

---

### Figure 1: Summary of attack lab phases

<style>
    tr#fig-1-Table { border-top: solid; }
</style>
<!-- https://getbootstrap.com/docs/4.0/content/tables/#responsive-tables-->
<div class="table-responsive">
    <table>
        <tr>
            <th>Phase</th>
            <th>Program</th>
            <th>Level</th>
            <th>Method</th>
            <th>Function</th>
            <th>Points</th>
        </tr>
        <tr>
            <td>1</td>
            <td>CTARGET</td>
            <td>1</td>
            <td>CI</td>
            <td><code>touch1</code></td>
            <td>10</td>
        </tr>
        <tr>
            <td>2</td>
            <td>CTARGET</td>
            <td>2</td>
            <td>CI</td>
            <td><code>touch2</code></td>
            <td>25</td>
        </tr>
        <tr>
            <td>3</td>
            <td>CTARGET</td>
            <td>3</td>
            <td>CI</td>
            <td><code>touch3</code></td>
            <td>25</td>
        </tr>
        <tr id="fig-1-Table">
            <td>4</td>
            <td>RTARGET</td>
            <td>2</td>
            <td>ROP</td>
            <td><code>touch2</code></td>
            <td>35</td>
        </tr>
        <tr>
            <td>5</td>
            <td>RTARGET</td>
            <td>3</td>
            <td>ROP</td>
            <td><code>touch3</code></td>
            <td>5</td>
        </tr>
    </table>
</div>

CI: Code injection
 
ROP: Return-oriented programming

---

## 4. Part I: Code Injection Attacks
For the first three phases, your exploit strings will attack `CTARGET`. This program is set up in a way that the stack positions will be consistent from one run to the next and so that data on the stack can be treated as executable code. These features make the program vulnerable to attacks where the exploit strings contain the byte encodings of executable code.

### 4.1 Phase 1 Level 1
For Phase 1, you will not inject new code. Instead, your exploit string will redirect the program to execute an existing procedure.

Function `getbuf` is called within `CTARGET` by a function `test` having the following C code:
```c
void test()
{
    int val;
    val = getbuf();
    printf("No exploit. Getbuf returned 0x%x\n", val);
}
```

When `getbuf` executes its return statement (line 5 of `getbuf`), the program ordinarily resumes execution within function `test` (at line 5 of this function). We want to change this behavior. Within the file `ctarget`, there is code for a function `touch1` having the following C representation:
```c
void touch1()
{
    vlevel = 1;       /* Part of validation protocol */
    printf("Touch1!: You called touch1()\n");
    validate(1);
    exit(0);
}
```

Your task is to get `CTARGET` to execute the code for `touch1` when `getbuf` executes its return statement, rather than returning to `test`. Note that your exploit string may also corrupt parts of the stack not directly related to this stage, but this will not cause a problem, since `touch1` causes the program to exit directly.

**Some Advice**:
- All the information you need to devise your exploit string for this level can be determined by examining a disassembled version of `CTARGET`. Use `objdump -d` to get this dissembled version.
- The idea is to position a byte representation of the starting address for `touch1` so that the `ret` instruction at the end of the code for `getbuf` will transfer control to `touch1`.
- Be careful about byte ordering.
- You might want to use GDB to step the program through the last few instructions of `getbuf` to make sure it is doing the right thing.
- The placement of `buf` within the stack frame for `getbuf` depends on the value of compile-time constant `BUFFER_SIZE`, as well the allocation strategy used by GCC. You will need to examine the disassembled code to determine its position.

### 4.2 Phase 2 Level 2
Phase 2 involves injecting a small amount of code as part of your exploit string.

Within the file `ctarget` there is code for a function `touch2` having the following C representation:
```c
void touch2(unsigned val)
{
    vlevel = 2;       /* Part of validation protocol */
    if (val == cookie) {
        printf("Touch2!: You called touch2(0x%.8x)\n", val);
        validate(2);
    } else {
        printf("Misfire: You called touch2(0x%.8x)\n", val);
        fail(2);
    }
    exit(0);
}
```
Your task is to get `CTARGET` to execute the code for `touch2` rather than returning to `test`. In this case, however, you must make it appear to `touch2` as if you have passed your cookie as its argument.

**Some Advice:**
- You will want to position a byte representation of the address of your injected code in such a way that `ret` instruction at the end of the code for `getbuf` will transfer control to it.
- Recall that the first argument to a function is passed in register `%rdi`.
- Your injected code should set the register to your cookie, and then use a `ret` instruction to transfer control to the first instruction in `touch2`.
- Do not attempt to use `jmp` or `call` instructions in your exploit code. The encodings of destination addresses for these instructions are difficult to formulate. Use `ret` instructions for all transfers of control, even when you are not returning from a call.
- See the discussion in [Appendix B](#appendix-b-generating-byte-codes) on how to use tools to generate the byte-level representations of instruction sequences.

### 4.3 Phase 3 Level 3
Phase 3 also involves a code injection attack, but passing a string as argument.

Within the file `ctarget` there is code for functions `hexmatch` and `touch3` having the following C representations:
```c
/* Compare string to hex represention of unsigned value */
int hexmatch(unsigned val, char *sval)
{
    char cbuf[110];
    /* Make position of check string unpredictable */
    char *s = cbuf + random() % 100;
    sprintf(s, "%.8x", val);
    return strncmp(sval, s, 9) == 0;
}

void touch3(char *sval)
{
    vlevel = 3; /* Part of validation protocol */
    if (hexmatch(cookie, sval)) {
        printf("Touch3!: You called touch3(\"%s\")\n", sval);
        validate(3);
    } else {
        printf("Misfire: You called touch3(\"%s\")\n", sval);
        fail(3);
    }
    exit(0);
}
```

Your task is to get `CTARGET` to execute the code for `touch3` rather than returning to `test`. You must make it appear to `touch3` as if you have passed a string representation of your cookie as its argument.

**Some Advice:**
- You will need to include a string representation of your cookie in your exploit string. The string should consist of the eight hexadecimal digits (ordered from most to least significant) without a leading “`0x.`”
- Recall that a string is represented in C as a sequence of bytes followed by a byte with value 0. Type “`man ascii`” on any Linux machine to see the byte representations of the characters you need.
- Your injected code should set register `%rdi` to the address of this string.
- When functions `hexmatch` and `strncmp` are called, they push data onto the stack, overwriting portions of memory that held the buffer used by `getbuf`. As a result, you will need to be careful where you place the string representation of your cookie.



## 5. Part II: Return-Oriented Programming
Performing code-injection attacks on program `RTARGET` is much more difficult than it is for `CTARGET`, because it uses two techniques to thwart such attacks:
- It uses randomization so that the stack positions differ from one run to another. This makes it impossible to determine where your injected code will be located.
- It marks the section of memory holding the stack as nonexecutable, so even if you could set the program counter to the start of your injected code, the program would fail with a segmentation fault.

<!--Footnote syntax: https://www.markdownguide.org/extended-syntax/#footnotes -->
Fortunately, clever people have devised strategies for getting useful things done in a program by executing existing code, rather than injecting new code. The most general form of this is referred to as *return-oriented programming* (ROP)[^1] [^2]. The strategy with ROP is to identify byte sequences within an existing program that consist of one or more instructions followed by the instruction `ret`. Such a segment is referred to as a `gadget`. [Figure 2](#figure-2) illustrates how the stack can be set up to execute a sequence of *n* gadgets. In this figure, the stack contains a sequence of gadget addresses. Each gadget consists of a series of instruction bytes, with the final one being `0xc3`, encoding the `ret` instruction. When the program executes a `ret` instruction
starting with this configuration, it will initiate a chain of gadget executions, with the `ret` instruction at the end of each gadget causing the program to jump to the beginning of the next.


### Figure 2
<figure class="image mx-auto" style="max-width: 1000px" id="figure-2">
    <img src="{% link assets/attack-lab/rop.png %}" alt="stack-diagram">
</figure>

A gadget can make use of code corresponding to assembly-language statements generated by the compiler, especially ones at the ends of functions. In practice, there may be some useful gadgets of this form, but not enough to implement many important operations. For example, it is highly unlikely that a compiled function would have `popq %rdi` as its last instruction before `ret`. Fortunately, with a byte-oriented instruction set, such as x86-64, a gadget can often be found by extracting patterns from other parts of the instruction byte sequence.

For example, one version of `rtarget` contains code generated for the following C function:
```c
void setval_210(unsigned *p)
{
    *p = 3347663060U;
}
```
The chances of this function being useful for attacking a system seem pretty slim. But, the disassembled machine code for this function shows an interesting byte sequence:
```
0000000000400f15 <setval_210>:
400f15: c7 07 d4 48 89 c7 movl $0xc78948d4,(%rdi)
400f1b: c3 retq
```

The byte sequence `48 89 c7` encodes the instruction `movq %rax, %rdi`. (See [Figure 3-A](#figure-3-a) for the encodings of useful `movq` instructions.) This sequence is followed by byte value `c3`, which encodes the `ret` instruction. The function starts at address `0x400f15`, and the sequence starts on the fourth byte of the function. Thus, this code contains a gadget, having a starting address of `0x400f18`, that will copy the 64-bit value in register `%rax` to register `%rdi`.

Your code for `RTARGET` contains a number of functions similar to the `setval_210` function shown above in a region we refer to as the *gadget farm*. Your job will be to identify useful gadgets in the gadget farm and use these to perform attacks similar to those you did in Phases 2 and 3.

<div class="alert alert-info" role="alert">
    <b>Important</b>: The gadget farm is demarcated by functions <code>start_farm</code> and <code>end_farm</code> in your copy of <code>rtarget</code>. Do not attempt to construct gadgets from other portions of the program code.
</div>


### 5.1 Phase 4 Level 2
For Phase 4, you will repeat the attack of Phase 2, but do so on program `RTARGET` using gadgets from your gadget farm. You can construct your solution using gadgets consisting of the following instruction types, and using only the first eight x86-64 registers (`%rax`–`%rdi`).

- `movq` : The codes for these are shown in [Figure 3-A](#figure-3-a).
- `popq` : The codes for these are shown in [Figure 3-B](#figure-3-b).
- `ret` : This instruction is encoded by the single byte `0xc3`.
- `nop` : This instruction (pronounced “no op,” which is short for “no operation”) is  encoded by the single byte `0x90`. Its only effect is to cause the program counter to be incremented by 1.

---
### Figure 3-A. 
Encodings of `movq` instructions.

`movq` *S*, *D*

<!-- This CSS attempts to add stronger vertical borders between the table data and border around the entire table. -->
<style>
    table#figTable { border: solid; }
    td { border-left: solid; }
</style>
<div class="table-responsive">
    <table id="figTable">
        <tr>
            <th rowspan=2>Source <em>S</em></th>
            <th colspan=8 style="text-align:center"> Destination <em>D</em></th>
        </tr>
        <tr>
            <th>%rax</th>
            <th> %rcx</th>
            <th> %rdx</th>
            <th> %rbx</th>
            <th> %rsp</th>
            <th> %rbp</th>
            <th> %rsi</th>
            <th> %rdi</th>
        </tr>
        <tr>
            <th>%rax</th>
            <td><code> 48 89 c0</code></td>
            <td><code> 48 89 c1</code></td>
            <td><code> 48 89 c2</code></td>
            <td><code> 48 89 c3</code></td>
            <td><code> 48 89 c4</code></td>
            <td><code> 48 89 c5</code></td>
            <td><code> 48 89 c6</code></td>
            <td><code> 48 89 c7</code></td>
        </tr>
        <tr>
            <th>%rcx</th>
            <td><code> 48 89 c8</code></td>
            <td><code> 48 89 c9</code></td>
            <td><code> 48 89 ca</code></td>
            <td><code> 48 89 cb</code></td>
            <td><code> 48 89 cc</code></td>
            <td><code> 48 89 cd</code></td>
            <td><code> 48 89 ce</code></td>
            <td><code> 48 89 cf</code></td>
        </tr>
        <tr>
            <th>%rdx</th>
            <td><code> 48 89 d0</code></td>
            <td><code> 48 89 d1</code></td>
            <td><code> 48 89 d2</code></td>
            <td><code> 48 89 d3</code></td>
            <td><code> 48 89 d4</code></td>
            <td><code> 48 89 d5</code></td>
            <td><code> 48 89 d6</code></td>
            <td><code> 48 89 d7</code></td>
        </tr>
        <tr>
            <th>%rbx</th>
            <td><code> 48 89 d8</code></td>
            <td><code> 48 89 d9</code></td>
            <td><code> 48 89 da</code></td>
            <td><code> 48 89 db</code></td>
            <td><code> 48 89 dc</code></td>
            <td><code> 48 89 dd</code></td>
            <td><code> 48 89 de</code></td>
            <td><code> 48 89 df</code></td>
        </tr>
        <tr>
            <th>%rsp</th>
            <td><code> 48 89 e0</code></td>
            <td><code> 48 89 e1</code></td>
            <td><code> 48 89 e2</code></td>
            <td><code> 48 89 e3</code></td>
            <td><code> 48 89 e4</code></td>
            <td><code> 48 89 e5</code></td>
            <td><code> 48 89 e6</code></td>
            <td><code> 48 89 e7</code></td>
        </tr>
        <tr>
            <th>%rbp</th>
            <td><code> 48 89 e8</code></td>
            <td><code> 48 89 e9</code></td>
            <td><code> 48 89 ea</code></td>
            <td><code> 48 89 eb</code></td>
            <td><code> 48 89 ec</code></td>
            <td><code> 48 89 ed</code></td>
            <td><code> 48 89 ee</code></td>
            <td><code> 48 89 ef</code></td>
        </tr>
        <tr>
            <th>%rsi</th>
            <td><code> 48 89 f0</code></td>
            <td><code> 48 89 f1</code></td>
            <td><code> 48 89 f2</code></td>
            <td><code> 48 89 f3</code></td>
            <td><code> 48 89 f4</code></td>
            <td><code> 48 89 f5</code></td>
            <td><code> 48 89 f6</code></td>
            <td><code> 48 89 f7</code></td>
        </tr>
        <tr>
            <th>%rdi</th>
            <td><code> 48 89 f8</code></td>
            <td><code> 48 89 f9</code></td>
            <td><code> 48 89 fa</code></td>
            <td><code> 48 89 fb</code></td>
            <td><code> 48 89 fc</code></td>
            <td><code> 48 89 fd</code></td>
            <td><code> 48 89 fe</code></td>
            <td><code> 48 89 ff</code></td>
        </tr>
    </table>
</div>

----
### Figure 3-B.
Encodings of `popq` instructions

<table id="figTable">
    <tr>
        <th rowspan=2>Operation</th>
        <th colspan=8 style="text-align:center"> Register <em>R</em></th>
    </tr>
    <tr>
        <td> %rax </td>
        <td> %rcx </td>
        <td> %rdx </td>
        <td> %rbx </td>
        <td> %rsp </td>
        <td> %rbp </td>
        <td> %rsi </td>
        <td> %rdi </td>
    </tr>
    <tr>
        <th><code>popq</code> <em>R</em></th>
        <td><code>58</code></td>
        <td><code>59</code></td>
        <td><code>5a</code></td>
        <td><code>5b</code></td>
        <td><code>5c</code></td>
        <td><code>5d</code></td>
        <td><code>5e</code></td>
        <td><code>5f</code></td>
    </tr>
</table>

---
### Figure 3-C.
Encodings of `movl` instructions

`movl` *S*, *D*

<div class="table-responsive">
    <table id="figTable">
        <tr>
            <th rowspan=2>Source <em>S</em></th>
            <th colspan=8 style="text-align:center"> Destination <em>R</em></th>
        </tr>
        <tr>
            <th>%eax</th>
            <th>%ecx</th>
            <th>%edx</th>
            <th>%ebx</th>
            <th>%esp</th>
            <th>%ebp</th>
            <th>%esi</th>
            <th>%edi</th>
        </tr>
        <tr>
            <th>%eax</th>
            <td><code>89 c0 </code></td>
            <td><code>89 c1 </code></td>
            <td><code>89 c2 </code></td>
            <td><code>89 c3 </code></td>
            <td><code>89 c4 </code></td>
            <td><code>89 c5 </code></td>
            <td><code>89 c6 </code></td>
            <td><code>89 c7 </code></td>
        </tr>
        <tr>
            <th>%ecx</th>
            <td><code>89 c8 </code></td>
            <td><code>89 c9 </code></td>
            <td><code>89 ca </code></td>
            <td><code>89 cb </code></td>
            <td><code>89 cc </code></td>
            <td><code>89 cd </code></td>
            <td><code>89 ce </code></td>
            <td><code>89 cf </code></td>
        </tr>
        <tr>
        <th>%edx</th>
            <td><code>89 d0 </code></td>
            <td><code>89 d1 </code></td>
            <td><code>89 d2 </code></td>
            <td><code>89 d3 </code></td>
            <td><code>89 d4 </code></td>
            <td><code>89 d5 </code></td>
            <td><code>89 d6 </code></td>
            <td><code>89 d7 </code></td>
        </tr>
        <tr>
            <th>%ebx</th>
            <td><code>89 d8 </code></td>
            <td><code>89 d9 </code></td>
            <td><code>89 da </code></td>
            <td><code>89 db </code></td>
            <td><code>89 dc </code></td>
            <td><code>89 dd </code></td>
            <td><code>89 de </code></td>
            <td><code>89 df </code></td>
        </tr>
        <tr>
            <th>%esp</th>
            <td><code>89 e0 </code></td>
            <td><code>89 e1 </code></td>
            <td><code>89 e2 </code></td>
            <td><code>89 e3 </code></td>
            <td><code>89 e4 </code></td>
            <td><code>89 e5 </code></td>
            <td><code>89 e6 </code></td>
            <td><code>89 e7 </code></td>
        </tr>
        <tr>
            <th>%ebp</th>
            <td><code>89 e8 </code></td>
            <td><code>89 e9 </code></td>
            <td><code>89 ea </code></td>
            <td><code>89 eb </code></td>
            <td><code>89 ec </code></td>
            <td><code>89 ed </code></td>
            <td><code>89 ee </code></td>
            <td><code>89 ef </code></td>
        </tr>
        <tr>
            <th>%esi</th>
            <td><code>89 f0 </code></td>
            <td><code>89 f1 </code></td>
            <td><code>89 f2 </code></td>
            <td><code>89 f3 </code></td>
            <td><code>89 f4 </code></td>
            <td><code>89 f5 </code></td>
            <td><code>89 f6 </code></td>
            <td><code>89 f7 </code></td>
        </tr>
        <tr>
            <th>%edi</th>
            <td><code>89 f8 </code></td>
            <td><code>89 f9 </code></td>
            <td><code>89 fa </code></td>
            <td><code>89 fb </code></td>
            <td><code>89 fc </code></td>
            <td><code>89 fd </code></td>
            <td><code>89 fe </code></td>
            <td><code>89 ff </code></td>
        </tr>
    </table>
</div>

---
### Figure 3-D.
Encodings of 2-byte functional nop instructions

<div class="table-responsive">
    <table id="figTable">
        <tr>
            <th rowspan=2>Operation</th>
            <th colspan=4 style="text-align:center"> Register <em>R</em></th>
        </tr>
        <tr>
            <th>%al</th>
            <th>%cl</th>
            <th>%dl</th>
            <th>%bl</th>
        </tr>    
        <tr>
            <th><code>andb </code><em>R</em>, <em>R</em></th>
            <td><code> 20 c0</code></td>
            <td><code> 20 c9</code></td>
            <td><code> 20 d2</code></td>
            <td><code> 20 db</code></td>
        </tr>
        <tr>
            <th><code>orb </code><em>R</em>, <em>R</em></th>
            <td><code> 08 c0</code></td>
            <td><code> 08 c9</code></td>
            <td><code> 08 d2</code></td>
            <td><code> 08 db</code></td>
        </tr>
        <tr>
            <th><code>cmpb </code><em>R</em>, <em>R</em></th>
            <td><code> 38 c0</code></td>
            <td><code> 38 c9</code></td>
            <td><code> 38 d2</code></td>
            <td><code> 38 db</code></td>
        </tr>
        <tr>
            <th><code>testb </code><em>R</em>, <em>R</em></th>
            <td><code> 84 c0</code></td>
            <td><code> 84 c9</code></td>
            <td><code> 84 d2</code></td>
            <td><code> 84 db</code></td>
        </tr>
    </table>
</div>

---

Figure 3: Byte encodings of instructions. All values are shown in hexadecimal.

**Some Advice**:
- All the gadgets you need can be found in the region of the code for `rtarget` demarcated by the functions `start_farm` and `mid_farm`.
- You can do this attack with just two gadgets.
- When a gadget uses a `popq` instruction, it will pop data from the stack. As a result, your exploit string will contain a combination of gadget addresses and data.

### 5.2 - Phase 5 Level 3 (Extra Credit)
Before you take on Phase 5, pause to consider what you have accomplished so far. In Phases 2 and 3, you caused a program to execute machine code of your own design. If `CTARGET` had been a network server, you could have injected your own code into a distant machine. In Phase 4, you circumvented two of the main devices modern systems use to thwart buffer overflow attacks. Although you did not inject your own code, you were able inject a type of program that operates by stitching together sequences of existing code.

Phase 5 requires you to do an ROP attack on `RTARGET` to invoke function `touch3` with a pointer to a string representation of your cookie. That may not seem significantly more difficult than using an ROP attack to invoke `touch2`, except that we have made it so. Moreover, Phase 5 counts for only 5 extra credit points, which is not a true measure of the effort it will require. This phase is challenging. If you have other pressing obligations consider stopping right now.

To solve Phase 5, you can use gadgets in the region of the code in `rtarget` demarcated by functions `start_farm` and `end_farm`. In addition to the gadgets used in Phase 4, this expanded farm includes the encodings of different `movl` instructions, as shown in [Figure 3-C](#figure-3-c). The byte sequences in this part of the
farm also contain 2-byte instructions that serve as *functional nops*, i.e., they do not change any register or memory values. These include instructions, shown in [Figure 3-D](#figure-3-d), such as `andb %al,%al`, that operate on the low-order bytes of some of the registers but do not change their values.

**Some Advice**:
- You’ll want to review the effect a `movl` instruction has on the upper 4 bytes of a register, as is described on page 183 of the text.
- The official solution requires eight gadgets (not all of which are unique).

Good luck and have fun!

## Appendix A: Using `HEX2RAW`
`HEX2RAW` takes as input a *hex-formatted string*. In this format, each byte value is represented by two hex digits. For example, the string “`012345`” could be entered in hex format as “`30 31 32 33 34 35 00`." (Recall that the ASCII code for decimal digit *x* is `0x3`*x*, and that the end of a string is indicated by a null byte.)

The hex characters you pass to `HEX2RAW` should be separated by whitespace (blanks or newlines). We recommend separating different parts of your exploit string with newlines while you’re working on it. `HEX2RAW` supports C-style block comments, so you can mark off sections of your exploit string. For example:
```c
48 c7 c1 f0 11 40 00 /* mov $0x40011f0,%rcx */
```
Be sure to leave space around both the starting and ending comment strings (“`/*`”, “`*/`”), so that the comments will be properly ignored.

If you generate a hex-formatted exploit string in the file `exploit.txt`, you can apply the raw string to `CTARGET` or `RTARGET` in several different ways:

1. You can set up a series of pipes to pass the string through `HEX2RAW`.
    ```
    unix> cat exploit.txt | ./hex2raw | ./ctarget
    ```

2. You can store the raw string in a file and use I/O redirection:
    ```
    unix> ./hex2raw < exploit.txt > exploit-raw.txt
    unix> ./ctarget < exploit-raw.txt
    ```
    This approach can also be used when running from within GDB:
    ```
    unix> gdb ctarget
    (gdb) run < exploit-raw.txt
    ```
3. You can store the raw string in a file and provide the file name as a command-line argument:
    ```
    unix> ./hex2raw < exploit.txt > exploit-raw.txt
    unix> ./ctarget -i exploit-raw.txt
    ```
    This approach also can be used when running from within GDB.

## Appendix B: Generating Byte Codes
Using `GCC` as an assembler and `OBJDUMP` as a disassembler makes it convenient to generate the byte codes for instruction sequences. For example, suppose you write a file `example.s` containing the following assembly code:

```asm
# Example of hand-generated assembly code
    pushq $0xabcdef     # Push value onto stack
    addq $17,%rax       # Add 17 to %rax
    movl %eax,%edx      # Copy lower 32 bits to %edx
```

The code can contain a mixture of instructions and data. Anything to the right of a ‘#’ character is a comment.
**Remember that when you are writing assembly code, you need to add prefixes to let the compiler know what type of values you are talking about. All literal values should be preceded with a `$` (ex: `pushq $0xabcdef`), and all registers should start with `%` (ex: `pop %rax`).**

You can now assemble and disassemble this file:
```
unix> gcc -c example.s
unix> objdump -d example.o > example.d
```
The generated file example.d contains the following:
```
example.o: file format elf64-x86-64

Disassembly of section .text:

0000000000000000 <.text>:
   0: 68 ef cd ab 00        pushq $0xabcdef
   5: 48 83 c0 11           add $0x11,%rax
   9: 89 c2                 mov %eax,%edx
```
The lines at the bottom show the machine code generated from the assembly language instructions. Each line has a hexadecimal number on the left indicating the instruction’s starting address (starting with 0), while the hex digits after the ‘:’ character indicate the byte codes for the instruction. Thus, we can see that the
instruction `push $0xABCDEF` has hex-formatted byte code `68 ef cd ab 00`.

From this file, you can get the byte sequence for the code:
```
68 ef cd ab 00 48 83 c0 11 89 c2
```
This string can then be passed through `HEX2RAW` to generate an input string for the target programs.. Alternatively, you can edit example.d to omit extraneous values and to contain C-style comments for readability, yielding:
```
68 ef cd ab 00      /* pushq $0xabcdef */
48 83 c0 11         /* add $0x11,%rax */
89 c2               /* mov %eax,%edx */
```

This is also a valid input you can pass through `HEX2RAW` before sending to one of the target programs.

## Resources

- [How to look at the stack in gdb](https://jvns.ca/blog/2021/05/17/how-to-look-at-the-stack-in-gdb/)

## References

[^1]: R. Roemer, E. Buchanan, H. Shacham, and S. Savage. Return-oriented programming: Systems, languages, and applications. *ACM Transactions on Information System Security*, 15(1):2:1–2:34, March 2012.
[^2]: E. J. Schwartz, T. Avgerinos, and D. Brumley. Q: Exploit hardening made easy. In *USENIX Security Symposium*, 2011.
