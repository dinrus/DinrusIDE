#include "DinrusIDE.h"

Ткст GetSvnDir(const Ткст& p)
{
	Ткст dir = p;
	while(dir.дайСчёт() > 4) {
		if(дирЕсть(приставьИмяф(dir, ".svn")) || дирЕсть(приставьИмяф(dir, "_svn")))
			return dir;
		dir = дайПапкуФайла(dir);
	}
	return Null;
}

byte passkey_sha256[32];

bool HasCredentialsPasskey()
{
	for(int i = 0; i < 32; i++)
		if(passkey_sha256[i])
			return true;
	return false;
}

Ткст CredentialsCrypt(const Ткст& src)
{
	if(!HasCredentialsPasskey())
		return src;
	int ci = 0;
	byte c[32];
	memcpy(c, passkey_sha256, 32);
	Ткст r;
	for(char b : src) {
		if(ci >= 32) { // poor mans acceptable encryption
			SHA256(c, c, 32);
			ci = 0;
		}
		r.конкат(b ^ c[ci++]);
	}
	return r;
}

struct PasskeyDlg : WithPasskeyLayout<ТопОкно> {
	PasskeyDlg();

	void синх() { passkey1.Password(!show_passkey); passkey2.Password(!show_passkey); }
};

PasskeyDlg::PasskeyDlg()
{
	CtrlLayoutOK(*this, "Passkey");
	
	show_passkey << [=] { синх(); };
	
	синх();
}

struct Credential {
	Ткст url;
	Ткст username;
	Ткст password;
};

Ткст CredentialsFile()
{
	return конфигФайл("repo.credentials");
}

bool LoadCredentials0(Массив<Credential>& r)
{
	r.очисть();
	Значение v = ParseJSON(CredentialsCrypt(загрузиФайл(CredentialsFile())));
	if(ошибка_ли(v))
		return false;
	try {
		for(Значение e : v) {
			Credential& c = r.добавь();
			c.url = e["url"];
			c.username = e["username"];
			c.password = e["password"];
		}
	}
	catch(ОшибкаТипаЗначения) {
		r.очисть();
		return false;
	}
	return true;
}

struct GetPasskeyDlg : WithGetPasskeyLayout<ТопОкно> {
	GetPasskeyDlg();
	
	void синх() { passkey.Password(!show_passkey); }
};

GetPasskeyDlg::GetPasskeyDlg()
{
	CtrlLayoutOK(*this, "Passkey");
	
	show_passkey << [=] { синх(); };
	
	синх();
}

bool LoadCredentials(Массив<Credential>& r)
{
	if(!файлЕсть(CredentialsFile()))
		return false;
	if(LoadCredentials0(r)) return true;
	if(!HasCredentialsPasskey()) {
		for(;;) {
			GetPasskeyDlg dlg;
			dlg.пуск();
			SHA256(passkey_sha256, ~dlg.passkey);
			if(LoadCredentials0(r)) return true;
			WithFailedPasskeyLayout<ТопОкно> p;
			CtrlLayoutOK(p, "Passkey");
			p.Breaker(p.clear, IDEXIT);
			if(p.пуск() == IDEXIT) {
				memset(passkey_sha256, 0, 32);
				удалифл(CredentialsFile());
				break;
			}
		}
	}
	return false;
}

bool GetCredentials(const Ткст& url, const Ткст& dir, Ткст& username, Ткст& password)
{
	Массив<Credential> cr;
	int best = 0;
	if(LoadCredentials(cr)) {
		for(const Credential& c : cr) {
			for(int pass = 0; pass < 2; pass++) {
				const Ткст& u = pass ? url : dir;
				int n = min(u.дайСчёт(), c.url.дайСчёт());
				int ml;
				for(ml = 0; ml < n; ml++)
					if(u[ml] != c.url[ml])
						break;
				if(ml > best) {
					best = ml;
					username = c.username;
					password = c.password;
				}
			}
		}
	}
	return best;
}

struct CredentialDlg : WithCredentialLayout<ТопОкно> {
	void Hints(const Вектор<Ткст>& url_hints);
	void синх() { password.Password(!show_password); }
	CredentialDlg();
};

void CredentialDlg::Hints(const Вектор<Ткст>& url_hints)
{
	for(Ткст s : url_hints)
		url.добавьСписок(s);
}

CredentialDlg::CredentialDlg()
{
	CtrlLayoutOKCancel(*this, "Credentials");
	синх();
	show_password << [=] { синх(); };
}

struct CredentialsDlg : WithCredentialsLayout<ТопОкно> {
	const Вектор<Ткст>& url_hints;

	void синх();
	void добавь();
	void Edit();
	void удали();

	void грузи();
	void сохрани();
	
	void Passkey();

	typedef CredentialsDlg ИМЯ_КЛАССА;
	CredentialsDlg(const Вектор<Ткст>& url_hints);
};

CredentialsDlg::CredentialsDlg(const Вектор<Ткст>& url_hints)
:	url_hints(url_hints)
{
	CtrlLayoutOKCancel(*this, "Credentials");
	list.добавьКолонку("Url (or directory)");
	list.добавьКолонку("Username");
	list.добавьКолонку("Password");
	list.ColumnWidths("500 200 200");
	list.EvenRowColor();
	list.SetLineCy(max(Draw::GetStdFontCy() + Zy(4), Zy(20)));
	list.WhenSel = [=] { синх(); };
	list.WhenLeftDouble = [=] { Edit(); };
	
	show_passwords << [=] { синх(); };
	add << [=] { добавь(); };
	edit << [=] { Edit(); };
	remove << [=] { удали(); };
	clear << [=] {
		if(PromptYesNo("Удалить все?"))
			list.очисть();
	};
	passkey << [=] { Passkey(); };
}

void CredentialsDlg::Passkey()
{
	PasskeyDlg dlg;
	Ткст k;
	for(;;) {
		if(dlg.пуск() == IDCANCEL)
			return;
		k = ~dlg.passkey1;
		if(k == ~dlg.passkey2) {
			if(k.дайСчёт() || PromptYesNo("Сохранить credentials как простой текст?"))
				break;
		}
		else
			Exclamation("Поля не совпадают!");
	}
	if(k.дайСчёт())
		SHA256(passkey_sha256, k);
	else
		memset(passkey_sha256, 0, 32);
}

void CredentialsDlg::грузи()
{
	list.очисть();
	Массив<Credential> cr;
	if(LoadCredentials(cr)) {
		for(const Credential& c : cr)
			list.добавь(c.url, c.username, c.password);
		list.идиВНач();
	}
}

void CredentialsDlg::сохрани()
{
	if(list.дайСчёт()) {
		МассивЗнач va;
		for(int i = 0; i < list.дайСчёт(); i++)
			va << МапЗнач()("url", list.дай(i, 0))
			                ("username", list.дай(i, 1))
			                ("password", list.дай(i, 2));
		сохраниФайл(CredentialsFile(), CredentialsCrypt(AsJSON(va)));
	}
	else
		удалифл(CredentialsFile());
}

void CredentialsDlg::добавь()
{
	CredentialDlg dlg;
	dlg.Hints(url_hints);
again:
	if(dlg.пуск() == IDOK) {
		Ткст url = ~dlg.url;
		if(list.FindSetCursor(url)) {
			Exclamation("Запись-дубликат!");
			goto again;
		}
		list.добавь(url, ~dlg.username, ~dlg.password);
		list.сортируй();
		list.FindSetCursor(url);
	}
}

void CredentialsDlg::Edit()
{
	if(!list.курсор_ли())
		return;
	CredentialDlg dlg;
	dlg.Hints(url_hints);
	dlg.url <<= list.дай(0);
	dlg.username <<= list.дай(1);
	dlg.password <<= list.дай(2);
again:
	if(dlg.пуск() == IDOK) {
		Ткст url = ~dlg.url;
		int ii = list.найди(url);
		if(ii >= 0 && ii != list.дайКурсор()) {
			Exclamation("Запись-дубликат!");
			list.устКурсор(ii);
			goto again;
		}
		list.уст(0, url);
		list.уст(1, ~dlg.username);
		list.уст(2, ~dlg.password);
		list.сортируй();
		list.FindSetCursor(url);
	}
}

void CredentialsDlg::удали()
{
	list.DoRemove();
}

void CredentialsDlg::синх()
{
	clear.вкл(list.дайСчёт());
	edit.вкл(list.курсор_ли());
	remove.вкл(list.курсор_ли());
	
	list.колонкаПо(2)
	    .SetConvert(show_passwords ? стдПреобр()
	                               : лямбдаПреобр([](const Значение& v) {
		                                 return Ткст('*', (~v).дайСчёт());
		                             })
		);
}

void EditCredentials(const Вектор<Ткст>& url_hints)
{
	CredentialsDlg dlg(url_hints);
	dlg.грузи();
	if(dlg.выполни() == IDOK)
		dlg.сохрани();
}
