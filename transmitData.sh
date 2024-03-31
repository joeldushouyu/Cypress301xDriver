#!/bin/sh
sudo renice -n -20 $$
./usb_bulk_transfer
