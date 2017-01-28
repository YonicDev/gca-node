#include "libusb.h"

#pragma once

namespace GCANode {

	void AddAdapter(libusb_device *dev);
	int Load();
	void Setup();
	bool IsAccessible(libusb_device *dev);

	const uint16_t GAMECUBE_VID = 0x057E;
	const uint16_t GAMECUBE_PID = 0x0337;
}