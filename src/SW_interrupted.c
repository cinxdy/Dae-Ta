#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <fcntl.h>
#include <sys/ioctl.h>
#include <sys/types.h>
#include <signal.h> 
#include <linux/i2c.h>
#include <stdbool.h>
#include <linux/i2c-dev.h>
#define MOD_LED 0x20

int bool_interrupt()
{ 
    int fd;
    int retv;
//    char value;
    char values[2]={0,};
    bool r_value=false;

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

    values[0] = 0x06;
    values[1] = 0x03;
    write(fd,values, 2);
    values[0] = 0x02;
    values[1] = 0xff;
    write(fd, values, 2);

    char reg;
    char getdata=0;
//    char led_state=0xff;
    char btn_state[2]={0,};

    // Set Input Address
    reg = 0x00;
    write(fd,&reg,1);
    read(fd,&getdata,1);

    if(!(getdata&0x01)){
        if(btn_state[0]==0){
            printf("눌림\n");
            btn_state[0] = 1;
            values[0]=0x02;
            values[1]=0xfb;
            printf("LED Data = %x!!!!!\n",values[1]);
            write(fd, values, 2);
            r_value=true;
        }
    }
    else {
        if(btn_state[0]==1){
            printf("뗌\n");
            btn_state[0]=0;
            values[0]=0x02;
            values[1]=0xff;
            printf("LED Data = %x\n",values[1]);
            write(fd, values, 2);
            r_value=false;
        }


        close(fd);
        return r_value;
    }
}
