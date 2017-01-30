#include "libusb.h"

#include <nan.h>

#pragma once

namespace GCANode {

	void AddAdapter(libusb_device *dev);
	NAN_METHOD(Load);
	NAN_METHOD(Setup);
	bool IsAccessible(libusb_device *dev);

	const uint16_t GAMECUBE_VID = 0x057E;
	const uint16_t GAMECUBE_PID = 0x0337;
}
