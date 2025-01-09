## Isolate users into an isolated shell that allows them their own seperate (and safe) bubble to interact with and perform their own data queries and management without fear of them deleting personal or pre-existing data!

## When the program is executed you can type the following commands to interact with the simulated file system:

- cd: change directory, allows for the traversal between directories
- ls: list directory contents, displays files and directories within the current directory
- pwd: print working directory, shows the full path of the current directory
- rm: remove a file
- rmdir: remove an empty directory
- touch: create a file
- tree: displays the file system hierarchy with all the created directories and files
- menu: prints the list of available commands
- quit: quit the program

The program prompt is output to stderr. So, if you want to redirect input from a file and redirect the output to a file without the prompts appearing as part of the output, you can run it like below:

```
./main < test01.in 2>/dev/null > output.txt
```
