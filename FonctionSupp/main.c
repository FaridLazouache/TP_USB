#include <libusb.h>
#include <stdio.h>
#include <stdlib.h>
#include <error.h>

int main()
{
    libusb_context *context;
    int status=libusb_init(&context);
    if(status!=0) {perror("libusb_init"); exit(-1);}
    /* ... some code ... */
    libusb_device **list;
    ssize_t count=libusb_get_device_list(context,&list);
    if(count<0) {perror("libusb_get_device_list"); exit(-1);}
    ssize_t i=0;
    for(i=0;i<count;i++)
    {
        libusb_device *device=list[i];
        struct libusb_device_descriptor desc;
        int status=libusb_get_device_descriptor(device,&desc);
        if(status!=0) 
            continue;
        uint8_t bus=libusb_get_bus_number(device);
        uint8_t address=libusb_get_device_address(device);
        printf("Device Found @ (Bus:Address) %d:%d\n",bus,address);
        printf("Vendor ID 0x0%x\n",desc.idVendor);
        printf("Product ID 0x0%x\n",desc.idProduct);
  }
    libusb_free_device_list(list,1);

    libusb_exit(context);
}

=''