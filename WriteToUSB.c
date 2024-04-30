#include <stdio.h>
#include <stdlib.h>
#include <libusb-1.0/libusb.h>
#include <signal.h>
#include <time.h>

#define VENDOR_ID 0x04b4    // Replace with the actual vendor ID of your USB device
#define PRODUCT_ID 0x00f1   // Replace with the actual product ID of your USB device
#define ENDPOINT_OUT 0x01   // Replace with the actual OUT endpoint address for bulk transfer
#define DATA_SIZE  16384 // 16384 *100  // 1024*4*4*2// 1024*4 *4   *2// Define the size of the data buffer

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
    // Fill data buffer with desired data
    // For example:
    unsigned char counter = 0;
    unsigned char dataOut = 0;
    struct timespec res;
    res.tv_sec = 0;
    res.tv_nsec = 86805;

    while (1)
    {
        res.tv_sec = 0;
        res.tv_nsec = 868050;
        char f = 0;
        dataOut = counter;
        for (int i = 0; i < DATA_SIZE; i ++)
        {
            // unsigned int *p = (unsigned int *)(data + i);
            // *p = counter + i;
            // if(i%(512*4) ==0){
            //     dataOut++;
            // }
            data[i] = dataOut;
            dataOut++;
            
            //  if (i < 1024 * 4 * 4)
            //  {
            //      data[i] = counter; // indicate first page
            //  }
            //  else
            //  {
            //      data[i] = counter;
            //  }
            //  if (f == 0)
            //  {
            //      data[i] = 0x0;
            //      f = 1;
            //  }
            //  else{
            //      data[i] = 0x0;
            //      f = 0;
            //  }

            // data[i] = 0xff;
        }

        // Send data
        int actual_sent;
        r = libusb_bulk_transfer(handle, ENDPOINT_OUT, data, DATA_SIZE, &actual_sent, 1000);
        if (r == 0)
        {
            printf("Data sent successfully, actual sent is %d\n", actual_sent);
            counter++;
            if(counter==8){break;}
        }
        else
        {
            fprintf(stderr, "Error sending data: %s\n", libusb_error_name(r));
        }

        clock_nanosleep(CLOCK_MONOTONIC, 0, &res, NULL);
    }

    // We never reach this point because of the infinite loop, Ctrl+C will trigger the signal handler

    return 0;
}