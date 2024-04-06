CC = gcc
CFLAGS = -Wall -g
LDFLAGS = -lusb-1.0

.PHONY: all clean

all: usb_bulk_transfer usb_bulk_receive usb_bulk_transfer_async

usb_bulk_transfer: WriteToUSB.c
	$(CC) $(CFLAGS) $^ -o $@ $(LDFLAGS)

usb_bulk_transfer_async: WriteToUSBAsync.c
	$(CC) $(CFLAGS) $^ -o $@ $(LDFLAGS)

usb_bulk_receive: readFromUsb.c
	$(CC) $(CFLAGS) $^ -o $@ $(LDFLAGS)

clean:
	rm -f usb_bulk_transfer usb_bulk_receive usb_bulk_transfer_async
