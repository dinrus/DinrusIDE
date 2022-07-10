#include "GridCtrl.h"

namespace РНЦП {
	
int GridCtrl::GetMouseCol(Точка &p, bool relative, bool fixed, bool full)
{
	if(!full && p.x < fixed_width)
		return -1;

	int dx = 0;

	if(relative)
		dx += sbx;

	int first_col = fixed ? 0 : max(firstVisCol, fixed_cols);
	int last_col = max(lastVisCol, fixed_cols - 1);

	if(!fixed && last_col >= total_cols)
		return -1;

	for(int i = first_col; i <= last_col; i++)
	{
		if(p.x >= hitems[i].nLeft(dx) &&
		   p.x  < hitems[i].nRight(dx))
			return i;
	}
	return -1;
}

int GridCtrl::GetMouseRow(Точка &p, bool relative, bool fixed, bool full)
{
	if(!full && p.y < fixed_height)
		return -1;

	int dy = 0;

	if(relative)
		dy += sby;

	int first_row = fixed ? 0 : max(firstVisRow, fixed_rows);
	int last_row = max(lastVisRow, fixed_rows - 1);
	
	if(!fixed && last_row >= total_rows)
		return -1;

	for(int i = first_row; i <= last_row; i++)
	{
		if(p.y >= vitems[i].nTop(dy) &&
		   p.y  < vitems[i].nBottom(dy))
			return i;
	}
	return -1;
}

Точка GridCtrl::дайПоз(Точка p)
{
	return Точка(GetMouseCol(p, true, false), GetMouseRow(p, true, false));
}

void GridCtrl::леваяВнизу(Точка p, dword keyflags)
{
	//popup.закрой();
	SetCapture();
	leftpnt = p;
	just_clicked = true;

	fixed_top_click  = p.x >= fixed_width && p.y < fixed_height;
	fixed_left_click = p.x < fixed_width && p.y >= fixed_height;
	fixed_click      = fixed_top_click || fixed_left_click;
	top_click        = p.y < fixed_height;

	resizing = curResizeCol || curResizeRow;

	if(resizing)
	{
		popup.закрой();
		
		splitCol  = curSplitCol;
		splitRow  = curSplitRow;
		resizeCol = curResizeCol;
		resizeRow = curResizeRow;

		разбей(GS_DOWN);
		return;
	}
	else if(fixed_click)
	{
		moveCol = oldMoveCol = GetMouseCol(p, fixed_top_click, 1);
		moveRow = oldMoveRow = GetMouseRow(p, fixed_left_click, 1);
		return;
	}

	устФокус();

	if(пустой() || толькочтен_ли())
		return;

	bool is_shift = keyflags & K_SHIFT;
	bool is_ctrl = keyflags & K_CTRL;
	
	Точка oldcur = curpos;

	CurState cs = устКурсор0(p, CU_MOUSE | CU_HIDECTRLS);
	bool state_change = cs.пригоден() && !cs.IsNew() && (is_ctrl || is_shift);

	if(!cs.IsAccepted() && !state_change)
		return;

	anchor = curpos;

	if(cs || state_change) {
		moveCol = curpos.x;
		moveRow = curpos.y;

		if((keyflags & K_CTRL) && multi_select) {
			SelectRange(curpos, curpos, !IsSelect(curpos.y, curpos.x, false), select_row);
		}
		else {
			очистьВыделение();
			if((keyflags & K_SHIFT) && multi_select && IsValidCursor(oldcur)) {
				if(oldcur.y >= 0 && oldcur.x >= 0)
					устКурсор0(oldcur, CU_HIDECTRLS);
				освежи();
			}
		}
	}

	#ifdef LOG_CALLBACKS
	//LGR(2, "ПриЛевКлике()");
	#endif

	ПриЛевКлике();

	if(editing && one_click_edit && cs.пригоден() ) //&& IsRowEditable() ?
		StartEdit();
	else
		RebuildToolBar();
	
	SetCapture();
}

void GridCtrl::двигМыши(Точка p, dword keyflags)
{
	mouse_move = true;

	if(resizing)
	{
		int si, lp, mp, off;
		RectItems *its;
		static int sub = 0;

		if(resizeCol)
		{
			mp = p.x;
			lp = leftpnt.x;
			si = splitCol;
			its = &hitems;
			bool top = fixed_top_click || (!fixed_left_click && full_col_resizing);
			off = top ? sbx : 0;
		}
		else
		{
			mp = p.y;
			lp = leftpnt.y;
			si = splitRow;
			its = &vitems;
			bool left = fixed_left_click || (!fixed_top_click && full_row_resizing);
			off = left ? sby : 0;
		}

		int right = (*its)[si].nRight(off);

		if(just_clicked)
		{
			sub = right - lp;
			just_clicked = false;
		}

		if(SetDiffItemSize(resizeCol, *its, si, mp - right + sub))
		{
			разбей(GS_MOVE);
		}

		return;
	}
	else if(fixed_click)
	{
		if((fixed_top_click && !moving_cols) ||
		   (fixed_left_click && !moving_rows) ||
		   moveCol < 0 || moveRow < 0)
		   return;

		if(!moving_header)
		{
			int diffx = p.x - leftpnt.x;
			int diffy = p.y - leftpnt.y;
			if(абс(diffx) < 5 && абс(diffy) < 5)
				return;

			p -= Точка(diffx, diffy);

			moving_header = true;
			int idx = hitems[moveCol].id;
			int idy = vitems[moveRow].id;
			pophdr.val = idy > 0 ? GetConvertedColumn(moveCol, items[idy][idx].val) : items[idy][idx].val;

			if(fixed_top_click)
			{
				pophdr.sortmode = hitems[moveCol].sortmode;
				pophdr.sortcol = hitems[moveCol].sortcol;
				pophdr.sortcnt = sortOrder.дайСчёт();

				UpdateHighlighting(GS_POPUP, p);
			}
			else
			{
				pophdr.sortmode = 0;
				pophdr.sortcol = -1;
				pophdr.sortcnt = 0;
			}

			dx = hitems[moveCol].nLeft(fixed_top_click ? sbx : 0) - p.x;
			dy = vitems[moveRow].nTop(fixed_left_click ? sby : 0) - p.y;
		}


		Точка pt = p + дайПрямЭкрана().верхЛево() + GetBarOffset();

		pophdr.дисплей = дисплей;
		pophdr.chameleon = chameleon;
		pophdr.PopUp(this, pt.x + dx, pt.y + dy, hitems[moveCol].nWidth(), vitems[moveRow].nHeight());

		if(fixed_top_click && curSplitCol != oldMoveCol)
		{
			moving_allowed = CanMoveCol(moveCol, curSplitCol);
			RefreshTop();
			//освежи(oldMoveCol >= 0 ? hitems[oldMoveCol].nRight(sbx) : 0, 0, hitems[curSplitCol].nRight(sbx), fixed_height);
			oldMoveCol = curSplitCol;
		}

		if(fixed_left_click && curSplitRow != oldMoveRow)
		{
			освежи(0, 0, fixed_width, fixed_height);
			RefreshLeft();
			oldMoveRow = curSplitRow;
		}
		return;
	}

	if(leftpnt != p && p.y < fixed_height)
	{
		UpdateHighlighting(GS_MOVE, p);
	}

	if(live_cursor && popup.открыт())
	{
		LG(2, "двигМыши:LiveCursor");
		if(IsMouseBody(p))
			устКурсор0(p, CU_MOUSE | CU_HIGHLIGHT);
		else
			устКурсор0(-1, -1, CU_HIGHLIGHT);
	}
	
	if(HasCapture())
	{
		if(!moving_body)
		{
			anchor = дайПоз(p);
			освежи();
			return;
	/*		if(keyflags & K_SHIFT)
			{
				if(устКурсор0(p, CU_MOUSE))
					DoShiftSelect();
				return;
			}

			bool select = true;
			if(select_row && !multi_select)
				select = false;

			if(select && (keyflags & K_CTRL))
			{
				if(устКурсор0(p, CU_MOUSE))
//					DoCtrlSelect();
				return;
			}*/
		}

		if(moveCol < 0 || moveRow < 0)
			return;

		if(!dragging)
			return;
		
		if(!moving_body)
		{
			popup.закрой();
			
			if(!top_click && valid_cursor &&
			   p.x < total_width &&
			   p.y < total_height &&
			   (абс(p.y - leftpnt.y) > 5 ||
			    абс(p.x - leftpnt.x) > 5))
				moving_body = true;

			oldMoveRow = -1;
		}
		else
		{
			Точка pt = p + дайПрямЭкрана().верхЛево();

			int row = curSplitRow - fixed_rows + 2;
			if(select_row)
			{
				int count = max(1, selected_rows);

				if(vitems[curpos.y].IsSelect())
					popup.val = фмт(t_("Moving selection (%d %s) before row %d"), count, count == 1 ? t_("row") : t_("rows"), row);
				else
					popup.val = фмт(t_("Moving row %d before row %d"), curpos.y - fixed_rows + 1, row);
			}
			else
			{
				int count = max(1, selected_items);
				popup.val = фмт(t_("Moving %d %s before row %d"), count, count == 1 ? t_("cell") : t_("cells"), row);
			}

			int px = pt.x + 15;
			int py = pt.y + GD_ROW_HEIGHT;

			popup.gd = дисплей;
			popup.gd->row = 0;
			popup.gd->col = 0;
			popup.fg = SColorText;
			popup.bg = SColorPaper;
			popup.fnt = StdFont();
			popup.style = 0;
			popup.PopUp(this, px, py, дайРазмТекста((Ткст) popup.val, StdFont()).cx + 10, GD_ROW_HEIGHT);
			устФокус();

			if(curSplitRow != oldMoveRow || scrollLeftRight)
			{
				int dy = sby;
				if(oldMoveRow >= 0)
					освежи(Прям(0, vitems[oldMoveRow].nBottom(dy) - 5, дайРазм().cx, vitems[oldMoveRow].nBottom(dy) + 5));
				else
					освежи(Прям(0, 0, дайРазм().cx, 5));
				if(curSplitRow >= 0)
					освежи(Прям(0, vitems[curSplitRow].nBottom(dy) - 5, дайРазм().cx, vitems[curSplitRow].nBottom(dy) + 5));
				else
					освежи(Прям(0, 0, дайРазм().cx, 5));

				oldMoveRow = curSplitRow;
				popup.освежи();

				scrollLeftRight = false;
			}
		}
	}
	else
		SyncPopup();
}

void GridCtrl::леваяВверху(Точка p, dword keyflags)
{
	LG(0, "леваяВверху");

	ReleaseCapture();
	освежи();

	fixed_click = false;

	UpdateHighlighting(resizing ? GS_MOVE : GS_UP, p);

	if(moving_header)
	{
		LG(0, "moving_header");
		pophdr.закрой();

		moving_header = false;
		if(fixed_top_click)
			MoveCol(moveCol, curSplitCol);
		else
			MoveRow(moveRow, curSplitRow);

		if(focused_ctrl)
			focused_ctrl->устФокус();

		fixed_top_click = false;
		fixed_left_click = false;

		return;
	}

	if(resizing)
	{
		разбей(GS_UP);
		resizeCol = resizeRow = resizing = false;
		return;
	}

	if(fixed_top_click && sorting && расстояние(leftpnt, p) < 3)
	{
		int i = GetMouseRow(leftpnt, false, true);
		int j = GetMouseCol(leftpnt, true, false);

		if(j >= fixed_cols && i == 0 && hitems[i].sortable)
		{
			int newSortCol = hitems[j].id;

			if(sorting_multicol && (keyflags & K_CTRL))
			{
				int colidx = InMultisort(newSortCol);

				if(colidx < 0)
				    sortOrder.добавь(newSortCol);

				int cnt = sortOrder.дайСчёт();

				hitems[j].ChangeSortMode(newSortCol == sortOrder[cnt - 1]);

				if(colidx >= 0)
				{
					if(hitems[j].sortmode == 0)
					{
						sortOrder.удали(colidx);
						cnt--;
					}
					
					if(WhenSort)
						WhenSort();
					else
					{
						if(hitems[j].sortmode > 0 && colidx == cnt - 1)
							GSort();
						else
							Multisort();
					}
				}
				else
				{
					hitems[j].sortcol = cnt;
					if(WhenSort)
						WhenSort();
					else
						GSort();
				}
			}
			else
			{
				if(sortCol >= 0 && sortCol != newSortCol)
				{
					int idx = GetIdCol(sortCol, true);
					hitems[idx].sortmode = 0;
				}

				ClearMultisort(1);
				hitems[j].ChangeSortMode();
				hitems[j].sortcol = 1;

				if(hitems[j].sortmode == 0)
					sortCol = -1;
				else
					sortCol = newSortCol;

				sortOrder.добавь(newSortCol);
				
				if(WhenSort)
					WhenSort();
				else
					GSort(newSortCol, hitems[j].sortmode, fixed_rows);
			}

			UpdateCursor();
			Repaint(false, true);
			
			if(WhenSorted)
				WhenSorted();
		}
    }

	if(moving_body)
	{
		popup.закрой();
		moving_body = false;
		MoveRows(curSplitRow + 1, !vitems[curpos.y].IsSelect());
		return;
	}
	
	if(IsValidCursor(anchor) && IsValidCursor(curpos) && multi_select)
		SelectRange(Прям(anchor, curpos), true, select_row);
	
	anchor = curpos;
}

void GridCtrl::леваяДКлик(Точка p, dword keyflags)
{
	LG(0, "леваяДКлик");

	if(full_col_resizing && curSplitCol >= 0)
		return;

	if(full_row_resizing && curSplitRow >= 0)
		return;

	if(пустой() || !IsMouseBody(p) || толькочтен_ли())
		return;

	if(keyflags & K_SHIFT || keyflags & K_CTRL)
		return;

	if(!valid_cursor)
		return;

	if(editing)
		StartEdit();

	if(!ктрл_ли(curpos))
	{
		popup.закрой();
		#ifdef LOG_CALLBACKS
		LGR(2, "WhenLeftDouble()");
		#endif
		WhenLeftDouble();
	}
}

void GridCtrl::леваяПовтори(Точка p, dword keyflags)
{
	if(!moving_header && !resizeCol && !resizeRow)
		MouseAccel(p, resize_col_mode == 0, resize_row_mode == 0, keyflags);
}

void GridCtrl::праваяВнизу(Точка p, dword keyflags)
{
	if(толькочтен_ли())
		return;

	if(total_rows > fixed_rows)
	{
		if(!EndEdit())
			return;

		устКурсор0(p, CU_MOUSE);
	}

	RebuildToolBar();
	устФокус(); //jak nie bedzie menu to fokous zostanie na danym wierszu
	БарМеню::выполни(WhenMenuBar);
	anchor = curpos;
}

void GridCtrl::выходМыши()
{
	if(live_cursor)
	{
		LG(2, "выходМыши:LiveCursor");
		устКурсор0(-1, -1, CU_HIGHLIGHT);
	}
	UpdateHighlighting(GS_BORDER, Точка(0, 0));
	oldSplitCol = -1;
	oldSplitRow = -1;
	//popup.закрой();
}

void GridCtrl::колесоМыши(Точка p, int zdelta, dword keyflags)
{
	if(resize_row_mode == 0)
	{
		sby.уст(sby - zdelta / 4);
	}
}

};