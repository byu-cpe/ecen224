---
title: Doorbell
number: 11
layout: lab
---

## Objectives

- Learn about daemons
- Create a program that runs on boot
- Deal with different modes of program operation

## Getting Started

Use the GitHub Classroom link posted in the Learning Suite for the lab to accept the assignment. Next, ssh into your Raspberry Pi using VSCode and clone the repository in your home directory. **This lab should be done in VSCode on your Raspberry Pi.**

## Overview

Welcome to the end of the semester! You have come very far in your abilities to understand how computers work under the hood and how you can control them programmatically. The difference between the computer you are using now and most single-use devices that exist on the market is that your Pi Z2W in its current state still acts as any normal desktop/laptop/phone. When you turn the device on, the operating system loads and then you are expected to start any program that you wish to have running.

This is not the case for most single-use devices. For example, when you turn on a microwave, it immediately starts heating up your food. When you turn on an electric kettle, it immediately starts heating up your water. When you turn on a doorbell, it immediately starts ringing. This is because these devices are programmed to start running as soon as they are turned on. In this lab, we will work to make the Pi Z2W act as a smart doorbell. This entails having your program run on boot.

### systemd Daemons

To enable our program to run on boot in Linux, we need to understand how Linux manages its processes. Linux uses a system called `systemd` to manage its processes. `systemd` is a system and service manager that runs as the first thing when your computer boots (process ID 1). It is responsible for starting and stopping processes, as well as managing the boot process.

`systemd` also manages programs that run in the background, called daemons. A daemon is a program that runs in the background and is not directly started by the user. Daemons are often used to provide services to the user, such as the `sshd` daemon that provides the ability to connect to the Pi Z2W over SSH.

#### Service Files

For our doorbell to run on boot, we need to create a daemon that runs our program. This is done by creating a `systemd` service file. A `systemd` service file is a file that contains information about a service that `systemd` can be used to start and stop the service.

One specific command that can be used to monitor and control the operations of daemons in `systemd` is called `systemctl`. `systemctl` is a command line utility that can be used to control the `systemd` system and service manager. It can be used to start, stop, restart, and check the status of services.

For example, let's say we want to observe the `sshd` daemon on our Pi Z2W. We can use the following command to see the status of the `sshd` daemon:

```bash
sudo systemctl status sshd    # Shows status of the sshd daemon
```

Likewise, if we wanted to start or stop the daemon, we could use the following commands (don't actually run these commands):

```bash
sudo systemctl start sshd     # Starts the sshd daemon
sudo systemctl stop sshd      # Stops the sshd daemon
```

However, these commands only work for daemons that are already installed on the Pi Z2W. If we want to create our own daemon, we need to create a service file that `systemd` can use to start and stop our program.

To create a service file, we need to create a file in the `/etc/systemd/system` directory. The name of the file must end in `.service`. The contents of the file must be in the following format:

```ini
[Unit]
Description=<Name of your service>

[Service]
WorkingDirectory=<Absolute path to your project folder>
ExecStart=<Absolute command to start your program>

[Install]
WantedBy=multi-user.target
```

**The file path to your executable must be absolute.** This means if your ran your code by doing:

```bash
./my-program
```

You would be running your program using a relative path. However, if you ran your program by doing:

```bash
/home/USERNAME/path/to/my-program
```

you would be using an absolute path. The path to your executable must be absolute in the `.service` file, so you must use the second command to run your program.

Once we have created the `.service` file, we can use the commands above to start and stop our program. However, what if I want to start my program when the computer boots? We need to **enable** our service. To do this, we can use the following command:

```bash
sudo systemctl enable my-service.service    # Enables the my-service service
```

This will make it so that my-service.service will run whenever I turn on the Pi Z2W. If I don't want my program to run when my Pi Z2W boots, I can use the following command to disable my service:

```bash
sudo systemctl disable my-service.service    # Disables the my-service service
```

## Requirements

- Copy all of your code, except for the README.md file from last lab into your newly cloned repository. Run your program to make sure it is still working properly.

- Change your program to be more doorbell-ish:

  - When the program starts, it should show a welcome message on the screen.

  - When the center button is pressed, do the following:

    - In a thread, take a picture. save it to your viewer folder, and send the image to the website.

    - Change the display to show the message to let the user know that doorbell was rung (e.g., "Ding dong!"). Make this message visible for 2 seconds and then change back to the initial welcome message.

    - **NO STATUS BAR LOGIC IS NECESSARY**

- Add a hidden menu to your doorbell. When the following sequence of buttons are pressed (Up-Up-Down), take the user to the file browser menu. To go back to the original welcome message, press the left button of the d-pad.

- Create a service file in `/etc/systemd/system` named `doorbell.service`. You will need to create the file with `sudo` privileges. Test the service by running `sudo systemctl start doorbell.service` and make sure your program starts successfully. If you run `sudo systemctl status doorbell.service`, you can see log messages from your program. To test out your service file, run `sudo systemctl start doorbell.service` to make sure that your program works as expected. If you run into problems, you can use `status` to see what the problem is: `sudo systemctl status doorbell.service`

- Before you set up your program to run on boot, add a 15 second sleep to the first line of your main function. This is because the program that prints the IP addresses also uses the displays and if they run at the same time, the display can get messed up. This ensures that the IP address program is finished before your program starts. Make sure to rebuild your program (`make`) so the change takes effect.

- Set the program to run on boot. Test it out by rebooting your Pi Z2W and making sure your program starts up.

## Submission

- Pass off with a TA, by demonstrating your doorbell running your program that fulfills all of the requirements outlined above.

- Take the Pass off Quiz on Learning Suite.

- Follow the instructions in the README file in the repository to write your own README for this lab. Include your name, section, semester, and lab title. A good README should answer the following questions:
  - What is the purpose of this project and its code/files?
  - What is the structure/organization of the project files?
  - How do you build and run the code in this project?

- Add and Commit all of your updated files (and your README) as explained under **Committing and Pushing Files** on the [Lab Setup]({{ site.baseurl }}/lab-setup) page. Remember that while these instructions give general information, you need to add and commit all of the files you have modified or created in this lab.

## Explore More

- [What is systemd?](https://en.wikipedia.org/wiki/Systemd)
- [systemd's website](https://systemd.io/)
- [Creating a systemd Service](https://medium.com/@benmorel/creating-a-linux-service-with-systemd-611b5c8b91d6)
- [Code for the server part of this lab](https://github.com/byu-ecen-224-classroom/client_server)
