---
layout: page
toc: false
title: Lab Setup
icon: fa-duotone fa-wrench
---

### Git Configuration
Every time you use a **new machine for the first time**, you will need to need to let the `git` program know who is making changes to any repository that you have cloned to your computer. To set the default email address and name associated 

```bash
git config --global user.name "Your name here"
git config --global user.email your_email@email.com
```

### GitHub Class Assignments

Every lab will have a corresponding GitHub **repository**, or code base. This will normally be a collection of files that you will need to complete the lab, including the `README.md` where you will answer lab questions. To access the GitHub Classroom link for a lab click on the corresponding lab assignment on Learning Suite.

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
