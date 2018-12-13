
    #include <stdio.h>  
    #include <sys/types.h>  
    #include <libusb.h>  
	#include <unistd.h>
	char usb_buf[8];      
   #define USB_VID  0x0461
   #define USB_PID  0x4e26
	int transferred;
   static libusb_device_handle *dev_handle=NULL; 
   static void print_devs(libusb_device **devs)  
    {  
        libusb_device *dev;  
		//int cnt;
        int num=0,i,j,k;  
      
        while ((dev = devs[num++]) != NULL) 
		{  
			//printf("\ncnt=%d\n",cnt++);
            struct libusb_device_descriptor desc;  
            int ret = libusb_get_device_descriptor(dev, &desc);  
            if (ret < 0) 
			{  
                fprintf(stderr, "failed to get device descriptor");  
                return;  
            }  
      		printf("number of configurations:%d\n",(int)desc.bNumConfigurations);
            printf("vid:%04x,pid:%04x (bus %d, device %d)\n",  
                desc.idVendor, desc.idProduct,  
                libusb_get_bus_number(dev), libusb_get_device_address(dev));  
            struct libusb_config_descriptor  *config;
			libusb_get_config_descriptor(dev,0,&config);
			printf("Interface num:%d\n",(int)config->bNumInterfaces);
			const struct libusb_interface *interface;
			const struct  libusb_interface_descriptor *interdesc;//接口描述符
			const struct libusb_endpoint_descriptor *epdesc;
			for( i=0;i<(int)config->bNumInterfaces;i++)//一个设备几个配置
			{
				interface=&config->interface[i];
				printf("alternate settings:%d\n",interface->num_altsetting);
				for(j=0;j<interface->num_altsetting;j++)
				{
					interdesc=&interface->altsetting[j];
					printf("Interface num:%d",(int)interdesc->bInterfaceNumber );
					printf("endpoint num:%d",(int)interdesc->bNumEndpoints );
					for(k=0;k<(int)interdesc->bNumEndpoints;k++)
					{
						epdesc=&interdesc->endpoint[k];
						printf("Descriptor Type: %d\n",(int)epdesc->bDescriptorType);
                        printf("EP Address: %d\n",(int)epdesc->bEndpointAddress);
					}
				}
			}

		}  
    }  
      
    int main(void)  
    {  
        libusb_device **devs;
		struct libusb_device_descriptor  dev_desc;
		libusb_context *ctx=NULL; 
        int ret;  
        ssize_t cnt;  
 		printf("welcome use libusb\n");     
        ret = libusb_init(NULL); //初始化libusb 
        if (ret < 0)  
            return ret;  
      	//获取usb设置
        cnt = libusb_get_device_list(NULL, &devs);  
        printf("device_list:%d\n",cnt);
		if (cnt < 0)  
            return (int) cnt;  
        //打开设备
		dev_handle=libusb_open_device_with_vid_pid(NULL,USB_VID,USB_PID);
		if(dev_handle==NULL)
		{
			perror("cannot open device\n");
		}
		else
		{
			printf("device opened!\n");
		}
		printf("********_________**************");
        print_devs(devs);  //打印所有usb设备
      	printf("*********_________**********\n");
		libusb_free_device_list(devs, 1);  
       //查看当前接口是否被占用
		if(libusb_kernel_driver_active(dev_handle,0)==1)
		{
			printf("kernel Driver Active\n");
			if(libusb_detach_kernel_driver(dev_handle,0)==0) //被占用则解除
			{
				printf("Kernel Driver Detached!\n");
			}
		}
		ret=libusb_claim_interface(dev_handle,0);
		if(ret<0)
        {
			//设备申请接口失败
			perror("Cannot Claim Interface\n");
			return 1;
        }
	    while(1)
		{
		    ret=libusb_interrupt_transfer(dev_handle,129,usb_buf,8,&transferred,0x0a);
			if(ret==0&&transferred==8)
			{
				printf("read success!\n");
			}
			else{
				printf("read error!\n");
			}
			printf("usb_buff:\n");
			for(cnt=0;cnt<8;cnt++)
	   		{
				printf("%02x\n",usb_buf[cnt]);
       		}
			printf("\n");
			sleep(1);
		}
		ret=libusb_release_interface(dev_handle,0);
		if(ret!=0)
		{
			printf("can not release interface!\n");
		}
		else
		{
			printf("release interface\n");
		}
        libusb_exit(NULL);  
        return 0;  
    }  



















