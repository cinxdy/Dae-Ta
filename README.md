# Serving Robot Dae-Ta!

## Introduction
식당에서 일하는 모든 알바생의 대타를 뛰어줄  
똑똑한 서빙 로봇 DaeTa!  
In IoT Design Class

## Build and Execute
1. Build the Dae-Ta.pro file using QT creator.
[Github URL](https://github.com/cinxdy/Dae-Ta)
2. Build the Dae-Ta-Client.pro file using QT creator.
[Github URL](https://github.com/cinxdy/Dae-Ta-Client)
3. Network setting 
```
sudo vi /etc/dhcpcd.conf
```
Add following codes to the dhcpcd.conf of the Raspberry Pi2 which Dae-Ta-Client will be excuted.
`````
interface wlan0
static ip_address=192.168.137.141/24
static routers=192.168.137.1
static domain_name_servers=192.168.137.1
`````
4. Run the Dae-Ta in the Raspberry Pi1
5. Run the Dae-Ta-Client in the Raspberry Pi2
* If you run two program at same Raspberry Pi, you should change the client.cpp 16-th line like this:
```
socket->connectToHost("127.0.0.1",8520);
```
default:
```
socket->connectToHost(QString("192.168.137.141"),8520);
```

### QT creator
[QT creator](https://www.qt.io/product/development-tools)

## Flow for UI

## Flow for Socket Communication

# License
Prof. Yunseok Cho (yscho@handong.edu)  
Jiyoung Shin (21800409@handong.edu)  
HyeJung Gong (21900030@handong.edu)  

