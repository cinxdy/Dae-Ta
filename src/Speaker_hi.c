#include<stdio.h>
#include<windows.h>
#include<mmsystem.h>
#pragma comment(lib,"winmm.lib");

int main(void){
    PlaySound(TEXT("hi.wav"),0, SND_FILENAME | SND_ASYNC);
    sleep(50000);
}