#include "Core.h"

namespace РНЦПДинрус {

/* Faster, but consuming more memory....
PteBase::Prec *PteBase::добавьУк()
{
	атомнИнк(prec->n);
	return prec;
}

void PteBase::отпустиУк(Prec *prec)
{
	if(prec && атомнДек(prec->n) == 0)
		delete prec;
}

PteBase::PteBase()
{
	prec = new Prec;
	prec->n = 1;
	prec->ptr = this;
}
*/

static СтатическийСтопор sPteLock;

PteBase::Prec *PteBase::добавьУк()
{
	Стопор::Замок __(sPteLock);
	if(prec)
		++prec->n;
	else {
		prec = tiny_new<Prec>();
		prec->n = 1;
		prec->ptr = this;
	}
	return const_cast<Prec *>(prec);
}

void PteBase::отпустиУк(Prec *prec)
{
	Стопор::Замок __(sPteLock);
	if(prec && --prec->n == 0) {
		if(prec->ptr)
			prec->ptr->prec = NULL;
		tiny_delete(prec);
	}
}

PteBase::PteBase()
{
	prec = NULL;
}

PteBase::~PteBase()
{
	Стопор::Замок __(sPteLock);
	if(prec)
		prec->ptr = NULL;
}

void УкОснова::отпусти()
{
	PteBase::отпустиУк(prec);
}

void УкОснова::уст(PteBase *p)
{
	prec = p ? p->добавьУк() : NULL;
}

void УкОснова::присвой(PteBase *p)
{
	отпусти();
	уст(p);
}

УкОснова::~УкОснова()
{
	отпусти();
}

}
