#include <nan.h>
#include "header.h"
#include <sstream>

using namespace Nan;

NAN_METHOD(AddPointer) {
    double x = 0, y = 0;
    double *p, *q;
    if(!info[0]->IsNumber() || !info[1]->IsNumber()) {
        ThrowError("Argument must be a number.");
    }
    p = &x; q = &y;
    *p = info[0]->NumberValue(); *q = info[1]->NumberValue();
    auto sum = New<v8::Number>(x + y);
    info.GetReturnValue().Set(sum);
}

NAN_METHOD(GetGameCubeIDs) {
	std::stringstream result;
	result << GAMECUBE_VID << ", " << GAMECUBE_PID;
    auto ids = New<v8::String>(result.str()).ToLocalChecked();
    info.GetReturnValue().Set(ids);
}

NAN_MODULE_INIT(Bridge) {
    NAN_EXPORT(target,AddPointer);
    NAN_EXPORT(target,GetGameCubeIDs);
}

NODE_MODULE(BridgeTest_1,Bridge)
