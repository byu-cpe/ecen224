#!/bin/bash

# Enable error tracing
set -e

# Variables
RPI_OS_URL="https://downloads.raspberrypi.com/raspios_lite_arm64/images/raspios_lite_arm64-2024-07-04/2024-07-04-raspios-bookworm-arm64-lite.img.xz"
IMG_FILE="raspios_lite_arm64_latest.img"
IMG_FILE_XZ="$IMG_FILE.xz"
BOOT_PARTITION="/media/$(whoami)/bootfs"

# Function to print in in color
function echo_red {
    echo -e "\033[31m$1\033[0m"
}
function echo_green {
    echo -e "\033[32m$1\033[0m"
}

echo_green "Welcome to the ECEn 225 Raspberry Pi Imager! This script will image your SD card and create a new user on your Raspberry Pi. You will be asked for a username and password for the user."
echo_red "Use a DIFFERENT password than you did for your BYU or CAEDM account."
echo_green ""

# Prompt the user if they want to proceed
read -p "Would you like to proceed? (y/n): " proceed
if [[ "$proceed" != "y" ]]; then
    exit 0
fi

echo ""
echo_green "Start by plugging in the SD card and adapter into the USB slot."
read -p "Press enter once you have plugged in the SD card..." proceed
echo ""

# Prompt for username
echo_green "We will now set up the credentials for the new account we will create on the Raspberry Pi."
echo ""
read -p "Enter NetID: " username

# Prompt for password and confirm it
while true; do
    read -sp "Enter password: " password
    echo ""
    read -sp "Confirm password: " password_confirm
    echo ""

    if [ "$password" == "$password_confirm" ]; then
        break
    else
        echo_red "Passwords do not match. Please try again."
    fi
done

# Hash the password using openssl (sha-256)
hashed_password=$(echo "$password" | openssl passwd -5 -stdin)

# List available drives using lsblk, filtering for sdX drives only
echo ""
echo_green "Checking for available drives..."
available_drives=$(lsblk -d -o NAME,SIZE,MODEL | grep -E '^sd' || true)

# Check if any sdX drives are found
if [[ -z "$available_drives" ]]; then
    echo_red "Error: No SD card detected. Please plug in your SD card and try again."
    exit 1
fi

# Display the list of available drives
echo ""
echo "Available drives:"
echo "$available_drives"
echo ""

# Prompt user for the target drive (must match sdX pattern)
read -p "Enter the target drive (e.g., sda): " drive

# Validate if the drive exists and matches the pattern sdX
if [[ ! $drive =~ ^sd[a-z]$ ]]; then
    echo_red "Error: /dev/$drive is not a valid sdX block device."
    exit 1
fi

# Check if the device exists as a block device
if [ ! -b "/dev/$drive" ]; then
    echo_red "Error: /dev/$drive is not a valid block device."
    exit 1
fi

# Download the Raspberry Pi OS Lite (64-bit) image
echo ""
echo_green "Downloading Raspberry Pi OS Lite..."
wget -O "$IMG_FILE_XZ" $RPI_OS_URL

# Uncompress the xz file (this might take a few minutes)
echo ""
echo_green "Uncompressing the image file... This may take a few minutes."
xz -d "$IMG_FILE_XZ"

# Write the image to the selected drive (this may also take a few minutes)
echo ""
echo_green "Writing the image to /dev/$drive... This may take a few minutes."
dd if="$IMG_FILE" of=/dev/$drive bs=4M status=progress conv=fsync

echo ""
echo_green "Next, we need to mount the drive. First, unplug your USB drive and plug it back in. After plugging it in, there should be two USB drive icons in the toolbar on the left. You can tell them apart by hovering your mouse over them. Click on the \"bootfs\" icon. This will mount the drive."

read -p "Press enter once you have mounted the drive..." proceed

# Write the firstrun.sh file dynamically with the user's username and hashed password
echo ""
echo_green "Writing the firstrun.sh file..."

cat <<EOF | tee $BOOT_PARTITION/firstrun.sh >/dev/null
#!/bin/bash

set +e

CURRENT_HOSTNAME=\$(cat /etc/hostname | tr -d " \t\n\r")
if [ -f /usr/lib/raspberrypi-sys-mods/imager_custom ]; then
    /usr/lib/raspberrypi-sys-mods/imager_custom set_hostname doorbell-$username
else
    echo doorbell-$username >/etc/hostname
    sed -i "s/127.0.1.1.*\$CURRENT_HOSTNAME/127.0.1.1\\tdoorbell-$username/g" /etc/hosts
fi

FIRSTUSER=\$(getent passwd 1000 | cut -d: -f1)
FIRSTUSERHOME=\$(getent passwd 1000 | cut -d: -f6)
if [ -f /usr/lib/raspberrypi-sys-mods/imager_custom ]; then
    /usr/lib/raspberrypi-sys-mods/imager_custom enable_ssh
else
    systemctl enable ssh
fi

if [ -f /usr/lib/userconf-pi/userconf ]; then
    /usr/lib/userconf-pi/userconf "$username" '$hashed_password'
else
    echo "\$FIRSTUSER:"'$hashed_password' | chpasswd -e
    if [ "\$FIRSTUSER" != "$username" ]; then
        usermod -l "$username" "\$FIRSTUSER"
        usermod -m -d "/home/$username" "$username"
        groupmod -n "$username" "\$FIRSTUSER"
        if grep -q "^autologin-user=" /etc/lightdm/lightdm.conf; then
            sed /etc/lightdm/lightdm.conf -i -e "s/^autologin-user=.*/autologin-user=$username/"
        fi
        if [ -f /etc/systemd/system/getty@tty1.service.d/autologin.conf ]; then
            sed /etc/systemd/system/getty@tty1.service.d/autologin.conf -i -e "s/\$FIRSTUSER/$username/"
        fi
        if [ -f /etc/sudoers.d/010_pi-nopasswd ]; then
            sed -i "s/^\$FIRSTUSER /\$username /" /etc/sudoers.d/010_pi-nopasswd
        fi
    fi
fi

if [ -f /usr/lib/raspberrypi-sys-mods/imager_custom ]; then
    /usr/lib/raspberrypi-sys-mods/imager_custom set_keymap 'us'
    /usr/lib/raspberrypi-sys-mods/imager_custom set_timezone 'America/Denver'
else
    rm -f /etc/localtime
    echo "America/Denver" >/etc/timezone
    dpkg-reconfigure -f noninteractive tzdata
    cat >/etc/default/keyboard <<'KBEOF'
XKBMODEL="pc105"
XKBLAYOUT="us"
XKBVARIANT=""
XKBOPTIONS=""

KBEOF
    dpkg-reconfigure -f noninteractive keyboard-configuration
fi

rm -f /boot/firstrun.sh
sed -i 's| systemd.run.*||g' /boot/cmdline.txt
exit 0
EOF

# Make the firstrun.sh file executable
chmod +x $BOOT_PARTITION/firstrun.sh

# Edit cmdline.txt
echo ""
echo_green "Updating cmdline.txt to configure firstrun.sh..."
sed -i '1 s/$/ systemd.run=\/boot\/firstrun.sh systemd.run_success_action=reboot systemd.unit=kernel-command-line.target/' $BOOT_PARTITION/cmdline.txt

# Cleanup
echo ""
echo_green "Cleaning up..."
rm -f "$IMG_FILE" "$IMG_FILE_XZ"

echo ""
echo_green "Raspberry Pi OS Lite (64-bit) has been written to /dev/$drive."
echo ""
echo_green "Eject the drive by right clicking on the USB drive icon and select \"Eject\"."
