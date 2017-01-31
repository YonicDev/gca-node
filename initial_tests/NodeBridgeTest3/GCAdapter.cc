#include <iostream>
#include <sstream>
#include <mutex>
#include <atomic>
#include <nan.h>
#include <bitset>
#include "GCAdapter.h"

using namespace std;

libusb_device_handle *device_handle = nullptr;
libusb_context *context = nullptr;

uint8_t controller_payload[37];
uint8_t controller_payload_swap[37];

uint8_t endpoint_out = 0;
uint8_t endpoint_in = 0;

int controller_payload_size = 0;

NAN_METHOD(Setup) {
	auto return_value = Nan::New<v8::Number>(0);
    if(info.Length()>0) {
        Nan::ThrowError("Load() must not have arguments");
        return;
    }
	libusb_init(&context);
	libusb_device **list;
	ssize_t count = libusb_get_device_list(context, &list);
	bool case_test = false;
	for (int i = 0; i < count && !case_test; i++) {
		libusb_device *device = list[i];
		case_test = IsAccessible(device);
		if (case_test) {
			return_value = Nan::New<v8::Number>(AddAdapter(device));
		}
	}
	libusb_free_device_list(list, 1);
    info.GetReturnValue().Set(return_value);
}
bool IsAccessible(libusb_device *dev) {
	int return_value=0,kernel_value=0;
	libusb_device_descriptor descriptor;
	return_value = libusb_get_device_descriptor(dev, &descriptor);
	if (return_value < 0) {
		cout << "Error getting descriptor of USB device. Error code: " << return_value << endl;
		return false;
	}

	if (descriptor.idVendor == GAMECUBE_VID && descriptor.idProduct == GAMECUBE_PID) {
		cout << "Found GameCube Adapter" << endl;
	}
	else {
		return false;
	}
	return_value = libusb_open(dev, &device_handle);
	switch (return_value) {
		case 0:
			cout << "This adapter seems reachable. Trying to reach..." << endl;
			break;
		case LIBUSB_ERROR_ACCESS:
			cout << "LIBUSB_ERROR_ACCESS: gca-node does not have access to this adapter." << endl;
			return false;
			break;
		default:
			cout << "gca-node couldn't open this adapter. Error code: " << return_value << endl;
			return false;
			break;
	}
	return_value = libusb_kernel_driver_active(device_handle, 0);
	if (return_value == 1) {
		kernel_value = libusb_detach_kernel_driver(device_handle, 0);
		if (kernel_value != 0) {
			cout << "gca-node can't attach the kernel of this adapter. Error code:" << return_value << endl;
			return false;
		}
	}
	if (return_value == 0 || kernel_value == 0) {
		return_value = libusb_claim_interface(device_handle, 0);
		if (return_value != 0) {
			cout << "gca-node couldn't claim interface 0 of adapter. Error code:" << return_value << endl;
		}
		return return_value == 0;
	}
	return false;
}
int AddAdapter(libusb_device *dev) {
    int endpoint_number = 0;
	libusb_config_descriptor *config = nullptr;
	libusb_get_config_descriptor(dev, 0, &config);
	for (uint8_t iface=0; iface < config->bNumInterfaces; iface++) {
		const libusb_interface *interfaceContainer = &config->interface[iface];
		for (int i = 0; i < interfaceContainer->num_altsetting; i++) {
			const libusb_interface_descriptor *idesc = &interfaceContainer->altsetting[i];
			for (uint8_t epoint = 0; epoint < idesc->bNumEndpoints; epoint++) {
				const libusb_endpoint_descriptor *endpoint = &idesc->endpoint[epoint];
                endpoint_number++;
				if (endpoint->bEndpointAddress & LIBUSB_ENDPOINT_IN) {
					endpoint_in = endpoint->bEndpointAddress;
				}
				else {
					endpoint_out = endpoint->bEndpointAddress;
				}
			}
		}
	}
    return endpoint_number;
}
NAN_METHOD(Load) {
    if(info.Length()>0) {
        Nan::ThrowError("Load() must not have arguments");
        return;
    }
	int payload_size = 0, tmp = 0;
	uint8_t payload = 0x13;
	auto return_value = Nan::New<v8::Number>(libusb_interrupt_transfer(device_handle, endpoint_out, &payload, sizeof(payload), &tmp, 16));
	info.GetReturnValue().Set(return_value);
}
NAN_METHOD(Read) {
	if(info.Length()>0) {
		Nan::ThrowError("Read() must not have arguments");
        return;
	}
	Sleep(200);
	int code = libusb_interrupt_transfer(device_handle, endpoint_in, controller_payload_swap, sizeof(controller_payload_swap), &controller_payload_size, 16);
	stringstream return_value;
	if (code == 0) {
		int i;
		return_value << "[";
		for (i = 0; i < 9; i++) {
			bitset<8> bits(controller_payload_swap[i]);
			return_value << bits << ",";
		}
		bitset<8> bits(controller_payload_swap[i]);
		return_value << bits << "]";
		auto returner = Nan::New<v8::String>(return_value.str()).ToLocalChecked();
		info.GetReturnValue().Set(returner);
	}
	else {
		return_value << "I couldn't get anything. Error: " << code;
		auto error_message = Nan::New<v8::String>(return_value.str()).ToLocalChecked();
		info.GetReturnValue().Set(error_message);
	}
}
NAN_MODULE_INIT(Bridge) {
    NAN_EXPORT(target,Load);
    NAN_EXPORT(target,Setup);
	NAN_EXPORT(target,Read);
}

NODE_MODULE(BridgeTest_2,Bridge)
