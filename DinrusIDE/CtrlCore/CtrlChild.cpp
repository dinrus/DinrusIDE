#include "CtrlCore.h"

namespace РНЦП {

#define LLOG(x)   // DLOG(x)

bool Ктрл::IsDHCtrl() const {
	return dynamic_cast<const DHCtrl *>(this);
}

void Ктрл::добавьОтпрыск(Ктрл *q, Ктрл *p)
{
	ЗамкниГип __;
	ПРОВЕРЬ(q);
	LLOG("добавь " << РНЦП::Имя(q) << " to: " << Имя());
	if(p == q) return;
	bool updaterect = true;
	if(q->parent) {
		ПРОВЕРЬ(!q->inframe);
		if(q->parent == this) {
			удалиОтпрыск0(q);
			updaterect = false;
		}
		else
			q->parent->удалиОтпрыск(q);
	}
	q->parent = this;
	if(p) {
		ПРОВЕРЬ(p->parent == this);
		q->prev = p;
		q->next = p->next;
		if(p == lastchild)
			lastchild = q;
		else
			p->next->prev = q;
		p->next = q;
	}
	else
		if(firstchild) {
			q->prev = NULL;
			q->next = firstchild;
			firstchild->prev = q;
			firstchild = q;
		}
		else {
			ПРОВЕРЬ(lastchild == NULL);
			firstchild = lastchild = q;
			q->prev = q->next = NULL;
		}
	q->CancelModeDeep();
	if(updaterect)
		q->обновиПрям();
	отпрыскДобавлен(q);
	q->ParentChange();
	if(updaterect && дайТопКтрл()->открыт())
		q->StateH(OPEN);
}

void Ктрл::добавьОтпрыск(Ктрл *child)
{
	добавьОтпрыск(child, lastchild);
}

void Ктрл::добавьОтпрыскПеред(Ктрл *child, Ктрл *insbefore)
{
	if(insbefore)
		добавьОтпрыск(child, insbefore->prev);
	else
		добавьОтпрыск(child);
}

void  Ктрл::удалиОтпрыск0(Ктрл *q)
{
	ЗамкниГип __;
	отпрыскУдалён(q);
	q->DoRemove();
	q->parent = NULL;
	if(q == firstchild)
		firstchild = firstchild->next;
	if(q == lastchild)
		lastchild = lastchild->prev;
	if(q->prev)
		q->prev->next = q->next;
	if(q->next)
		q->next->prev = q->prev;
	q->next = q->prev = NULL;
}

void  Ктрл::удалиОтпрыск(Ктрл *q)
{
	ЗамкниГип __;
	if(q->parent != this) return;
	q->освежиФрейм();
	удалиОтпрыск0(q);
	q->ParentChange();
	if(дайТопКтрл()->открыт())
		q->StateH(CLOSE);
}

void  Ктрл::удали()
{
	ЗамкниГип __;
	if(parent)
		parent->удалиОтпрыск(this);
}

int Ктрл::GetChildIndex(const Ктрл *child) const
{
	ЗамкниГип __;
	int i = 0;
	for (Ктрл *c = дайПервОтпрыск(); c; c = c->дайСледщ()) {
		if(c == child) return i;
		i++;
	}
	return -1;
}

int Ктрл::GetChildCount() const
{
	ЗамкниГип __;
	int n = 0;
	for (Ктрл *c = дайПервОтпрыск(); c; c = c->дайСледщ())
		n++;
	return n;
}

Ктрл * Ктрл::GetIndexChild(int ii) const
{
	ЗамкниГип __;
	Ктрл *c = дайПервОтпрыск();
	for(int i = 0; i < ii && c; i++)
		c = c->дайСледщ();
	return c;
}

int Ктрл::GetViewChildIndex(const Ктрл *child) const
{
	ЗамкниГип __;
	int i = 0;
	for (Ктрл *c = дайПервОтпрыск(); c; c = c->дайСледщ())
		if(!c->InFrame()) {
			if(c == child) return i;
			i++;
		}
	return -1;
}

int Ктрл::GetViewChildCount() const
{
	ЗамкниГип __;
	int n = 0;
	for (Ктрл *c = дайПервОтпрыск(); c; c = c->дайСледщ())
		if(!c->InFrame())
			n++;
	return n;
}

Ктрл * Ктрл::GetViewIndexChild(int ii) const
{
	ЗамкниГип __;
	int i = 0;
	for (Ктрл *c = дайПервОтпрыск(); c; c = c->дайСледщ())
		if(!c->InFrame()) {
			if(i == ii)
				return c;
			i++;
		}
	return NULL;
}

bool Ктрл::HasChild(Ктрл *q) const
{
	ЗамкниГип __;
	return q && q->отпрыск_ли() && q->parent == this;
}

bool Ктрл::HasChildDeep(Ктрл *q) const
{
	ЗамкниГип __;
	while(q && q->отпрыск_ли()) {
		if(q->parent == this) return true;
		q = q->parent;
	}
	return false;
}

static bool IterateFocusFw(Ктрл *ctrl, bool noframe, bool init, bool all)
{
	LLOG("IterateFocusFw(" << РНЦП::Имя(ctrl) << ")");
	while(ctrl) {
		if(ctrl->открыт() && ctrl->видим_ли() && ctrl->включен_ли()) {
			if(!(noframe && ctrl->InFrame())) {
				if(all) {
					ctrl->устФокус();
					return true;
				}
				if((!init || ctrl->IsInitFocus()) && ctrl->SetWantFocus())
					return true;
			}
			if(IterateFocusFw(ctrl->дайПервОтпрыск(), noframe, init, all))
				return true;
		}
		ctrl = ctrl->дайСледщ();
	}
	return false;
}

bool Ктрл::IterateFocusForward(Ктрл *ctrl, Ктрл *top, bool noframe, bool init, bool all)
{
	ЗамкниГип __;
	LLOG("IterateFocusForward(" << РНЦП::Имя(ctrl) << ", top " << РНЦП::Имя(top) << ", noframe " << noframe << ", init " << init << ")");
	if(!ctrl) return false;
	if(IterateFocusFw(ctrl->дайПервОтпрыск(), noframe, init, all))
		return true;
	if(ctrl->дайСледщ() && IterateFocusFw(ctrl->дайСледщ(), noframe, init, all))
		return true;
	while(ctrl->дайРодителя() != top && (ctrl = ctrl->дайРодителя()) != NULL)
		if(IterateFocusFw(ctrl->дайСледщ(), noframe, init, all))
			return true;
	return false;
}

static bool IterateFocusBw(Ктрл *ctrl, bool noframe, bool all)
{
	while(ctrl) {
		if(ctrl->открыт() && ctrl->видим_ли() && ctrl->включен_ли()) {
			if(IterateFocusBw(ctrl->GetLastChild(), noframe, all))
				return true;
			if(!(noframe && ctrl->InFrame())) {
				if(all) {
					ctrl->устФокус();
					return true;
				}
				if(ctrl->SetWantFocus())
					return true;
			}
		}
		ctrl = ctrl->дайПредш();
	}
	return false;
}

bool Ктрл::IterateFocusBackward(Ктрл *ctrl, Ктрл *top, bool noframe, bool all)
{
	ЗамкниГип __;
	if(!ctrl || ctrl == top) return false;
	if(IterateFocusBw(ctrl->дайПредш(), noframe, all))
		return true;
	while(ctrl->дайРодителя() != top) {
		ctrl = ctrl->дайРодителя();
		if(ctrl->SetWantFocus())
			return true;
		if(IterateFocusBw(ctrl->дайПредш(), noframe, all))
			return true;
	}
	return false;
}

Ктрл *Ктрл::дайТопКтрл()
{
	ЗамкниГип __;
	Ктрл *q = this;
	while(q->parent)
		q = q->parent;
	return q;
}

const Ктрл *Ктрл::дайТопКтрл() const      { return const_cast<Ктрл *>(this)->дайТопКтрл(); }
const Ктрл *Ктрл::дайВладельца() const        { return const_cast<Ктрл *>(this)->дайВладельца(); }
Ктрл       *Ктрл::GetTopCtrlOwner()       { return дайТопКтрл()->дайВладельца(); }
const Ктрл *Ктрл::GetTopCtrlOwner() const { return дайТопКтрл()->дайВладельца(); }

Ктрл       *Ктрл::GetOwnerCtrl()          { ЗамкниГип __; return !отпрыск_ли() && top ? top->owner : NULL; }
const Ктрл *Ктрл::GetOwnerCtrl() const    { return const_cast<Ктрл *>(this)->GetOwnerCtrl(); }

ТопОкно *Ктрл::дайТопОкно()
{
	ЗамкниГип __;
	Ктрл *q = this;
	while(q) {
		q = q->дайТопКтрл();
		ТопОкно *w = dynamic_cast<ТопОкно *>(q);
		if(w) return w;
		q = q->дайВладельца();
	}
	return NULL;
}

const ТопОкно *Ктрл::дайТопОкно() const
{
	return const_cast<Ктрл *>(this)->дайТопОкно();
}

ТопОкно *Ктрл::дайГлавнОкно()
{
	ЗамкниГип __;
	Ктрл *q = дайТопКтрл();
	for(;;) {
		Ктрл *w = q->дайВладельца();
		if(!w)
			return dynamic_cast<ТопОкно *>(q);
		q = w;
	}
}

const ТопОкно *Ктрл::дайГлавнОкно() const
{
	return const_cast<Ктрл *>(this)->дайГлавнОкно();
}

}
