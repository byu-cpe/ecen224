---
layout: page
toc: false
title: GitHub Setup
icon: fa-duotone fa-wrench
---

## First Time Setup

Every time you use a **new machine for the first time**, you will need to follow these steps.

### Git Configuration

First, you will need to need to let the `git` program know who is making changes to any repository that you have cloned to your computer. To set the default email address and name your computer (or Raspberry Pi) will use, run the following commands:

```bash
git config --global user.name "Your name here"
git config --global user.email your_email@email.com
```

You should use the email address you used to sign up for GitHub.

### GitHub SSH Keys

1. To generate an SSH key, use the VSCode terminal (by clicking `Terminal > New Terminal`) - this creates a new instance of a terminal that is connected to your Pi Z2W. Type in the following:

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

## Cloning and Pushing Files

### Cloning a Repository

Git organizes code into **repositories**, which is just another name for a collection of files. We have two repositories in this class: One that contains several starter labs to teach you basic principles, and one that we will use as the basic template to create our doorbell. They both contain a `README.md` file which gives you instructions on what to do with the code. To organize all your the repositories together as a class, we use **GitHub Classroom**. To get access to a specific repository, click the corresponding lab assignment on Learning Suite.

Once you have accepted the assignment on Github Classroom, a copy of the lab repository will be added to the GitHub user account you are using for this class. To **clone** these files (download them to your computer or Pi), you will need to need the repository link. **This NOT just the URL of your repository.** 

To obtain the repository link click on the green `<> Code` button and make sure the **SSH** tab is selected.

<figure class="image mx-auto" style="max-width: 750px">
    <img src="{% link assets/lab-setup/url.png %}" alt="select-device">
</figure>

Copy the repository link in the textbox below and then on the machine where you wish to clone the repository type in

```bash
git clone your_github_repository
```

### Committing and Pushing Files

Once you have finished editing all the files you need to on cloned repository, you will need to `commit` and `push` the files to synchronize them with the online version of your repository.

First you will navigate in a terminal to your git repository. If you are not in your repository folder you can run

```bash
cd path/to/your/github_repository_folder
```

Next, add all the files to a list items you wish to sync with the cloud. You only need to add files that you've changed since the repo was last cloned or had changes made.

```bash
# You can list individual files
git add file1.txt file2.md ...
# OR use -A for all
git add -A
```

Then you need to group the files into one package called a commit. Git allows you to use commits as a checkpoint to save your progress.

```bash
git commit -m"Add a message here"
```

Finally, to push these changes to the repository online, run:

```bash
git push origin main
```

the `origin` tells Git which branch to push this commit to. See branching below.

### Git Branches

Git branches are a fundamental feature in Git that allow you to work on multiple versions of a project simultaneously. A branch in Git represents an independent line of development, enabling you to make changes without affecting the main project or other developers' work. The default branch in a new Git repository is typically called `main` (or `master` in some older repositories), but you can create additional branches for specific features, bug fixes, or experiments. By using branches, you can isolate work, test new ideas, and merge them back into the main branch when they're ready.

Here is a graph that shows some of the intuition of version control and git branches.

<figure class="image mx-auto" style="max-width: 750px">
    <img src="{% link assets/lab-setup/git_branches.png %}" alt="select-device">
</figure>

To create a new branch, use

```bash
git branch <new-branch-name>
```

You can then switch your active branch to this new branch with

```bash
git checkout <new-branch-name>
```

You can also list all of your branches with

```bash
git branch
```

To merge a branch into another, first checkout the branch you want to merge **into**. Then, use

```bash
git merge <feature_branch>
```

You can also use the git panel in VS code to perform all of these operations. The branch menu has of all this functionality.

<figure class="image mx-auto" style="max-width: 750px">
    <img src="{% link assets/lab-setup/vs_code_git.png%}" alt="select-device">
</figure>
