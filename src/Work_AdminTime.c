#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <fcntl.h>
#include <sys/ioctl.h>
#include <sys/types.h>
#include <signal.h> 
#include <linux/i2c.h>
#include <linux/i2c-dev.h>

#define MOD_LED 0x20
volatile int g_loop=1;
void sigint_handle(int sig)
{ printf("...Terminated!\n");
g_loop = 0;
}
int main(int argc, char** argv)
{ int cled_fd;
int retv;
char value;
char values[2]={0,};
// Signal Setting
signal(SIGINT, sigint_handle); 
cled_fd = open("/dev/i2c-1",O_RDWR);
if(cled_fd<3)
{
printf("Device Open Error!\n");
return -1;
}
retv=ioctl(cled_fd, I2C_SLAVE, MOD_LED);
if(retv!=0)
{
printf("Ioctl Error!\n");
return -1;
}
// GPIO Exapnder Port 
values[0] = 0x06; 
values[1] = 0x00; 
write(cled_fd,values, 2);
// CLED Initial 
values[0] = 0x02; // Output Port 1
values[1] = 0xff; 
write(cled_fd, values, 2);
values[1] = 0x30; 
write(cled_fd,values,2);
close(cled_fd);
return 0;
}