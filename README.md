_This project has been created as part of the 42 curriculum by vcoevert_
_Or well it would've been if it remained part of the 42 curriculum_

# Pipex
This program should at a certain point behave exactly the same as this shell command:
```Shell
< file1 cmd1 | cmd2 > file2
```
The arrow file1 makes file1 the stdin of cmd1, cmd1 stdout gets piped to cmd2 stdin and cmd2 stdout gets put in file2

fork() returns 0 to the child and the process indentifier of the child to the parent.

### Recources
https://linuxvox.com/blog/almost-perfect-c-shell-piping/
fork man page
