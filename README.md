# simple_shell
Simple Unix-like shell for Linux written in C.

- Supports a pipeline with **two commands** separated by `|`  
  (e.g. `ls | wc -l`).
- Can be exited by typing `exit` or pressing `Ctrl+D` (EOF).
- Uses `fork`, `execvp`, `wait` and `pipe` instead of `system(3)`.
