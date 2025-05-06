---
title: Getting Started
number: 1
layout: lab
---

## Objectives

- Become familiar with the tools used for this class
- Be able to gain access remotely to the Raspberry Pi Zero 2 W
- Begin building skills needed to construct the doorbell

## Introduction

**This lab, like all others in the course, requires the ECEn 225 Lab Kit. If you do not have a kit, purchase one from the Experiential Learning Center (The Shop) in CB 416.**

<figure class="image mx-auto" style="max-width: 750px">
  <img src="{% link assets/getting-started/pi-z2w-annotated.png %}" alt="Pi Z2W with boxes.">
  <figcaption style="text-align: center;">The Raspberry Pi Zero 2 W with the SD card port outlined in magenta, the mini HDMI port outlined in light blue, the micro USB port for PoE outlined in red, and the micro USB port for power outlined in yellow.</figcaption>
</figure>

This course uses the [Raspberry Pi Zero 2 W](https://www.raspberrypi.com/products/raspberry-pi-zero-2-w/) (Pi Z2W) to teach core computing principles. In this lab, you will prepare this [single-board computer](https://en.wikipedia.org/wiki/Single-board_computer) to support all course activities.

This lab consists of four parts:

- Flash and configure the Pi Z2W with Linux  
- Gain remote access to the Pi Z2W over SSH  
- Set up a C development environment  
- Assemble the hardware components into a doorbell

## Procedure

### Part 1: Flash and Configure your PI Z2W

#### Setting up the SD Card

To use the Pi Z2W, install an operating system on its SD card. An [**operating system**](https://en.wikipedia.org/wiki/Operating_system) (OS) is a set of programs that allows users to interact with hardware. Windows, macOS, and Linux are examples of operating systems, each tailored to different platforms. Linux offers a variety of [distributions](https://itsfoss.com/what-is-linux-distribution/#:~:text=Your%20distributions%20also%20takes%20the,as%20Linux%2Dbased%20operating%20systems.) suited for different needs.

This lab uses [Raspberry Pi OS](https://www.raspberrypi.com/software/), a Linux distribution designed for Raspberry Pi devices.

Normally, you would use the [Raspberry Pi Imager](https://www.raspberrypi.com/news/raspberry-pi-imager-imaging-utility/) tool to install the OS. However, due to lab machine limitations, follow this alternative method.

1. Open a terminal on your lab machine using the **Activities** menu or the shortcut `Ctrl+Alt+T`. Download and run the imaging script by entering:

    ```bash
    wget https://byu-cpe.github.io/ecen224/assets/scripts/imager.sh
    chmod +x imager.sh
    ./imager.sh
    ```

    *(Use `Ctrl+Shift+V` to paste into the terminal after copying the command.)*

2. Wait for the script to complete. It may take a long time; it will prompt you for input during the process.

3. After the script finishes, remove the SD card from the computer and insert it into the SD card slot of the Pi Z2W.

#### Assemble the Display

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

In order to see that your Z2W was configured correctly, we need to assemble the display. Assemble the display portion of your doorbell kit using the following steps:

1. Unpackage the display, standoffs, screws, nuts, and washers.

    <figure class="image mx-auto" style="max-width: 750px">
      <img src="{% link assets/getting-started/assembly/step_1.jpeg %}" alt="Step 1 parts">
    </figure>

2. Insert the screws through the Raspberry Pi Zero from the side with the USB ports, so the threads protrude on the same side as the USB connectors.

3. Place one plastic washer on each screw.

4. Thread a brass standoff onto each screw.

    <figure class="image mx-auto" style="max-width: 750px">
      <img src="{% link assets/getting-started/assembly/step_4_assembly.jpeg %}" alt="Step 4 assembly state">
    </figure>

5. Align the black plastic socket on the back of the display PCB with the Raspberry Pi’s GPIO pins. Press the display gently into place, ensuring all pins seat correctly. The two mounting holes on the display should align with the standoffs.

    <figure class="image mx-auto" style="max-width: 750px">
      <img src="{% link assets/getting-started/assembly/step_6_1.jpeg %}" alt="Step 6 assembly state">
    </figure>

6. Secure the display by screwing the nuts onto the standoffs. Remove the protective film from the display surface.

    <figure class="image mx-auto" style="max-width: 750px">
      <img src="{% link assets/getting-started/assembly/step_6_2.jpeg %}" alt="Step 6 assembly state">
    </figure>

7. Set aside the remaining parts of the kit. We will come back to them in **Part 4**.

### Part 2: Connect to Pi Z2W over SSH

Use the Power over Ethernet (PoE) adapters—the white bricks at each lab computer—to power the Pi Z2W. PoE transmits both power and internet through a single Ethernet cable, eliminating the need for separate power cables. While not all Ethernet ports support PoE, every port in the Digital Lab does.

To power the Pi Z2W, unplug the Ethernet cable from the PoE adapter. Connect the adapter to the **first** micro USB port (labeled "USB" and located on the left in the figure at the beginning of this lab). Reconnect the Ethernet cable to the PoE adapter. This sequence ensures the network assigns an IP address to the Pi over the Ethernet connection.

Allow up to three minutes for the Pi to boot. When the system is ready, the display will show a blue screen with two addresses: the localhost address (127.0.0.1) and an assigned IP address in the form 10.32.X.X. If the IP address does not appear, unplug the Ethernet cable from the PoE adapter and reconnect it. If the issue continues, notify a TA.

With Raspberry Pi OS Lite installed and the Pi Z2W connected to the lab network, you can now access it remotely using `ssh`. This allows you to log into the Pi Z2W from another machine, such as the lab computer.

1. Verify that your lab machine can detect the Pi Z2W. Open the **terminal** (`Ctrl+Alt+T`) and run:

    ```bash
    # replace NETID with your NetID
    ping doorbell-NETID.local
    ```

    If the output shows:

    ```bash
    ping: doorbell-NETID.local: Name or service not known
    ```

    then the Pi Z2W is not connected to the lab network. Confirm the Ethernet cable is connected properly to both the correct port on the Pi and the wall jack. Wait several minutes before retrying. If it still fails, seek assistance.

    If the output shows:

    ```bash
    PING doorbell-NETID.local (192.168.86.48) 56(84) bytes of data.
    64 bytes from doorbell-NETID.local (192.168.86.48): icmp_seq=1 ttl=64 time=95.7 ms
    ...
    # it will keep pinging until you stop it
    ```

    press `Ctrl+C` to stop the ping. Your machine can now communicate with the Pi Z2W.

2. Log in via SSH:

    ```bash
    ssh NETID@doorbell-NETID.local
    ```

    Enter the password set earlier. Nothing will appear as you type. Press `Enter` when finished.

3. When prompted to confirm trust in the remote machine, type `yes`.

4. Once logged in, confirm you are in the Pi by checking the prompt:

    ```text
    NETID@doorbell-NETID:~$
    ```

    <figure class="image mx-auto" style="max-width: 750px">
      <img src="{% link assets/getting-started/ssh.png %}" alt="ssh">
    </figure>

5. Download the setup script to install required tools:

    ```bash
    wget https://byu-cpe.github.io/ecen224/assets/scripts/install.sh
    ```

6. Make the script executable:

    ```bash
    chmod +x install.sh
    ```

7. Execute the script:

    ```bash
    ./install.sh
    ```

    You will see various outputs in the terminal as packages install. When prompted for your password, enter the Pi Z2W password you set earlier.

8. Reboot the Pi Z2W:

    ```bash
    sudo reboot
    ```

    This will disconnect your Pi Z2W from the lab machine in the process. You will reconnect to it in the next part.

### Part 3: Set Up your Development Environment

#### Setting Up VSCode on the Z2W

1. Launch VSCode from the **Activities** menu.

2. Check the lower-left corner of the VSCode window for the green icon:

    <figure class="image mx-auto" style="max-width: 750px">
      <img src="{% link assets/getting-started/vscode-icon.png %}" alt="vscode-icon">
    </figure>

3. If the icon is missing, install the **Remote - SSH** extension:

    - Click the extensions icon:
      ![extensions]({% link assets/getting-started/vscode-extensions.png %}){:width="4%"}
    - Search for `Remote - SSH`
    - Click **Install** on the extension authored by **Microsoft**

4. After installation, click the green icon in the lower-left.

5. Select **Connect to Host > Add New SSH Host**.

6. Enter the full SSH command you used in **Part 2**:

    ```bash
    ssh NETID@doorbell-NETID.local
    ```

    <figure class="image mx-auto" style="max-width: 750px">
      <img src="{% link assets/getting-started/vs-ssh.png %}" alt="vs-ssh">
    </figure>

7. Save to your local SSH config (e.g., `/home/...` or `/fsi/...`):

    <figure class="image mx-auto" style="max-width: 750px">
      <img src="{% link assets/getting-started/ssh-conf.png %}" alt="ssh-conf">
    </figure>

8. Click the Remote SSH icon again, then choose `Connect to Host` and then click the `doorbell-NETID.local` entry you just set up.

9. A new window opens. Enter your Pi Z2W password. Wait for some dependencies to install if prompted.

10. Click the file explorer icon in the left sidebar:
    ![extensions]({% link assets/getting-started/files.png %}){:width="4%"}

11. Click **Open Folder**, then **OK**. This opens the Pi Z2W home directory in VSCode.

    <figure class="image mx-auto" style="max-width: 750px">
      <img src="{% link assets/getting-started/open-folder.png %}" alt="open-folder">
    </figure>

    You can now access and edit files on your Pi Z2W directly through VSCode.

#### Set Up GitHub and Clone your Starter Labs
<!-- Doing this step at this point because we don't want to make someone do all the assembly work just to realize they flashed their SD wrong and have to take it back apart -->

All labs require code to be cloned from and uploaded to GitHub. Create a Github account at [github.com/join](http://github.com/join) if you don't already have one. You may use your personal account if you preferr.

To configure GitHub on your Pi Z2W:

1. Navigate to the `GitHub Setup` tab.
2. Complete all steps in the **First Time Setup** section. This includes:
    - Setting your username and email in git
    - Generating an SSH key
    - Adding the SSH key to your GitHub account
    - Verifying the SSH connection to GitHub

3. Proceed to the **Cloning A Repository** section.
4. Follow the instructions to clone your `Starter Labs` repository.
    - The GitHub Classroom link is provided in Learning Suite under the schedule entry for Lab 1.

After cloning, the code will be available in your Pi Z2W’s filesystem for development and submission.

<!-- Doing this step at this point because we don't want to make someone do all the assembly work just to realize they flashed their SD wrong and have to take it back apart -->

### Part 4: Assemble the Remaining Kit

Next we will proceed to assemble the remaining components of your doorbell kit.

1. Unpack the camera kit and case lid. Use the **brown** ribbon cable (not the white one). The ribbon cables are fragile.Handle them with care, and do not crease or bend them extremely sharply.

    <figure class="image mx-auto" style="max-width: 750px">
      <img src="{% link assets/getting-started/assembly/step_7_parts.jpeg %}" alt="Step 7 parts">
    </figure>
    <figure class="image mx-auto" style="max-width: 750px">
      <img src="{% link assets/camera/pi-camera.png %}" alt="Units of the course.">
      <figcaption style="text-align: center;">The Raspberry Pi Zero 2 W with the camera unit and connection cable. In this lab we will be using the larger cable and wrap it around the back of the Pi Z2W.</figcaption>
    </figure>

2. Familiarize yourself with the connector on your camera PCB. Notice the small dark grey shroud around the white plastic of the connector. This shroud "locks" the connector, preventing the cable from being removed. Unlock it by *gently* pulling on the edges of the shroud until it slides out. Again, use caution.

    <figure class="image mx-auto" style="max-width: 750px">
      <img src="{% link assets/getting-started/assembly/step_8_camera_closeup %}" alt="Camera connector closeup">
    </figure>

3. Insert the narrow end of the brown ribbon cable into the Pi's connector. Copper contacts should face downwards toward the circuit board.

    <figure class="image mx-auto" style="max-width: 750px">
      <img src="{% link assets/getting-started/assembly/step_10_ribbon %}" alt="Step 10 - ribbon cable inserted, locked.">
    </figure>

4. Route the ribbon cable between the Pi and the display HAT. Avoid bending the metal part of the ribbon near the connector.

    <figure class="image mx-auto" style="max-width: 750px">
      <img src="{% link assets/getting-started/assembly/step_11_bent.jpeg %}" alt="Bent ribbon cable">
    </figure>

5. Insert the wide end of the cable into the camera module. Copper contacts should face downwards towards the circuit board. Lock the connector after insertion.

    <figure class="image mx-auto" style="max-width: 750px">
      <img src="{% link assets/getting-started/assembly/step_9_ribbon %}" alt="Step 9 - ribbon cable inserted but not locked">
    </figure>

6. There is a small adhesive strip on the back of the camera lens. Remove the protective layer and stick it to the camera's PCB.

    <figure class="image mx-auto" style="max-width: 750px">
      <img src="{% link assets/getting-started/assembly/camera_sticker.jpeg %}" alt="Attach camera to PCB board.">
    </figure>

7. In this lab, you were also given a 3D printed enclosure for your Pi Z2W kit.
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

    *Notice: Your kit does not contain a button, as the buttons in this model did not function well enough to be produced.*

8. Remove the lens film from the camera. Mount the camera board to the lid of the case using the 4 screws provided.

    <figure class="image mx-auto" style="max-width: 750px">
      <img src="{% link assets/getting-started/assembly/step_13_mounted_camera.jpeg %}" alt="Mounted camera and ribbon cable routing">
    </figure>

9. Insert the Pi into the case by aligning the remaining two holes with the case pegs.

    <figure class="image mx-auto" style="max-width: 750px">
      <img src="{% link assets/getting-started/assembly/step_14_case_positioning.jpeg %}" alt="Positioning of components in the case">
    </figure>

10. Tuck the slack of the ribbon cable under the camera. Be careful not to pinch or crease the ribbon cable.

    <figure class="image mx-auto" style="max-width: 750px">
      <img src="{% link assets/getting-started/assembly/step_15_case.jpeg %}" alt="Positioning of components in the case">
    </figure>

11. Snap the lid onto the bottom of the case.

    <figure class="image mx-auto" style="max-width: 750px">
      <img src="{% link assets/getting-started/assembly/step_16_completed.jpeg %}" alt="Completed assembly">
    </figure>

Details about the display and camera hardware will be presented in future labs.

Turn your Pi Z2W back on by unplugging the ethernet from the PoE adapter, plugging your PoE adapter into the Pi, and reconnecting the ethernet. Re-connect to the Pi over SSH in VS Code like we did earlier.

## Lab Submission

- Demonstrate to a TA that your Z2W is set up correctly. Show them that:
  - you can connect to your pi over SSH on VSCode
  - your Z2W uses `zsh` (the shell installed with the second install script).
- Complete the Learning Suite Pass-off quiz.
- Take a look at the `submission.md` file included in your Starter-Labs repo. You will fill this out over the next 4 labs.

## Explore More

- [Model for doorbell case](https://www.printables.com/model/360823-smart-doorbell-case-for-raspberry-pi-zero-2-w)
- [Learn more about the terminal](https://kitras.io/linux/shells/)
- [Customize the terminal](https://www.geeksforgeeks.org/how-to-customize-linux-terminal-using-powerline10k/)
- [Ditch passwords for keys](https://dev.to/risafj/ssh-key-authentication-for-absolute-beginners-in-plain-english-2m3f)
