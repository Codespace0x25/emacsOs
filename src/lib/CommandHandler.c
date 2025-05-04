#include "Com1.h"
#include "Power.h"
#include "PC_Speaker.h"
#include "Screen.h"
#include "Static.h"
#include "RTC.h"
#include "std/string.h"
#include "std/transform.h"
#include <stddef.h>
#include <stdbool.h>
#include <stdint.h>

// --- Command System ---

typedef void (*CommandFunction)(int argc, char **argv);

typedef struct {
    const char *name;
    const char *help;
    CommandFunction func;
} Command;

#define REGISTER_COMMAND(name, help_text, func) \
    { name, help_text, func }

// --- Forward declarations ---
void cmd_help(int argc, char **argv);
void cmd_echo(int argc, char **argv);
void cmd_clear(int argc, char **argv);
void cmd_setUser(int argc, char **argv);
void cmd_time(int argc, char **argv);
void cmd_beep(int argc, char **argv);
void cmd_poweroff(int argc, char **argv);
void cmd_vim(int argc, char **argv);
void cmd_music(int argc, char **argv);
// --- Command list ---
static Command commands[] = {
    REGISTER_COMMAND("help", "Show available commands", cmd_help),
    REGISTER_COMMAND("echo", "Echo the input arguments", cmd_echo),
    REGISTER_COMMAND("clear", "Clears the screen", cmd_clear),
    REGISTER_COMMAND("setuser", "Set the username (arg1 = name)", cmd_setUser),
    REGISTER_COMMAND("time", "gets the time", cmd_time),
    REGISTER_COMMAND("beep", "used to play sound", cmd_beep),
    REGISTER_COMMAND("poweroff", "suths down the pc", cmd_poweroff),
    REGISTER_COMMAND("exit", "closes the seshioin", cmd_poweroff),
    REGISTER_COMMAND("vim", "vim", cmd_vim),
    REGISTER_COMMAND("music", "plays a tuen", cmd_music),
};

static const int command_count = sizeof(commands) / sizeof(commands[0]);

// --- Tokenizer ---
static void tokenize(char *input, int *argc, char **argv, int max_args) {
    *argc = 0;
    while (*input && *argc < max_args) {
        // Skip whitespace
        while (*input == ' ') input++;
        if (!*input) break;

        argv[*argc] = input;
        (*argc)++;

        while (*input && *input != ' ') input++;
        if (*input) *input++ = '\0'; // Null-terminate
    }
}

// --- Command Handler Entry Point ---
void handle_command(const char *input_raw) {
    char input[256];
    strncpy(input, input_raw, sizeof(input) - 1);
    input[sizeof(input) - 1] = '\0';

    char *argv[16];
    int argc;

    tokenize(input, &argc, argv, 16);

    if (argc == 0) return;

    for (int i = 0; i < command_count; ++i) {
        if (strcmp(argv[0], commands[i].name) == 0) {
            commands[i].func(argc, argv);
            return;
        }
    }

    putS("Unknown command. Type 'help' for a list.\n");
}

// --- Built-in Commands ---

void cmd_help(int argc, char **argv) {
    putS("Available commands:\n");
    for (int i = 0; i < command_count; ++i) {
        putS("  ");
        putS(commands[i].name);
        putS(" - ");
        putS(commands[i].help);
        putS("\n");
    }
}

void cmd_echo(int argc, char **argv) {
    for (int i = 1; i < argc; ++i) {
        putS(argv[i]);
        if (i < argc - 1)
            putS(" ");
    }
    putS("\n");
}

void cmd_clear(int argc, char **argv){
  (void) argc;
  (void) argv;

  Screen_Clear();
}

void cmd_setUser(int argc, char **argv) {
    if (argc < 2) {
        putS("You must provide a name in arg 1\n");
        return;
    }

    SetUserName(argv[1]);
    serial_putS("name was chaned to");
    serial_putS(argv[1]);
    serial_putS("\n");
}

void cmd_time(int argc, char **argv){
  (void) argc;
  (void) argv;
  uint8_t s,m,h;
  rtc_get_time(&h,&m,&s);
  char* buf;
  int_to_string(h, buf);
  putS(buf);
  putS(":");
  int_to_string(m, buf);
  putS(buf);
  putS(":");
  int_to_string(s, buf);
  putS(buf);
  putS("\n");
}

void cmd_beep(int argc, char **argv) {
    if (argc < 3) {
        putS("You must previde a frec and dir\n");
        return;
    }
    pc_beep(string_to_int(argv[1]), string_to_int(argv[1]));
    putS("Frequency: ");
    putS(argv[1]);
    putS("\n");
    putS("duration ms: ");
    putS(argv[2]);
    putS("\n");
}

void cmd_poweroff(int argc, char **argv){
  poweroff();
}

void cmd_vim(int argc, char **argv){
  putS("you silly goose, you are running vim on the emacs\n");
}

