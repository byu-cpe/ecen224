---
title: Client
number: 9
layout: lab
---

## Objectives

- Interact with `struct`s
- Handle raw data in a buffer
- Send data over a network socket

## Introduction

You have come very far in understanding how to control your computer! Up until this point, we have covered programming peripheral devices, handling inputs from a user, and many other skills that allow you to control your Pi Z2W to the fullest! In this lab, we will take you from influencing processes on your own system to interacting with programs on other systems! In this lab you will gain practice with network programming by sending your saved photos over a network to a server.

### Config Struct

#### IP Addresses and Ports

Before we can access a program on a different computer, we need to know where that computer "lives" within the internet. Just like a virtual city, all computers connected to the wider internet have an address for you to find them! For example, when you are on your web browser and you want to visit [https://byu.edu](https://byu.edu), the browser will use a special protocol to translate the **domain name** you provided in the URL bar (byu.edu) to an **IP address** (128.187.16.184) which represents the actual virtual address of the computer.

If you want to find an IP address of a certain website (most popular ones have many), you can use `ping` and observe the output:

```bash
$ ping byu.edu

PING byu.edu (128.187.16.184) 56(84) bytes of data.
64 bytes from byu.com (128.187.16.184): icmp_seq=1 ttl=58 time=2.53 ms
64 bytes from email.byu.edu (128.187.16.184): icmp_seq=2 ttl=58 time=2.92 ms
64 bytes from email.byu.edu (128.187.16.184): icmp_seq=3 ttl=58 time=11.1 ms

```

Note that `128.187.16.184` is the IP address of the server that responded to our `ping`.

In addition to the address of the computer, we need to know what port is being used by the program we want to communicate with.

A **port** is a number representing a virtual entry point that a program uses to communicate with other programs. It acts like just a port at a dock, where certain ships (applications) are directed to certain ports to control traffic flow and security.

For example, a web server that is hosting a website normally does this under port `80` or `443`. The default port for an `ssh` connection (like the one between your lab computer and your RPi) is port `22`. To improve security, computers usually refuse connections or traffic directed at the wrong port.

When we connect to another computer that is *listening* on a specific port, we refer to the listening computer as the **server** and the computer that is trying to connect to the server as the **client**.

#### Struct Data Members

As mentioned previously, in order to talk to a server, we need to know several things about it. In this lab, the `Config` `struct` in the provided `client.h` contains the information you will need in order to connect to the server:

```c
typedef struct Config {
    char *port;
    char *host;
    uint8_t *payload;
    uint32_t payload_size;
    char *hw_id;
} Config;
```

| Data Member    | Description                                                                                                                                                                                                                            |
| -------------- | -------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------- |
| `port`         | The port number of the application you are trying to connect to. Notice that port is a `char *`, not a number.                                                                                                                         |
| `host`         | This is the hostname of the server you are trying to connect to.                                                                                                                                                                       |
| `payload`      | This is the data you want to send to the server. In this lab, that will be the image data.                                                                                                                                             |
| `payload_size` | The size of the data in `payload`.                                                                                                                                                                                                     |
| `hw_id`        | This is your Learning Suite assigned homework ID for this class. In order to find your unique homework ID, go to Learning Suite > ECEn 225 > Grades and then click on the small link at the top that says "Show Course Homework ID" |

The location you will be connecting to to upload your photos in this lab will be:

- Host: ecen224.byu.edu (Note: this host is only available from the campus network)
- Port: 2240

### Network Socket

Now that we know the server name and port that we want to connect to, how do we actually form a connection and transfer data? The answer is surprisingly simple: talking to a different computer in C is much like writing or reading data to a file. However, instead of writing to a file, we will be writing to a **socket**.

When we want to write to a file in C, we need to use the `fopen()` function to open the file. In network programming, opening a socket is a bit more complicated than opening a file, so we have provided The function `client_connect()`, which connects to a server port and opens a socket. Similar to how `fopen()` returns a `FILE` pointer that you use to interact with a file, `client_connect()` returns a file descriptor (with the data type of `int`) that lets your program keep track of which port your computer will use to talk to the remote computer.

### Sending Data

We used `fwrite()` to write to a file; in network programming we use `send()` to write to a socket (see the link in **Explore More** for more details about this function).

**Be Careful!** While `send()` always *tries* to send all the data you give it, the socket can't guarantee that the all the data in your buffer will be sent at once. To help us keep track, `send()` returns the number of bytes successfully sent.

For example, if I am sending data that is 100 bytes long, I call `send()`, and it returns `50`, then only the first 50 bytes were sent. I now need to call `send()` again, except this time I only pass in bytes 51 through 100 from my buffer. I repeat this process until the total number of bytes sent over all my `send()` calls equals 100.

You will be responsible for writing the logic to ensure that **all** of the data is sent correctly by implementing a function called `client_send_image` in `client.c`. Read the code below to see how the function behaves and get an idea of how to overcome this hurdle (this is just an example, but it works almost identically in your code).

```c
int socket = ...
uint8_t *data = ...
int data_length = ...
int total_sent = 0
int num_sent = 0

while total_sent < data_length:
    num_sent = send(socket, data + total_sent, data_length - total_sent, 0)

    if num_sent == 0:
        print("Error while sending data")
        break

    total_sent += num_sent
```

In this lab, **you will need to send your homework ID and your image data together, will no null terminator or space between them**. The easiest way of doing this is combining the data into one block of data (using `malloc` and `memcpy`). Your homework ID must go first, followed by the image data.

The results will be displayed at `http://ecen224.byu.edu:2241/<homework_id>`, replacing `<homework_id>` with your actual homework ID. For example, if your homework ID was 123456789, then the URL where your pictures are stored are at [http://ecen224.byu.edu:2241/123456789](http://ecen224.byu.edu:2241/123456789). Remember, this server is only available when you are on the campus network.

### Deallocating Resources

When calling functions that create or allocate system resources, you need to remember to free those resources when you are done with them.
If this is not done, unexpected behavior may occur on your system. So remember the following:

| Resource Allocation                                                                                                                    | Freeing Call |
| -------------------------------------------------------------------------------------------------------------------------------------- | ------------ |
| `malloc`                                                                                                                               | `free`       |
| `fopen`                                                                                                                                | `fclose`     |
| `opendir`                                                                                                                              | `closedir`   |
| `socket` (don't worry about calling this function in this lab, this is done for you already in the provided `client_connect` function) | `close`      |

## Procedure

### Requirements

Same as last lab, when you press the center button, you should take a picture and show the picture, allowing the user to apply different filters. When you press the center button again, instead of exiting the picture and going to the menu right away, first connect to the server and send the picture. Once you have sent the picture, show the menu. Specifically, when the center button gets pressed while showing the picture you should do the following:
  
1. Create an empty `Config` `struct` and load it with the appropriate data. The `port`, `host`, and `hw_id` can be hard-coded. Use the picture buffer obtained in the last lab using the `camera_capture_data` function, fill in the `payload` field. The size of the payload should be the size of the image data.

2. Start the client connection using `client_connect()` and passing the `Config` `struct`. This function is implemented for you and will print out the status of the connection to the server.

3. Call the `client_send_image` function to send the data, passing the `Config` `struct`. You will need to implement this function yourself. This function will combine the homework ID and the image data into one buffer and send it to the server using the `send` function. You will need to make sure all of the data is sent correctly.

4. Call the `client_receive_response` function. This function is implemented for you and will print out the response from the server. This function can help debug any problems you might have with sending your image data.

5. Close the connection using `client_close()`. You will need to implement this function yourself.

6. Show the menu.

### Pass Off & Submission

- Your program must compile without warnings or errors.
- Pass off to a TA by demonstrating your doorbell running your program that fulfills all of the requirements outlined above.
- **Make sure to upload your changes back to your GitHub repository. Follow the instructions on the Github Setup page.**
- Take the Pass off Quiz on Learning Suite.
- Follow the instructions to update your `README.md` file with the new features of this lab.

## Explore More

- [`memcpy()` in C](https://www.tutorialspoint.com/c_standard_library/c_function_memcpy.htm)
- [`send()` in C](https://man7.org/linux/man-pages/man2/send.2.html)
- [Ultimate Guide to Network Programming in C](https://beej.us/guide/bgnet/html/)
- [Code for the server part of this lab](https://github.com/byu-ecen-224-classroom/client_server)
