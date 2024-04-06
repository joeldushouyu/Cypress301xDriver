#include <stdio.h>
#include <stdlib.h>
#include <libusb-1.0/libusb.h>
#include <signal.h>
#include <time.h>

#define VENDOR_ID 0x04b4     // Replace with the actual vendor ID of your USB device
#define PRODUCT_ID 0x00f1    // Replace with the actual product ID of your USB device
#define ENDPOINT_OUT 0x01    // Replace with the actual OUT endpoint address for bulk transfer
#define DATA_SIZE 4 * (4096) //   4*4096 // 16384 *100  // 1024*4*4*2// 1024*4 *4   *2// Define the size of the data buffer

#define TOTAL_DATA_ARRAY (480*640)/4096
libusb_device_handle *handle = NULL;


unsigned int counter= 0;

static void read_callback(struct libusb_transfer *transfer)
{

    int res;
    if (transfer->status == LIBUSB_TRANSFER_COMPLETED)
    {
        // success
    }
    else
    {
        printf("ERROR: %s\n", libusb_error_name(transfer->status));
    }

    // resubmit the transfer object back to queue
    res = libusb_submit_transfer(transfer);
}

void sigint_handler(int signum)
{

    // TODO: release the async resources also
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

static struct libusb_transfer *create_transfer(libusb_device_handle *handle, size_t length)
{
    struct libusb_transfer *transfer;
    unsigned char *buf;

    // setup the transfer object
    buf = malloc(length);
    transfer = libusb_alloc_transfer(0); // Zero since is a bulk transfer
    // TODO: user data pass back to callback function

    //TODO: testing only for data initialization
    unsigned char dataOut = counter;
    for(unsigned int i = 0; i < length; i++){
        buf[i] = dataOut;
        dataOut ++;
    }
    counter++;
    

    libusb_fill_bulk_transfer(transfer, handle, ENDPOINT_OUT, buf, length, read_callback, NULL, 5000);
    return transfer;
}



int main()
{
    int r;
    // The context
    struct libusb_context *usb_context;
    // Initialize libusb
    r = libusb_init(&usb_context);
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
        libusb_exit(usb_context);
        return 1;
    }

    // Set up signal handler for SIGINT (Ctrl+C)
    signal(SIGINT, sigint_handler);


    // create the transfer
    for (unsigned int i = 0; i < TOTAL_DATA_ARRAY; i++)
    {
        struct libusb_transfer *transfer = create_transfer(handle, DATA_SIZE);
        libusb_submit_transfer(transfer); // start the intitial transfer
    }


    int res;
    while (1)
    {
        res = libusb_handle_events(usb_context);
        if (res < 0)
        {
            printf("handle_events()error # %d\n",res);
            /* Break out of this loop only on fatal error.*/
            if (res != LIBUSB_ERROR_BUSY &&
                res != LIBUSB_ERROR_TIMEOUT &&
                res != LIBUSB_ERROR_OVERFLOW &&
                res != LIBUSB_ERROR_INTERRUPTED)
            {
                break;
            }
        }
    }
    return 0;
}