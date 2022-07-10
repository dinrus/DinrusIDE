#include "SqlCtrl.h"
#include "SqlDlg.h"
#include <Report/Report.h>
#include <CodeEditor/CodeEditor.h>

namespace РНЦП {

#define IMAGEFILE <SqlCtrl/SqlCtrl.iml>
#define IMAGECLASS SqlConsoleImg
#include <Draw/iml.h>

static bool scmpw(const byte *s, const char *w) {
	for(;;) {
		if(*w == '\0')
			return *s == ' ' || *s == '\t';
		if(взаг(*s++) != взаг(*w++))
			return false;
	}
}

static const int MAX_NESTING_LEVEL = 10;
static const int PROGRESS_RANGE = 1000000000;

bool SqlRunScript(int dialect, Поток& script_stream, 
	Врата<Ткст> executor, Врата<int, int> progress,
	Индекс<Ткст>& script_file_names, int progress_min, int progress_max)
{
	int line_number = 1;
	for(int c; (c = script_stream.прекрати()) >= 0;) {
		if(c <= ' ') {
			if(c == '\n')
				line_number++;
			script_stream.дай();
		}
		else if(c == '@') {
			script_stream.дай();
			int64 start = script_stream.дайПоз() - 1;
			bool rel = false;
			if(script_stream.прекрати() == '@') {
				script_stream.дай();
				rel = true;
			}
			ТкстБуф subscript_fn;
			while((c = script_stream.дай()) >= 0 && c != '\n')
				subscript_fn.конкат((char)c);
			int e = subscript_fn.дайДлину();
			while(e > 0 && subscript_fn[e - 1] <= ' ')
				e--;
			Ткст фн(~subscript_fn, e);
			фн = нормализуйПуть(фн, дайДиректориюФайла(script_file_names[rel ? script_file_names.дайСчёт() - 1 : 0]));
			Ткст norm = фн;
#if !PLATFORM_PATH_HAS_CASE
			norm = впроп(norm);
#endif
			if(script_file_names.найди(norm) >= 0)
				throw Искл(фмтЧ(t_("circular script inclusion of %s at %s:%d"),
					фн, script_file_names.верх(), line_number));
			if(script_file_names.дайСчёт() >= MAX_NESTING_LEVEL)
				throw Искл(фмтЧ(t_("script nesting too deep (%d levels)"), script_file_names.дайСчёт()));
			int64 end = script_stream.дайПоз();
			
			int new_min = progress_min + (int)((progress_max - progress_min) * start / script_stream.дайРазм());
			int new_max = progress_min + (int)((progress_max - progress_min) * end / script_stream.дайРазм());
			
			ФайлВвод fi;
			if(!fi.открой(фн))
				throw Искл(фмтЧ(t_("Ошибка opening script file '%s'"), фн));
			
			script_file_names.добавь(norm);
			if(!SqlRunScript(dialect, fi, executor, progress, script_file_names, new_min, new_max))
				return false;
			script_file_names.сбрось();
		}
		else {
			ТкстБуф statement;
			bool exec = false;
			bool body = false;
			bool chr = false;

			while(!script_stream.кф_ли() && !exec) {
				Ткст line = script_stream.дайСтроку();
				line_number++;
				
				int l = line.дайДлину();
				if(l && line[0] == '/' && !body && !chr)
					exec = true;
				else
				if(l && line[0] == '.' && body && !chr)
					body = false;
				else {
					if(statement.дайДлину() && !chr)
						statement.конкат(' ');
					bool spc = true;
					bool create = false;
					for(const byte *s = (const byte *)(const char *)line; *s; s++) {
						if(s[0] == '-' && s[1] == '-' && !chr) break;
						if(*s == '\'') chr = !chr;
						if(!chr && spc && scmpw(s, "create"))
							create = true;
						if(!chr && spc && (scmpw(s, "begin") || scmpw(s, "declare") ||
						   create && (scmpw(s, "procedure") || scmpw(s, "function") || scmpw(s, "trigger"))
						))
							body = true;
						if(*s == ';' && !chr && !body) {
							exec = true;
							break;
						}
						if(*s > ' ' || chr) {
							statement.конкат(*s);
							spc = false;
						}
						else
						if(*s == '\t' || *s == ' ') {
							if(!spc) statement.конкат(' ');
							spc = true;
						}
						else
							spc = false;
					}
					if(chr) statement.конкат("\r\n");
				}
			}
			if(progress((int)(progress_min + script_stream.дайПоз()
				* (progress_max - progress_min) / script_stream.дайРазм()), PROGRESS_RANGE))
				throw ПрервиИскл();
			if(!executor(statement))
				return false;
		}
	}
	return true;
}

bool SqlRunScript(int dialect, Поток& script_stream, const Ткст& file_name,
                  Врата<Ткст> executor, Врата<int, int> progress)
{
	Индекс<Ткст> script_file_names;
	Ткст фн = нормализуйПуть(file_name);
#if !PLATFORM_PASH_HAS_CASE
	фн = впроп(фн);
#endif
	script_file_names.добавь(фн);
	return SqlRunScript(dialect, script_stream, executor, progress, script_file_names, 0, PROGRESS_RANGE);
}

void RunDlgSqlExport(Sql& cursor, Ткст command, Ткст tablename);

class SqlConsole : public ТопОкно {
public:
	virtual bool Ключ(dword ключ, int count);
	virtual void сериализуй(Поток& s);

	void ListMenu(Бар& bar);
	void ListPrintRow();
	void ListPrintList();
	void ListExport();

protected:
	Sql                        cursor;
	Ткст                     lastquery;
	Сплиттер                   lires, vsplit;
	КтрлМассив                  list;
	СтатичПрям                 rec_err;
	ДокРедакт                    errortext;
	КтрлМассив                  record;
	КтрлМассив                  trace;
	РедакторКода                 command;
	Кнопка                     execute;
	Кнопка                     schema;
	Кнопка                     csv;
	КтрлРодитель                 command_pane;
//	CallbackSet                hide;
	Вектор<int>                cw;
	Вектор<bool>               visible;
	Вектор<bool>               lob;
	Ткст                     LastDir;
	Надпись                      info1, info2;

	enum {
		NORMAL,
		RERUN,
		SCRIPT,
		QUIET,
	};

	void    ColSize();
	void    скрой(int i);
	void    Record();
	void    выполни(int тип = NORMAL);
	void    TraceToCommand();
	void    TraceToExecute();
	void    ListToCommand(КтрлМассив *list);
	void    SaveTrace();
	void    RunScript(bool quiet);
	void    TraceMenu(Бар& menu);
	void    ObjectTree() { SQLObjectTree(cursor.GetSession()); }
	void    Csv();

	class Exec;
	friend class Exec;
	class Exec : public StatementExecutor {
	public:
		typedef Exec ИМЯ_КЛАССА;
		Exec(bool quiet) : quiet(quiet) {}
		SqlConsole *me;
		bool quiet;
		virtual bool выполни(const Ткст& stmt) {
			me->command <<= stmt; me->синх(); me->выполни(quiet ? QUIET : SCRIPT); return true;
		}
		bool GateExec(Ткст stmt) { return выполни(stmt); }
		operator Врата<Ткст> ()   { return THISBACK(GateExec); }
		
	};

	void ViewRecord();

public:
	typedef SqlConsole ИМЯ_КЛАССА;

	void    Perform();

	void    AddInfo(Ктрл& tgt, Надпись& info, const char *txt);

	SqlConsole(SqlSession& session);
};

void SqlConsole::выполни(int тип) {
	list.переустанов();
	list.HeaderObject().Absolute().Moving();
	visible.очисть();
	lob.очисть();
	record.очисть();
	Ткст s = ~command;
	while(*s.последний() == ';')
		s.обрежь(s.дайДлину() - 1);
	int ms0 = msecs();
	cursor.сотриОш();
	lastquery = s;
	Ткст ttl = s;
	ttl.замени("\t", " ");
	ttl.замени("\n", " ");
	ttl.замени("\r", "");
	info1.удали();
	info2.удали();
	if(!cursor.выполни(s)) {
	Ошибка:
		record.скрой();
		errortext.покажи();
		list.добавьКолонку(t_("Ошибка"));
		Ткст err = cursor.GetLastError();
		errortext <<= err;
		list.добавь(err);
		trace.добавь(s, err, "");
		trace.идиВКон();
		Титул((ttl + " - " + err).вШТкст());
		return;
	}
	if(тип == QUIET)
		return;
	bool onecol = cursor.дайКолонки() == 1;
	record.покажи(!onecol);
	errortext.покажи(onecol);
	int ms1 = msecs();
	cw.устСчёт(cursor.дайКолонки());
	visible.устСчёт(cw.дайСчёт(), true);
	int margins;
	for(int i = 0; i < cursor.дайКолонки(); i++) {
		const SqlColumnInfo& ci = cursor.GetColumnInfo(i);
		Ткст n = впроп(ci.имя);
		list.добавьКолонку(n);
		list.HeaderTab(i).WhenAction = THISBACK1(скрой, i);
		margins = HorzLayoutZoom(2) + 2 * list.HeaderTab(i).GetMargin();
		cw[i] = дайРазмТекста(n, StdFont()).cx + margins;
		record.добавь(n, Null);
		lob.добавь(ci.тип == -1 || ci.тип == -2); // !! BLOB / CLOB hack
	}
	Progress pi;
	pi.устТекст(t_("Fetched %d line(s)"));
	while(cursor.Fetch()) {
		Вектор<Значение> row = cursor.GetRow();
		for(int i = 0; i < cursor.дайКолонки(); i++)
		{
			if(lob[i])
			{
				Ткст temp;
				cursor.дайКолонку(i, temp);
				row[i] = temp;
			}
			cw[i] = max(cw[i], дайРазмТекста(стдФормат(row[i]), StdFont()).cx + margins);
			cw[i] = min(cw[i], list.дайРазм().cx / 3);
		}
		list.добавь(row);
		if(pi.StepCanceled()) break;
	}
	if(cw.дайСчёт() && cursor.WasError()) {
		list.переустанов();
		goto Ошибка;
	}
	visible.устСчёт(list.дайСчётКолонок(), true);
	ColSize();
	if(list.дайСчёт() > 0)
		list.устКурсор(0);
	Титул(фмтЧ(t_("%s (%d rows)"), ttl, list.дайСчёт()));
	Ткст rrows = фмт(t_("%d rows"), max(list.дайСчёт(), cursor.GetRowsProcessed()));
	Ткст rms = фмт(t_("%d ms"), ms1 - ms0);
	if(тип == RERUN && trace.курсор_ли()) {
		trace.уст(1, rrows);
		trace.уст(2, rms);
	}
	else {
		trace.добавь(s, rrows, rms);
		trace.идиВКон();
	}
	command.удали(0, command.дайДлину());
	command.устВыделение(0, 0);
}

void SqlConsole::ColSize() {
	int maxw = list.дайРазм().cx;
	int wx = 0;
	for(int i = 0; i < list.дайСчётКолонок(); i++)
		if(visible[i]) {
			int w = min(maxw, cw[i]);
			wx += w;
			list.HeaderObject().SetTabRatio(i, w);
			list.HeaderObject().ShowTab(i);
		}
		else
			list.HeaderObject().HideTab(i);
}

void SqlConsole::скрой(int i) {
	if(i < visible.дайСчёт())
		visible[i] = false;
	ColSize();
}

void SqlConsole::Record() {
	if(list.дайСчётИндексов() == 1)
		errortext <<= стдФормат(list.дай(0));
	for(int i = 0; i < list.дайСчётИндексов(); i++)
		record.уст(i, 1, list.курсор_ли() ? list.дай(i) : Значение());
}

bool SqlConsole::Ключ(dword ключ, int count) {
	switch(ключ) {
	case K_F5:
		выполни();
		return true;
	case K_CTRL_R:
		RunScript(false);
		return true;
	case K_CTRL_Q:
		RunScript(true);
		return true;
	case K_CTRL_S:
		SaveTrace();
		return true;
	}
	return ТопОкно::Ключ(ключ, count);
}

void SqlConsole::сериализуй(Поток& s) {
	int version = 0;
	s / version;
	s.Magic();
	Прям r = дайПрям();
	s % r;
	устПрям(r);
	vsplit.сериализуй(s);
	record.сериализуйЗаг(s);
	lires.сериализуй(s);
	trace.сериализуйЗаг(s);
	if(s.грузится())
		trace.очисть();
	Вектор<МассивЗнач> ar;
	for(int i = 0; i < trace.дайСчёт(); i++)
		ar.добавь(trace.дайМассив(i));
	s % ar;
	if(s.грузится()) {
		for(int i = 0; i < ar.дайСчёт(); i++)
			trace.устМассив(i, ar[i]);
		trace.идиВКон();
	}
	if(version >= 1)
		s % LastDir;
	s.Magic();
}

void SqlConsole::Perform() {
	const char cfg[] = "SqlConsole.cfg";
	грузиИзФайла(*this, cfg);
	Титул(t_("SQL Commander"));
	Иконка(SqlConsoleImg::database_edit(), SqlConsoleImg::SqlConsoleIconLarge());
	Sizeable();
	Zoomable();
	ActiveFocus(command);
	пуск();
	cursor.сотриОш();
	сохраниВФайл(*this, cfg);
}

void SqlConsole::TraceToCommand() {
	if(trace.курсор_ли()) {
		command.устДанные(trace.дай(0));
		command.устКурсор(command.дайДлину());
	}
}

void SqlConsole::ListToCommand(КтрлМассив *l)
{
	int c = l->GetClickColumn();
	if(дайКтрл() && l->курсор_ли() && c >= 0 && c < l->дайСчётКолонок()) {
		command.Paste(какТкст(l->дай(c)).вШТкст());
		command.устФокус();
	}
}

void SqlConsole::TraceToExecute() {
	выполни(RERUN);
}

void SqlConsole::SaveTrace() {
	FileSel fsel;
	fsel.ActiveDir(LastDir);
	fsel.DefaultExt("sql");
	fsel.Type(t_("SQL scripts (*.sql)"), "*.sql");
	fsel.AllFilesType();
	if(!fsel.ExecuteSaveAs(t_("сохрани trace as"))) return;
	ФайлВывод out(~fsel);
	if(!out) return;
	LastDir = дайДиректориюФайла(~fsel);
	for(int i = 0; i < trace.дайСчёт(); i++) {
		out.помести((Ткст)trace.дай(i, 0));
		out.помести(";\n");
	}
}

void SqlConsole::RunScript(bool quiet) {
//	РНЦП::RunScript runscript = cursor.GetSession().GetRunScript();
//	if(!runscript) {
//		Exclamation(t_("Database connection doesn't support running scripts."));
//		return;
//	}
	FileSel fsel;
	fsel.ActiveDir(LastDir);
	fsel.DefaultExt("sql");
	fsel.Type(t_("SQL scripts (*.sql)"), "*.sql");
	fsel.AllFilesType();
	if(!fsel.ExecuteOpen(t_("выполни script"))) return;
	Exec exec(quiet);
	exec.me = this;
	LastDir = дайДиректориюФайла(~fsel);
	Progress progress(t_("Executing script"));
	ФайлВвод fi;
	if(!fi.открой(~fsel)) {
		Exclamation(фмтЧ(t_("Cannot open file [* \1%s\1]."), ~fsel));
		return;
	}
	try {
		SqlRunScript(cursor.GetDialect(), fi, ~fsel, exec, progress);
	}
	catch(Искл e) {
		ShowExc(e);
	}
}

void SqlConsole::TraceMenu(Бар& menu) {
	menu.добавь(t_("сохрани as script.."), THISBACK(SaveTrace)).Ключ(K_CTRL_S);
	menu.добавь(t_("выполни script.."), THISBACK1(RunScript, false)).Ключ(K_CTRL_R);
	menu.добавь(t_("выполни script quietly.."), THISBACK1(RunScript, true)).Ключ(K_CTRL_Q);
}

void SqlConsole::ListMenu(Бар& bar)
{
	bar.добавь(t_("Print record"), THISBACK(ListPrintRow));
	bar.добавь(t_("Print list"), THISBACK(ListPrintList));
	bar.добавь(t_("Export..."), THISBACK(ListExport));
}

void SqlConsole::Csv()
{
	SelectSaveFile("Csv\t*.csv", list.AsCsv());
}

void SqlConsole::ListPrintRow()
{
	Ткст qtf;
	qtf << "[A1 ";
	if(!пусто_ли(lastquery))
		qtf << "[4* \1" << lastquery << "\1]&&";
	for(int i = 0; i < record.дайСчёт(); i++) {
		qtf << (i ? "--" : "++") << "::10@(240.240.240) \1" << стдФормат(record.дай(i, 0)) << "\1"
			"||::30@(255.255.255) \1" << стдФормат(record.дай(i, 1)) << "\1";
	}
	qtf << "++";
	Report report;
	report << qtf;
	РНЦП::Perform(report);
}

void SqlConsole::ListPrintList()
{
	Ткст qtf;
	qtf << "[A1 ";
	if(!пусто_ли(lastquery))
		qtf << "[4* \1" << lastquery << "\1]&&";
	for(int i = 0; i < record.дайСчёт(); i++)
		qtf << (i ? "||" : "++") << "::@(240.240.240) [* \1"
		<< стдФормат(record.дай(i, 0)) << "\1]";
	for(int i = 0; i < list.дайСчёт(); i++)
		for(int j = 0; j < list.дайСчётИндексов(); j++)
			qtf << (j ? "||" : "--") << "::@(255.255.255) \1" << стдФормат(list.дай(i, j)) << '\1';
	qtf << "++";
	Report report;
	report << qtf;
	РНЦП::Perform(report);
}

void SqlConsole::ListExport()
{
	RunDlgSqlExport(cursor, lastquery, Null);
}

struct SqlValueViewDlg : public WithSqlValueViewLayout<ТопОкно> {
	typedef SqlValueViewDlg ИМЯ_КЛАССА;
	
	Ткст значение;
	
	void синх();
	void сохрани();

	SqlValueViewDlg();
};

void SqlValueViewDlg::синх()
{
	if(~формат) {
		ТкстПоток ss;
		HexDumpData(ss, ~значение, значение.дайДлину(), false, 1000000);
		text <<= ss.дайРез();
	}		
	else
		text <<= значение;
}

void SqlValueViewDlg::сохрани()
{
	SelectSaveFile("Файл\t*.*", значение);
}

SqlValueViewDlg::SqlValueViewDlg()
{
	CtrlLayout(*this, "");
	text.устШрифт(Monospace(GetStdFont().дайВысоту()));
	формат <<= THISBACK(синх);
	save <<= THISBACK(сохрани);
	формат = 0;
}

void SqlViewValue(const Ткст& title, const Ткст& значение)
{
	SqlValueViewDlg dlg;
	dlg.Титул(title);
	dlg.значение = значение;
	dlg.синх();
	dlg.выполни();
}

void SqlConsole::ViewRecord()
{
	if(!record.курсор_ли())
		return;
	SqlViewValue(record.дай(0), какТкст(record.дай(1)));
}

void SqlConsole::AddInfo(Ктрл& tgt, Надпись& info, const char *txt)
{
	info = txt;
	tgt.добавь(info.SizePos());
//	tgt.добавь(info.BottomPos(0, GetStdFontCy()).RightPos(0, дайРазмТекста(txt, StdFont()).cx));
}

SqlConsole::SqlConsole(SqlSession& session)
: cursor(session)
{
	int ecy = EditField::GetStdHeight();
	lires.гориз(list, rec_err);
	rec_err << record.SizePos() << errortext.SizePos();
	errortext.устТолькоЧтен();
	errortext.устШрифт(Courier(12));
	errortext.скрой();
	vsplit.верт(lires, trace);
	vsplit.добавь(command_pane);
	vsplit.устПоз(6500);
	vsplit.устПоз(8500, 1);
	lires.устПоз(7000);
	record.добавьКолонку(t_("Колонка"), 5);
	record.добавьКолонку(t_("Значение"), 10);
	record.WhenLeftDouble = THISBACK(ViewRecord);
	record.ПриЛевКлике = THISBACK1(ListToCommand, &record);
	AddInfo(record, info1, "\1[g= Use [@B Ктрл+Click] to copy data into SQL&[@B DoubleClick] for detailed view.");
	trace.добавьКолонку(t_("Command"), 8);
	trace.добавьКолонку(t_("результат"), 1);
	trace.добавьКолонку(t_("Duration"), 1);
	trace.ПриЛевКлике = THISBACK(TraceToCommand);
	trace.WhenLeftDouble = THISBACK(TraceToExecute);
	trace.WhenBar = THISBACK(TraceMenu);
	trace.NoWantFocus();
	list.WhenSel = THISBACK(Record);
	list.ПриЛевКлике = THISBACK1(ListToCommand, &list);
	list.WhenBar = THISBACK(ListMenu);
	list.HeaderObject().Absolute();
	AddInfo(list, info2, "\1[g= Use [@B Ктрл+Click] to copy data into SQL.");
	добавь(vsplit.SizePos());
	command.устШрифт(Courier(GetStdFontCy()));
	command_pane.добавь(command.VSizePos().HSizePos(0, HorzLayoutZoom(100)));
	ecy = max(24, ecy);
	command_pane.добавь(execute.TopPos(0, ecy).RightPos(4, HorzLayoutZoom(90)));
	command_pane.добавь(schema.TopPos(ecy + 4, ecy).RightPos(4, HorzLayoutZoom(90)));
	command_pane.добавь(csv.TopPos(2 * ecy + 4, ecy).RightPos(4, HorzLayoutZoom(90)));
	command.Highlight("sql");
	schema.устНадпись(t_("&Schema"));
	schema <<= THISBACK(ObjectTree);
	schema.устРисунок(SqlConsoleImg::bricks());
	execute <<= THISBACK1(выполни, NORMAL);
	execute.устРисунок(SqlConsoleImg::lightning());
	execute.устНадпись(t_("выполни (F5)"));
	csv <<= THISBACK(Csv);
	csv.устНадпись(t_("Export.."));
	csv.устРисунок(SqlConsoleImg::database_save());
	ActiveFocus(command);
}

void SQLCommander(SqlSession& session) {
	SqlConsole con(session);
	con.Perform();
}

bool IsSqlConsoleActive__()
{
	return dynamic_cast<SqlConsole *>(Ктрл::GetActiveWindow());
}

}
