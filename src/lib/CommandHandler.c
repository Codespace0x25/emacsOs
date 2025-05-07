#include "Com1.h"
#include "PC_Speaker.h"
#include "Power.h"
#include "RTC.h"
#include "Screen.h"
#include "Static.h"
#include "std/string.h"
#include "std/transform.h"
#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>

// --- Command System ---

typedef void (*CommandFunction)(int argc, char **argv);

typedef struct {
  const char *name;
  const char *help;
  CommandFunction func;
  const char* manPage;
} Command;

#define REGISTER_COMMAND(name, help_text, func,ManPage) {name, help_text, func,ManPage}

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
void cmd_sleep(int argc, char **argv);
void cmd_setColor(int argc, char **argv);
void cmd_man(int argc, char **argv) ;
// --- Command list ---
static Command commands[] = {
  REGISTER_COMMAND("help", "Show available commands", cmd_help,"this is used to help see the commadns"),
  REGISTER_COMMAND("echo", "Echo the input arguments", cmd_echo,"this will diaplsy any text put in after it"),
  REGISTER_COMMAND("clear", "Clears the screen", cmd_clear,"used to clear the screen "),
  REGISTER_COMMAND("time", "gets the time", cmd_time,"gets local time in 12 haur"),
  REGISTER_COMMAND("beep", "used to play sound", cmd_beep,"plays a touen based on the input."),
  REGISTER_COMMAND("poweroff", "suths down the pc", cmd_poweroff,"shuts down the os"),
  REGISTER_COMMAND("exit", "closes the seshioin", cmd_poweroff,"will exit the os"),
  REGISTER_COMMAND("vim", "vim", cmd_vim,"text editer, i think???"),
  REGISTER_COMMAND("music", "plays a tuen", cmd_music,"play music form a list of songs using beeper. must have a beeper on the pc"),
  REGISTER_COMMAND("sleep", "adds a delay bases in secints", cmd_sleep,"sleeps for amount of secenits set by arg 1"),
  REGISTER_COMMAND("color", "sets the color.", cmd_setColor, "the dos command for color. works just like it dose in windows"),
  REGISTER_COMMAND("man", "the comman unix utily", cmd_man, "alows one to find help for each commadn in more detail. comman in unix"),
};

static const int command_count = sizeof(commands) / sizeof(commands[0]);

// --- Tokenizer ---
static void tokenize(char *input, int *argc, char **argv, int max_args) {
  *argc = 0;
  while (*input && *argc < max_args) {
    // Skip whitespace
    while (*input == ' ')
      input++;
    if (!*input)
      break;

    argv[*argc] = input;
    (*argc)++;

    while (*input && *input != ' ')
      input++;
    if (*input)
      *input++ = '\0'; // Null-terminate
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

  if (argc == 0)
    return;

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
  (void)argc;
  (void)argv;
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
  (void) argc;
  (void) argv;
  for (int i = 1; i < argc; ++i) {
    putS(argv[i]);
    if (i < argc - 1)
      putS(" ");
  }
  putS("\n");
}

void cmd_clear(int argc, char **argv) {
  (void)argc;
  (void)argv;
  Screen_Clear();
}

void cmd_time(int argc, char **argv) {
  (void)argc;
  (void)argv;
  uint8_t s, m, h;
  rtc_get_time(&h, &m, &s);
  char *buf;
  int_to_string(h%12, buf);
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
  (void) argc;
  (void) argv;
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

void cmd_poweroff(int argc, char **argv) {
  (void) argc;
  (void) argv;
  poweroff();
}

static uint8_t Vimamount = 0;
void cmd_vim(int argc, char **argv) {
  (void) argc;
  (void) argv;
  Vimamount++;
  switch (Vimamount) {
  case 1:
    printf("this is your only waring... dont run vim on this os\n");
    break;
  case 2:
    printf("strick 2, stop trying.\n");
    break;
  case 3:
    printf("I tryed to tell you ill give you one more chanse.\n");
    break;
  case 4:
    printf("I waorned you...\n");
    __asm__ __volatile__("cli");
    printf("1\n");
    delay(100000);
    printf("2\n");
    delay(100000);
    printf("3\n");
    delay(100000);
    poweroff();
    break;
  }
}

void cmd_sleep(int argc, char **argv) {
  (void) argc;
  (void) argv;
  delay(100000 * string_to_int(argv[1]));
}

void cmd_setColor(int argc, char **argv) {
    if (argc < 2 || argv[1][0] == '\0' || argv[1][1] == '\0') {
        printf("Usage: color <2-digit hex>\n");
        return;
    }

    uint8_t fg = hex_char_to_int(argv[1][0]);
    uint8_t bg = hex_char_to_int(argv[1][1]);

    if (fg == 0xFF || bg == 0xFF) {
        printf("Invalid hex digit in color code.\n");
        return;
    }

    Screen_SetColor(fg, bg);
}

void cmd_man(int argc, char **argv) {
  if(argc < 2){
    printf("you must previde a name of a command\n");
  }

  for (int i = 0; i < command_count; ++i) {
    if (strcmp(argv[1], commands[i].name) == 0) {
      Screen_Clear();
      printf("%s\n",commands[i].manPage);
      return;
    }
  }
  printf("unable to find page for\n",argv[1]);
}
