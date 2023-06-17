#include <DinrusPro/DinrusCore.h>

/* Faster, but consuming more memory....
PteBase::Prec *PteBase::добавьУк()
{
	атомнИнк(prec->n);
	return prec;
}

проц PteBase::отпустиУк(Prec *prec)
{
	if(prec && атомнДек(prec->n) == 0)
		delete prec;
}

PteBase::PteBase()
{
	prec = new Prec;
	prec->n = 1;
	prec->укз = this;
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
		prec->укз = this;
	}
	return const_cast<Prec *>(prec);
}

проц PteBase::отпустиУк(Prec *prec)
{
	Стопор::Замок __(sPteLock);
	if(prec && --prec->n == 0) {
		if(prec->укз)
			prec->укз->prec = NULL;
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
		prec->укз = NULL;
}

проц УкОснова::отпусти()
{
	PteBase::отпустиУк(prec);
}

проц УкОснова::уст(PteBase *p)
{
	prec = p ? p->добавьУк() : NULL;
}

проц УкОснова::присвой(PteBase *p)
{
	отпусти();
	уст(p);
}

УкОснова::~УкОснова()
{
	отпусти();
}
