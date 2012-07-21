TECS
====

Build instructions:
Type 'scons' to build
Type 'scons -c' to remove all object files (clean)


Test directory:
Contains unit tests for each source file

Misc. Git commands:
'git status' - shows which changes have been committed locally
'git log --graph' - shows list of commits 
'git branch' - shows current branch (e.g. master)
'git pull https://github.com/Colin-S/TECS.git' - update your local repository from the remote 		repository
'git push https://github.com/Colin-S/TECS.git' - updates remote repo from local
'git checkout <file>' - checkout single file from remote repository
'git checkout <branch>' - Switch branches
'git add . -A' - Prepares (stages) new and deleted files for a commit command 
'git rm --cached <file>' - stop tracking <file> in git
'git diff parser.c' - Show how parser.c differs from the commited local version (useful after using git pull)

Cscope commands (C file browser):
'cscope -RbqU' - Generate Cscope files at top level
