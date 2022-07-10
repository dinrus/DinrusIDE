#ifndef _ide_Android_Devices_h_
#define _ide_Android_Devices_h_

#include <Core/Core.h>

namespace РНЦП {

class AndroidDevice : public Движимое<AndroidDevice> {
public:
	Ткст GetSerial() const { return serial; }
	Ткст GetState() const  { return state; }
	Ткст GetUsb() const    { return usb; }
	Ткст GetModel() const  { return model; }
	
	void SetSerial(const Ткст& serial) { this->serial = serial; }
	void SetState(const Ткст& state)   { this->state = state; }
	void SetUsb(const Ткст& usb)       { this->usb = usb; }
	void SetModel(const Ткст& model)   { this->model = model; }
	
public:
	bool IsEmulator() const       { return !usb.пустой(); }
	bool IsPhysicalDevice() const { return !IsEmulator(); }
	
private:
	Ткст serial;
	Ткст state;
	Ткст usb;
	Ткст model;
};

class AndroidVirtualDevice : public Движимое<AndroidVirtualDevice> {
public:
	Ткст дайИмя() const       { return имя; }
	Ткст GetDeviceType() const { return deviceType; }
	Ткст дайПуть() const       { return path; }
	Ткст GetTarget() const     { return target; }
	Ткст GetAbi() const        { return abi; }
	Размер   дайРазм() const       { return size; }
	
	void SetName(const Ткст& имя)             { this->имя = имя; }
	void SetDeviceType(const Ткст& deviceType) { this->deviceType = deviceType; }
	void SetPath(const Ткст& path)             { this->path = path; }
	void SetTarget(const Ткст& target)         { this->target = target; }
	void SetAbi(const Ткст& abi)               { this->abi = abi; }
	void устРазм(const Размер& size)               { this->size = size; }
	
private:
	Ткст имя;
	Ткст deviceType;
	Ткст path;
	Ткст target;
	Ткст abi;
	Размер   size;
};

}

#endif
