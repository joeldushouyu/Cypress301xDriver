#!/bin/sh
#https://stackoverflow.com/questions/225845/how-do-i-configure-modprobe-to-find-my-modulemodu
driver_name="usb-skeleton"
kernel_version=$(uname -r)


#1 try to delete the old, running module
sudo modprobe -r $driver_name

#2 remove the old .ko file
sudo rm -r /lib/modules/$kernel_version/$driver_name.ko
#3 copy the new driver 
sudo cp ./$driver_name.ko /lib/modules/$kernel_version/
#4
sudo depmod
#5 modprobe
sudo modprobe $driver_name
#6 change the privilage
sudo chmod 777 /dev/skel3