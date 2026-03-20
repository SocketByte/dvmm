#ifndef PROCESS_H
#define PROCESS_H

int run_process(char* const argv[]);
int run_process_async(char* const argv[]);
char* run_command_capture(const char *command);

#endif // PROCESS_H
