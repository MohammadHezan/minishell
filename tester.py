#!/usr/bin/env python3
import os
import sys
import subprocess
import time
import pty
import select
import signal
import termios
import shutil
import re
import argparse

# Colors
GREEN = "\033[92m"
RED = "\033[91m"
YELLOW = "\033[93m"
BLUE = "\033[94m"
MAGENTA = "\033[95m"
CYAN = "\033[96m"
BOLD = "\033[1m"
RESET = "\033[0m"

# Global Config
MINISHELL_PATH = "./minishell"
VALGRIND_PATH = "valgrind"

def clean_files(*files):
    for f in files:
        if os.path.exists(f):
            if os.path.isdir(f):
                shutil.rmtree(f)
            else:
                os.remove(f)

# Custom Setup/Teardown Helpers
def setup_dummy_a_out():
    with open("a.c", "w") as f:
        f.write('#include <stdio.h>\nint main(void) { printf("a.out executed\\n"); return 42; }\n')
    subprocess.run(["cc", "a.c", "-o", "a.out"], stdout=subprocess.DEVNULL, stderr=subprocess.DEVNULL)
    clean_files("a.c")

def teardown_dummy_a_out():
    clean_files("a.out")

def setup_redirection_files():
    with open("in.txt", "w") as f:
        f.write("hello from in.txt\n")
    with open("in2.txt", "w") as f:
        f.write("hello from in2.txt\n")
    with open("in3.txt", "w") as f:
        f.write("hello from in3.txt\n")
    with open("file", "w") as f:
        f.write("line 1\nline foo 2\nline 3\nline foo 4\n")

def teardown_redirection_files():
    clean_files("in.txt", "in2.txt", "in3.txt", "out.txt", "out2.txt", "out3.txt", "file", "o1", "o2", "old2")

def setup_file_test():
    with open("file_test", "w") as f:
        f.write("#!/bin/sh\necho 'file_test executed'\n")
    os.chmod("file_test", 0o755)

def teardown_file_test():
    clean_files("file_test")

# List of Tests
TESTS = [
    # ================= EVAL SHEET =================
    {
        "category": "eval_sheet",
        "name": "basic_ls",
        "commands": ["/bin/ls"],
        "expected_code": 0,
        "stdout_contains": "Makefile",
    },
    {
        "category": "eval_sheet",
        "name": "ls_nonexistent",
        "commands": ["/bin/ls filethatdoesntexist"],
        "expected_code": 2,
        "stderr_contains": "No such file or directory",
    },
    {
        "category": "eval_sheet",
        "name": "empty_command",
        "commands": ["", ""],
        "expected_code": 0,
    },
    {
        "category": "eval_sheet",
        "name": "spaces_tabs",
        "commands": ["   \t   "],
        "expected_code": 0,
    },
    {
        "category": "eval_sheet",
        "name": "echo_hello",
        "commands": ["echo hello world"],
        "expected_code": 0,
        "expected_stdout": "hello world\n",
    },
    {
        "category": "eval_sheet",
        "name": "echo_no_newline",
        "commands": ["echo -n hello"],
        "expected_code": 0,
        "expected_stdout": "hello",
    },
    {
        "category": "eval_sheet",
        "name": "exit_status_0",
        "commands": ["exit"],
        "expected_code": 0,
    },
    {
        "category": "eval_sheet",
        "name": "exit_status_42",
        "commands": ["exit 42"],
        "expected_code": 42,
    },
    {
        "category": "eval_sheet",
        "name": "expr_arithmetic",
        "commands": ["expr $? + $?"],
        "expected_code": 1,
        "expected_stdout": "0\n",
    },
    {
        "category": "eval_sheet",
        "name": "echo_single_quote_var",
        "commands": ["echo '$USER'"],
        "expected_code": 0,
        "expected_stdout": "$USER\n",
    },
    {
        "category": "eval_sheet",
        "name": "env_test",
        "commands": ["env"],
        "expected_code": 0,
        "stdout_contains": "PATH=",
    },
    {
        "category": "eval_sheet",
        "name": "pwd_test",
        "commands": ["pwd"],
        "expected_code": 0,
        "stdout_contains": "minishell",
    },
    {
        "category": "eval_sheet",
        "name": "relative_path",
        "setup": setup_dummy_a_out,
        "teardown": teardown_dummy_a_out,
        "commands": ["./a.out"],
        "expected_code": 42,
        "expected_stdout": "a.out executed\n",
    },
    {
        "category": "eval_sheet",
        "name": "ls_with_path",
        "commands": ["ls"],
        "expected_code": 0,
        "stdout_contains": "Makefile",
    },
    {
        "category": "eval_sheet",
        "name": "unset_path_ls",
        "commands": ["unset PATH", "ls"],
        "expected_code": 127,
        "stderr_contains": ["command not found", "No such file or directory"],
    },
    {
        "category": "eval_sheet",
        "name": "restore_path",
        "commands": ["PATH=/bin:/usr/bin", "ls"],
        "expected_code": 0,
        "stdout_contains": "Makefile",
    },
    {
        "category": "eval_sheet",
        "name": "redirection_in_out",
        "setup": setup_redirection_files,
        "teardown": teardown_redirection_files,
        "commands": ["cat < in.txt > out.txt"],
        "expected_code": 0,
        "custom_verify": lambda: os.path.exists("out.txt") and open("out.txt").read() == "hello from in.txt\n",
    },
    {
        "category": "eval_sheet",
        "name": "redirection_append",
        "setup": setup_redirection_files,
        "teardown": teardown_redirection_files,
        "commands": ["echo hello >> out.txt", "echo hello >> out.txt"],
        "expected_code": 0,
        "custom_verify": lambda: os.path.exists("out.txt") and open("out.txt").read() == "hello\nhello\n",
    },
    {
        "category": "eval_sheet",
        "name": "heredoc_basic",
        "commands": ["cat << EOF", "hello", "world", "EOF"],
        "expected_code": 0,
        "expected_stdout": "hello\nworld\n",
    },
    {
        "category": "eval_sheet",
        "name": "pipe_cat_grep_wc",
        "setup": setup_redirection_files,
        "teardown": teardown_redirection_files,
        "commands": ["cat file | grep foo | wc -l"],
        "expected_code": 0,
        "expected_stdout": "2\n",
    },
    {
        "category": "eval_sheet",
        "name": "pipe_error_first",
        "commands": ["ls filethatdoesntexist | grep foo"],
        "stderr_contains": "No such file or directory",
        "expected_stdout": "",
    },
    {
        "category": "eval_sheet",
        "name": "pipe_to_redirection",
        "setup": setup_redirection_files,
        "teardown": teardown_redirection_files,
        "commands": ["cat file | grep foo > out.txt"],
        "expected_code": 0,
        "custom_verify": lambda: os.path.exists("out.txt") and open("out.txt").read() == "line foo 2\nline foo 4\n",
    },
    {
        "category": "eval_sheet",
        "name": "random_command_not_found",
        "commands": ["abcd123"],
        "expected_code": 127,
        "stderr_contains": "command not found",
    },
    {
        "category": "eval_sheet",
        "name": "very_long_command",
        "commands": ["echo " + ("a" * 5000)],
        "expected_code": 0,
        "custom_verify": lambda: True,
    },
    {
        "category": "eval_sheet",
        "name": "echo_env_user",
        "commands": ["echo $USER"],
        "expected_code": 0,
        "expected_stdout": os.environ.get("USER", "") + "\n",
    },
    {
        "category": "eval_sheet",
        "name": "echo_quoted_env_user",
        "commands": ["echo \"$USER\""],
        "expected_code": 0,
        "expected_stdout": os.environ.get("USER", "") + "\n",
    },

    # ================= CUSTOM TESTS =================
    {
        "category": "custom_tests",
        "name": "double_heredoc_pipe",
        "setup": setup_redirection_files,
        "teardown": teardown_redirection_files,
        "commands": ["cat << ls > out | cat << ls", "1", "ls", "2", "ls"],
        "expected_code": 0,
        "expected_stdout": "2\n",
        "custom_verify": lambda: os.path.exists("out") and open("out").read() == "1\n",
    },
    {
        "category": "custom_tests",
        "name": "long_mixed_pipeline",
        "setup": setup_redirection_files,
        "teardown": teardown_redirection_files,
        "commands": [
            "ls <<1 | cat -e | cat -e | cat -e > o1 | cat <<1 | wc -l > o2 | cat <o2 >old2| pwd | cat -e >o2",
            "word1",
            "1",
            "word2",
            "1"
        ],
        "expected_code": 0,
        "custom_verify": lambda: os.path.exists("o1") and os.path.exists("o2") and open("o1").read().endswith("$\n"),
    },
    {
        "category": "custom_tests",
        "name": "syntax_error_nested_redirection",
        "commands": ["cat << 1 >"],
        "expected_code": 2,
        "stderr_contains": "syntax error",
    },

    # ================= SYNTAX =================
    {
        "category": "syntax",
        "name": "syntax_missing_output_file",
        "commands": ["ls >"],
        "expected_code": 2,
        "stderr_contains": "syntax error",
    },
    {
        "category": "syntax",
        "name": "syntax_triple_greater",
        "commands": [">>>"],
        "expected_code": 2,
        "stderr_contains": "syntax error",
    },
    {
        "category": "syntax",
        "name": "syntax_triple_lesser",
        "commands": ["<<<"],
        "expected_code": 2,
        "stderr_contains": "syntax error",
    },
    {
        "category": "syntax",
        "name": "syntax_pipe_newline",
        "commands": ["ls |"],
        "expected_code": 2,
        "stderr_contains": "syntax error",
    },
    {
        "category": "syntax",
        "name": "syntax_double_pipe",
        "commands": ["ls ||"],
        "expected_code": 2,
        "stderr_contains": "syntax error",
    },
    {
        "category": "syntax",
        "name": "syntax_heredoc_newline",
        "commands": ["<<"],
        "expected_code": 2,
        "stderr_contains": "syntax error",
    },
    {
        "category": "syntax",
        "name": "syntax_append_newline",
        "commands": [">>"],
        "expected_code": 2,
        "stderr_contains": "syntax error",
    },

    # ================= EXPANDER =================
    {
        "category": "expander",
        "name": "non_exist_and_shell",
        "env": {"SHELL": "minishell"},
        "commands": ["echo $non_exist $SHELL"],
        "expected_code": 0,
        "stdout_contains": "minishell",
    },
    {
        "category": "expander",
        "name": "multiple_spaces_expansion",
        "commands": ["export test=\"            \"", "echo $test $test $test abcd $non_exist $test 123 $test"],
        "expected_code": 0,
        "expected_stdout": "abcd 123\n",
    },
    {
        "category": "expander",
        "name": "quoted_vs_unquoted_expansion",
        "commands": ["export test=\"      abcd     123 \"", "echo $test \"$test\""],
        "expected_code": 0,
        "expected_stdout": "abcd 123       abcd     123 \n",
    },
    {
        "category": "expander",
        "name": "special_vars_positional",
        "commands": ["echo $0$1230$0"],
        "expected_code": 0,
        "stdout_contains": "230",
    },
    {
        "category": "expander",
        "name": "command_expansion_execution",
        "commands": ["export test=\"ls -la\"", "$test"],
        "expected_code": 0,
        "stdout_contains": "Makefile",
    },

    # ================= ENV =================
    {
        "category": "env",
        "name": "shlvl_increment",
        "commands": ["echo $SHLVL"],
        "expected_code": 0,
    },
    {
        "category": "env",
        "name": "underscore_last_arg",
        "commands": ["ls -la", "echo $_"],
        "expected_code": 0,
        "stdout_contains": "-la",
    },

    # ================= EXPORT =================
    {
        "category": "export",
        "name": "export_multiple",
        "commands": ["export test1=\"abcd\" test2=\"123\" test3=\"minishell\"", "env"],
        "expected_code": 0,
        "stdout_contains": "test1=abcd",
    },
    {
        "category": "export",
        "name": "export_invalid_identifier",
        "commands": ["export 1test"],
        "expected_code": 1,
        "stderr_contains": "not a valid identifier",
    },
    {
        "category": "export",
        "name": "export_mix_valid_invalid",
        "commands": ["export test=\"123\" 1test=\"test\" test3=\"abcd\"", "env"],
        "expected_code": 1,
        "stdout_contains": "test=123",
    },
    {
        "category": "export",
        "name": "export_empty_val",
        "commands": ["export test_empty", "env"],
        "expected_code": 0,
        "custom_verify": lambda: "test_empty=" not in subprocess.run(
            ["./minishell"], input="export test_empty\nenv\n", text=True, capture_output=True
        ).stdout,
    },

    # ================= PIPES =================
    {
        "category": "pipes",
        "name": "pipe_multiple_cats",
        "commands": ["echo hello | cat | cat | cat"],
        "expected_code": 0,
        "expected_stdout": "hello\n",
    },
    {
        "category": "pipes",
        "name": "pipe_cycles_codec",
        "commands": ["cat Makefile | base64 | xxd -p | xxd -r -p | base64 --decode"],
        "expected_code": 0,
        "stdout_contains": "minishell",
    },
    {
        "category": "pipes",
        "name": "pipe_deep_cat_e",
        "commands": ["echo hello | cat -e | cat -e | cat -e | cat -e | cat -e"],
        "expected_code": 0,
        "expected_stdout": "hello$$$$$\n",
    },
    {
        "category": "pipes",
        "name": "pipe_sigpipe_yes",
        "commands": ["yes yes | head -n 100 | wc"],
        "expected_code": 0,
        "stdout_contains": "100",
    },
    {
        "category": "pipes",
        "name": "pipe_many_echos",
        "commands": ["echo | echo | echo | echo | echo | echo | echo | echo | echo | echo | echo | echo"],
        "expected_code": 0,
        "expected_stdout": "\n",
    },
    {
        "category": "pipes",
        "name": "pipe_exit_status",
        "commands": ["exit 1 | exit 255 | exit 2"],
        "expected_code": 2,
    },

    # ================= ECHO =================
    {
        "category": "echo",
        "name": "echo_n_flag",
        "commands": ["echo -n test"],
        "expected_code": 0,
        "expected_stdout": "test",
    },
    {
        "category": "echo",
        "name": "echo_n_multiple",
        "commands": ["echo -nnnnnnn -n -nnnnnnb test"],
        "expected_code": 0,
        "expected_stdout": "-nnnnnnb test",
    },
    {
        "category": "echo",
        "name": "echo_empty_quotes",
        "commands": ["echo \"\"\"\""],
        "expected_code": 0,
        "expected_stdout": "\n",
    },
    {
        "category": "echo",
        "name": "echo_mix_quotes",
        "commands": ["echo A\"\"''"],
        "expected_code": 0,
        "expected_stdout": "A\n",
    },

    # ================= UNSET =================
    {
        "category": "unset",
        "name": "unset_user",
        "commands": ["unset USER", "env"],
        "expected_code": 0,
        "custom_verify": lambda: "USER=" not in subprocess.run(
            ["./minishell"], input="unset USER\nenv\n", text=True, capture_output=True
        ).stdout,
    },
    {
        "category": "unset",
        "name": "unset_multiple",
        "commands": ["unset USER PATH", "env"],
        "expected_code": 0,
        "custom_verify": lambda: "PATH=" not in subprocess.run(
            ["./minishell"], input="unset USER PATH\nenv\n", text=True, capture_output=True
        ).stdout,
    },

    # ================= CD =================
    {
        "category": "cd",
        "name": "cd_nonexistent",
        "commands": ["cd test/"],
        "expected_code": 1,
        "stderr_contains": "No such file or directory",
    },
    {
        "category": "cd",
        "name": "cd_permission_denied",
        "commands": ["cd /root"],
        "expected_code": 1,
        "stderr_contains": "Permission denied",
    },
    {
        "category": "cd",
        "name": "cd_parent_parent",
        "commands": ["cd ../../", "pwd"],
        "expected_code": 0,
        "stdout_contains": "Work",
    },
    {
        "category": "cd",
        "name": "cd_too_many_args",
        "commands": ["cd a b"],
        "expected_code": 1,
        "stderr_contains": "too many arguments",
    },

    # ================= EXIT =================
    {
        "category": "exit",
        "name": "exit_negative_one",
        "commands": ["exit -1"],
        "expected_code": 255,
    },
    {
        "category": "exit",
        "name": "exit_too_large_neg_overflow",
        "commands": ["exit 9223372036854775808-"],
        "expected_code": 2,
        "stderr_contains": "numeric argument required",
    },
    {
        "category": "exit",
        "name": "exit_min_long_long",
        "commands": ["exit -9223372036854775808"],
        "expected_code": 0,
    },
    {
        "category": "exit",
        "name": "exit_max_long_long_overflow",
        "commands": ["exit 9223372036854775808"],
        "expected_code": 2,
        "stderr_contains": "numeric argument required",
    },
    {
        "category": "exit",
        "name": "exit_max_long_long",
        "commands": ["exit 9223372036854775807"],
        "expected_code": 255,
    },
    {
        "category": "exit",
        "name": "exit_too_many_args",
        "commands": ["exit 1 abc"],
        "expected_code": 1,
        "stderr_contains": "too many arguments",
    },
    {
        "category": "exit",
        "name": "exit_non_numeric",
        "commands": ["exit abc"],
        "expected_code": 2,
        "stderr_contains": "numeric argument required",
    },

    # ================= ERROR HANDLING =================
    {
        "category": "error_handling",
        "name": "dot_slash",
        "commands": ["./"],
        "expected_code": 126,
        "stderr_contains": "directory",
    },
    {
        "category": "error_handling",
        "name": "slash_only",
        "commands": ["/"],
        "expected_code": 126,
        "stderr_contains": "directory",
    },
    {
        "category": "error_handling",
        "name": "file_slash",
        "commands": ["Makefile/"],
        "expected_code": 126,
        "stderr_contains": "Not a directory",
    },
    {
        "category": "error_handling",
        "name": "fd_leak_check",
        "setup": setup_file_test,
        "teardown": teardown_file_test,
        "commands": ["./file_test"],
        "expected_code": 0,
        "expected_stdout": "file_test executed\n",
    },

    # ================= REDIRECTIONS =================
    {
        "category": "redirections",
        "name": "multi_in_multi_out",
        "setup": setup_redirection_files,
        "teardown": teardown_redirection_files,
        "commands": ["< in.txt < in2.txt < in3.txt cat > out.txt > out2.txt > out3.txt"],
        "expected_code": 0,
        "custom_verify": lambda: os.path.exists("out3.txt") and open("out3.txt").read() == "hello from in3.txt\n",
    },
    {
        "category": "redirections",
        "name": "heredoc_with_flags",
        "setup": setup_redirection_files,
        "teardown": teardown_redirection_files,
        "commands": ["<< 123 cat << 234 -e > out.txt", "line1", "123", "line2", "234"],
        "expected_code": 0,
        "custom_verify": lambda: os.path.exists("out.txt") and open("out.txt").read().startswith("line2"),
    },
    {
        "category": "redirections",
        "name": "redirect_to_directory",
        "commands": ["> /"],
        "expected_code": 1,
        "stderr_contains": "directory",
    },
    {
        "category": "redirections",
        "name": "heredoc_eof_warning",
        "commands": ["cat << 123", "1234"],
        "expected_code": 0,
        "stderr_contains": "warning: here-document",
    },
]

# Interactive / PTY-specific tests (Signals)
INTERACTIVE_TESTS = [
    {
        "category": "signals",
        "name": "sigint_empty_prompt",
        "steps": [
            {"send_signal": signal.SIGINT},
            {"send_line": "echo $?"},
        ],
        "expected_code": 0,
        "stdout_contains": "130",
    },
    {
        "category": "signals",
        "name": "sigquit_empty_prompt",
        "steps": [
            {"send_signal": signal.SIGQUIT},
            {"send_line": "echo $?"},
        ],
        "expected_code": 0,
        "stdout_contains": "0",
    },
    {
        "category": "signals",
        "name": "eof_empty_prompt",
        "steps": [
            {"send_eof": True},
        ],
        "expected_code": 0,
    },
    {
        "category": "signals",
        "name": "sigint_running_command",
        "steps": [
            {"send_line": "sleep 100"},
            {"delay": 0.2},
            {"send_signal": signal.SIGINT},
            {"send_line": "echo $?"},
        ],
        "expected_code": 0,
        "stdout_contains": "130",
    },
    {
        "category": "signals",
        "name": "sigquit_running_command",
        "steps": [
            {"send_line": "sleep 100"},
            {"delay": 0.2},
            {"send_signal": signal.SIGQUIT},
            {"send_line": "echo $?"},
        ],
        "expected_code": 0,
        "stdout_contains": "131",
    },
    {
        "category": "signals",
        "name": "zombie_process_check",
        "steps": [
            {"send_line": "sleep 1 | ls | sleep 2"},
            {"delay": 1.2},
            {"check_zombies": True},
            {"delay": 1.5},
        ],
        "expected_code": 0,
    },
    {
        "category": "redirections",
        "name": "heredoc_sigint",
        "steps": [
            {"send_line": "cat << EOF"},
            {"delay": 0.1},
            {"send_signal": signal.SIGINT},
            {"send_line": "echo $?"},
        ],
        "expected_code": 0,
        "stdout_contains": "130",
    }
]

# Helper to remove shell prompts, echoes, and exit messages
def clean_shell_artifacts(text, commands):
    if not text:
        return text
    
    sent_set = {c.strip() for c in commands}
    
    lines = []
    for line in text.splitlines():
        has_prompt = False
        cleaned_line = line
        
        # Strip leading prompt patterns
        for prompt in ["minishell$ ", "minishell$", "minishell> ", "> "]:
            if cleaned_line.startswith(prompt):
                cleaned_line = cleaned_line[len(prompt):]
                has_prompt = True
                break
            
        # If the line had a prompt and matches a command or exit, it is a shell artifact/echo, so ignore it
        if has_prompt and (cleaned_line.strip() in sent_set or cleaned_line.strip() == "exit"):
            continue
            
        # Strip trailing prompt remnants (e.g. from no-newline echoes)
        cleaned_line = re.sub(r'minishell\$(\s*exit)?', '', cleaned_line)
        cleaned_line = re.sub(r'minishell\>(\s*exit)?', '', cleaned_line)
        cleaned_line = re.sub(r'\bexit\b', '', cleaned_line)
        
        if cleaned_line.strip() in ("", ">", "minishell$", "minishell>"):
            continue
            
        lines.append(cleaned_line)
        
    return "\n".join(lines)

# Helper to find any zombie processes owned by minishell PID
def get_zombies(ppid):
    zombies = []
    if not os.path.exists("/proc"):
        return zombies
    for pid_str in os.listdir("/proc"):
        if pid_str.isdigit():
            pid = int(pid_str)
            try:
                with open(f"/proc/{pid}/stat", "r") as f:
                    stat = f.read().split()
                    state = stat[2]
                    parent_pid = int(stat[3])
                    if parent_pid == ppid and state == 'Z':
                        zombies.append(pid)
            except Exception:
                continue
    return zombies

# Run standard test in non-interactive mode
def run_non_interactive(test, valgrind=False):
    if "setup" in test:
        test["setup"]()

    cmd_str = "\n".join(test["commands"]) + "\n"
    
    exec_args = []
    if valgrind:
        valgrind_log = f"valgrind_{test['name']}.log"
        exec_args = [VALGRIND_PATH, "--leak-check=full", "--show-leak-kinds=all", f"--log-file={valgrind_log}", "--track-fds=yes", MINISHELL_PATH]
    else:
        exec_args = [MINISHELL_PATH]

    env = os.environ.copy()
    if "env" in test:
        env.update(test["env"])
        
    proc = subprocess.Popen(
        exec_args,
        stdin=subprocess.PIPE,
        stdout=subprocess.PIPE,
        stderr=subprocess.PIPE,
        env=env,
        text=True
    )
    
    try:
        stdout, stderr = proc.communicate(input=cmd_str, timeout=5.0)
    except subprocess.TimeoutExpired:
        proc.kill()
        stdout, stderr = proc.communicate()
        stderr += "\n[TESTER] Timeout Expired (5s)"
    
    code = proc.returncode

    # Valgrind report parsing
    valgrind_ok = True
    valgrind_msg = ""
    if valgrind:
        if os.path.exists(valgrind_log):
            log_content = open(valgrind_log).read()
            os.remove(valgrind_log)
            def_lost = re.search(r"definitely lost:\s*([0-9,]+)\s*bytes", log_content)
            ind_lost = re.search(r"indirectly lost:\s*([0-9,]+)\s*bytes", log_content)
            fds_open = re.search(r"FILE DESCRIPTORS:\s*(\d+)\s*open at exit", log_content)
            errors = re.search(r"ERROR SUMMARY:\s*(\d+)\s*errors", log_content)
            
            leaked_bytes = 0
            if def_lost:
                leaked_bytes += int(def_lost.group(1).replace(",", ""))
            if ind_lost:
                leaked_bytes += int(ind_lost.group(1).replace(",", ""))
                
            open_fds = 3
            if fds_open:
                open_fds = int(fds_open.group(1))

            err_count = 0
            if errors:
                err_count = int(errors.group(1))
                
            if leaked_bytes > 0 or open_fds > 3 or err_count > 0:
                valgrind_ok = False
                valgrind_msg = f"Leaks: {leaked_bytes}B, FDs open: {open_fds}, Valgrind errors: {err_count}"

    return code, stdout, stderr, valgrind_ok, valgrind_msg

# Run interactive test in PTY mode
def run_interactive_pty(test, valgrind=False):
    if "setup" in test:
        test["setup"]()

    master_fd, slave_fd = pty.openpty()
    
    exec_args = []
    if valgrind:
        valgrind_log = f"valgrind_pty_{test['name']}.log"
        exec_args = [VALGRIND_PATH, "--leak-check=full", "--show-leak-kinds=all", f"--log-file={valgrind_log}", "--track-fds=yes", MINISHELL_PATH]
    else:
        exec_args = [MINISHELL_PATH]

    env = os.environ.copy()
    if "env" in test:
        env.update(test["env"])
        
    proc = subprocess.Popen(
        exec_args,
        stdin=slave_fd,
        stdout=slave_fd,
        stderr=slave_fd,
        preexec_fn=os.setsid,
        env=env,
        text=True
    )
    os.close(slave_fd)
    os.set_blocking(master_fd, False)

    stdout_buf = []
    zombies_detected = []
    
    def read_all_available():
        time.sleep(0.05)
        while True:
            r, _, _ = select.select([master_fd], [], [], 0.02)
            if not r:
                break
            try:
                data = os.read(master_fd, 1024)
                if not data:
                    break
                stdout_buf.append(data.decode("utf-8", errors="ignore"))
            except OSError:
                break

    try:
        for step in test["steps"]:
            read_all_available()
            if "send_line" in step:
                os.write(master_fd, (step["send_line"] + "\n").encode())
            elif "send_signal" in step:
                os.killpg(os.getpgid(proc.pid), step["send_signal"])
            elif "send_eof" in step:
                os.write(master_fd, b"\x04")
            elif "delay" in step:
                time.sleep(step["delay"])
            elif "check_zombies" in step:
                zombies_detected = get_zombies(proc.pid)

        read_all_available()
        if proc.poll() is None:
            try:
                os.write(master_fd, b"exit\n")
                time.sleep(0.1)
            except OSError:
                pass
            if proc.poll() is None:
                proc.terminate()
                proc.wait(timeout=1.0)
    except Exception as e:
        proc.kill()
        proc.wait()
    finally:
        try:
            os.close(master_fd)
        except OSError:
            pass

    code = proc.returncode
    stdout = "".join(stdout_buf)
    stderr = ""

    valgrind_ok = True
    valgrind_msg = ""
    if valgrind:
        if os.path.exists(valgrind_log):
            log_content = open(valgrind_log).read()
            os.remove(valgrind_log)
            def_lost = re.search(r"definitely lost:\s*([0-9,]+)\s*bytes", log_content)
            ind_lost = re.search(r"indirectly lost:\s*([0-9,]+)\s*bytes", log_content)
            fds_open = re.search(r"FILE DESCRIPTORS:\s*(\d+)\s*open at exit", log_content)
            errors = re.search(r"ERROR SUMMARY:\s*(\d+)\s*errors", log_content)
            
            leaked_bytes = 0
            if def_lost:
                leaked_bytes += int(def_lost.group(1).replace(",", ""))
            if ind_lost:
                leaked_bytes += int(ind_lost.group(1).replace(",", ""))
                
            open_fds = 3
            if fds_open:
                open_fds = int(fds_open.group(1))

            err_count = 0
            if errors:
                err_count = int(errors.group(1))
                
            if leaked_bytes > 0 or open_fds > 3 or err_count > 0:
                valgrind_ok = False
                valgrind_msg = f"Leaks: {leaked_bytes}B, FDs open: {open_fds}, Valgrind errors: {err_count}"

    if "zombie_process_check" in test["name"] and len(zombies_detected) > 0:
        stderr = f"[TESTER] Zombie processes detected: {zombies_detected}"
        code = -1

    return code, stdout, stderr, valgrind_ok, valgrind_msg

# Perform validation of actual results
def validate_test(test, code, stdout, stderr):
    issues = []
    
    clean_out = clean_shell_artifacts(stdout, test.get("commands", []))
    clean_err = clean_shell_artifacts(stderr, test.get("commands", []))
    
    if "expected_code" in test:
        if code != test["expected_code"]:
            issues.append(f"Exit status: expected {test['expected_code']}, got {code}")

    if "expected_stdout" in test:
        expected = test["expected_stdout"]
        if clean_out.strip() != expected.strip():
            issues.append(f"Stdout mismatch:\nExpected: {repr(expected.strip())}\nGot (Cleaned): {repr(clean_out.strip())}")

    if "stdout_contains" in test:
        if test["stdout_contains"] not in clean_out and test["stdout_contains"] not in stdout:
            issues.append(f"Stdout missing expected pattern '{test['stdout_contains']}'")

    if "stderr_contains" in test:
        pats = test["stderr_contains"] if isinstance(test["stderr_contains"], list) else [test["stderr_contains"]]
        if not any(pat in clean_err or pat in stderr or pat in stdout for pat in pats):
            issues.append(f"Stderr missing expected pattern '{test['stderr_contains']}'")

    if "custom_verify" in test:
        if not test["custom_verify"]():
            issues.append("Custom file/redirection verification failed")

    return issues

def main():
    parser = argparse.ArgumentParser(description="Minishell automated test runner")
    parser.add_argument("--category", type=str, help="Run only tests from this category")
    parser.add_argument("--valgrind", action="store_true", help="Enable memory leak & FD checking with Valgrind")
    parser.add_argument("--list", action="store_true", help="List all available tests")
    parser.add_argument("--interactive-only", action="store_true", help="Only run interactive (PTY) tests")
    parser.add_argument("--run-one", type=str, help="Run a single test by name")
    args = parser.parse_args()

    if not os.path.exists(MINISHELL_PATH):
        print(f"{RED}{BOLD}Error:{RESET} '{MINISHELL_PATH}' binary not found. Please compile it first.")
        sys.exit(1)

    if args.valgrind and not shutil.which(VALGRIND_PATH):
        print(f"{YELLOW}Warning:{RESET} Valgrind not found on system. Disabling Valgrind checks.")
        args.valgrind = False

    all_tests = []
    for t in TESTS:
        t["interactive"] = False
        all_tests.append(t)
    for t in INTERACTIVE_TESTS:
        t["interactive"] = True
        all_tests.append(t)

    if args.list:
        print(f"{BOLD}Available tests by category:{RESET}")
        categories = {}
        for t in all_tests:
            categories.setdefault(t["category"], []).append(t["name"])
        for cat, names in categories.items():
            print(f"  {CYAN}{cat}{RESET}: {', '.join(names)}")
        return

    filtered = all_tests
    if args.category:
        filtered = [t for t in filtered if t["category"] == args.category]
    if args.interactive_only:
        filtered = [t for t in filtered if t["interactive"]]
    if args.run_one:
        filtered = [t for t in filtered if t["name"] == args.run_one]

    if not filtered:
        print(f"{RED}No tests matching filters.{RESET}")
        return

    total = len(filtered)
    passed = 0
    failed_tests = []

    print(f"\n{BOLD}{BLUE}=== Minishell Automated Test Suite ==={RESET}\n")
    print(f"Running {total} tests (Valgrind: {'ENABLED' if args.valgrind else 'DISABLED'})\n")

    for idx, test in enumerate(filtered, 1):
        print(f"[{idx}/{total}] Category: {CYAN}{test['category']}{RESET} | Test: {BOLD}{test['name']}{RESET} ... ", end="", flush=True)

        try:
            if test["interactive"]:
                code, stdout, stderr, val_ok, val_msg = run_interactive_pty(test, valgrind=args.valgrind)
            else:
                code, stdout, stderr, val_ok, val_msg = run_non_interactive(test, valgrind=args.valgrind)

            issues = validate_test(test, code, stdout, stderr)
            if not val_ok:
                issues.append(f"Valgrind issue: {val_msg}")

            if not issues:
                print(f"{GREEN}[ PASS ]{RESET}")
                passed += 1
            else:
                print(f"{RED}[ FAIL ]{RESET}")
                for issue in issues:
                    print(f"   -> {YELLOW}{issue}{RESET}")
                failed_tests.append((test["name"], issues))

        except Exception as ex:
            print(f"{RED}[ ERROR ]{RESET}")
            print(f"   -> Exception: {ex}")
            failed_tests.append((test["name"], [str(ex)]))
        finally:
            if "teardown" in test:
                try:
                    test["teardown"]()
                except Exception:
                    pass

    success_rate = (passed / total) * 100 if total > 0 else 0
    print(f"\n{BOLD}{BLUE}=== Summary of Results ==={RESET}")
    print(f"Total Tests:   {total}")
    print(f"Passed:        {GREEN}{passed}{RESET}")
    print(f"Failed:        {RED}{len(failed_tests)}{RESET}")
    print(f"Success Rate:  {BOLD}{success_rate:.1f}%{RESET}\n")

    if failed_tests:
        print(f"{BOLD}{RED}Failed Tests Detail:{RESET}")
        for name, issues in failed_tests:
            print(f"  * {BOLD}{name}{RESET}:")
            for issue in issues:
                print(f"    - {issue}")
        sys.exit(1)
    else:
        print(f"{GREEN}{BOLD}All checks passed successfully!{RESET}")
        sys.exit(0)

if __name__ == "__main__":
    main()
