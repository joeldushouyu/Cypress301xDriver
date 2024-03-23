#include <stdio.h>
#include <stdlib.h>
#include <libusb-1.0/libusb.h>
#include <signal.h>
#include <time.h>

#define VENDOR_ID 0x04b4    // Replace with the actual vendor ID of your USB device
#define PRODUCT_ID 0x00f1   // Replace with the actual product ID of your USB device
#define ENDPOINT_OUT 0x01   // Replace with the actual OUT endpoint address for bulk transfer
#define DATA_SIZE 16384 * 1 // 16384 *100  // 1024*4*4*2// 1024*4 *4   *2// Define the size of the data buffer

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
void cb(struct libusb_transfer *transfer)
{   printf("Here\n");
    int *completed = transfer->user_data;
    *completed = 1;
    printf("complete tranfer\n");
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

    // create a libusb_transfer
    struct libusb_transfer *bulkTransfer;
    unsigned char data[DATA_SIZE];

    // Data to send

    // Fill data buffer with desired data
    // For example:
    unsigned int counter = 0;
    int completed = 1;
    bulkTransfer = libusb_alloc_transfer(0);

    while (1)
    {
        while(completed == 0){
            // spin to wait after completed
            libusb_handle_events(NULL);
        }
        completed = 0;
        for (int i = 0; i < DATA_SIZE; i++)
        {
            data[i] = counter;

        }

        // Send data
        int actual_sent;
        printf("About to transfer data\n");
        //TODO: different stream id?
        // TODO: exit at counter==9?
        uint32_t transfer_id = libusb_transfer_get_stream_id(bulkTransfer);
        printf("Transfer stream id is %d\n", transfer_id);
        libusb_fill_bulk_stream_transfer(bulkTransfer, handle, ENDPOINT_OUT, transfer_id, data, DATA_SIZE,cb, &completed,1000 );
        r = libusb_submit_transfer(bulkTransfer);
         printf("Starting to transfer data at counter = %d\n",counter);
        // r = libusb_bulk_transfer(handle, ENDPOINT_OUT, data, DATA_SIZE, &actual_sent, 1000);
        if (r == 0)
        {
            printf("Data sent successfully, actual sent is %d\n", actual_sent);
            counter++;
        }
        else
        {
            fprintf(stderr, "Error sending data: %s\n", libusb_error_name(r));
     
        }

    }

    // We never reach this point because of the infinite loop, Ctrl+C will trigger the signal handler

    return 0;
}
