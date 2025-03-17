---
layout: page
toc: true
title: Resources
icon: fa-duotone fa-book
---

### Textbook

The class has one required textbook: ***Computer Systems: A Programmer's Perspective, 3rd Edition by Bryant and O'Hallaron***. We will also be using an online Wikibook, [C Programming](https://en.wikibooks.org/wiki/C_Programming), when we learn about C programing.

### Learning Resources

Here are some other resources that might help you:

- *The* original book on C, [The C Programming Language](https://www.amazon.com/dp/0131103628/).

- Jan Schaumann has a lot of great videos on [Advance Programming in the UNIX Environment](https://www.youtube.com/@cs631apue/videos).

- Ben Eater has some great [videos](https://www.youtube.com/@BenEater) on how to build a computer, write assembly, and other computer topics.
  
- [Computerphile](https://www.youtube.com/results?search_query=computerphile) has approachable videos on various computing topics.

- Chris has some tutorials on various Linux topics on his [blog](https://kitras.io).

### Linux Command Summary
For more information on the following commands, type `man` and the name of the command to see a help manual for that command. You can open another terminal window at the same time to type the `man` command and see the helpful text. Some commands, like `alias`, are built into bash. For these, type `man bash` to get help. If you type `man <command>` and the response is `No manual entry for <command>`, it is likely built into bash. Online help is also available for [shell built-in commands](https://www.gnu.org/software/bash/manual/html_node/Shell-Builtin-Commands.html) and [other core commands](https://www.gnu.org/software/coreutils/manual/html_node/index.html).

| Command    | Description                                          |
| :--------- | :--------------------------------------------------- |
| alias      | define an alternative to a command                   |
| cat        | concatenate files and print on standard output​       |
| cd         | change the current directory to another​              |
| chmod      | change file mode bits                                |
| clear      | clear the terminal screen​                            |
| cp         | copy files and directories​                           |
| df         | report file system disk space usage                  |
| diff       | compare files line by line                           |
| echo       | print a string of text                               |
| exit       | end shell session and close terminal                 |
| find       | search for files in a directory hierarchy            |
| free       | display amount of free and used memory in the system |
| grep       | scan files and print lines that match patterns       |
| groups     | print the groups a user is in                        |
| head       | output the first part of files                       |
| history    | list recently typed commands                         |
| kill       | send a signal to a process (usually to end it)       |
| less, more | show the contents of a text file​                     |
| ls         | list directory contents​                              |
| man        | show help information about Linux commands           |
| mkdir      | make directories                                     |
| mv         | move or rename files​                                 |
| passwd     | change user password                                 |
| ping       | send an echo request to another machine              |
| ps         | list the current processes                           |
| pwd        | print name of current directory​                      |
| rmdir      | remove empty directories                             |
| ssh        | login to a remote machine                            |
| tail       | output the last part of files                        |
| tar        | an archive utility for (de)compressing files         |
| top        | display Linux processes                              |
| uname      | print system information                             |
| who        | show who is logged on                                |

If you want even more commands at a glance, check out [this cheat sheet](https://github.com/trinib/Linux-Bash-Commands#quick-cheat-sheet-).

### Setting up Wi-Fi

If you want to connect to your doorbell off campus, you can follow the following steps:

**1: Set up Wi-Fi connection**

You will need to do this step in the lab unless you have any other way to connect to your raspberry pi.

When connected to your pi, run the command `sudo raspi-config`.  This will bring up a terminal-based GUI that you can navigate using the arrow and enter keys.

Once inside, navigate to `1 System Options` > `S1 Wireless Lan`.  Put your network name in the SSID field, and your network password in the passphrase field. You may complete this step as many times as you wish for as many networks as you wish.

Note that certain secure networks such as eduroam *will not* work with this connection method.

Once you have entered all the network information, leave the configuration GUI by selecting `<Finish>`.

You can confirm that your network was properly configured by running `sudo cat /etc/wap_supplicant/wap_supplicant.conf`.  Your network and password should be listed there.

**Step 2: Connecting at home**

Your pi should now be able to connect to the Wi-Fi network(s) that you entered in at the lab. When you're at home, power your pi with a micro-usb cord plugged into the *bottom port*. On a computer connected to the same network, you can set up VS code to ssh into your computer as we described in Lab 1.

If you use a Mac, you may need to adjust some system security settings to allow the VS code remote connection extension to SSH into your doorbell: Go to the settings app, `Privacy & Security` > `Local Network` > `Visual Studio Code`, and toggle it to allowed.

You should now be able to control your Pi at home!

### Terminal customization

- [Phil's Computer Setup](https://byunetlab.notion.site/Phil-s-Computer-Setup-0722e33e22e74460aa53f58d5f2babb8)

- [Chris' Computer Setup](https://kitras.io/setup/)

### Helpful Tutorials

- [BYU Computing Bootcamp: Git](https://byu-cpe.github.io/ComputingBootCamp/tutorials/git/)
- [BYU Computing Bootcamp: GitHub](https://byu-cpe.github.io/ComputingBootCamp/tutorials/github/)
- [Add WiFi credentials after installation](https://howchoo.com/g/ndy1zte2yjn/how-to-set-up-wifi-on-your-raspberry-pi-without-ethernet)
- [Install ZSH and Oh My ZSH with Powerlevel10k Theme](https://dev.to/abdfnx/oh-my-zsh-powerlevel10k-cool-terminal-1no0)

    - [Meslo LGS NF Font with Icons](https://github.com/ryanoasis/nerd-fonts/raw/master/patched-fonts/Meslo/M/Regular/complete/Meslo%20LG%20M%20Regular%20Nerd%20Font%20Complete.ttf)
    - [Change terminal font](https://help.gnome.org/users/gnome-terminal/stable/app-fonts.html.en#:~:text=Custom%20font.-,Set%20a%20custom%20font,-To%20set%20a) to the font above.

- [GitHub Markdown Examples](https://gist.github.com/ww9/44f08d44327a40d2ab309a349bebec57)

- [GDB Cheatsheet](https://darkdust.net/files/GDB%20Cheat%20Sheet.pdf)
