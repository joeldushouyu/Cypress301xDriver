#include <stdio.h>
#include <stdlib.h>
#include <libusb-1.0/libusb.h>
#include <signal.h>
#include <time.h>
#include "AsyncUSB.h"


unsigned int RGBto16BIT(unsigned char R, unsigned G, unsigned B)
{
    R /= 8;
    G /= 8;
    B /= 8;
    // now, try to combine
    unsigned int returnResult;
    return returnResult = (0 << 12) | (R << 8 & 0xF00) | (G << 4 & 0xF0) | (B & 0xF);
}



int initializeUSBDevice( libusb_device_handle **handle_p, struct libusb_context **usb_context_p)
{
    libusb_device_handle *handle = *(handle_p);
    struct libusb_context *usb_context = *(usb_context_p);
    int r;
    // Initialize libusb
    r = libusb_init(&usb_context);
    if (r < 0)
    {
        fprintf(stderr, "Error initializing libusb: %s\n", libusb_error_name(r));
        return -1;
    }

    // Open the device
    handle = libusb_open_device_with_vid_pid(NULL, VENDOR_ID, PRODUCT_ID);
    if (!handle)
    {
        fprintf(stderr, "Failed to open device\n");
        libusb_exit(usb_context);
        return -1;
    }

    // Claim the interface
    r = libusb_claim_interface(handle, 0);
    if (r < 0)
    {
        fprintf(stderr, "Error claiming interface: %s\n", libusb_error_name(r));
        libusb_close(handle);
        libusb_exit(usb_context);
        return -1;
    }

    *handle_p = handle;
    *usb_context_p = usb_context;

    return 0;
}