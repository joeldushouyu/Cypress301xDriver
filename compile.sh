rm usb_bulk_transfer usb_bulk_receive
gcc WriteToUSB.c -o usb_bulk_transfer -lusb-1.0
gcc readFromUsb.c -o usb_bulk_receive -lusb-1.0