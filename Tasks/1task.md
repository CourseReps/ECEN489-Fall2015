# Code Management System


## Git and Source Code Management

[Git](http://git-scm.com/) is a popular source code management system.
Every Git working directory is a full-fledged repository, with complete history and full version tracking capabilities, not dependent on network access or a central server.
In this sense, it trades off space for speed.
Git is primarily developed on Linux, although it also supports most major operating systems including BSD, OS X, and Microsoft Windows.
Pertinent information about Git can be found on the project website.

### Action Items

* __Peruse__: [Git documentation](http://git-scm.com/doc/).
* __Complete__: [Git tutorial](https://try.github.io/levels/1/challenges/1).
* __Download and Install__: [Git software](http://git-scm.com/downloads) (if needed).


## GitHub

[GitHub](https://github.com/) is a web-based hosting service for source code management (SCM).
It is built on the Git revision control system and offers free accounts for open source and educational projects.
We will employ a Git archive for programming challenges throughout the semester.
The master Git repository for this course is hosted on GitHub.

https://github.com/CourseReps/ECEN489-Fall2015.git

Active participants are given write access to the master repository.
A short list of frequently used commands appears below.

### Common Actions

* __Init__:
The `init` command creates a new local repository.
* __Clone__:
Use `clone` to instantiate a working copy from a master repository.
This is usually the first command employed to establish a local working hierarchy under this paradigm.
* __Add__:
The `add` command is used to add one or more files to staging.
Only add pertinent files to the repository.
* __Commit__:
The `commit` command incorporates changes to your working copy of the repository.
* __Push__:
The `push` command sends changes to the master branch, typically a remote repository.
* __Pull__:
The `pull` command fetches and merges changes on the remote server to the local working directory.
* __Mergetool__:
Sometimes, there may be a discrepancy between the latest version of a file and its working copy on a given host.
In such cases, the developer may need to take action to resolve these issues.
This can be achieved through normal editing, followed by the Git `add` command.
Alternatively, one can use the  `mergetool` command, which initiates a visual tool.
* __Status__:
The `status` command lists the status of working files and directories.

### Action Items

* __Account__: Go to [GitHub](https://github.com) and create a developer account (if needed).
Complete the GitHub fields `Name`, `Public email`, and upload a picture.
Email your GitHubID to your instructor via your TAMU account.
You will need write permission before you can proceed further.
* __Web__: Locate the `README.md` under `Students`.
Add your GitHubID next to you name.

```
* <Full Name>, [GitHubID](https://github.com/GitHubID)
```

* __Clone__: Once you have been added to the list of collaborators, use `git` to clone the master repository.
* __Directory__: Under `Students` (in the master branch), make a directory named `GitHubID`.
This location is where you will commit all your individual work.
Within this directory, create a file labeled `README.md` that contains the following information.

```
# Identity

* Name: Full Name
* GitHubID: GitHub ID
* NetID: TAMU NetID
```

Finally, `add` and `commit` your modifications to your local repository, then `pull` the latest revisions from the master repository and `push` your contribution to the master repository.


## GitHub Pages

Every user with a GitHub account can get a website.
[GitHub Pages](https://pages.github.com/) are hosted directly from GitHub repositories.

### Action Items

* __Read__: [GitHub Pages](https://pages.github.com/).
* __Create__: Repository `GitHubID.github.io`.
* __Edit and Push__: Changes.
