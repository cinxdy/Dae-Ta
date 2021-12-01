#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <fcntl.h>
#include <sys/ioctl.h>
#include <sys/types.h>
#include <linux/i2c-dev.h>
int main(int argc, char** argv){
int led_fd;
int retv;
char i2c_reg;
char led_data=0;
char temp_values[2]={0,};
char values[2]={0,};
// open i2c
led_fd = open("/dev/i2c-1",O_RDWR);
if(led_fd < 3) {
printf("Device node open error!!!: %s", "/dev/i2c-1");
return -1;
}
retv=ioctl(led_fd, I2C_SLAVE,0x20);
if(retv!=0) {
printf("Ioctl Error!\n");
return -1;
}
read(led_fd,temp_values,8);
printf("%d\n",temp_values[1]);
//Configuration Port 1
i2c_reg = 0x07;
values[0] = i2c_reg;
values[1] = 0x00; 
write(led_fd,values, 2);
printf("%d\n",temp_values[1]);
printf("%d\n",values[1]);
//Output Port 1
i2c_reg = 0x03;
values[0] = i2c_reg; 
write(led_fd, values, 2);

values[1] = temp_values[1];
write(led_fd,values,2);
read(led_fd,temp_values,8);
values[1]= 0xFD&temp_values[1];
write(led_fd,values,2);
close(led_fd);
return 0;
}