#include "GridCtrl.h"

namespace РНЦП {

bool GridCtrl::Ключ(dword ключ, int)
{
	auto NewAnchor = [&] {
		очистьВыделение();
		anchor = curpos;
	};

	auto NewSelection = [&] {
		if(IsValidCursor(anchor) && IsValidCursor(curpos))
			SelectRange(Прям(anchor, curpos), true, select_row);
		освежи();
	};

	if(!толькочтен_ли())
	switch(ключ)
	{
		case K_ENTER:
			очистьВыделение();
			WhenEnter();
			#ifdef LOG_CALLBACKS
			LGR(2, "WhenEnter()");
			#endif

			if(enter_like_tab)
				return TabKey(true);
			else if(!SwitchEdit())
				return true;
			/*
			if(th.IsSorted())
			{
				th.Multisort();
				освежи();
			}*/
			NewAnchor();
			return true;
		case K_ESCAPE:
		{
			bool quit = true;
			if(search_string.дайСчёт() > 0)
			{
				ClearFound();
				quit = false;
			}
			else if(HasCtrls())
			{
				bool canceled = CancelEdit();
				quit = !canceled;
			}
			if(quit)
			{
				WhenEscape();
				return false;
			}
			else
				return true;
		}
		case K_SHIFT|K_LEFT:
			GoLeft();
			NewSelection();
			return true;
		case K_SHIFT|K_RIGHT:
			GoRight();
			NewSelection();
			return true;
		case K_SHIFT|K_UP:
			идиПредш();
			NewSelection();
			return true;
		case K_SHIFT|K_DOWN:
			идиСледщ();
			NewSelection();
			return true;
		case K_SHIFT|K_PAGEUP:
			GoPageUp();
			NewSelection();
			return true;
		case K_SHIFT|K_PAGEDOWN:
			GoPageDn();
			NewSelection();
			return true;
		case K_SHIFT_HOME:
			идиВНач();
			NewSelection();
			return true;
		case K_SHIFT_END:
			идиВКон();
			NewSelection();
			return true;
		case K_UP:
			идиПредш();
			NewAnchor();
			return true;
		case K_DOWN:
			идиСледщ();
			NewAnchor();
			return true;
		case K_LEFT:
			GoLeft();
			NewAnchor();
			return true;
		case K_RIGHT:
			GoRight();
			NewAnchor();
			return true;

		case K_HOME:
		case K_CTRL_HOME:
		case K_CTRL_PAGEUP:
			идиВНач();
			NewAnchor();
			return true;
		case K_END:
		case K_CTRL_END:
		case K_CTRL_PAGEDOWN:
			идиВКон();
			NewAnchor();
			return true;
		case K_PAGEUP:
			GoPageUp();
			NewAnchor();
			return true;
		case K_PAGEDOWN:
			GoPageDn();
			NewAnchor();
			return true;
		case K_TAB:
			return TabKey(false);
		case K_SHIFT|K_TAB:
			if(HasCtrls())
			{
				bool isprev = ShowPrevCtrl();
				return focused_ctrl ? true : isprev;
			}
			else if(tab_changes_row)
			{
				bool isprev = false;
				if(select_row)
					isprev = идиПредш();
				else
					isprev = GoLeft();
				очистьВыделение();

				return isprev;
			}
			else
				return false;
		case K_CTRL|K_F:
			if(searching)
			{
				find.устФокус();
				return true;
			}
			else
				return false;
		case K_BACKSPACE:
		{
			if(searching)
			{
				int cnt = search_string.дайСчёт();
				if(cnt > 0)
				{
					search_string.удали(cnt - 1);
					find <<= search_string;
					ShowMatchedRows(search_string);
				}
				return true;
			}
			else
				return false;
		}
		case K_F3:
			if(rowfnd >= 0)
			{
				for(int i = rowfnd + 1; i < total_rows; i++)
				{
					if(vitems[i].IsFound())
					{
						rowfnd = i;
						устКурсор0(i);
						курсорПоЦентру();
						WhenSearchCursor();
						return true;
					}
				}
				for(int i = fixed_rows; i < rowfnd; i++)
				{
					if(vitems[i].IsFound())
					{
						rowfnd = i;
						устКурсор0(i);
						курсорПоЦентру();
						WhenSearchCursor();
						return true;
					}
				}

				return true;
			}
			return false;
		case K_CTRL_W:
			WriteClipboardText(GetColumnWidths());
			return true;
		default:
			if(searching && !isedit && ищи(ключ))
				return true;
	}

	return БарМеню::скан(WhenMenuBar, ключ);
}

};