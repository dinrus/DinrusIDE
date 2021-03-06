#include "Core.h"

#ifdef PLATFORM_WIN32

namespace РНЦПДинрус {

УзелСети::УзелСети()
{
	memset(&net, 0, sizeof(NETRESOURCE));
}

УзелСети& УзелСети::operator=(const УзелСети& s)
{
	net = s.net;
	local = clone(s.local);
	remote = clone(s.remote);
	comment = clone(s.comment);
	provider = clone(s.provider);
	имя = s.имя;
	path = s.path;
	SetPtrs();
	return *this;
}

Ткст DosИниtCaps(const char *имя)
{
	for(const char *s = имя; *s; s++)
		if(буква_ли(*s) && заг_ли(*s))
			return имя;
	return иницШапки(имя);
}

void УзелСети::SetPtrs()
{
	auto устУк = [](Вектор<char16>& s, char16 *& ptr) { if(ptr) ptr = s; };
	устУк(local, net.lpLocalName);
	устУк(remote, net.lpRemoteName);
	устУк(comment, net.lpComment);
	устУк(provider, net.lpProvider);
}

/*
void УзелСети::сериализуй(Поток& s)
{
	s % net.dwScope % net.dwType % net.dwDisplayType % net.dwUsage;
	s % local % remote % comment % provider % имя % path;
	SetPtrs();
}
*/

Массив<УзелСети> УзелСети::Enum() const
{
	HANDLE hEnum;
	if(::WNetOpenEnum(RESOURCE_GLOBALNET, RESOURCETYPE_DISK, 0, (NETRESOURCE *)&net, &hEnum))
		return Массив<УзелСети>();
	return Enum0(hEnum);
}

Массив<УзелСети> УзелСети::EnumRoot()
{
	HANDLE hEnum;
	if(::WNetOpenEnum(RESOURCE_CONTEXT, RESOURCETYPE_DISK, 0, NULL, &hEnum))
		return Массив<УзелСети>();
	return Enum0(hEnum);
}

Массив<УзелСети> УзелСети::EnumRemembered()
{
	HANDLE hEnum;
	if(::WNetOpenEnum(RESOURCE_REMEMBERED, RESOURCETYPE_DISK, 0, NULL, &hEnum))
		return Массив<УзелСети>();
	return Enum0(hEnum);
}

Массив<УзелСети> УзелСети::Enum0(HANDLE hEnum)
{
	Массив<УзелСети> r;
	DWORD cEntries = (DWORD)-1, cbBuffer = 0x4000;
	Буфер<NETRESOURCEW> lpnr(cbBuffer);
	while(::WNetEnumResourceW(hEnum, &cEntries, lpnr, &cbBuffer) == 0) {
		for(int i = 0; i < (int)cEntries; i++) {
			NETRESOURCEW& sn = lpnr[i];
			const WCHAR *s = sn.lpRemoteName;
			УзелСети& nn = r.добавь();
			NETRESOURCEW& n = nn.net;
			n = sn;
			auto get = [](const char16 *s) { Вектор<char16> x; while(s && *s) x.добавь(*s++); x.добавь(0); return x; };
			nn.local = get(n.lpLocalName);
			nn.remote = get(n.lpRemoteName);
			nn.comment = get(n.lpComment);
			nn.provider = get(n.lpProvider);
			nn.SetPtrs();
			if(s) {
				if(s[0] == '\\' && s[1] == '\\')
					nn.имя = DosИниtCaps(дайИмяф(изСисНабсимаШ(s)));
				else
					nn.имя = изСисНабсимаШ(s);
			}
			if(n.lpComment && *n.lpComment) {
				if(nn.имя.дайСчёт())
					nn.имя = Ткст().конкат() << изСисНабсимаШ(n.lpComment)
					                         << " (" << nn.имя << ")";
				else
					nn.имя = изСисНабсимаШ(n.lpComment);
			}
			if(!(n.dwUsage & RESOURCEUSAGE_CONTAINER))
				nn.path = изСисНабсимаШ(n.lpRemoteName);
		}
	}
	::WNetCloseEnum(hEnum);
	return r;
}

int УзелСети::GetDisplayType() const
{
	switch(net.dwDisplayType) {
	case RESOURCEDISPLAYTYPE_NETWORK:
		return NETWORK;
	case RESOURCEDISPLAYTYPE_DOMAIN:
	case RESOURCEDISPLAYTYPE_GROUP:
	case RESOURCEDISPLAYTYPE_TREE:
	case /*RESOURCEDISPLAYTYPE_NDSCONTAINER*/0x0000000B:
		return GROUP;
	case RESOURCEDISPLAYTYPE_SERVER:
		return SERVER;
	case RESOURCEDISPLAYTYPE_SHARE:
	case RESOURCEDISPLAYTYPE_SHAREADMIN:
	case RESOURCEDISPLAYTYPE_DIRECTORY:
		return SHARE;
	}
	return UNKNOWN;
}

}

#endif
