#ifndef _DropGrid_DropGrid_h_
#define _DropGrid_DropGrid_h_

#include <CtrlLib/CtrlLib.h>
#include <GridCtrl/GridCtrl.h>

namespace РНЦП {

class DropGrid : public Преобр, public GridDisplay, public Ктрл
{
	public:
		enum
		{
			BTN_SELECT,
			BTN_LEFT,
			BTN_RIGHT,
			BTN_UP,
			BTN_DOWN,
			BTN_PLUS,
			BTN_CLEAN
		};

		class PopUpGrid : public GridCtrl
		{
			private:

				void CloseNoData();
				void CloseData();

			public:
				Callback WhenPopDown;
				Callback WhenClose;
				Callback WhenCloseData;
				Callback WhenCloseNoData;

				typedef PopUpGrid ИМЯ_КЛАССА;

				virtual void откл();
				void PopUp(Ктрл *owner, const Прям &r);

				PopUpGrid();
		};

	protected:

		int key_col;
		int find_col;
		int value_col;
		Вектор<int> value_cols;
		PopUpGrid list;
		MultiButtonFrame drop;
		GridButton clear;

	private:
	
		int rowid;
		int trowid;
		Значение значение;
		bool change;

		int list_width;
		int list_height;
		int drop_lines;

		bool display_all:1;
		bool header:1;
		bool valuekey:1;
		bool notnull:1;
		bool display_columns:1;
		bool drop_enter:1;
		bool data_action:1;
		bool searching:1;
		bool must_change:1;
		bool null_action:1;
		bool clear_button:1;
		bool nodrop:1;

		GridDisplay *дисплей;
		
		Ткст must_change_str;

		void Change(int dir);
		void SearchCursor();
		void DoAction(int row, bool action = true, bool chg = true);
		Вектор<Ткст> MakeVector(int r) const;
		Значение MakeValue(int r = -1, bool columns = true) const;
		void UpdateValue();
		Значение Format0(const Значение& q, int rowid) const;

	public:

		typedef DropGrid ИМЯ_КЛАССА;
		DropGrid();

		void закрой();
		void CloseData();
		void CloseNoData();
		void сбрось();

		DropGrid& устШирину(int w);
		DropGrid& устВысоту(int h);
		DropGrid& SetKeyColumn(int n);
		DropGrid& SetFindColumn(int n);
		DropGrid& SetValueColumn(int n);
		DropGrid& AddValueColumn(int n);
		DropGrid& AddValueColumns(int first = -1, int last = -1);
		DropGrid& DisplayAll(bool b = true);
		DropGrid& SetDropLines(int d);
		DropGrid& Header(bool b = true);
		DropGrid& NoHeader();
		DropGrid& Resizeable(bool b = true);
		DropGrid& ColorRows(bool b = true);
		DropGrid& неПусто(bool b = true);
		DropGrid& ValueAsKey(bool b = true);
		DropGrid& устДисплей(GridDisplay &d);
		DropGrid& DisplayColumns(bool b = true);
		DropGrid& DropEnter(bool b = true);
		DropGrid& DataAction(bool b = true);
		DropGrid& Searching(bool b = true);
		DropGrid& MustChange(bool b = true, const char* s = "");
		DropGrid& NullAction(bool b = true);
		DropGrid& ClearButton(bool b = true);
		DropGrid& NoDrop(bool b = true);
		DropGrid& устДанные();
		DropGrid& SearchHideRows(bool b = true);

		GridCtrl::ItemRect& добавьКолонку(const char *имя, int width = -1, bool idx = false);
		GridCtrl::ItemRect& добавьКолонку(Ид id, const char *имя, int width = -1, bool idx = false);
		GridCtrl::ItemRect& добавьИндекс(const char *имя = "");
		GridCtrl::ItemRect& добавьИндекс(Ид id);

		MultiButton::SubButton& AddButton(int тип, const Callback &cb);
		MultiButton::SubButton& AddSelect(const Callback &cb);
		MultiButton::SubButton& AddPlus(const Callback &cb);
		MultiButton::SubButton& AddEdit(const Callback &cb);
		MultiButton::SubButton& AddClear();
		MultiButton::SubButton& добавьТекст(const char* label, const Callback& cb);
		MultiButton::SubButton& GetButton(int n);
		
		int AddColumns(int cnt);

		void GoTop();

		int SetIndex(int n);
		int дайИндекс() const;

		int дайСчёт() const;

		void переустанов();
		void очисть();
		void Ready(bool b = true);
		void ClearValue();
		void DoClearValue();

		virtual Значение дайДанные() const;
		virtual void устДанные(const Значение& v);

		Значение дайЗначение() const;
		Значение дайЗначение(int r) const;
		Значение найдиЗначение(const Значение& v) const;
		Вектор<Ткст> FindVector(const Значение& v) const;
		bool FindMove(const Значение& v);
		Значение дайКлюч() const;

		virtual bool Ключ(dword k, int);
		virtual void рисуй(Draw& draw);
		virtual void леваяВнизу(Точка p, dword keyflags);
		virtual void сфокусирован();
		virtual void расфокусирован();
		virtual void сериализуй(Поток& s);
		virtual bool прими();
		virtual Размер дайМинРазм() const;
		virtual void State(int reason);

		void рисуй0(Draw &w, int lm, int rm, int x, int y, int cx, int cy, const Значение &val, dword style, Цвет &fg, Цвет &bg, Шрифт &fnt, bool found = false, int fs = 0, int fe = 0);
		virtual void рисуй(Draw &w, int x, int y, int cx, int cy, const Значение &val, dword style, Цвет &fg, Цвет &bg, Шрифт &fnt, bool found = false, int fs = 0, int fe = 0);

		Значение дай(int c) const;
		Значение дай(Ид id) const;
		Значение дай(int r, int c) const;
		Значение дай(int r, Ид id) const;
		Значение дайПредш(int c) const;
		Значение дайПредш(Ид id) const;
		void  уст(int c, const Значение& v);
		void  уст(Ид id, const Значение& v);
		void  уст(int r, int c, const Значение& v);
		void  уст(int r, Ид id, const Значение& v);
		void  уст(int r, const Вектор<Значение> &v, int data_offset = 0, int column_offset = 0);
		void  добавь(const Вектор<Значение> &v, int offset = 0, int count = -1, bool hidden = false);
		Ткст дайТкст(Ид id);

		Значение& operator() (int r, int c);
		Значение& operator() (int c);
		Значение& operator() (Ид id);
		Значение& operator() (int r, Ид id);

		GridCtrl::ItemRect& GetRow(int r);

		bool выделен();
		bool пустой();
		bool IsChange();
		bool IsInit();
		
		void ClearChange();

		int найди(const Значение& v, int col = 0, int opt = 0);
		int найди(const Значение& v, Ид id, int opt = 0);
		int GetCurrentRow() const;

		void CancelUpdate();

		GridCtrl& GetList() { return list; }

		virtual Значение фмт(const Значение& q) const;
		
		Callback WhenLeftDown;
		Callback WhenDrop;

		GridCtrl::ItemRect& AddRow(int n = 1, int size = -1);
		DropGrid& добавь() { AddRow(); return *this; }

		//$-DropCtrl& добавь(const Значение& [, const Значение& ]...);
		#define  E__Add(I)      DropGrid& добавь(__List##I(E__Value));
			__Expand(E__Add)
		#undef   E__Add
		//$+

		//$-GridCtrl::ItemRect& добавь(const Значение& [, const Значение& ]...);
		#define  E__Add(I)      GridCtrl::ItemRect& AddRow(__List##I(E__Value));
			__Expand(E__Add)
		#undef   E__Add
		//$+

		DropGrid& добавьСепаратор(Цвет c);

};

}

#endif
