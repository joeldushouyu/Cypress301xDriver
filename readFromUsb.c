#include <stdio.h>
#include <stdlib.h>
#include <libusb-1.0/libusb.h>

#define VENDOR_ID 0x04b4  // Replace with the actual vendor ID of your USB device
#define PRODUCT_ID 0x00f1 // Replace with the actual product ID of your USB device
#define ENDPOINT_IN 0x81 // Replace with the actual OUT endpoint address for bulk transfer
#define DATA_SIZE 16384//1024*4 *4// Define the size of the data buffer
#define FILENAME "received_data.bin" // Define the filename for the local file

int main() {
    libusb_device_handle *handle;
    int r;

    // Initialize libusb
    r = libusb_init(NULL);
    if (r < 0) {
        fprintf(stderr, "Error initializing libusb: %s\n", libusb_error_name(r));
        return 1;
    }

    // Open the device
    handle = libusb_open_device_with_vid_pid(NULL, VENDOR_ID, PRODUCT_ID);
    if (!handle) {
        fprintf(stderr, "Failed to open device\n");
        libusb_exit(NULL);
        return 1;
    }

    // Claim the interface
    r = libusb_claim_interface(handle, 0);
    if (r < 0) {
        fprintf(stderr, "Error claiming interface: %s\n", libusb_error_name(r));
        libusb_close(handle);
        libusb_exit(NULL);
        return 1;
    }

    // Data buffer to store received data
    unsigned char data[DATA_SIZE];

    // Receive data
    int actual_received;
    r = libusb_bulk_transfer(handle, ENDPOINT_IN, data, DATA_SIZE, &actual_received, 1000);
    if (r == 0) {
        printf("Data received successfully\n");

        // Open a file for writing
        FILE *file = fopen(FILENAME, "wb");
        if (!file) {
            fprintf(stderr, "Error opening file for writing\n");
            libusb_release_interface(handle, 0);
            libusb_close(handle);
            libusb_exit(NULL);
            return 1;
        }

        // Write received data to the file
        fwrite(data, sizeof(unsigned char), actual_received, file);

        // Close the file
        fclose(file);
        printf("Data written to file: %s\n", FILENAME);
    } else {
        fprintf(stderr, "Error receiving data: %s\n", libusb_error_name(r));
    }

    // Release the interface and close the device
    libusb_release_interface(handle, 0);
    libusb_close(handle);

    // Cleanup libusb
    libusb_exit(NULL);

    return 0;
}
