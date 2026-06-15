*This project has been created as part of the 42 curriculum by zaalrafa, mohammad-hezan.*

# Minishell

Minishell is a lightweight, fully functional command-line interpreter (shell) built in C. It serves as a recreation of a simplified version of Bash, adhering to UNIX standards and handling system processes, pipelines, redirections, and signals.

## Description

The goal of Minishell is to implement a command-line prompt that parses, executes, and manages system commands similarly to Bash.

### Key Features
* **Interactive Prompt**: Display a prompt when waiting for a new command, supporting command history via the `readline` library.
* **Executables and System Paths**: Locate and execute commands using the `PATH` environment variable or direct paths (absolute/relative).
* **Builtins**: Fully custom implementations of standard shell builtins:
  * `echo` (with option `-n`)
  * `cd` (with absolute/relative paths and `-`)
  * `pwd` (with no options)
  * `export` (with or without arguments)
  * `unset` (with or without arguments)
  * `env` (with no options or arguments)
  * `exit` (with exit status support)
* **Redirections**:
  * `<` (Redirect input)
  * `>` (Redirect output, truncating)
  * `<<` (Here-document with variable expansion support, interrupted gracefully by SIGINT)
  * `>>` (Redirect output, appending)
* **Pipelines**: Connect multiple commands using `|`, where the output of each command is piped as input to the next command.
* **Environment Variables**: Dynamically expand variables (e.g. `$VAR`, `$$` for shell PID, `$?` for exit status).
* **Signal Handling**: Replicates Bash behavior for `Ctrl+C` (interrupts prompt or running commands), `Ctrl+\` (ignores or quits running commands), and `Ctrl+D` (exits the shell).

---

## Instructions

### Compilation
The project is compiled using the provided `Makefile`. To build the executable, run:
```bash
make
```

To rebuild the project from scratch, use:
```bash
make re
```

To clean intermediate object files:
```bash
make clean
```

To clean all build artifacts and the binary:
```bash
make fclean
```

### Execution
Run the shell in interactive mode:
```bash
./minishell
```

### Requirements & Testing
The shell relies on the GNU `readline` library. Ensure it is installed on your system.
For memory and file descriptor leak validation, the project can be run under Valgrind:
```bash
valgrind --leak-check=full --show-leak-kinds=all --track-fds=yes ./minishell
```

---

## Resources

### Documentation & Reference Materials
1. [42 Cursus Guide - Minishell](https://42-cursus.gitbook.io/guide/3-rank-03/minishell/functions) - Functions and reference checklist.
2. [Minishell: Building a mini-Bash](https://m4nnb3ll.medium.com/minishell-building-a-mini-bash-a-42-project-b55a10598218) - Structural guide to creating a shell in C.
3. [Minishell Architecture Diagram](https://whimsical.com/minishell-architecture-big-picture-7b9N8PL3qHrddbs977mQ2J) - Architectural overview.
4. [Writing Your Own Shell](https://www.cs.purdue.edu/homes/grr/SystemsProgrammingBook/Book/Chapter5-WritingYourOwnShell.pdf) - Academic guide on shell design, parsing, and execution.
5. [ExplainShell](https://explainshell.com/) - Visual tool to match commands and arguments.

### AI Usage Disclosure
During the development of this project, an AI Coding Assistant was utilized for minor debugging, correcting Norminette errors, and helping fix some issues with the heredoc and exit status.
