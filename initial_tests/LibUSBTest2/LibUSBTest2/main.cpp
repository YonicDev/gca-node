#include <iostream>
#include <string>
#include "libusb.h"

using namespace std;

void printdev(libusb_device *dev);
void print_specific_device(libusb_device **devs,ssize_t count, uint16_t vid, uint16_t pid);
libusb_device* get_device(libusb_device **list, ssize_t count, uint16_t vid, uint16_t pid);
void add_adapter(libusb_device **devs, ssize_t count, uint8_t *endpoint_out, uint8_t *endpoint_in);

const uint16_t GAMECUBE_VID = 0x57E;
const uint16_t GAMECUBE_PID = 0x337;

int main() {

	libusb_device **devs; //pointer to pointer of device, used to retrieve a list of devices
	libusb_device_handle *dev_gpad_handle = nullptr;
	libusb_context *ctx = nullptr; //a libusb session
	int r; //for return values
	ssize_t cnt; //holding number of devices in list

	uint8_t controller_payload[37];
	uint8_t controller_payload_swap[37];

	uint8_t endpoint_out = 0;
	uint8_t endpoint_in = 0;

	int controller_payload_size = { 0 };
	int payload_size = 0;

	r = libusb_init(&ctx); //initialize a library session
	if (r < 0) {
		cout << "Init Error " << r << endl; //there was an error
		return 1;
	}
	libusb_set_debug(ctx, 3); //set verbosity level to 3, as suggested in the documentation
	cnt = libusb_get_device_list(ctx, &devs); //get the list of devices
	if (cnt < 0) {
		cout << "Get Device Error" << endl; //there was an error
	}
	cout << cnt << " Devices in list." << endl; //print total number of usb devices

	add_adapter(devs, cnt, &endpoint_out, &endpoint_in);

	dev_gpad_handle = libusb_open_device_with_vid_pid(ctx, GAMECUBE_VID, GAMECUBE_PID);

	if (dev_gpad_handle == nullptr) {
		cout << "Cannot open device" << endl;
	}
	else {
		cout << "Adapter has been opened" << endl;
	}
	libusb_free_device_list(devs, 1);

	if (libusb_kernel_driver_active(dev_gpad_handle, 0)==1) {
		cout << "Kernel is attached" << endl;
		if (libusb_detach_kernel_driver(dev_gpad_handle, 0)==0) {
			cout << "Kernel detached!" << endl;
		}
	}
	r = libusb_claim_interface(dev_gpad_handle, 0);
	if (r < 0) {
		cout << "Can't claim interface" << endl;
		return 2;
	}
	cout << "Claimed interface" << endl;
	r = libusb_bulk_transfer(dev_gpad_handle, endpoint_in, controller_payload_swap,sizeof(controller_payload_swap),&payload_size,16);
	controller_payload_size = payload_size;
	if (r == 0) {
		cout << "I think we got something out of the adapter!" << endl;
	}
	else {
		libusb_error error = static_cast<libusb_error>(r);
		string code = "";
		switch (error) {
			case LIBUSB_ERROR_TIMEOUT:
				code = "LIBUSB_ERROR_TIMEOUT";
				break;
		}
		cout << "Read error: " << code << endl;
	}
	r = libusb_release_interface(dev_gpad_handle, 0);
	if (r != 0) {
		cout << "Cannot release interface" << endl;
		return 1;
	}
	cin.ignore();
	libusb_close(dev_gpad_handle);
	libusb_exit(ctx);
	return 0;
}
void print_specific_device(libusb_device **devs,ssize_t count, uint16_t vid, uint16_t pid) {
	libusb_device *dev = get_device(devs, count, vid, pid);
	if (dev != nullptr)
		printdev(dev);
	else
		cout << "No device has been found." << endl;
}
void printdev(libusb_device *dev) {
	libusb_device_descriptor desc;
	int r = libusb_get_device_descriptor(dev, &desc);
	if (r < 0) {
		cout << "failed to get device descriptor" << endl;
		return;
	}
	cout << "Number of possible configurations: " << (int)desc.bNumConfigurations << "  ";
	cout << "Device Class: " << (int)desc.bDeviceClass << "  ";
	cout << "VendorID: " << desc.idVendor << "  ";
	cout << "ProductID: " << desc.idProduct << endl;
	libusb_config_descriptor *config;
	libusb_get_config_descriptor(dev, 0, &config);
	cout << "Interfaces: " << (int)config->bNumInterfaces << endl;
	const libusb_interface *inter;
	const libusb_interface_descriptor *interdesc;
	const libusb_endpoint_descriptor *epdesc;
	for (int i = 0; i<(int)config->bNumInterfaces; i++) {
		inter = &config->interface[i];
		cout << "\tNumber of alternate settings: " << inter->num_altsetting <<endl;
		for (int j = 0; j<inter->num_altsetting; j++) {
			interdesc = &inter->altsetting[j];
			cout << "\tInterface Number: " << (int)interdesc->bInterfaceNumber <<endl;
			cout << "\tNumber of endpoints: " << (int)interdesc->bNumEndpoints <<endl<<endl;
			for (int k = 0; k<(int)interdesc->bNumEndpoints; k++) {
				epdesc = &interdesc->endpoint[k];
				cout << "\t\tDescriptor Type: " << (int)epdesc->bDescriptorType <<endl;
				cout << "\t\tEP Address: " << (int)epdesc->bEndpointAddress <<endl;
				if (epdesc->bEndpointAddress & LIBUSB_ENDPOINT_IN) {
					cout << "\t\tEP Type: IN" << endl << endl;
				}
				else {
					cout << "\t\tEP Type: OUT" << endl << endl;
				}
			}
		}
	}
	cout << endl << endl << endl;
	libusb_free_config_descriptor(config);
}
void add_adapter(libusb_device **devs,ssize_t count,uint8_t *endpoint_out,uint8_t *endpoint_in) {
	libusb_device *adapter = get_device(devs,count, GAMECUBE_VID, GAMECUBE_PID);
	if (adapter != nullptr) {
		libusb_config_descriptor *config;
		libusb_get_config_descriptor(adapter, 0, &config);
		const libusb_interface *inter;
		const libusb_interface_descriptor *interdesc;
		const libusb_endpoint_descriptor *epdesc;
		for (int i = 0; i < (int)config->bNumInterfaces; i++) {
			inter = &config->interface[i];
			for (int j = 0; j < inter->num_altsetting; j++) {
				interdesc = &inter->altsetting[j];
				for (int k = 0; k < (int)interdesc->bNumEndpoints; k++) {
					epdesc = &interdesc->endpoint[k];
					if (epdesc->bEndpointAddress & LIBUSB_ENDPOINT_IN) {
						*endpoint_in = epdesc->bEndpointAddress;
					}
					else {
						*endpoint_out = epdesc->bEndpointAddress;
					}
				}
			}
		}
	}
}
libusb_device* get_device(libusb_device **list, ssize_t count, uint16_t vid, uint16_t pid) {
	libusb_device *result = nullptr;
	for (ssize_t i=0; result == nullptr && i < count; i++) {
		libusb_device_descriptor desc;
		int return_value = libusb_get_device_descriptor(list[i], &desc);
		if (return_value < 0) {
			cout << "Failed to get descriptor of this device!" << endl;
			return nullptr;
		}
		else if (vid == desc.idVendor && pid == desc.idProduct) {
			result = list[i];
		}
	}
	return result;
}