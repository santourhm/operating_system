#ifndef SHELL_COMMANDS_H
#define SHELL_COMMANDS_H

#define MAX_ARGS 8
#define MAX_CMD_LENGTH 64

typedef struct {
    const char* name;
    const char* description;
    void (*handler)(int argc, char* argv[]);
} shell_command_t;

// Command handler declarations
void cmd_help(int argc, char* argv[]);
void cmd_ps(int argc, char* argv[]);
void cmd_echo(int argc, char* argv[]);
void cmd_sleep(int argc, char* argv[]);
void cmd_clear(int argc, char* argv[]);

#endif