#include <stdio.h>
#include <stdlib.h>
#include <libusb-1.0/libusb.h>
#include <signal.h>
#include <time.h>

#define VENDOR_ID 0x04b4    // Replace with the actual vendor ID of your USB device
#define PRODUCT_ID 0x00f1   // Replace with the actual product ID of your USB device
#define ENDPOINT_OUT 0x01   // Replace with the actual OUT endpoint address for bulk transfer
#define DATA_SIZE (480 * 640 * 4)  // Size of the data buffer (640x480 pixels with 32-bit color)

libusb_device_handle *handle = NULL;

void sigint_handler(int signum)
{
    if (handle != NULL)
    {
        // Release the interface and close the device
        libusb_release_interface(handle, 0);
        libusb_close(handle);
    }

    // Cleanup libusb
    libusb_exit(NULL);

    exit(signum);
}

int main()
{
    int r;

    // Initialize libusb
    r = libusb_init(NULL);
    if (r < 0)
    {
        fprintf(stderr, "Error initializing libusb: %s\n", libusb_error_name(r));
        return 1;
    }

    // Open the device
    handle = libusb_open_device_with_vid_pid(NULL, VENDOR_ID, PRODUCT_ID);
    if (!handle)
    {
        fprintf(stderr, "Failed to open device\n");
        libusb_exit(NULL);
        return 1;
    }

    // Claim the interface
    r = libusb_claim_interface(handle, 0);
    if (r < 0)
    {
        fprintf(stderr, "Error claiming interface: %s\n", libusb_error_name(r));
        libusb_close(handle);
        libusb_exit(NULL);
        return 1;
    }

    // Set up signal handler for SIGINT (Ctrl+C)
    signal(SIGINT, sigint_handler);

    // Data to send
    unsigned char data[DATA_SIZE];
    unsigned char counter = 0;

    while (1)
    {
        for (unsigned int y = 0; y < 480; y++) {
            for (unsigned int x = 0; x < 640; x++) {
                if (x < 320) {
                    // Set the left half of the screen to red
                    data[(y * 640 + x) * 4] = 0xFF;     // Red
                    data[(y * 640 + x) * 4 + 1] = 0x00; // Green
                    data[(y * 640 + x) * 4 + 2] = 0x00; // Blue
                    data[(y * 640 + x) * 4 + 3] = 0x00; // Alpha
                } else {
                    // Set the right half of the screen to blue
                    data[(y * 640 + x) * 4] = 0x00;     // Red
                    data[(y * 640 + x) * 4 + 1] = 0x00; // Green
                    data[(y * 640 + x) * 4 + 2] = 0xFF; // Blue
                    data[(y * 640 + x) * 4 + 3] = 0xFF; // Alpha
                }
            }
        }

        // Send data
        int actual_sent;
        r = libusb_bulk_transfer(handle, ENDPOINT_OUT, data, DATA_SIZE, &actual_sent, 1000);
        if (r == 0)
        {
            printf("Data sent successfully, actual sent is %d\n", actual_sent);
            counter++;
            //if(counter==12){break;}
        }
        else
        {
            fprintf(stderr, "Error sending data: %s\n", libusb_error_name(r));
        }
    }

    // We never reach this point because of the infinite loop, Ctrl+C will trigger the signal handler

    return 0;
}