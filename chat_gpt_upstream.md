
RO access to GNU/git Gama from GitHub repo

    https://chatgpt.com/c/688f3c5d-f824-8332-9e5e-173db278fa8a

A) I have a git repository for one of my project on git://git.sv.gnu.org/gama.git

B) Also I have a testing repository (not to burden A repo with experiment, blunders etc)
 elsewere, mamely on github, with several branches etc.

Now my question is, how can I give the B repository readonly access to the production repository A. 
In other words I do not know how to suply data from A as a remote branch at B (if it is possible).

Yes, it's absolutely possible to add a read-only remote (like your GNU Savannah repository) 
to your GitHub testing repository. This is a very common setup for maintaining 
an upstream source (A) while working in a separate repo (B), e.g., for experiments, 
integration, or personal changes.

Here’s a step-by-step guide to add A (production repo) as a read-only remote to B (testing repo):

### Step 1: Go to your B repository

Open a terminal and cd into your testing repository (B) cloned from GitHub:

    cd path/to/your/github/testing/repo

### Step 2: Add A as a remote

Add the GNU Savannah repository as a remote, usually called upstream:

    git remote add upstream git://git.sv.gnu.org/gama.git

Now, origin refers to GitHub (your testing repo), and upstream refers to the GNU production repo (read-only).

You can verify:

    git remote -v

Output will be something like:

    origin    git@github.com:yourname/gama.git (fetch)
    origin    git@github.com:yourname/gama.git (push)
    upstream  git://git.sv.gnu.org/gama.git (fetch)
    upstream  git://git.sv.gnu.org/gama.git (push)

Note: Since git:// is read-only, the push line won't actually work — but Git won’t stop 
you from configuring it.

### Step 3: Fetch from A (upstream)

    git fetch upstream

This fetches all branches, tags, etc., from A (Savannah repo). You can now inspect 
upstream branches with:

    git branch -r

For example, you might see something like:

    origin/main
    upstream/master

#### Step 4: Work with the upstream branch

To create a local branch that tracks the upstream one:

    git checkout -b upstream-master upstream/master

Or merge/rebase specific changes:

    git checkout main
    git merge upstream/master

Or cherry-pick:

    git cherry-pick upstream/master~2  # For example

### Read-only Access Is Enforced by Protocol

Since git:// is inherently read-only, you don’t have to worry about accidentally pushing changes to A. Even if you try:

    git push upstream master

It will fail with a permissions error.

### Summary

Your B repository (GitHub) will now:

* Track your work (new branches, experiments, etc.)

* Be able to fetch from A (Savannah) to stay up to date

* Avoid any risk of altering A (due to the read-only nature of git://)

Let me know if you'd like to automate syncing upstream into a mirror branch or tag versions for comparison.
