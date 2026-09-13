---
layout: page
toc: false
title: Doorbell Setup
icon: fa-duotone fa-wrench
---

## !!! Under Construction !!!

## SSH Keys

1. To generate an SSH key, type the following at a terminal:

    ```bash
    ssh-keygen -t ed25519 -C "your_email_address"
    ```

    The tool will ask you several questions. For our purposes, the default values will suffice (i.e. just hit `Enter` until it finishes) unless you desire to protect your key with a password (not recommended for this class; it would require you to enter in a password any time you want to use the SSH key).

2. Once this is done you can find the contents of your new SSH keys by typing in

    ```bash
    cat ~/.ssh/id_ed25519.pub
    ```

    **NOTE:** Make sure that you `cat` the values of `id_ed25519.pub` and **NOT** `id_ed25519`. The contents in the `.pub` are meant to be shared with the `pub`lic and the contents of the other file are not meant to be shared with anyone else.
