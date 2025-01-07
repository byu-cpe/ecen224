#!/bin/bash
set -xe

echo "----------------- Upgrading System -----------------"
sudo apt update
sudo apt upgrade -y

echo "----------------- Installing Dependencies -----------------"
sudo apt install -y git zsh gdb libcamera-dev libjpeg-dev libtiff5-dev cmake libboost-program-options-dev libdrm-dev libexif-dev tmux vim

echo "------------------- Setting up Swapfile -------------------"
sudo dphys-swapfile swapoff
sudo sed -i 's/CONF_SWAPSIZE=.*/CONF_SWAPSIZE=1024/g' /etc/dphys-swapfile
sudo dphys-swapfile setup
sudo dphys-swapfile swapon

echo "------------------ Installing Oh My Zsh! ------------------"
sh -c "$(curl -fsSL https://raw.githubusercontent.com/ohmyzsh/ohmyzsh/master/tools/install.sh)" "" --unattended

echo "----------- Changing Shell ---------------"
chsh -s /usr/bin/zsh
