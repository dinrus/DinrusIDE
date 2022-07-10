#ifndef _Geom_Ctrl_CoordCtrl_h_
#define _Geom_Ctrl_CoordCtrl_h_

namespace РНЦП {

class EditDegree : public EditValue<double, ConvertDegree>
{
public:
	typedef EditDegree ИМЯ_КЛАССА;
	EditDegree();

	virtual bool  Ключ(dword ключ, int repcnt);

private:
	void          OnInc();
	void          OnDec();

private:
	СпинКнопки   spin;
};

}

#endif
