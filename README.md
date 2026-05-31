# get_next_line

A C function that reads a file line by line from a file descriptor.

## Description

`get_next_line` returns a single line from a file descriptor each time it is called, including the newline character if present. It keeps track of the reading state between calls using static variables.

This project focuses on file I/O, memory management, and handling buffers efficiently in C.

## Function

```c
char *get_next_line(int fd);
```