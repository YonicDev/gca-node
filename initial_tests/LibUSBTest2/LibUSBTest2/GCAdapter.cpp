#include <iostream>
#include <mutex>
#include <atomic>

#include "GCAdapter.h"

namespace GCANode {
	libusb_device_handle *device_handle = nullptr;
	libusb_context *context = nullptr;

	uint8_t controller_payload[37];
	uint8_t controller_payload_swap[37];

	uint8_t endpoint_out = 0;
	uint8_t endpoint_in = 0;

	int controller_payload_size = 0;

	void Setup() {
		libusb_init(&context);
		libusb_device **list;
		ssize_t count = libusb_get_device_list(context, &list);
		bool case_test = false;
		for (int i = 0; i < count && !case_test; i++) {
			libusb_device *device = list[i];
			case_test = IsAccessible(device);
			if (case_test) {
				AddAdapter(device);
			}
		}
		libusb_free_device_list(list, 1);
	}
	bool IsAccessible(libusb_device *dev) {
		int return_value=0,kernel_value=0;
		libusb_device_descriptor descriptor;
		return_value = libusb_get_device_descriptor(dev, &descriptor);
		if (return_value < 0) {
			std::cout << "Error getting descriptor of USB device. Error code: " << return_value << std::endl;
			return false;
		}
		
		if (descriptor.idVendor == GAMECUBE_VID && descriptor.idProduct == GAMECUBE_PID) {
			std::cout << "Found GameCube Adapter" << std::endl;
		}
		else {
			return false;
		}
		return_value = libusb_open(dev, &device_handle);
		switch (return_value) {
			case 0:
				std::cout << "This adapter seems reachable. Trying to reach..." << std::endl;
				break;
			case LIBUSB_ERROR_ACCESS:
				std::cout << "LIBUSB_ERROR_ACCESS: gca-node does not have access to this adapter." << std::endl;
				return false;
				break;
			default:
				std::cout << "gca-node couldn't open this adapter. Error code: " << return_value << std::endl;
				return false;
				break;
		}
		return_value = libusb_kernel_driver_active(device_handle, 0);
		if (return_value == 1) {
			kernel_value = libusb_detach_kernel_driver(device_handle, 0);
			if (kernel_value != 0) {
				std::cout << "gca-node can't attach the kernel of this adapter. Error code:" << return_value << std::endl;
				return false;
			}
		}
		if (return_value == 0 || kernel_value == 0) {
			return_value = libusb_claim_interface(device_handle, 0);
			if (return_value != 0) {
				std::cout << "gca-node couldn't claim interface 0 of adapter. Error code:" << return_value << std::endl;
			}
			return return_value == 0;
		}
		return false;
	}
	void AddAdapter(libusb_device *dev) {
		libusb_config_descriptor *config = nullptr;
		libusb_get_config_descriptor(dev, 0, &config);
		for (uint8_t iface=0; iface < config->bNumInterfaces; iface++) {
			const libusb_interface *interfaceContainer = &config->interface[iface];
			for (int i = 0; i < interfaceContainer->num_altsetting; i++) {
				const libusb_interface_descriptor *idesc = &interfaceContainer->altsetting[i];
				for (uint8_t epoint = 0; epoint < idesc->bNumEndpoints; epoint++) {
					const libusb_endpoint_descriptor *endpoint = &idesc->endpoint[epoint];
					if (endpoint->bEndpointAddress & LIBUSB_ENDPOINT_IN) {
						endpoint_in = endpoint->bEndpointAddress;
					}
					else {
						endpoint_out = endpoint->bEndpointAddress;
					}
				}
			}
		}
	}
	int Load() {
		int payload_size = 0, return_value = 0, tmp = 0;
		uint8_t payload = 0x13;
		return_value = libusb_interrupt_transfer(device_handle, endpoint_out, &payload, sizeof(payload), &tmp, 16);
		if (return_value < 0) {
			std::cout << "Error on transfer: " << return_value << std::endl;
		}
		else {
			std::cout << "We got something from the adapter!" << std::endl;
		}
		return return_value;
	}
}