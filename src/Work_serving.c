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

int main(int argc, char** argv)
{ int cled_fd;
int retv;
char value;
char values[2]={0,};
// Signal Setting
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
// GPIO Exapnder Port 1
values[0] = 0x06; 
values[1] = 0x00; 
write(cled_fd,values, 2);

values[0] = 0x02; // Output Port 1
values[1] = 0xff; 
write(cled_fd, values, 2);
values[1] = 0x50; 
printf("Set Value : 0x%02x...\n",values[1]);
write(cled_fd,values,2);

close(cled_fd);
return 0;
}