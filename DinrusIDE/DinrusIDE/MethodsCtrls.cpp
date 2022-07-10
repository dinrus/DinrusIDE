#include "DinrusIDE.h"

void TextOption::устДанные(const Значение& data)
{
	Ткст s = data;
	уст(!(пусто_ли(s) || s == "0"));
}

Значение TextOption::дайДанные() const
{
	return дай() ? "1" : "0";
}

void  TextSwitch::устДанные(const Значение& data)
{
	Ткст s = data;
	Switch::устДанные(atoi((Ткст)data));
}

Значение TextSwitch::дайДанные() const
{
	return какТкст(Switch::дайДанные());
}
