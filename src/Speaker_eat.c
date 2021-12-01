#include<stdio.h>
#include<curses.h>
#include<mmsystem.h>
#pragma comment(lib,"winmm.lib");

int main(void){
    PlaySound(TEXT("eat.wav"),0, SND_FILENAME | SND_ASYNC);
    sleep(50000);
}