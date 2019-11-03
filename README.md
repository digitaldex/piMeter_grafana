# piMeter - a SmartMeter for Raspberry Pi

This is the software for the [piMeter_hardware](https://github.com/digitaldex/piMeter_hardware).
You need a running InfluxDB and Grafana installation. A Tutorial on how to setup can be found [here](https://www.digitaldex.de/new-frontend-for-pimeter/)

## The Software
![alt text](https://github.com/digitaldex/piMeter_grafana/blob/master/visu/GrafanaExample.png "Eagle Board Screenshot")

## Setup Raspberry Pi

### Enable SPI
```
sudo raspi-config
-> Interface Option
-> Enable SPI
```
### Install Dependencies
```
// Install bcm2835 Library
wget http://www.airspayce.com/mikem/bcm2835/bcm2835-1.52.tar.gz
tar zxvf bcm2835-1.52.tar.gz
./configure
make
sudo make check
sudo make install
```
### Change influxdb credentials in main.cpp
ˋˋˋ
IP, DB, user and password
### Build piMeter
```
g++ main.cpp spiComm.cpp -o piMeter -lbcm2835
```
### Run piMeter
```
// bcm2835 Lib needs root
sudo ./piMeter &
