#ifndef _CtrlLib_Lang_h_
#define _CtrlLib_Lang_h_

class LNGCtrl : public СтатичПрям
{
public:
	virtual Значение дайДанные() const;
	virtual void  устДанные(const Значение& v);

	virtual Размер  дайМинРазм() const;

private:
	СписокБроса hi;
	СписокБроса lo;

	void LoadLo();
	void Hi();
	void Lo();

public:
	typedef LNGCtrl ИМЯ_КЛАССА;

	LNGCtrl& DropFocus(bool b = true)    { hi.DropFocus(b); lo.DropFocus(b); return *this; }
	LNGCtrl& NoWantFocus()               { hi.NoWantFocus(); lo.NoWantFocus(); return *this; }

	LNGCtrl();
	virtual ~LNGCtrl();
};

#endif
