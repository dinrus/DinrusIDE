#include "DinrusIDE.h"

struct MainConfigDlg : public WithConfigLayout<ТопОкно> {
	EditString ce, fe;
	ФреймПраво<Кнопка> cb;
	const РОбласть& wspc;
	
	void FlagDlg();

	bool Perform(const Ткст& startwith);

	typedef MainConfigDlg ИМЯ_КЛАССА;

	MainConfigDlg(const РОбласть& wspc);
};

bool SetSw(const Ткст& flag, Ктрл& sw, const char *flg) {
	if(flag == flg) {
		sw <<= 1;
		return true;
	}
	return false;
}

Ткст GetSw(Ктрл& sw, const char *flag) {
	if((int)~sw)
		return Ткст(flag) + ' ';
	return Null;
}


static
void sSetOption(Один<Ктрл>& ctrl)
{
	ctrl.создай<Опция>().NoWantFocus();
}

void MainConfigDlg::FlagDlg()
{
	ВекторМап<Ткст, Индекс<Ткст>> flags;
	{
		CodeBaseLock __;
		const CppBase& b = CodeBase();
		for(int i = 0; i < b.дайСчёт(); i++) {
			Ткст nest = b.дайКлюч(i);
			const Массив<CppItem>& ci = b[i];
			for(int j = 0; j < ci.дайСчёт(); j++) {
				const CppItem& m = ci[j];
				if(m.kind == FLAGTEST) {
					Ткст n = m.имя;
					n.обрежьСтарт("flag");
					flags.дайДобавь(n).найдиДобавь(GetSourcePackage(GetSourceFilePath(m.file)));
				}
			}
		}
	}
	SortByKey(flags);
	
	WithConfLayout<ТопОкно> cfg;
	CtrlLayoutOKCancel(cfg, "Флаги конфигурации");
	cfg.Sizeable().MaximizeBox();
	Вектор<Ткст> flg = SplitFlags0(~~fe);
	Вектор<Ткст> accepts = wspc.GetAllAccepts(0);
	сортируй(accepts, GetLanguageInfo());
	enum { CC_SET, CC_NAME, CC_PACKAGES, CC_COUNT };
	cfg.accepts.добавьКолонку("Набор").Ctrls(sSetOption);
	cfg.accepts.добавьКолонку("Флаг");
	cfg.accepts.добавьКолонку("Пакеты");
	cfg.accepts.SetLineCy(Zy(20));
	cfg.accepts.ColumnWidths("26 122 204");
	for(const auto& f : ~flags)
		cfg.accepts.добавь(false, f.ключ, Join(f.значение.дайКлючи(), ", "));
	
	cfg.other.устФильтр(FlagFilterM);
	cfg.gui <<= false;
	cfg.debugcode <<= false;
	Ткст other;
	for(int i = 0; i < flg.дайСчёт(); i++) {
		Ткст f = flg[i];
		if(!SetSw(f, cfg.gui, "GUI") &&
		   !SetSw(f, cfg.debugcode, "DEBUGCODE")) {
			int x = (*f == '.' ? cfg.accepts.найди(f.середина(1), CC_NAME) : -1) || cfg.accepts.найди(f, CC_NAME);
			if(x >= 0)
				cfg.accepts.уст(x, CC_SET, true);
			else {
				if(!other.пустой())
					other << ' ';
				other << f;
			}
		}
	}
	cfg.other <<= other;
	if(cfg.пуск() == IDOK) {
		Ткст flags;
		flags
		    << GetSw(cfg.gui, "GUI")
		    << GetSw(cfg.debugcode, "DEBUGCODE");
		for(int i = 0; i < cfg.accepts.дайСчёт(); i++)
			if(cfg.accepts.дай(i, CC_SET))
				flags << cfg.accepts.дай(i, CC_NAME) << ' ';
		flags << cfg.other.дайТекст().вТкст();
		fe = Join(SplitFlags0(flags), " ").вШТкст();
	}
}

MainConfigDlg::MainConfigDlg(const РОбласть& wspc_) : wspc(wspc_) {
	CtrlLayoutOKCancel(*this, "Конфигурация(ции) главного пакета");
	fe.добавьФрейм(cb);
	fe.устФильтр(FlagFilterM);
	cb.устРисунок(CtrlImg::smallright()).NoWantFocus();
	cb <<= THISBACK(FlagDlg);
	list.добавьКолонку("Флаги", 3).Edit(fe);
	list.добавьКолонку("Опционное имя", 2).Edit(ce);
	list.Appending().Removing().Moving().Duplicating();
	
	list.WhenDrag = [=] {
		list.DoDragAndDrop(InternalClip(list, "main_config-элт"), list.дайСэиплТяга(), DND_MOVE);
	};
	list.WhenDropInsert = [=](int line, PasteClip& d) {
		if(GetInternalPtr<КтрлМассив>(d, "main_config-элт") == &list && list.курсор_ли() && d.прими()) {
			int q = list.дайКурсор();
			if(q == line)
				return;
			Вектор<Значение> h = list.читайРяд(q);
			list.удали(q);
			if(q < line)
				line--;
			if(line >= 0 && line <= list.дайСчёт()) {
				list.вставь(line);
				list.уст(line, h);
				list.устКурсор(line);
			}
		}
	};

}

bool MainConfigDlg::Perform(const Ткст& startwith) {
	list.устКурсор(0);
	list.FindSetCursor(startwith);
	return пуск() == IDOK;
}

void Иср::MainConfig() {
	package.устКурсор(0);
	if(package.дайКурсор() != 0) return;
	MainConfigDlg dlg(роблИср());
	for(int i = 0; i < actual.config.дайСчёт(); i++) {
		const Пакет::Конфиг& f = actual.config[i];
		dlg.list.добавь(f.param, f.имя);
	}
	if(!dlg.Perform(mainconfigname)) return;
	actual.config.очисть();
	for(int i = 0; i < dlg.list.дайСчёт(); i++) {
		Пакет::Конфиг& f = actual.config.добавь();
		f.param = dlg.list.дай(i, 0);
		f.имя = dlg.list.дай(i, 1);
	}
	SavePackage();
	if(dlg.list.курсор_ли()) {
		mainconfigparam = dlg.list.дай(0);
		mainconfigname = Nvl((Ткст)dlg.list.дай(1), mainconfigparam);
		MakeTitle();
	}
	SyncMainConfigList();
	SetHdependDirs();
	MakeTitle();
}
