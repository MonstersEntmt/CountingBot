# CountingBot
![](https://img.shields.io/badge/license-GPL%203.0-yellow)
![](https://github.com/MarcaGames/CountingBot/workflows/build/badge.svg)<br>
The bot used on a Counting Server to handle messages.

## How do I contribute?
The first step in contributing is to download all the necessary tools.
You will need Premake5 ([see Premake](#Premake)) and a C++ compiler (if you're using Visual Studio you're all set!).

Next, you'll need to fork this repository.
You don't have permissions to push code directly to this repository, so you'll need to make your own copy.
To do this, press the "Fork" button in the upper-right corner of this page, and Github magic will take care of the rest!

Now that you've got a fork, you'll need to clone it. This means that you create a local (i.e. on your own hard drive) copy of the repository.
```bash
git clone --recurse-submodule -j8 https://github.com/<MyGithubAccount>/CountingBot.git
cd CountingBot
```
If that didn't work, due to using an older version of git, you might want to try
```bash
git clone https://github.com/<MyGithubAccount>/CountingBot.git
cd CountingBot
git submodule update --init --recursive
```
Excelent, you've got a local copy of this repository.
Now you need to build the code ([see Building](#Building)) and make your changes (for a basic Git workflow, [see Git](#Git)).
Remember to re-run Premake whenever you add a new file.

Changes committed and pushed?
Great! Create a Pull Request by going to the "Pull Requests" tab in Github.
This allows others to review your code, and maybe ask you to make changes if needed.
If all goes well your changes will be *merged*, nad you will see them appear in the main repository.

## Premake
This project uses Premake for project configuration. ([What is Premake?](https://github.com/premake/premake-core/wiki/What-Is-Premake))
You can get the latest version [here](https://premake.github.io/download)! Make sure you have it in your PATH or in the project root directory.

## Building
1. Open a terminal in the project root directory.
2. Run premake with the action of your choice. It's as simple as: `premake5 [action]`.<br>
   A few examples of actions are: `vs2017`/`vs2019` (Visual Studio), `gmake2` (GNU Make) and `xcode4` (Xcode).<br>
   [Here is a full list of available actions](https://github.com/premake/premake-core/wiki/Using-Premake).
   
   *(Psst! There are also a few [third-party premake extensions](https://github.com/premake/premake-core/wiki/Modules#third-party-modules) at your disposal, in case none of the official generates pique your interest)*
3. Premake will now have generated project files for the action you specified.<br>
   You should see a workspace file in the project root directory.<br>
   For Visual Studio this takes the shape of a `.sln` file. For GNU Make; a `Makefile`, etc..<br>
   This means that you can now build the project using the corresponding build tool on your computer.<br>
   If you're using GNU Make, for instance, you can now simply call `make`.<br>
   Otherwise, if you're using an IDE, you should open the workspace file with said IDE and you will be able to build the project.<br>

## FAQ
- I get `invalid value 'Objective-C++' for compileas`, what should I do?
  - Update Premake, you are likely not using the latest version
- Visual Studio keeps asking me to "Retarget Solution", how can I avoid this?
  - Use the right Premake action, `vs2017` for Visual Studio 2017 and `vs2019` for Visual Studio 2019

## Git
Note: This is just a basic workflow, for more detailed descriptions see [the documentation](https://git-scm.com/docs).

Checking the status of your repository:
```bash
git status
```
Saving (committing) your changes (do this regularly):
```bash
# For a list of files
git add <file1> <file2>
# For all files
git add *

git commit -m "Message"
```
Branch management:
```bash
git branch branchName       # Creates the branch
git checkout branchName     # To switch to that branch
git branch -d branchName    # Deletes the branch (local)
git push origin :branchName # Deletes the branch (remote)
```
Pushing your changes to a remote repository:
```bash
git push
```
Reverting back to an older commit: (**WARNING**: you will lose all uncommitted work)
```bash
# Find the commit ID (e.g. a1b2c3d4)
git log --oneline

git reset --hard a1b2c3d4
```
Ensuring your local repository is up-to-date with the original repository:
```bash
# Only do this once:
git remote add upstream https://github.com/MarcaGames/CountingBot.git

git fetch upstream
git checkout master
git merge upstream/master
```
Rebasing your branch (first, ensure your local repository is up-to-date with the original repository):
```bash
git checkout branchName
git rebase master
```
