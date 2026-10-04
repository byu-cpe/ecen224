---
layout: page
toc: true
title: Doorbell Setup
icon: fa-duotone fa-wrench
---

This course uses the smart doorbell unit based on the [Raspberry Pi Zero 2 W](https://www.raspberrypi.com/products/raspberry-pi-zero-2-w/) (Pi Z2W) to teach core computing principles. The following instructions will help you prepare this [single-board computer](https://en.wikipedia.org/wiki/Single-board_computer) to support course activities.

<figure class="image mx-auto" style="max-width: 750px">
  <img src="{% link assets/getting-started/assembly/step_16_completed.jpeg %}" alt="smart doorbell unit">
  <figcaption style="text-align: center;">Smart doorbell unit.</figcaption>
</figure>

## Install Linux on SD Card

To use the doorbell, an operating system must be installed on an SD card. When inserted into the doorbell unit, the SD card acts as the main file system. An [**operating system**](https://en.wikipedia.org/wiki/Operating_system) (OS) is a set of programs that allows users to interact with hardware. Windows, macOS, and Linux are examples of operating systems, each tailored to different platforms. Linux offers a variety of [distributions](https://itsfoss.com/what-is-linux-distribution/#:~:text=Your%20distributions%20also%20takes%20the,as%20Linux%2Dbased%20operating%20systems.) suited for different needs. The doorbell uses [Raspberry Pi OS](https://www.raspberrypi.com/software/), a Linux distribution designed for Raspberry Pi devices.

Normally, you would use the [Raspberry Pi Imager](https://www.raspberrypi.com/news/raspberry-pi-imager-imaging-utility/) tool to install the OS. However, due to lab machine limitations, follow this alternative method.

1. Open a terminal on your lab machine using the **Activities** menu or the shortcut `Ctrl+Alt+T`. Download and run the imaging script by entering:

    ```bash
    cd /tmp
    wget https://raw.githubusercontent.com/philipbl/ecen225-image/refs/heads/main/imager.sh
    chmod +x imager.sh
    ./imager.sh
    ```

    *(Use `Ctrl+Shift+V` to paste into the terminal after copying the command.)*

2. Wait for the script to complete. It may take a long time; it will prompt you for input during the process.

3. After the script finishes, remove the SD card from the computer and insert it into the SD card slot of the doorbell unit.

## Connect to Doorbell over SSH

Use a Power over Ethernet (PoE) adapter—a white brick at each lab computer—to power the doorbell. PoE transmits both power and internet through a single Ethernet cable, eliminating the need for separate power cables. While not all Ethernet ports support PoE, every port in the Digital Lab does.

To power the doorbell, unplug the Ethernet cable from the PoE adapter. Connect the adapter to the **left** Micro USB port (labeled USB). Reconnect the Ethernet cable to the PoE adapter. This sequence ensures the network assigns an IP address to your doorbell unit over the Ethernet connection.

Allow up to three minutes for the doorbell to boot. When the system is ready, the display will show a blue screen with two addresses: the localhost address (127.0.0.1) and an assigned IP address in the form 10.32.X.X. If the IP address does not appear, unplug the Ethernet cable from the PoE adapter and reconnect it. If the issue continues, notify a TA.

With Raspberry Pi OS Lite installed and the doorbell connected to the lab network, you can now access it remotely using `ssh`. This allows you to log into the doorbell from another machine, such as the lab computer.

1. Verify that your lab machine can detect the doorbell. Open a **terminal** (`Ctrl+Alt+T`) and run:

    ```bash
    # replace NETID with your NetID
    ping doorbell-NETID.local
    ```

    If the output shows:

    ```bash
    ping: doorbell-NETID.local: Name or service not known
    ```

    then the doorbell is not connected to the lab network. Confirm the Ethernet cable is connected properly to both the correct port on the doorbell unit and the wall jack in the lab. Wait several minutes before retrying. If it still fails, seek assistance.

    If the output shows:

    ```bash
    PING doorbell-NETID.local (192.168.86.48) 56(84) bytes of data.
    64 bytes from doorbell-NETID.local (192.168.86.48): icmp_seq=1 ttl=64 time=95.7 ms
    ...
    # it will keep pinging until you stop it
    ```

    press `Ctrl+C` to stop the ping. Your lab machine can now communicate with the doorbell.

2. Log in via SSH:

    ```bash
    ssh NETID@doorbell-NETID.local
    ```

    Enter the password set earlier. Nothing will appear as you type. Press `Enter` when finished.

3. When prompted to confirm trust in the remote machine, type `yes`.

4. Once logged in, look for the command line prompt:

    ```text
    ➜ ~
    ```
    ![ssh]({% link assets/getting-started/ssh.png %})

5. Press the enter key a few times to make sure the prompt is working. It will just repeat itself every time you press enter.

6. Type `exit` to logout of the doorbell and return to the lab machine command prompt.

## Generate and Copy SSH Keys

Setting up an SSH key on your doorbell unit will allow you to login to your unit and synchronize files without typing a password each time.

1. To generate an SSH key, type the following at a lab machine terminal:

    ```bash
    ssh-keygen -t ed25519 -C "your_email_address"
    ```

    The tool will ask you several questions. For our purposes, the default values will suffice (i.e., just hit `Enter` until it finishes) unless you desire to protect your key with a password (not recommended for this class; it would require you to enter in a password any time you want to use the SSH key).

2. Once this is done, you can find the contents of your new SSH key by typing:

    ```bash
    cat ~/.ssh/id_ed25519.pub
    ```

    **NOTE:** Make sure that you `cat` the values of `id_ed25519.pub` and **NOT** `id_ed25519`. The contents in the `.pub` are meant to be shared with the `pub`lic and the contents of the other file are not meant to be shared with anyone else.

3. Copy the key to your doorbell unit. Replace NETID with your NetID in the command below.

    ```bash
    ssh-copy-id NETID@doorbell-NETID.local
    ```

4. Test logging into the doorbell unit through SSH. After typing the following, you should be logged directly into the doorbell without a password prompt.

    ```bash
    ssh NETID@doorbell-NETID.local
    ```

## Install Rsync Extension in VS Code

After the Rsync extension is set up, it allows you to automatically synchronize your workspace files on a lab machine to the doorbell file system using the rsync utility. Every time you hit `Save`, the extension instantly pushes the changes to the doorbell in the background.

1. Launch VS Code from your `ecen225/doorbell` repository or set the VS Code workspace to this directory.

2. Install the [Sync-Rsync](https://marketplace.visualstudio.com/items?itemName=vscode-ext.sync-rsync) extension:

    - Click the extensions icon:
      ![extensions]({% link assets/getting-started/vscode-extensions.png %}){:width="4%"}
    - Search for `Sync-Rsync`
    - Click **Install** on the extension by **vscode-ext**

3. Within your doorbell repository, edit the provided `.vscode/settings.json` file. Find the line shown below and replace NETID with your NetID.

    - "sync-rsync.remote": "NETID@doorbell-NETID.local:doorbell",

4. Save the `settings.json` file.

## Install BCM2835 Library

To access the LCD and button module in your doorbell unit, we need to install the **BCM2835 library**.

1. First, make sure that you are logged into your doorbell unit and in your home directory:

    ```bash
    ssh NETID@doorbell-NETID.local
    cd ~
    ```

2. Then use `wget` to download the compressed archive with the library from the developer's website:

    ```bash
    wget http://www.airspayce.com/mikem/bcm2835/bcm2835-1.71.tar.gz
    ```

3. Once the file has downloaded, uncompress the archive using the `tar` utility:

    ```bash
    tar xvfz bcm2835-1.71.tar.gz
    ```

4. Go into the uncompressed directory:

    ```bash
    cd bcm2835-1.71
    ```

5. Run the `configure` script. This will gather operating system information before building the library so it knows how to work specifically on the Pi Z2W architecture:

    ```bash
    ./configure
    ```

6. Now, run the `make` command to compile the library's source files (i.e., all its `.c` files):

    ```bash
    make
    ```

7. Finally, copy the compiled binaries to the system library folder:

    ```bash
    sudo make install
    ```

Now that the `bcm2835` library is installed, we can use it in any C program that we like! However, since this is an installed library and not a default one, we have to let `gcc` know that we are trying to include it in the compilation process. This is done by adding the `-l bcm2835` flag to our normal `gcc` compilation command. The `-l` lets `gcc` know we are including a custom system library, while the `bcm2835` part is just the name of the library itself.

**You don't need to worry about including this flag in your lab projects** since the Makefile in the starting code already includes the flag.

## Proper Shutdown of Doorbell Unit

**Always shut down the doorbell gracefully before unplugging power.** Pulling the power on a running Raspberry Pi can corrupt the SD card. Linux temporarily saves file data in memory and only flushes changes to the SD card periodically; if power is cut before flushes complete, the filesystem can be left in an inconsistent state, which often results in a Pi that will no longer boot. `shutdown` flushes pending writes and halts the system cleanly. After that, unplug the PoE adapter from the Micro USB port.

1. When logged into your doorbell (through SSH), type the following:

    ```bash
    sudo shutdown now
    ```

2. After the screen goes blank, unplug the PoE adapter from the Micro USB port to remove power.
