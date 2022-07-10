#include "CtrlLib.h"

namespace РНЦП {

struct PromptDlgWnd__ : ТопОкно {
	bool    esc;
	Кнопка *b;

	virtual bool горячаяКлав(dword ключ) {
		if(ТопОкно::горячаяКлав(ключ))
			return true;
		if(IsAlpha(ключ))
			return ТопОкно::горячаяКлав(K_ALT_A + взаг((int)ключ) - 'A');
		if(ключ == K_ESCAPE && esc)
			b->PseudoPush();
		return false;
	}
};

static void sAdd(Ктрл& dlg, int fcy, int bcy, int& bx, int bcx, int gap, Кнопка& b, const char *button, const Рисунок& img)
{
	if(button) {
		dlg << b.BottomPos(fcy, bcy).LeftPos(bx, bcx);
		b.устНадпись(button);
		if(!пусто_ли(img))
			b.устРисунок(img);
		bx += gap + bcx;
	}
}

void sExecutePrompt(PromptDlgWnd__ *dlg, int *result)
{
	dlg->открой();
	Вектор<Ктрл *> wins = Ктрл::дайТопОкна();
	for(int i = 0; i < wins.дайСчёт(); i++) {
		ТопОкно *w = dynamic_cast<ТопОкно *>(wins[i]);
		if(w && w->наиверхнее_ли()) {
			dlg->наиверхнее();
			break;
		}
	}
	*result = dlg->пускПрилМодально();
	dlg->закрой();
}
                        
RedirectPromptFn RedirectPrompt;

void RedirectPrompts(RedirectPromptFn r)
{
	RedirectPrompt = r;
}

ВекторМап<Ткст, int> dsa_history;

void ClearPromptOptHistory()
{
	ЗамкниГип __;
	dsa_history.очисть();
}

void ClearPromptOptHistory(Врата<Ткст> filter)
{
	ЗамкниГип __;
	for(int i = 0; i < dsa_history.дайСчёт(); i++)
		if(filter(dsa_history.дайКлюч(i)))
			dsa_history.отлинкуй(i);
	dsa_history.смети();
}

void SerializePromptOptHistory(Поток& s)
{
	int version = 0;
	s / version;
	s % dsa_history;
}

int Prompt(int dontshowagain, const char *dsa_id_, int beep,
           Событие<const Ткст&> WhenLink,
           const char *title, const Рисунок& iconbmp, const char *qtf, bool okcancel,
           const char *button1, const char *button2, const char *button3,
		   int cx,
		   Рисунок im1, Рисунок im2, Рисунок im3)
{
	if(RedirectPrompt)
		return (*RedirectPrompt)(WhenLink, title, iconbmp, qtf, okcancel,
                                 button1, button2, button3,
                                 cx, im1, im2, im3);
	Ткст dsa_id;
	if(dontshowagain) {
		dsa_id = dsa_id_;
		if(пусто_ли(dsa_id)) {
			Ткст body = title;
			body << "\1" << qtf;
			if(button1) body << "\1" << button1;
			if(button2) body << "\1" << button2;
			if(button3) body << "\1" << button3;
			dsa_id = SHA256String(body);
		}
		ЗамкниГип __;
		int q = dsa_history.найди(dsa_id);
		if(q >= 0)
			return dsa_history[q];
	}
	switch(beep) {
	case BEEP_INFORMATION:
		бипИнформация();
		break;
	case BEEP_EXCLAMATION:
		бипВосклицание();
		break;
	case BEEP_QUESTION:
		бипВопрос();
		break;
	case BEEP_ERROR:
		бипОшибка();
		break;
	}
	int fcy = Draw::GetStdFontCy();
	войдиВСтопорГип(); // Ктрл derived classes can only be initialized with ЗамкниГип
	PromptDlgWnd__ dlg;
	RichTextCtrl qtfctrl;
	Иконка         icon;
	Опция       dsa;
	dsa.устНадпись(t_("Больше не показывать"));
	qtfctrl.WhenLink = WhenLink;
	icon.устРисунок(iconbmp);
	Кнопка b1, b2, b3;
	qtfctrl.SetQTF(Ткст("[g ") + qtf, GetRichTextStdScreenZoom());
	int bcy = Ктрл::VertLayoutZoom(24);
	int bcx = Ктрл::HorzLayoutZoom(72);
	if(button1)
		bcx = max(2 * fcy + дайРазмТекста(button1, Draw::GetStdFont()).cx, bcx);
	if(button2)
		bcx = max(2 * fcy + дайРазмТекста(button2, Draw::GetStdFont()).cx, bcx);
	if(button3)
		bcx = max(2 * fcy + дайРазмТекста(button3, Draw::GetStdFont()).cx, bcx);
	Размер bsz = icon.дайСтдРазм();
	if(cx == 0) {
		cx = qtfctrl.дайШирину();
		if(!cx)
			cx = 350;
		if(dontshowagain)
			cx = max(cx, dsa.дайМинРазм().cx);
		cx += 2 * fcy;
		if(bsz.cx)
			cx += bsz.cx + fcy;
	}
	int nbtn = !!button1 + !!button2 + !!button3;
	dlg.esc = okcancel && nbtn == 1;
	cx = min(Zx(520), max(nbtn * bcx + (1 + nbtn) * fcy, cx));
	int qcx = cx - 2 * fcy;
	if(bsz.cx)
		qcx -= bsz.cx + fcy;
	int ccy = qtfctrl.дайВысоту(qcx);
	int qcy = min(Zy(400), ccy);
	if(qcy <= ccy) {
		qcx += размПромотБара() + fcy;
		cx += размПромотБара() + fcy;
	}
	int mcy = max(qcy, bsz.cy);
	int cy = mcy + 48 * fcy / 10;
	if(dontshowagain)
		cy += fcy;
	dlg.устПрям(Размер(cx, cy));
	dlg << icon.TopPos(fcy, bsz.cy).LeftPos(fcy, bsz.cx);
	dlg << qtfctrl.TopPos(fcy + (mcy - qcy) / 2, qcy).RightPos(fcy, qcx);
	if(okcancel) {
		b1.Ok();
		if(nbtn == 2)
			b2.Cancel();
		if(nbtn == 3)
			b3.Cancel();
	}
	b1.WhenAction = dlg.Breaker(1);
	b2.WhenAction = dlg.Breaker(0);
	b3.WhenAction = dlg.Breaker(-1);
	dlg.b = &b1;
	int bx = bcx;
	int gap = fcy / 2;
	fcy = 8 * fcy / 10;
	if(button2)
		bx += gap + bcx;
	if(button3)
		bx += gap + bcx;
	bx = (cx - bx) / 2;
	if(dontshowagain) {
		dlg << dsa.BottomPos(bcy + 2 * fcy).RightPos(fcy, qcx);
	}
	if(SwapOKCancel()) {
		sAdd(dlg, fcy, bcy, bx, bcx, gap, b2, button2, im2);
		sAdd(dlg, fcy, bcy, bx, bcx, gap, b3, button3, im3);
		sAdd(dlg, fcy, bcy, bx, bcx, gap, b1, button1, im1);
	}
	else {
		sAdd(dlg, fcy, bcy, bx, bcx, gap, b1, button1, im1);
		sAdd(dlg, fcy, bcy, bx, bcx, gap, b2, button2, im2);
		sAdd(dlg, fcy, bcy, bx, bcx, gap, b3, button3, im3);
	}
	dlg.WhenClose = dlg.Breaker(button3 ? -1 : 0);
	dlg.Титул(title);
	покиньСтопорГип();
	int result;
	Ктрл::Call(callback2(sExecutePrompt, &dlg, &result));
	if(dontshowagain && dsa && (dontshowagain > 0 || result > 0))
		dsa_history.добавь(dsa_id, result);
	return result;
}

int Prompt(Событие<const Ткст&> WhenLink,
           const char *title, const Рисунок& icon, const char *qtf, bool okcancel,
           const char *button1, const char *button2, const char *button3,
		   int cx, Рисунок im1, Рисунок im2, Рисунок im3)
{
	return Prompt(false, NULL, BEEP_NONE, WhenLink, title, icon, qtf, okcancel, button1, button2, button3, cx, im1, im2, im3);
}

int Prompt(const char *title, const Рисунок& icon, const char *qtf, bool okcancel,
           const char *button1, const char *button2, const char *button3,
		   int cx)
{
	return Prompt(callback(запустиВебБраузер), title,
	              icon, qtf, okcancel, button1, button2, button3, cx, Null, Null, Null);
}

int Prompt(Событие<const Ткст&> WhenLink,
            const char *title, const Рисунок& icon, const char *qtf, bool okcancel,
            const char *button1, const char *button2, const char *button3, int cx)
{
	return Prompt(false, NULL, BEEP_NONE, WhenLink, title, icon, qtf, okcancel,
	              button1, button2, button3, cx, Null, Null, Null);
}

int Prompt(const char *title, const Рисунок& icon, const char *qtf,
           const char *button1, const char *button2, const char *button3,
		   int cx)
{
	return Prompt(title, icon, qtf, true, button1, button2, button3, cx);
}

void PromptOK(const char *qtf) {
	бипИнформация();
	Prompt(Ктрл::дайИмяПрил(), CtrlImg::information(), qtf, t_("OK"));
}

void Exclamation(const char *qtf) {
	бипВосклицание();
	Prompt(Ктрл::дайИмяПрил(), CtrlImg::exclamation(), qtf, t_("OK"));
}

void ShowExc(const Искл& exc) {
	бипВосклицание();
	Prompt(Ктрл::дайИмяПрил(), CtrlImg::exclamation(), DeQtf(exc), t_("OK"));
}

void ErrorOK(const char *qtf) {
	бипОшибка();
	Prompt(Ктрл::дайИмяПрил(), CtrlImg::error(), qtf, t_("OK"));
}

int PromptOKCancel(const char *qtf) {
	бипВопрос();
	return Prompt(Ктрл::дайИмяПрил(), CtrlImg::question(), qtf, t_("OK"), t_("Отмена"));
}

int PromptOKCancelAll(const char *qtf) {
	бипВопрос();
	return Prompt(Ктрл::дайИмяПрил(), CtrlImg::question(), qtf, t_("OK"), t_("Отмена"), t_("Все"));
}

int ErrorOKCancel(const char *qtf) {
	бипОшибка();
	return Prompt(Ктрл::дайИмяПрил(), CtrlImg::error(), qtf, t_("OK"), t_("Отмена"));
}

CH_IMAGE(YesButtonImage, Null);
CH_IMAGE(NoButtonImage, Null);
CH_IMAGE(AbortButtonImage, Null);
CH_IMAGE(RetryButtonImage, Null);

int PromptYesNo(const char *qtf) {
	бипВопрос();
	return Prompt(callback(запустиВебБраузер),
	              Ктрл::дайИмяПрил(), CtrlImg::question(), qtf, false,
	              t_("&Да"), t_("&Нет"), NULL, 0,
	              YesButtonImage(), NoButtonImage(), Null);
}

int PromptYesNoAll(const char *qtf) {
	бипВопрос();
	return Prompt(callback(запустиВебБраузер),
	              Ктрл::дайИмяПрил(), CtrlImg::question(), qtf, false,
	              t_("&Да"), t_("&Нет"), t_("Все"), 0,
	              YesButtonImage(), NoButtonImage(), Null);
}

int ErrorYesNo(const char *qtf) {
	бипОшибка();
	return Prompt(callback(запустиВебБраузер),
	              Ктрл::дайИмяПрил(), CtrlImg::error(), qtf, false,
	              t_("&Да"), t_("&Нет"), NULL, 0,
	              YesButtonImage(), NoButtonImage(), Null);
}

int PromptYesNoCancel(const char *qtf) {
	бипВопрос();
	return Prompt(callback(запустиВебБраузер),
	              Ктрл::дайИмяПрил(), CtrlImg::question(), qtf, true,
	              t_("&Да"), t_("&Нет"), t_("Отмена"), 0,
	              YesButtonImage(), NoButtonImage(), Null);
}

int ErrorYesNoCancel(const char *qtf) {
	бипОшибка();
	return Prompt(callback(запустиВебБраузер),
	              Ктрл::дайИмяПрил(), CtrlImg::error(), qtf, true,
	              t_("&Да"), t_("&Нет"), t_("Отмена"), 0,
	              YesButtonImage(), NoButtonImage(), Null);
}

int ErrorYesNoAll(const char *qtf) {
	бипОшибка();
	return Prompt(callback(запустиВебБраузер),
	              Ктрл::дайИмяПрил(), CtrlImg::error(), qtf, true,
	              t_("&Да"), t_("&Нет"), t_("Все"), 0,
	              YesButtonImage(), NoButtonImage(), Null);
}

int PromptAbortRetry(const char *qtf) {
	бипВосклицание();
	return Prompt(callback(запустиВебБраузер),
	              Ктрл::дайИмяПрил(), CtrlImg::exclamation(), qtf, false,
	              t_("&Прервать"), t_("&Повторить"), NULL, 0,
	              AbortButtonImage(), RetryButtonImage(), Null);
}

int ErrorAbortRetry(const char *qtf) {
	бипОшибка();
	return Prompt(callback(запустиВебБраузер),
	              Ктрл::дайИмяПрил(), CtrlImg::error(), qtf, false,
	              t_("&Прервать"), t_("&Повторить"), NULL, 0,
	              AbortButtonImage(), RetryButtonImage(), Null);
}

int PromptRetryCancel(const char *qtf) {
	бипВосклицание();
	return Prompt(callback(запустиВебБраузер),
	              Ктрл::дайИмяПрил(), CtrlImg::exclamation(), qtf, true,
	              t_("&Повторить"), t_("Отмена"), NULL, 0,
	              RetryButtonImage(), Null, Null);
}

int ErrorRetryCancel(const char *qtf) {
	бипОшибка();
	return Prompt(callback(запустиВебБраузер),
	              Ктрл::дайИмяПрил(), CtrlImg::error(), qtf, true,
	              t_("&Повторить"), t_("Отмена"), NULL, 0,
	              RetryButtonImage(), Null, Null);
}

int PromptAbortRetryIgnore(const char *qtf) {
	бипВосклицание();
	return Prompt(callback(запустиВебБраузер),
	              Ктрл::дайИмяПрил(), CtrlImg::exclamation(), qtf, false,
	              t_("&Прервать"), t_("&Повторить"), t_("&Игнорировать"), 0,
	              AbortButtonImage(), RetryButtonImage(), Null);
}

int ErrorAbortRetryIgnore(const char *qtf) {
	бипОшибка();
	return Prompt(callback(запустиВебБраузер),
	              Ктрл::дайИмяПрил(), CtrlImg::error(), qtf, false,
	              t_("&Прервать"), t_("&Повторить"), t_("&Игнорировать"), 0,
	              AbortButtonImage(), RetryButtonImage(), Null);
}

int PromptSaveDontSaveCancel(const char *qtf) {
	бипВопрос();
	return Prompt(callback(запустиВебБраузер),
	              Ктрл::дайИмяПрил(), CtrlImg::question(), qtf, true,
	              t_("&Сохранить"), t_("&Не сохранять"), t_("Отмена"), 0,
	              CtrlImg::save(), NoButtonImage(), Null);
}

int PromptOpt(const char *opt_id, int beep, Событие<const Ткст&> WhenLink,
              const char *title, const Рисунок& icon, const char *qtf, bool okcancel,
              const char *button1, const char *button2, const char *button3,
		      int cx, Рисунок im1, Рисунок im2, Рисунок im3)
{
	return Prompt(true, opt_id, beep, WhenLink, title, icon, qtf, okcancel, button1, button2, button3, cx, im1, im2, im3);
}

int PromptOpt(const char *opt_id, int beep,
              const char *title, const Рисунок& icon, const char *qtf, bool okcancel,
              const char *button1, const char *button2, const char *button3,
		      int cx)
{
	return PromptOpt(opt_id, beep, callback(запустиВебБраузер), title,
	              icon, qtf, okcancel, button1, button2, button3, cx, Null, Null, Null);
}

int PromptOpt(const char *opt_id, int beep,
              const char *title, const Рисунок& icon, const char *qtf,
              const char *button1, const char *button2, const char *button3,
		      int cx)
{
	return PromptOpt(opt_id, beep, title, icon, qtf, true, button1, button2, button3, cx);
}

void PromptOKOpt(const char *qtf, const char *opt_id) {
	PromptOpt(opt_id, BEEP_INFORMATION, Ктрл::дайИмяПрил(), CtrlImg::information(), qtf, t_("OK"));
}

void ExclamationOpt(const char *qtf, const char *opt_id) {
	PromptOpt(opt_id, BEEP_EXCLAMATION,  Ктрл::дайИмяПрил(), CtrlImg::exclamation(), qtf, t_("OK"));
}

void ShowExcOpt(const Искл& exc, const char *opt_id) {
	PromptOpt(opt_id, BEEP_EXCLAMATION, Ктрл::дайИмяПрил(), CtrlImg::exclamation(), DeQtf(exc), t_("OK"));
}

void ErrorOKOpt(const char *qtf, const char *opt_id) {
	PromptOpt(opt_id, BEEP_ERROR, Ктрл::дайИмяПрил(), CtrlImg::error(), qtf, t_("OK"));
}

int PromptOKCancelOpt(const char *qtf, const char *opt_id) {
	return PromptOpt(opt_id, BEEP_QUESTION, Ктрл::дайИмяПрил(), CtrlImg::question(), qtf, t_("OK"), t_("Отмена"));
}

int PromptOKCancelAllOpt(const char *qtf, const char *opt_id) {
	return PromptOpt(opt_id, BEEP_QUESTION, Ктрл::дайИмяПрил(), CtrlImg::question(), qtf, t_("OK"), t_("Отмена"), t_("Все"));
}

int ErrorOKCancelOpt(const char *qtf, const char *opt_id) {
	return PromptOpt(opt_id, BEEP_ERROR, Ктрл::дайИмяПрил(), CtrlImg::error(), qtf, t_("OK"), t_("Отмена"));
}

int PromptYesNoOpt(const char *qtf, const char *opt_id) {
	return PromptOpt(opt_id, BEEP_QUESTION, callback(запустиВебБраузер),
	              Ктрл::дайИмяПрил(), CtrlImg::question(), qtf, false,
	              t_("&Да"), t_("&Нет"), NULL, 0,
	              YesButtonImage(), NoButtonImage(), Null);
}

int PromptYesNoAllOpt(const char *qtf, const char *opt_id) {
	return PromptOpt(opt_id, BEEP_QUESTION, callback(запустиВебБраузер),
	              Ктрл::дайИмяПрил(), CtrlImg::question(), qtf, false,
	              t_("&Да"), t_("&Нет"), t_("Все"), 0,
	              YesButtonImage(), NoButtonImage(), Null);
}

int ErrorYesNoOpt(const char *qtf, const char *opt_id) {
	return PromptOpt(opt_id, BEEP_ERROR, callback(запустиВебБраузер),
	              Ктрл::дайИмяПрил(), CtrlImg::error(), qtf, false,
	              t_("&Да"), t_("&Нет"), NULL, 0,
	              YesButtonImage(), NoButtonImage(), Null);
}

int PromptYesNoCancelOpt(const char *qtf, const char *opt_id) {
	return PromptOpt(opt_id, BEEP_QUESTION, callback(запустиВебБраузер),
	              Ктрл::дайИмяПрил(), CtrlImg::question(), qtf, true,
	              t_("&Да"), t_("&Нет"), t_("Отмена"), 0,
	              YesButtonImage(), NoButtonImage(), Null);
}

int ErrorYesNoCancelOpt(const char *qtf, const char *opt_id) {
	return PromptOpt(opt_id, BEEP_ERROR, callback(запустиВебБраузер),
	              Ктрл::дайИмяПрил(), CtrlImg::error(), qtf, true,
	              t_("&Да"), t_("&Нет"), t_("Отмена"), 0,
	              YesButtonImage(), NoButtonImage(), Null);
}

int ErrorYesNoAllOpt(const char *qtf, const char *opt_id) {
	return PromptOpt(opt_id, BEEP_ERROR, callback(запустиВебБраузер),
	              Ктрл::дайИмяПрил(), CtrlImg::error(), qtf, true,
	              t_("&Да"), t_("&Нет"), t_("Все"), 0,
	              YesButtonImage(), NoButtonImage(), Null);
}

int PromptAbortRetryOpt(const char *qtf, const char *opt_id) {
	return PromptOpt(opt_id, BEEP_EXCLAMATION, callback(запустиВебБраузер),
	              Ктрл::дайИмяПрил(), CtrlImg::exclamation(), qtf, false,
	              t_("&Прервать"), t_("&Повторить"), NULL, 0,
	              AbortButtonImage(), RetryButtonImage(), Null);
}

int ErrorAbortRetryOpt(const char *qtf, const char *opt_id) {
	return PromptOpt(opt_id, BEEP_ERROR, callback(запустиВебБраузер),
	              Ктрл::дайИмяПрил(), CtrlImg::error(), qtf, false,
	              t_("&Прервать"), t_("&Повторить"), NULL, 0,
	              AbortButtonImage(), RetryButtonImage(), Null);
}

int PromptRetryCancelOpt(const char *qtf, const char *opt_id) {
	return PromptOpt(opt_id, BEEP_EXCLAMATION, callback(запустиВебБраузер),
	              Ктрл::дайИмяПрил(), CtrlImg::exclamation(), qtf, true,
	              t_("&Повторить"), t_("Отмена"), NULL, 0,
	              RetryButtonImage(), Null, Null);
}

int ErrorRetryCancelOpt(const char *qtf, const char *opt_id) {
	return PromptOpt(opt_id, BEEP_ERROR, callback(запустиВебБраузер),
	              Ктрл::дайИмяПрил(), CtrlImg::error(), qtf, true,
	              t_("&Повторить"), t_("Отмена"), NULL, 0,
	              RetryButtonImage(), Null, Null);
}

int PromptAbortRetryIgnoreOpt(const char *qtf, const char *opt_id) {
	return PromptOpt(opt_id, BEEP_EXCLAMATION, callback(запустиВебБраузер),
	              Ктрл::дайИмяПрил(), CtrlImg::exclamation(), qtf, false,
	              t_("&Прервать"), t_("&Повторить"), t_("&Игнорировать"), 0,
	              AbortButtonImage(), RetryButtonImage(), Null);
}

int ErrorAbortRetryIgnoreOpt(const char *qtf, const char *opt_id) {
	return PromptOpt(opt_id, BEEP_ERROR, callback(запустиВебБраузер),
	              Ктрл::дайИмяПрил(), CtrlImg::error(), qtf, false,
	              t_("&Прервать"), t_("&Повторить"), t_("&Игнорировать"), 0,
	              AbortButtonImage(), RetryButtonImage(), Null);
}

int PromptSaveDontSaveCancelOpt(const char *qtf, const char *opt_id) {
	return PromptOpt(opt_id, BEEP_QUESTION, callback(запустиВебБраузер),
	              Ктрл::дайИмяПрил(), CtrlImg::question(), qtf, true,
	              t_("&Сохранить"), t_("&Не сохранять"), t_("Отмена"), 0,
	              CtrlImg::save(), NoButtonImage(), Null);
}

int PromptOpt1(const char *opt_id, int beep, Событие<const Ткст&> WhenLink,
              const char *title, const Рисунок& icon, const char *qtf, bool okcancel,
              const char *button1, const char *button2, const char *button3,
		      int cx, Рисунок im1, Рисунок im2, Рисунок im3)
{
	return Prompt(-1, opt_id, beep, WhenLink, title, icon, qtf, okcancel, button1, button2, button3, cx, im1, im2, im3);
}

int PromptOpt1(const char *opt_id, int beep,
              const char *title, const Рисунок& icon, const char *qtf, bool okcancel,
              const char *button1, const char *button2, const char *button3,
		      int cx)
{
	return PromptOpt1(opt_id, beep, callback(запустиВебБраузер), title,
	              icon, qtf, okcancel, button1, button2, button3, cx, Null, Null, Null);
}

int PromptOpt1(const char *opt_id, int beep,
              const char *title, const Рисунок& icon, const char *qtf,
              const char *button1, const char *button2, const char *button3,
		      int cx)
{
	return PromptOpt1(opt_id, beep, title, icon, qtf, true, button1, button2, button3, cx);
}

void PromptOKOpt1(const char *qtf, const char *opt_id) {
	PromptOpt1(opt_id, BEEP_INFORMATION, Ктрл::дайИмяПрил(), CtrlImg::information(), qtf, t_("OK"));
}

void ExclamationOpt1(const char *qtf, const char *opt_id) {
	PromptOpt1(opt_id, BEEP_EXCLAMATION,  Ктрл::дайИмяПрил(), CtrlImg::exclamation(), qtf, t_("OK"));
}

void ShowExcOpt1(const Искл& exc, const char *opt_id) {
	PromptOpt1(opt_id, BEEP_EXCLAMATION, Ктрл::дайИмяПрил(), CtrlImg::exclamation(), DeQtf(exc), t_("OK"));
}

void ErrorOKOpt1(const char *qtf, const char *opt_id) {
	PromptOpt1(opt_id, BEEP_ERROR, Ктрл::дайИмяПрил(), CtrlImg::error(), qtf, t_("OK"));
}

int PromptOKCancelOpt1(const char *qtf, const char *opt_id) {
	return PromptOpt1(opt_id, BEEP_QUESTION, Ктрл::дайИмяПрил(), CtrlImg::question(), qtf, t_("OK"), t_("Отмена"));
}

int PromptOKCancelAllOpt1(const char *qtf, const char *opt_id) {
	return PromptOpt1(opt_id, BEEP_QUESTION, Ктрл::дайИмяПрил(), CtrlImg::question(), qtf, t_("OK"), t_("Отмена"), t_("Все"));
}

int ErrorOKCancelOpt1(const char *qtf, const char *opt_id) {
	return PromptOpt1(opt_id, BEEP_ERROR, Ктрл::дайИмяПрил(), CtrlImg::error(), qtf, t_("OK"), t_("Отмена"));
}

int PromptYesNoOpt1(const char *qtf, const char *opt_id) {
	return PromptOpt1(opt_id, BEEP_QUESTION, callback(запустиВебБраузер),
	              Ктрл::дайИмяПрил(), CtrlImg::question(), qtf, false,
	              t_("&Да"), t_("&Нет"), NULL, 0,
	              YesButtonImage(), NoButtonImage(), Null);
}

int PromptYesNoAllOpt1(const char *qtf, const char *opt_id) {
	return PromptOpt1(opt_id, BEEP_QUESTION, callback(запустиВебБраузер),
	              Ктрл::дайИмяПрил(), CtrlImg::question(), qtf, false,
	              t_("&Да"), t_("&Нет"), t_("All"), 0,
	              YesButtonImage(), NoButtonImage(), Null);
}

int ErrorYesNoOpt1(const char *qtf, const char *opt_id) {
	return PromptOpt1(opt_id, BEEP_ERROR, callback(запустиВебБраузер),
	              Ктрл::дайИмяПрил(), CtrlImg::error(), qtf, false,
	              t_("&Да"), t_("&Нет"), NULL, 0,
	              YesButtonImage(), NoButtonImage(), Null);
}

int PromptYesNoCancelOpt1(const char *qtf, const char *opt_id) {
	return PromptOpt1(opt_id, BEEP_QUESTION, callback(запустиВебБраузер),
	              Ктрл::дайИмяПрил(), CtrlImg::question(), qtf, true,
	              t_("&Да"), t_("&Нет"), t_("Отмена"), 0,
	              YesButtonImage(), NoButtonImage(), Null);
}

int ErrorYesNoCancelOpt1(const char *qtf, const char *opt_id) {
	return PromptOpt1(opt_id, BEEP_ERROR, callback(запустиВебБраузер),
	              Ктрл::дайИмяПрил(), CtrlImg::error(), qtf, true,
	              t_("&Да"), t_("&Нет"), t_("Отмена"), 0,
	              YesButtonImage(), NoButtonImage(), Null);
}

int ErrorYesNoAllOpt1(const char *qtf, const char *opt_id) {
	return PromptOpt1(opt_id, BEEP_ERROR, callback(запустиВебБраузер),
	              Ктрл::дайИмяПрил(), CtrlImg::error(), qtf, true,
	              t_("&Да"), t_("&Нет"), t_("All"), 0,
	              YesButtonImage(), NoButtonImage(), Null);
}

int PromptAbortRetryOpt1(const char *qtf, const char *opt_id) {
	return PromptOpt1(opt_id, BEEP_EXCLAMATION, callback(запустиВебБраузер),
	              Ктрл::дайИмяПрил(), CtrlImg::exclamation(), qtf, false,
	              t_("&Прервать"), t_("&Повторить"), NULL, 0,
	              AbortButtonImage(), RetryButtonImage(), Null);
}

int ErrorAbortRetryOpt1(const char *qtf, const char *opt_id) {
	return PromptOpt1(opt_id, BEEP_ERROR, callback(запустиВебБраузер),
	              Ктрл::дайИмяПрил(), CtrlImg::error(), qtf, false,
	              t_("&Прервать"), t_("&Повторить"), NULL, 0,
	              AbortButtonImage(), RetryButtonImage(), Null);
}

int PromptRetryCancelOpt1(const char *qtf, const char *opt_id) {
	return PromptOpt1(opt_id, BEEP_EXCLAMATION, callback(запустиВебБраузер),
	              Ктрл::дайИмяПрил(), CtrlImg::exclamation(), qtf, true,
	              t_("&Повторить"), t_("Отмена"), NULL, 0,
	              RetryButtonImage(), Null, Null);
}

int ErrorRetryCancelOpt1(const char *qtf, const char *opt_id) {
	return PromptOpt1(opt_id, BEEP_ERROR, callback(запустиВебБраузер),
	              Ктрл::дайИмяПрил(), CtrlImg::error(), qtf, true,
	              t_("&Повторить"), t_("Отмена"), NULL, 0,
	              RetryButtonImage(), Null, Null);
}

int PromptAbortRetryIgnoreOpt1(const char *qtf, const char *opt_id) {
	return PromptOpt1(opt_id, BEEP_EXCLAMATION, callback(запустиВебБраузер),
	              Ктрл::дайИмяПрил(), CtrlImg::exclamation(), qtf, false,
	              t_("&Прервать"), t_("&Повторить"), t_("&Игнорировать"), 0,
	              AbortButtonImage(), RetryButtonImage(), Null);
}

int ErrorAbortRetryIgnoreOpt1(const char *qtf, const char *opt_id) {
	return PromptOpt1(opt_id, BEEP_ERROR, callback(запустиВебБраузер),
	              Ктрл::дайИмяПрил(), CtrlImg::error(), qtf, false,
	              t_("&Прервать"), t_("&Повторить"), t_("&Игнорировать"), 0,
	              AbortButtonImage(), RetryButtonImage(), Null);
}

int PromptSaveDontSaveCancelOpt1(const char *qtf, const char *opt_id) {
	return PromptOpt1(opt_id, BEEP_QUESTION, callback(запустиВебБраузер),
	              Ктрл::дайИмяПрил(), CtrlImg::question(), qtf, true,
	              t_("&Сохранить"), t_("&Don't сохрани"), t_("Отмена"), 0,
	              CtrlImg::save(), NoButtonImage(), Null);
}

}
