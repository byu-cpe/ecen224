---
title: Getting Started
number: 1
layout: lab
---

## GitHub Classroom
Use the GitHub Classroom link posted in the Learning Suite for the lab to accept the assignment.

## Objectives

- Become familiar with the tools used for this lab
- Understand how to setup a single board computer
- Be able to gain access remotely to the Raspberry Pi Zero 2 W

## Overview

<figure class="image mx-auto" style="max-width: 750px">
  <img src="{% link assets/getting-started/pi-z2w.png %}" alt="Units of the course.">
  <figcaption style="text-align: center;">The Raspberry Pi Zero 2 W with the SD card port outlined in magenta, the mini HDMI port outlined in light blue, and the power micro USB port outlined in yellow.</figcaption>
</figure>

In this class we will be using the [Raspberry Pi Zero 2 W](https://www.raspberrypi.com/products/raspberry-pi-zero-2-w/) (or Pi Z2W for short) as the platform to explore the fundamentals of computing taught throughout the semester. The Pi Z2W is included in your lab kit that you should have already bought from the Experiential Learning Center (also known as The Shop) in CB 416. In this lab you will be setting up the [single board computer](https://en.wikipedia.org/wiki/Single-board_computer) so that it is capable of handling all the labs in this course. There are 3 parts to this lab:

- Flash and configure the Pi Z2W
- Gain remote access to the Pi Z2W 
- Development environment setup
- Put all of the components together for future labs

### Setting up the SD Card 
In order to make use of the Pi Z2W, we will need to put an operating system onto its SD card. An [**operating system**](https://en.wikipedia.org/wiki/Operating_system) (OS), simply put, is a collection of programs which allows a user to interact with a device's hardware. For users of PCs, this collection of programs is called [Microsoft Windows](https://www.microsoft.com/en-us/windows?r=1), for Mac users [macOS](https://www.apple.com/za/macos/what-is/), and for Linux users there exists a wide variety of [distros](https://itsfoss.com/what-is-linux-distribution/#:~:text=Your%20distributions%20also%20takes%20the,as%20Linux%2Dbased%20operating%20systems.) to choose from. 

In this lab we will become familiar with a distinct version of Linux called [Raspberry Pi OS](https://www.raspberrypi.com/software/) (formerly known as Raspbian) which was made specifically for devices like the Pi Z2W.

Normally, we would encourage you to use the [Raspberry Pi Imager](https://www.raspberrypi.com/news/raspberry-pi-imager-imaging-utility/) tool to download the image and write it to the SD card. However, due to the limitations of the lab machines, we will need to do it a different way.

1.  Download the script that will walk you through the process of imaging your SD card. A script is like a recipe of commands that will run on your computer. You will download the script by opening up the **terminal** on your lab machine. This can be done either through finding it in the **Activities** menu or simply pressing `Ctrl+Alt+T`. Once you have the terminal open, copy and paste the following commands:

    ```bash
    wget https://byu-cpe.github.io/ecen224/assets/scripts/imager.sh
    chmod +x imager.sh
    ./imager.sh
    ```

    *(For a long command like this, it's easiest to copy and paste it into the terminal.  Use the copy button on the top right of the command box to copy the command. Use `Ctrl+Shift+V` to paste into the terminal.)*

2. This script will take a long time to run and will ask you some questions along the way.

3. Once the writing process finishes, remove the SD card from your laptop and insert it into the SD card slot of the Pi Z2W. 

4. Plug in the Power over Ethernet (PoE) adapter (the white brick) into the **first** micro USB port (the only one *not* circled in the figure at the beginning of this lab) to power up and supply internet to Pi Z2W. Power over Ethernet is a technology that provides both Internet and power at the same time. Only some Ethernet ports have it, but all of the ports in the Digital Lab do. It is not necessary to have power and the PoE adapter plugged in at the same time. 

The boot process will take a while, so wait at least **three minutes** (or until the green light starts flashing) to move to the next section.

### Connect to Pi Z2W
Now that your Pi Z2W has Raspberry Pi OS Lite installed and is connected to the lab network, we are able to connect to the it remotely using `ssh`. A remote connection means that you are able to log into a computer (like the Pi Z2W) _from_ a different computer (like the lab machines). 

1. Make sure you are able to find your Pi Z2W by opening up the **terminal** on your lab machine. This can be done either through finding it in the **Activities** menu or simply pressing `Ctrl+Alt+T`. Once the terminal has opened, search for it by entering the command:

    ```bash
    ping doorbell-<your_netid>.local
    ```

    If you receive a message like the following:

    ```
    ping: doorbell-kitras.local: Name or service not known
    ```

    then the Pi Z2W is not connected to the lab network. Check the ethernet cable and make sure that everything is plugged in correctly then try again. Sometimes it can take awhile for the Pi Z2W to connect to the network. Wait 5 minutes and try again before asking for help.

    If you receive messages like:

    ```
    PING doorbell-kitras.local (192.168.86.48) 56(84) bytes of data.
    64 bytes from doorbell-kitras.local (192.168.86.48): icmp_seq=1 ttl=64 time=95.7 ms
    64 bytes from doorbell-kitras.local (192.168.86.48): icmp_seq=2 ttl=64 time=5.47 ms
    64 bytes from doorbell-kitras.local (192.168.86.48): icmp_seq=3 ttl=64 time=25.0 ms
    64 bytes from doorbell-kitras.local (192.168.86.48): icmp_seq=4 ttl=64 time=21.6 ms
    64 bytes from doorbell-kitras.local (192.168.86.48): icmp_seq=5 ttl=64 time=6.53 ms
    ```

    Press `Ctrl+C` to stop the command. This means your lab machine can see the Pi Z2W and you are able to login with `ssh`.

1. Login to the Pi Z2W via `ssh` by typing in:

    ```bash
    ssh <your_netid>@doorbell-<your_netid>.local
    ```

    You will be asked to enter the password you chose earlier.  When you type your password in Linux you won't see anything show up in the Terminal window, but it is still being entered.  Just type your password and press `Enter` when you are done.

2. A prompt will appear asking if you want to add the Pi Z2W to a list of trusted remote computers. Type in `yes` to add the Pi Z2W to the trusted list. 

3. After entering in your login credentials, you should receive a prompt like the following to show that you are inside of the Pi Z2W:

    <figure class="image mx-auto" style="max-width: 750px">
      <img src="{% link assets/getting-started/ssh.png %}" alt="ssh">
    </figure>

    You can tell you are inside the Pi Z2W by looking at the string before the cursor. It should be `username@computer_name` or specifically `<your_netid>@doorbell-<your_netid>` on the Pi Z2W.
    
4. Now that you are logged into your Pi Z2W, we will download a script that will install all the dependencies we will need for future labs.

    First, run this command:

    ```bash
    wget https://byu-cpe.github.io/ecen224/assets/scripts/install.sh
    ```

    *(For a long command like this, it's easiest to copy and paste it into the terminal.  Use `Ctrl+Shift+V` to paste into the terminal.)*

   This will download the script to your Pi. 
   
   Next, run:

    ```bash
    chmod +x install.sh
    ```

    This command will change permissions on the script, allowing you to execute (run) the .sh file. Lastly, run the script with:

    ```bash
    ./install.sh
    ```

    You will see print statements in the terminal logging the installation process. For the last step, it will ask for your password to apply all of the changes. This is the password you set on your Pi Z2W. Once you have entered your password and the script has finished, reboot your Pi with this command:

    ```bash
    sudo reboot
    ```
  
### Connect with Visual Studio Code
Next, we will connect to your Pi Z2W using VSCode using the **Remote - SSH** extension. 

1. Open VSCode through the **Activities** menu on your lab machine. Look at the bottom status bar of the window. There should be an icon at the bottom left side of the screen:

    <figure class="image mx-auto" style="max-width: 750px">
      <img src="{% link assets/getting-started/vscode-icon.png %}" alt="vscode-icon">
    </figure>

2. If this icon is not there, that means the **Remote - SSH** extension is not installed. To install it, click on the extensions ![extensions]({% link assets/getting-started/vscode-extensions.png %}){:width="4%"} icon on the leftmost toolbar. This will open up the **Extensions Manager** where you can search for any type of tool you wish to add to your VSCode editor. 

3. Type in `Remote - SSH` into the search bar and click the **Install** button on the entry that says **Microsoft**  underneath. 

4. Once the extension has successfully installed, you can now click on the icon in the bottom left corner as seen in the figure above.

5. Click on this icon then select **Connect to Host > Add New SSH Host**. In this input box we will put the `ssh` command we used before to connect to the Pi Z2W through the terminal: `ssh <your_username>@doorbell-<your_netid>.local`. 

    <figure class="image mx-auto" style="max-width: 750px">
      <img src="{% link assets/getting-started/vs-ssh.png %}" alt="vs-ssh">
    </figure>

6. Then save this entry to your local `ssh` configuration file on our lab machine (either /home/... or /fsi/...):

    <figure class="image mx-auto" style="max-width: 750px">
      <img src="{% link assets/getting-started/ssh-conf.png %}" alt="ssh-conf">
    </figure>

7. Click on the Remote SSH icon in the bottom left corner again and this time select the new `ssh` entry you just made by clicking **Connect to Host > doorbell-\<your\_netid\>.local**. 

8. A new window will pop up and prompt you to enter in your Pi Z2W password. Enter in your password. If this is the first time you have connected to this device, it might take a few minutes to install all the necessary dependencies. After it connects, you can browse through files and code on your Pi Z2W from VSCode on your lab machine.

9. In the left toolbar on the window, click on files ![extensions]({% link assets/getting-started/files.png %}){:width="4%"} icon. 

10. Click on the **Open Folder** button and then click **OK** on the pop-up. This should put you into your home folder on Raspberry Pi OS.
    <figure class="image mx-auto" style="max-width: 750px">
      <img src="{% link assets/getting-started/open-folder.png %}" alt="open-folder">
    </figure>

    Now you can use the left pane of VS Code to view and edit files inside the home directory on the Pi Z2W.

### Create SSH key for GitHub
All of the labs require you to write code and upload it to a version control service called GitHub. A GitHub account is required for this class. If you do not have an account you can create one [here](http://github.com/join) otherwise you are allowed to use your personal account for this class. 

In order to allow our Pi Z2W to speak with GitHub, we will need to create an **SSH key** that will allow GitHub to know that the changes to the code online came from a trusted source (i.e. your Pi Z2W) and not from some impostor's device (i.e. your arch-nemesis's Pi Z2W).

1. To generate an SSH key, use the VSCode terminal (by clicking Terminal > New Terminal) - this creates a new instance of a terminal that is connected to your Pi Z2W. Type in the following:

    ```bash
    ssh-keygen -t ed25519 -C "your_email_address"
    ```

    The tool will ask you several questions. For our purposes, the default values will suffice (i.e. just hit `Enter` until it finishes) unless you desire to protect your key with a password (not recommended for this class; it would require you to enter in a password any time you want to use the SSH key).

2. Once this is done you can find the contents of your new SSH keys by typing in

    ```bash
    cat ~/.ssh/id_ed25519.pub
    ```

    **NOTE:** Make sure that you `cat` the values of `id_ed25519.pub` and **NOT** `id_ed25519`. The contents in the `.pub` are meant to be shared with the `pub`lic and the contents of the other file are not meant to be shared with anyone else.

3. Copy the output of this file by selecting it and pressing `Ctrl-Shift-C`. Then navigate in a web browser to your GitHub [keys console](https://github.com/settings/keys) (you must be signed into GitHub for this step to work).

4. At the top of the page will be a big green button that says **New SSH key**. Click on this and then you should be taken to page like the one below:

    <figure class="image mx-auto" style="max-width: 750px">
      <img src="{% link assets/getting-started/github-ssh-key.png %}" alt="github-ssh-key">
    </figure>

5. Paste the contents that we copied into the **Key** box and feel free to add whatever value you desire into the **Title**. Make sure the dropdown menu for **Key type** is set to `Authentication Key`. 

6. Finally, click **Add SSH key** and now your Pi Z2W should be able to talk to your GitHub account.

### Setup GitHub Repository
Next, we need to ensure that [`git`](https://git-scm.com/) is installed on our Pi Z2W. This will be the terminal program that we use to communicate with GitHub to version control and submit our assignments. 

1. If the terminal window on VSCode is not already open, press `` Ctrl+` `` and then enter in the command:

    ```bash
    sudo apt install git
    ```
    
    Once `git` has installed, we use this to download the first the repository (or code-base) for this lab. Since this lab requires no code to be written, the repository will be pretty uneventful. But remember these steps as future labs will use these steps to be set up.

2. Click on the **GitHub Classroom Link** located on Learning Suite for the "Getting Started Lab" assignment. You will be taken to a page that asks you to accept the assignment. Accept it and then you will be transferred to a page with a link of your repository for this assignment. Click on that link and you should be brought to the default view of the repository.

3. Click on the green button that says **Code**. Make sure you are on the **SSH** tab as shown below and copy the text in the box beneath.

    <figure class="image mx-auto" style="max-width: 750px">
      <img src="{% link assets/getting-started/ssh-repo.png %}" alt="ssh-repo">
    </figure>

4. Back on the Pi Z2W's terminal type in

```bash
git clone <GitHub Repo SSH URL>
```

This downloads a copy of the lab files from to your Pi Z2W. Since this lab is focused on setting up the Pi Z2W, there are no special files that you will need to interact with other than the `README.md` for your lab submission. 

<figure class="image mx-auto" style="max-width: 750px">
    <img src="{% link assets/getting-started/dl-repo.png %}" alt="dl-repo">
</figure>

In future labs, any starter code or special resources for completing that lab will be included alongside that lab's `README.md`.


Now it is time to start assembling the other parts of the kit. Shut down your Raspberry Pi with this command:

```
sudo shutdown -h now
```

The command `sudo` indicates to your Raspberry Pi that you want to perform the following command with admin-level priveleges. In this case, we are running the `shutdown` command, directing it to enter the `-h` halt state, and to do so immediately (`now`). You will need to provide your password when prompted. Be aware that unlike traditional password prompts, this one won't show any characters as you type your password. After running this command, wait for the lights to stop blinking on your Pi before unplugging the PoE adapter.

<!-- Doing this step at this point because we don't want to make someone do all the assembly work just to realize they flashed their SD wrong and have to take it back apart -->
### Assemble the remaining kit
We will proceed to assemble the remaining components of your doorbell.

Your kit should look something like this:

<div class="container text-center">
    <div class="row">
        <div class="col">
            <figure class="image mx-auto" style="max-width: 750px">
                <img src="{% link assets/getting-started/assembly/kit_packed.jpeg %}" alt="Parts of kit packed">
                <p>All parts still in packages</p>
            </figure>
        </div>
        <div class="col">
            <figure class="image mx-auto" style="max-width: 750px">
                <img src="{% link assets/getting-started/assembly/kit_unpacked.jpeg %}" alt="Parts of kit unpacked">
                <p>All parts unpacked</p>
            </figure>
        </div>
    </div>
</div>

1. Unpackage and prepare your display, standoffs, screws, nuts, and washers for assembly.
    <figure class="image mx-auto" style="max-width: 750px">
      <img src="{% link assets/getting-started/assembly/step_1.jpeg %}" alt="Step 1 parts">
    </figure>

2. Put the screws through the holes of the Raspberry Pi Zero, on the side with the USB ports, so that the threads of the screw are on the same side of the PCB as the USBs.

3. Put the plastic washers on the screws, one washer per screw.

4. Thread the brass standoffs onto the screws
    <figure class="image mx-auto" style="max-width: 750px">
      <img src="{% link assets/getting-started/assembly/step_4_assembly.jpeg %}" alt="Step 4 assembly state">
    </figure>

5. Take the display PCB and line up the black plastic socket (on the back side of the PCB) with the metal pins sticking out of the Raspberry Pi. Gently press the display into the socket, making sure all pins seated into their corresponding holes without getting bent. Two of the holes on the display should have slid over the brass standoffs installed earlier.
    <figure class="image mx-auto" style="max-width: 750px">
      <img src="{% link assets/getting-started/assembly/step_6_1.jpeg %}" alt="Step 6 assembly state">
    </figure>

6. Screw the nuts onto the standoffs to secure the display to the Raspberry Pi. Peel off the packaging protecting the surface of the display.
    <figure class="image mx-auto" style="max-width: 750px">
      <img src="{% link assets/getting-started/assembly/step_6_2.jpeg %}" alt="Step 6 assembly state">
    </figure>

7. Unpackage and prepare your camera kit and the case lid for assembly. Your kit came with two ribbon cables of different lengths. **WE WILL BE USING THE BROWN RIBBON CABLE, NOT THE WHITE CABLE**. Be aware that these ribbon cables are fragile.
    <figure class="image mx-auto" style="max-width: 750px">
      <img src="{% link assets/getting-started/assembly/step_7_parts.jpeg %}" alt="Step 7 parts">
    </figure>
    <figure class="image mx-auto" style="max-width: 750px">
      <img src="{% link assets/camera/pi-camera.png %}" alt="Units of the course.">
      <figcaption style="text-align: center;">The Raspberry Pi Zero 2 W with the camera unit and connection cable. In this lab we will be using the larger cable and wrap it around the back of the Pi Z2W.</figcaption>
    </figure>

8. Familiarize yourself with the connector on your camera PCB. Note the orientation of the metal pins inside the connector and the plastic shroud along the edges. This shroud "locks" the connector, preventing the cable from being removed. Unlock it by gently pulling on the edges of the shroud until it slides out
    <figure class="image mx-auto" style="max-width: 750px">
      <img src="{% link assets/getting-started/assembly/step_8_camera_closeup %}" alt="Camera connector closeup">
    </figure>

9. Insert the narrow end of the cable on the Raspberry Pi, making sure the copper contacts on the ribbon are oriented correctly. *Note - in this image, the connector is in the "locked" state*
    <figure class="image mx-auto" style="max-width: 750px">
      <img src="{% link assets/getting-started/assembly/step_10_ribbon %}" alt="Step 10 - ribbon cable inserted, locked.">
    </figure>

10. Gently wrap the ribbon cable between the Raspberry Pi and the display HAT. Be careful not to bend the metal part of the ribbon cable that is coming out of the Raspberry Pi.
    <figure class="image mx-auto" style="max-width: 750px">
      <img src="{% link assets/getting-started/assembly/step_11_bent.jpeg %}" alt="Bent ribbon cable">
    </figure>

11. Insert the wider end of the cable into the camera module, making sure the copper contacts on the ribbon are oriented correctly. *Note - in this image, the connector is in the "unlocked" state. Make sure to lock the connector once you have installed the ribbon.*
    <figure class="image mx-auto" style="max-width: 750px">
      <img src="{% link assets/getting-started/assembly/step_9_ribbon %}" alt="Step 9 - ribbon cable inserted but not locked">
    </figure>

12. There is a small adhesive strip on the back of the camera. Remove the protective layer and stick it to the camera's PCB. This will ensure the camera stays in place.
    <figure class="image mx-auto" style="max-width: 750px">
      <img src="{% link assets/getting-started/assembly/camera_sticker.jpeg %}" alt="Attach camera to PCB board.">
    </figure>

13. In this lab, you were also given a 3D printed enclosure for your Pi Z2W kit.
    <!-- Import maps polyfill -->
    <!-- Remove this when import maps will be widely supported -->
    <script async src="https://unpkg.com/es-module-shims@1.3.6/dist/es-module-shims.js"></script>

    <script type="importmap">
        {
            "imports": {
                "three": "../../assets/three.module.js"
            }
        }
    </script>

    <script type="module">

        import * as THREE from 'three';

        import { OrbitControls } from '../../assets/OrbitControls.js';
        import { ThreeMFLoader } from '../../assets/3MFLoader.js';

        let camera, scene, renderer, object, loader, controls;

        var container = document.getElementById('camera-lid');

        init();

        function init() {

            renderer = new THREE.WebGLRenderer( { antialias: true, alpha: true } );
            renderer.setPixelRatio( window.devicePixelRatio );
            renderer.setSize( 750, 750 );
            renderer.setClearColor( 0x000000, 0 ); // the default
            container.appendChild( renderer.domElement );
            renderer.domElement.style.cursor = "grab";


            scene = new THREE.Scene();

            scene.add( new THREE.AmbientLight( 0xffffff, 0.2 ) );

            camera = new THREE.PerspectiveCamera( 30, window.innerWidth / window.innerHeight, 1, 500 );

            // Z is up for objects intended to be 3D printed.

            camera.up.set( 0, 0, 1 );
            camera.position.set( - 100, - 250, 100 );
            scene.add( camera );

            controls = new OrbitControls( camera, renderer.domElement );
            controls.addEventListener( 'change', render );
            controls.minDistance = 100;
            controls.maxDistance = 1000;
            controls.enablePan = false;
            controls.update();

            const pointLight = new THREE.PointLight( 0xffffff, 0.8 );
            camera.add( pointLight );

            const manager = new THREE.LoadingManager();

            manager.onLoad = function () {

                const aabb = new THREE.Box3().setFromObject( object );
                const center = aabb.getCenter( new THREE.Vector3() );

                object.position.x += ( object.position.x - center.x );
                object.position.y += ( object.position.y - center.y );
                object.position.z += ( object.position.z - center.z );

                controls.reset();

                scene.add( object );
                render();

            };

            loader = new ThreeMFLoader( manager );
            loadAsset( '../../assets/camera/smart-doorbell-case.3mf' );

            // window.addEventListener( 'resize', onWindowResize );

        }

        function loadAsset( asset ) {

            loader.load( asset, function ( group ) {

                if ( object ) {

                    object.traverse( function ( child ) {

                        if ( child.material ) child.material.dispose();
                        if ( child.material && child.material.map ) child.material.map.dispose();
                        if ( child.geometry ) child.geometry.dispose();

                    } );

                    scene.remove( object );

                }

                object = group;

            } );

        }

        function onWindowResize() {

            camera.aspect = window.innerWidth / window.innerHeight;
            camera.updateProjectionMatrix();

            renderer.setSize( window.innerWidth, window.innerHeight );

            render();

        }

        function render() {

            renderer.render( scene, camera );

        }

    </script>

    <script type="module">

        import * as THREE from 'three';

        import { OrbitControls } from '../../assets/OrbitControls.js';
        import { ThreeMFLoader } from '../../assets/3MFLoader.js';

        let camera, scene, renderer, object, loader, controls;

        var container = document.getElementById('camera-body');

        init();

        function init() {

            renderer = new THREE.WebGLRenderer( { antialias: true, alpha: true } );
            renderer.setPixelRatio( window.devicePixelRatio );
            renderer.setSize( 500, 300 );
            renderer.setClearColor( 0x000000, 0 ); // the default
            container.appendChild( renderer.domElement );
            renderer.domElement.style.cursor = "grab";

            scene = new THREE.Scene();

            scene.add( new THREE.AmbientLight( 0xffffff, 0.2 ) );

            camera = new THREE.PerspectiveCamera( 15, window.innerWidth / window.innerHeight, 1, 500 );

            // Z is up for objects intended to be 3D printed.

            camera.up.set( 0, 0, 1 );
            camera.position.set( - 100, - 250, 100 );
            scene.add( camera );

            controls = new OrbitControls( camera, renderer.domElement );
            controls.addEventListener( 'change', render );
            controls.minDistance = 50;
            controls.maxDistance = 400;
            controls.enablePan = false;
            controls.update();

            const pointLight = new THREE.PointLight( 0xffffff, 0.8 );
            camera.add( pointLight );

            const manager = new THREE.LoadingManager();

            manager.onLoad = function () {

                const aabb = new THREE.Box3().setFromObject( object );
                const center = aabb.getCenter( new THREE.Vector3() );

                object.position.x += ( object.position.x - center.x );
                object.position.y += ( object.position.y - center.y );
                object.position.z += ( object.position.z - center.z );

                controls.reset();

                scene.add( object );
                render();

            };

            loader = new ThreeMFLoader( manager );
            loadAsset( '../../assets/cam-case-sss.3mf' );

            // window.addEventListener( 'resize', onWindowResize );

        }

        function loadAsset( asset ) {

            loader.load( asset, function ( group ) {

                if ( object ) {

                    object.traverse( function ( child ) {

                        if ( child.material ) child.material.dispose();
                        if ( child.material && child.material.map ) child.material.map.dispose();
                        if ( child.geometry ) child.geometry.dispose();

                    } );

                    scene.remove( object );

                }

                object = group;

            } );

        }

        function onWindowResize() {

            camera.aspect = window.innerWidth / window.innerHeight;
            camera.updateProjectionMatrix();

            renderer.setSize( window.innerWidth, window.innerHeight );

            render();

        }

        function render() {

            renderer.render( scene, camera );

        }

    </script>

    <span>
    <figure class="image mx-auto" style="max-width: 750px">
        <div id="camera-lid"></div>
      <figcaption style="text-align: center;">Doorbell case 3D printed file. You will need to attach the camera module to the standoffs on the lid. Note that the button in this file is not included in the case you will receive in this lab.</figcaption>
    </figure>
    As visible in the 3D model above, there are several components that comprise this enclosure. The top of the case is the long, rectangular part with holes for the camera, LCD screen, and button respectively. 
    
13. You'll notice that around the camera holes are four standoffs. These standoffs correspond to the holes in the camera module mentioned in the section before. Remove the packaging film covering the lense of your camera, then mount the camera board to the lid of the case using the 4 screws. It is easiest to do this with the Raspberry Pi face down.
    <figure class="image mx-auto" style="max-width: 750px">
      <img src="{% link assets/getting-started/assembly/step_13_mounted_camera.jpeg %}" alt="Mounted camera and ribbon cable routing">
    </figure>
    
14. Insert the Raspberry Pi into the case. Make sure the remaining 2 empty holes on the Raspberry Pi line up with the pegs in the case.
    <figure class="image mx-auto" style="max-width: 750px">
      <img src="{% link assets/getting-started/assembly/step_14_case_positioning.jpeg %}" alt="Positioning of components in the case">
    </figure>

15. Tuck the extra slack of the ribbon cable under the camera. Be careful not to pinch the ribbon cable.
    <figure class="image mx-auto" style="max-width: 750px">
      <img src="{% link assets/getting-started/assembly/step_15_case.jpeg %}" alt="Positioning of components in the case">
    </figure>

16. Snap the lid onto the bottom of the case.
    <figure class="image mx-auto" style="max-width: 750px">
      <img src="{% link assets/getting-started/assembly/step_16_completed.jpeg %}" alt="Completed assembly">
    </figure>

17. Slide the 3D button on the square peg of the display HAT.

Details about the display and camera hardware will be presented in future labs.

Turn your Pi Z2W on by plugging in your PoE adapter and re-connect to the Pi over SSH like we did earlier. 


## Lab Submission
- Answer the questions in the `README.md`. 

- To successfully submit your lab, you will need to follow the instructions in the [Lab Setup]({{ site.baseurl }}/lab-setup) page, especially the **Committing and Pushing Files** section.


## Explore More!
- [Model for doorbell case](https://www.printables.com/model/360823-smart-doorbell-case-for-raspberry-pi-zero-2-w)
- [Learn more about the terminal](https://kitras.io/linux/shells/)
- [Customize the terminal](https://www.geeksforgeeks.org/how-to-customize-linux-terminal-using-powerline10k/)
- [Ditch passwords for keys](https://dev.to/risafj/ssh-key-authentication-for-absolute-beginners-in-plain-english-2m3f)
