# Battleship - CaNS Assignment 2
A hive-mind battleship network.

## Project Prerequisites
Linux:
- wine
- mingw-w64-gcc
- cmake

Windows:
- VisualStudio
- Git bash (https://git-scm.com/downloads)

Both:
- SSH keys added to github (will need to be done for whatever computer you're working on)

## Adding SSH keys to github
1. Generate key with terminal or git bash `ssh-keygen -t rsa`
2. Press enter until complete, only noobs add passphrases to their keys
3. Login to github, go to `settings->SSH and GPG keys->New SSH key`
4. Enter whatever title you'd like
5. Copy the output of `cat ~/.ssh/id_rsa.pub` into key
6. Click `Add SSH key`

## Cloning source code
1. In terminal or git bash, locate to the directory you wish `cd ~/Documents`
2. Clone the source with git `git clone git@github.com:novucs/battleship.git`

## Compiling from source
Linux:

1. In terminal, locate to the project `cd ~/Documents/battleship`
2. Execute the compile and run shell script `./battleship`

Windows:

1. Open VisualStudio `File->New->Project From Existing Code...`
2. Select `Visual C++` then click `Next`
3. Set the file location to where you cloned it in src e.g. `~/Documents/battleship/src`
4. Set the project name to `battleship` then click `Next`
5. Set project type to `Console application project` then click `Finish`
6. Press the `Local Windows Debugger` button to compile and run the project

## Pulling updates
1. In terminal or git bash, locate to the project `cd ~/Documents/battleship`
2. Pull recent changes from github `git pull origin master`

## Commiting changes
1. In terminal or git bash, locate to the project `cd ~/Documents/battleship`
2. Add all modified files to the commit `git add .`
3. Create a commit message for the changes made `git commit -m "Your message here"`
4. Push commit to the github repository `git push origin master`

## Reset code to what was last pulled
This is useful for when you wish to pull more recent changes, but you've made changes that you want to discard. Normally you'd have to start over with a new project, but this will save you a lot of time.

1. Find the latest commit hash `git log` then `q` (it should be the first one listed)
2. A commit hash looks something like this `a2746de68499fadf45b0cd38fc091b4d4e7d1e75`
3. Reset the code back to the original state, discarding your changes `git reset --hard a2746de68499fadf45b0cd38fc091b4d4e7d1e75`

## Battleship tactics documentation
You will be required to request access. Let me know when you've requested and I can confirm you are the one with the link.

**[Click here for the documentation](https://docs.google.com/a/novucs.net/document/d/1yhaYjSL_HKnegUqZeZKtO8CgC-vR6Yzp3QstVKx_64c/edit?usp=sharing)**
