#include <string.h>
#include "shell_commands.h"
#include "console.h"
#include "uart.h"
#include <cpu.h>
#define SHELL_PROMPT "$ "














static char cmdline[MAX_CMD_LENGTH];
static int cmd_pos = 0;

void shell_init(void) {
    uart_puts("Mini Shell v1.0");
    uart_puts(SHELL_PROMPT);
}

static void process_command(void) {
    char* argv[MAX_ARGS];
    int argc;

    // Remove newline
    if (cmd_pos > 0 && cmdline[cmd_pos-1] == '\n') {
        cmdline[cmd_pos-1] = '\0';
    }

    // Parse command line
    argc = parse_command(cmdline, argv, MAX_ARGS);
    
    if (argc > 0) {
        shell_command_t* cmd = get_command(argv[0]);
        if (cmd) {
            cmd->handler(argc, argv);
        } else {
            printf("Unknown command: %s\n", argv[0]);
        }
    }

    // Reset command buffer
    cmd_pos = 0;
    uart_puts(SHELL_PROMPT);
}

void shell_run(void) {
    char c;

    while (1) {
        if (uart_char_available())
        {
           c = uart_getchar();

        if (c == '\r' || c == '\n') {
            uart_putchar('\n');
            cmdline[cmd_pos] = '\0';
            process_command();
        }
        else if (c == '\b' || c == 127) {
            if (cmd_pos > 0) {
                cmd_pos--;
                uart_puts("\b \b");
            }
        }
        else if (cmd_pos < MAX_CMD_LENGTH - 1) {
            cmdline[cmd_pos++] = c;
            uart_putchar(c);
        }
        }
        else 
        {
        
            hlt();
        }
        
    }
}





int parse_command(char* cmdline, char* argv[], int max_args) {
    int argc = 0;
    char* token = strtok(cmdline, " \t\n");
    
    while (token != NULL && argc < max_args) {
        argv[argc++] = token;
        token = strtok(NULL, " \t\n");
    }
    
    return argc;
}

shell_command_t* get_command(const char* name) {
    /* for (const shell_command_t* cmd = commands; cmd->name != NULL; cmd++) {
        if (strcmp(cmd->name, name) == 0) {
            return (shell_command_t*)cmd;
        }
    } */
    return NULL;
}