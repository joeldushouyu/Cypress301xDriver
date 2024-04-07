// #include <stdio.h>
// #include <stdlib.h>
// #include <libusb-1.0/libusb.h>
// #include <signal.h>
// #include <time.h>

// #define VENDOR_ID 0x04b4     // Replace with the actual vendor ID of your USB device
// #define PRODUCT_ID 0x00f1    // Replace with the actual product ID of your USB device
// #define ENDPOINT_OUT 0x01    // Replace with the actual OUT endpoint address for bulk transfer
// #define CYPRESS_BULK_SIZE 4 * (4096) //   4*4096 // 16384 *100  // 1024*4*4*2// 1024*4 *4   *2// Define the size of the data buffer

// #define TOTAL_BULK_ARRAY (480 * 640) / 4096

#include "AsyncUSB.h"

libusb_device_handle *handle = NULL;
struct libusb_context *usb_context = NULL;

unsigned int counter = 0;

// two buffer of the image
// 1 pointer to the current image
// 1 lock on the current image

static void read_callback(struct libusb_transfer *transfer)
{

    if (transfer->status == LIBUSB_TRANSFER_COMPLETED)
    {
        // success
    }
    else
    {
        printf("ERROR: %s\n", libusb_error_name(transfer->status));
    }

    // resubmit the transfer object back to queue
    libusb_submit_transfer(transfer);
}

// static struct libusb_transfer *create_transfer(libusb_device_handle *handle, size_t length)
// {
//     struct libusb_transfer *transfer;
//     unsigned char *buf;

//     // setup the transfer object
//     buf = malloc(length);
//     transfer = libusb_alloc_transfer(0); // Zero since is a bulk transfer
//     // TODO: user data pass back to callback function

//     // TODO: testing only for data initialization
//     unsigned char dataOut = counter;
//     // for(unsigned int i = 0; i < length; i++){
//     //     buf[i] = dataOut;
//     //     dataOut ++;
//     // }
//     // counter++;
//     unsigned int dataToSend;
//     for (unsigned int i = 0; i < length; i += 4)
//     {
//         if (counter < 50)
//         {
//             dataToSend = RGBto16BIT(255, 255, 0);
//         }
//         else
//         {
//             dataToSend = RGBto16BIT(255, 0, 255);
//         }

//         // neglect top t
//         buf[i + 2] = (dataToSend >> 8) & 0xFF;
//         buf[i + 3] = (dataToSend) & 0xFF;
//     }
//     if (counter == 74)
//     {
//         counter = 0;
//     }
//     else
//     {
//         counter++;
//     }

//     libusb_fill_bulk_transfer(transfer, handle, ENDPOINT_OUT, buf, length, read_callback, NULL, 5000);
//     return transfer;
// }

int main()
{
    if (initializeUSBDevice(&handle, &usb_context) == -1)
    {
        printf("Error in Initialize the USB device\n");
        return 1;
    }

    // // create the transfer
    // for (unsigned int i = 0; i < TOTAL_BULK_ARRAY; i++)
    // {
    //     struct libusb_transfer *transfer = create_transfer(handle, CYPRESS_BULK_SIZE);
    //     libusb_submit_transfer(transfer); // start the intitial transfer
    // }

    // initalize a frame

    // now, create a list of transfers to point to them
    USBImageTransfer bulkTransferList;
    unsigned int imageSize = (unsigned int )(CYPRESS_BULK_SIZE) * (unsigned int)(TOTAL_BULK_ARRAY);


    int totalFrameSize = 20;
    for (unsigned int i = 0; i < totalFrameSize; i++)
    {
        bulkTransferList.transfer_buf[i] = calloc(imageSize, 1);
        bulkTransferList.transfers[i] = libusb_alloc_transfer(0);
    }

    // // initialize the image
    for(unsigned int i = 0; i < totalFrameSize; i++){
        unsigned int pixelCount = 0;
        for (unsigned int bufIndex = 0; bufIndex< imageSize; bufIndex+=4, pixelCount++){
            int dataToSend;
            if(pixelCount < (640*480)/2){
                dataToSend = RGBto16BIT(255,0,0);
            }else{
                dataToSend = RGBto16BIT(0,0,255);
            }

            unsigned char *buf = bulkTransferList.transfer_buf[i];
            buf[bufIndex + 2] = (dataToSend >> 8) & 0xFF;
            buf[bufIndex + 3] = (dataToSend ) & 0xFF;
        }
    }

    
    for (unsigned int i = 0; i < totalFrameSize; i++)
    {

        // start sending
        libusb_fill_bulk_transfer(bulkTransferList.transfers[i], handle, 
        ENDPOINT_OUT, bulkTransferList.transfer_buf[i], imageSize, 
        read_callback, NULL, 5000);
        libusb_submit_transfer(bulkTransferList.transfers[i]);
    }

    int res;
    while (1)
    {
        res = libusb_handle_events(usb_context);
        if (res < 0)
        {
            printf("handle_events()error # %d\n", res);
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