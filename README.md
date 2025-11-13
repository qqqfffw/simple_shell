# simple_shell
Simple unix shell for Linux written in C.

- Supports a pipeline with **two commands** separated by `|`  
- Can be exited by typing `exit` or pressing `Ctrl+D` (EOF).
- Uses `fork`, `execvp`, `wait` and `pipe`.
