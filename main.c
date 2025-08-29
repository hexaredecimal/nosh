
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/wait.h>
#include <unistd.h>

#include <readline/history.h>
#include <readline/readline.h>

#define NOB_IMPLEMENTATION
#define NOB_STRIP_PREFIX
#define SHLEX_IMPLEMENTATION
#include "nob.h"
#include "shlex.h"

#define MAX_LINE_INPUT 1024

typedef struct {
  char *leftSymbol;
  char *rightSymbol;
  char *userSymbol;
  bool nextLine;
} CommandLineFormat;

typedef struct {
  char **items;
  size_t count;
  size_t capacity;
} StringList;

int getCurrentDirectory(char *cwd) {
  if (getcwd(cwd, sizeof(cwd) * 1024) != NULL) {
    return 0;
  } else {
    perror("getcwd() error");
    return 1;
  }
}

void displayPrompt(CommandLineFormat fmt, char *cwd) {
  printf(
    "%s%s%s%s%s", 
    fmt.leftSymbol, 
    cwd, 
    fmt.rightSymbol, 
    fmt.userSymbol ? fmt.userSymbol : "",
    fmt.nextLine ? "\n > " : ""
  );
}

char *getPrompt(CommandLineFormat fmt, char *cwd) {

  Shlex s = {0};
  if (fmt.leftSymbol)
    shlex_append_quoted(&s, fmt.leftSymbol);

  if (cwd)
    shlex_append_quoted(&s, cwd);

  if (fmt.rightSymbol)
    shlex_append_quoted(&s, fmt.rightSymbol);
  
  if (fmt.userSymbol)
    shlex_append_quoted(&s, fmt.userSymbol);

  char* result = strdup(shlex_join(&s));
  shlex_free(&s);
  return result;
}

StringList splitString(char *input) {
  StringList list = {0};
  Shlex s = {0};
  shlex_init(&s, input, input + strlen(input));
  while (shlex_next(&s)) {
    da_append(&list, strdup(s.string));
  }
  shlex_free(&s);
  return list;
}

void freeStringList(StringList* list) {
    for (size_t i = 0; i < list->count; ++i) {
        free(list->items[i]);
    }

    list->capacity = list->count = 0;
}

int main() {
  CommandLineFormat cmd = {0};
  cmd.leftSymbol = "🭈🭃🮘🮘";
  cmd.rightSymbol = "🮙🮙🮙🭏🬽";
  cmd.userSymbol = " ";
  cmd.nextLine = true;

  using_history();

  for (;;) {
    char cwd[1024];
    getCurrentDirectory(cwd);
    displayPrompt(cmd, cwd);
    char * input = readline ("");

    if (strcmp(input, "exit") == 0) {
        free(input);
        return 1;
    }

    int len = strlen(input);
    if (len == 0) {
        free(input);
        continue;
    } else if (len == 1 || *input == ' ') {
        free(input);
        continue;
    }

    add_history(input);

    StringList strings = splitString(input);

    // TODO: Use The new PIPE API to run commands
    // If a command does not exist in /usr/bin or in the $PATH
    // then look for a file with that name in the current directory
    // If found, try and execute it else, try running the command with bash
    // and then print an error if all fails
    
    // TODO: Handle cases for input with a `|` or `>>`, '>', `<<`, '<'

    freeStringList(&strings);
  }
}
