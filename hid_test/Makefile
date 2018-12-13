CC=arm-none-linux-gnueabi-gcc
INLCUDE=-I ../arm_lib/include/libusb-1.0
LIBS=-L ../arm_lib/lib -lusb-1.0 -lpthread -static
all:demo
demo:
	$(CC)   -g -Wall -o  demo usb_hid.c  $(INLCUDE)  $(LIBS)
clean:
	rm -rf *.o demo

