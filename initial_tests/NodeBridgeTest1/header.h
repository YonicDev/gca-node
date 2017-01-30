#pragma once;

const uint16_t GAMECUBE_VID = 0x057E;
const uint16_t GAMECUBE_PID = 0x0337;

char* UintToString(uint16_t number);
NAN_METHOD(AddPointer);
NAN_METHOD(GetGameCubeIDs);
