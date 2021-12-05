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
int bool_sw()
{ int fd;
int retv;
char value;
char values[2]={0,};
char i2c_event=0;
// Signal Setting

fd = open("/dev/i2c-1",O_RDWR);
if(fd<3)
{
printf("Device Open Error!\n");
return -1;
}
retv=ioctl(fd, I2C_SLAVE, MOD_LED);
if(retv!=0)
{
printf("Ioctl Error!\n");
return -1;
}
// GPIO Exapnder Port 1 Setting
values[0] = 0x06; // GPIO Register - TCA9535 Data Sheet 23p
values[1] = 0x03; // Setting Value XBGRLLSS = 00000011
write(fd,values, 2);
// CLED Initial 
values[0] = 0x02; // In/Output Port 1
values[1] = 0xff; // All Off XBGRLLSS = 11111111
write(fd, values, 2);

char reg;
char getdata=0;
char led_state=0xff;
char btn_state[2]={0,};
int r_value;
int B_value;

// Set Input Address
reg = 0x00;
write(fd,&reg,1);
read(fd,&getdata,1);
if(!(getdata&0x01))
{
if(btn_state[0]==0)
{
 printf("1\n");
 btn_state[0] = 1;
 r_value=1;
}
} else {
if(btn_state[0]==1)
{
 printf("2\n");
 r_value=0;
 btn_state[0]=0;
 i2c_event = 1;
 if(led_state&0x04)
 led_state&=~(1<<2);
 else
 led_state|=(1<<2); 
}
}
if(!(getdata&0x02))
{
if(btn_state[1]==0)
{
 printf("3\n");
 B_value=1;
 btn_state[1] = 1;
}
} else {
if(btn_state[1]==1)
{
 printf("4\n");
 B_value=0;
 btn_state[1]=0;
 i2c_event = 1;
 if(led_state&0x08)
 led_state&=~(1<<3);
 else
 led_state|=(1<<3);
}
}
if(i2c_event!=0)
{
printf("Event Processing...\n");
i2c_event = 0;
values[0]=0x02;
values[1]=led_state;
printf("LED Data = %x\n",values[1]);
write(fd, values, 2);
}
close(fd);
}




