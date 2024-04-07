
#include <stdio.h>
#include <stdlib.h>
#include <libusb-1.0/libusb.h>
#include <signal.h>
#include <time.h>

#define VENDOR_ID 0x04b4     // Replace with the actual vendor ID of your USB device
#define PRODUCT_ID 0x00f1    // Replace with the actual product ID of your USB device
#define ENDPOINT_OUT 0x01    // Replace with the actual OUT endpoint address for bulk transfer
#define CYPRESS_BULK_SIZE 4 * (4096) //   4*4096 // 16384 *100  // 1024*4*4*2// 1024*4 *4   *2// Define the size of the data buffer

#define TOTAL_BULK_ARRAY (480 * 640) / 4096


unsigned int RGBto16BIT(unsigned char R, unsigned G, unsigned B);
int initializeUSBDevice( libusb_device_handle **handle_p, struct libusb_context **usb_context_p);





typedef struct {
      struct libusb_transfer *transfers [TOTAL_BULK_ARRAY] ;
      unsigned char *transfer_buf[TOTAL_BULK_ARRAY];
}USBImageTransfer;
