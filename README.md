# piMeter - a SmartMeter for Raspberry Pi

This repo contains the software for the [piMeter_hardware](https://github.com/digitaldex/piMeter_hardware) project.
You need a running InfluxDB and Grafana installation. A Tutorial on how to setup can be found
at [www.digitaldex.de](https://www.digitaldex.de/blog/new-frontend-for-opensource-smartmeter/)

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
```
influxdb_cpp::server_info si("IP", 8086, "DB", "user", "password");
```
### Build piMeter
```
g++ main.cpp spiComm.cpp -o piMeter -lbcm2835
```
### Run piMeter
```
// bcm2835 Lib needs root
sudo ./piMeter &
```
## Visualization
![alt text](https://github.com/digitaldex/piMeter_grafana/blob/master/visu/dashboard.png "Grafana Dashboard")
```
Example Grafana Export can be found in the /visu folder
``` 