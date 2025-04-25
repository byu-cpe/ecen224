---
layout: page
toc: false
title: GitHub Setup
icon: fa-duotone fa-wrench
---

### Git Configuration
Every time you use a **new machine for the first time**, you will need to need to let the `git` program know who is making changes to any repository that you have cloned to your computer. To set the default email address and name associated run the following commands:

```bash
git config --global user.name "Your name here"
git config --global user.email your_email@email.com
```

Your email address should be the same one you used to sign up for GitHub.

### GitHub SSH Keys
You should only need to do this once and it is already onlined in Lab 1, but is included here for your reference.

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

### GitHub Class Assignments

Every lab will have a corresponding GitHub **repository**, or code base. This will normally be a collection of files that you will need to complete the lab, including a `README.md` outline or prompt that you will need to fill in. To access the GitHub Classroom link for a lab click on the corresponding lab assignment on Learning Suite.

Once you have accepted the assignment, a copy of the lab repository will be added to the GitHub user account you are using for this class. To **clone** these files, or download them to your computer (or for this lab, to the Pi Z2W), you will need to need the repository link. **This is different than the URL of your repository.** 

To obtain this link click on the green `<> Code` button and make sure the **SSH** tab is selected. 

<figure class="image mx-auto" style="max-width: 750px">
    <img src="{% link assets/lab-setup/url.png %}" alt="select-device">
</figure>

Copy the repository link in the textbox below and then on the machine where you wish to clone the repository type in
```bash
git clone your_github_repository
```

### Committing and Pushing Files

Once you have finished editing all the files you need to on cloned repository, you will need to `commit` and `push` the files to synchronize them with the repository online. This will be necessary for me to view your files so I can give you a grade.

First you will need be in the folder with your git repository. If you are not in your repository folder you can run
```
cd your_github_repository_folder
```
Then you can add all the files to the list of files that you wish to sync. This can be done with
```bash
git add your.txt edited.txt files.txt
```
If you wish to add all the files to the sync list in your cloned repository, you can simply run
```bash
git add -A
```

Next you will need to group the files ready to sync into one package called a commit. This will is done by
```bash
git commit -m"Add a message here"
```

Finally, to push these changes to the repository online
```bash
git push origin main
```

### Git Branches
Git branches are a fundamental feature in Git that allow you to work on multiple versions of a project simultaneously. A branch in Git represents an independent line of development, enabling you to make changes without affecting the main project or other developers' work. The default branch in a new Git repository is typically called main (or master in some older repositories), but you can create additional branches for specific features, bug fixes, or experiments. By using branches, you can isolate work, test new ideas, and merge them back into the main branch when they're ready.

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

<!-- 
### Tagging Submissions

To submit your lab, you must create a **tag** named `final` on your git repository. The following command will tag your most recent commit and push that tag to GitHub:

```bash
git tag final
git push origin final
```

This tag will point to your most recent commit of whichever branch you are currently located on (so make sure all of your changes are committed before running this). If you are not confident you did this correctly, you may want to go to a new directory (not in your repo) and run `git clone --branch final <repo_url>` to clone your tag and verify that it builds and runs correctly.

If, after you create this tag, you want to change it (i.e., re-submit your code), you can run the following commands and include the –force option to overwrite the tag:
```bash
git tag --force final
git push --force origin final
```
If you don’t use the correct tag name (`final`), the lab will not be counted as submitted. -->
