_This project has been created as part of the 42 curriculum by vcoevert_
_Or well it would've been if it remained part of the 42 curriculum_

# Pipex
This program should at a certain point behave exactly the same as this shell command:
```Shell
< file1 cmd1 | cmd2 > file2
```
The arrow file1 makes file1 the stdin of cmd1, cmd1 stdout gets piped to cmd2 stdin and cmd2 stdout gets put in file2

### Description
First compile the program using the ```make``` command<br>
Then run the program using the following structure:
```Shell
./pipex infile <program> <program> outfile
```
More programs can be given as parameters, but this program requires at least two.

### Recources
My peers!!! Including but not limited to:<br>
mgroos<br>

https://linuxvox.com/blog/almost-perfect-c-shell-piping/<br>
The man pages for fork(2), open(2), pipe(2), wait(2) and access(2)
