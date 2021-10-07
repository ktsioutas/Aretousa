
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

int main () {
   char command[100];

   strcpy( command, "pactl load-module module-loopback latency_msec=1" );
   system(command);

   return(0);
} 
