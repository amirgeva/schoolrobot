#include "iface.h"
#include <string.h>
#include <stdlib.h>

typedef void (*cmd_func)(int*,int);
struct CommandMapping
{
  char cmd;
  cmd_func func;
};

CommandMapping all_cmds[] = {
  {'D',process_drive_command}, // Drive <left> <right>
  {'S',process_step_command},  // Step <left> <left_count> <right> <right_count>
  {'R',process_servo_command}, // Servo <state> (150-600)
  {'L',process_lidar_command}  // LIDAR <on>
};
const int commands_number = sizeof(all_cmds)/sizeof(CommandMapping);
int params[8];

const char* process_command(char* msg)
{
  for(int i=0;i<8;++i) params[i]=0;
  char* cmd=strtok(msg," ");
  char* param=0;
  int n=0;
  while ((param = strtok(NULL," ")) != NULL && n<8)
    params[n++] = atoi(param);
  for(int i=0;i<commands_number;++i)
    if (cmd[0]==all_cmds[i].cmd) (*(all_cmds[i].func))(params,n);
  return NULL;
}


