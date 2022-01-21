# gpsterm
quick &amp; dirty serial port reader for gps on RHEL

Assumes 4800,n,8,1 on /dev/ttyUSB0

0. add $USER to group "dialout" in order to access serial port device
1. Requires https://github.com/gbmhunter/CppLinuxSerial
2. Then build as: g++ -o gpsterm main.cpp -lCppLinuxSerial
