#include <GridCtrl/GridCtrl.h>
#include <GridCtrl/GridCtrl.h>

namespace РНЦП {

#ifdef COMPILER_MSC
#pragma warning(disable: 4355)
#endif

#define TFILE <GridCtrl/GridCtrl.t>
#include <Core/t.h>

bool GridCtrl::index_as_column = false;
bool GridCtrl::reverse_sort_icon = false;

GridCtrl::GridCtrl() : holder(*this)
{
	sortCol = -1;
	hcol = -1;
	hrow = -1;

	fixed_click = false;
	fixed_top_click = false;
	fixed_left_click = false;
	size_changed = true;

	resize_panel_open = false;

	synced = false;
	sc_fr = -1;
	sc_lr = -1;

	resizeCol = false;
	resizeRow = false;

	ready = false;
	doscroll = true;
	firstRow = lastRow = -1;
	firstCol = lastCol = -1;

	firstVisCol = -1;
	lastVisCol  = -1;
	firstVisRow = -1;
	lastVisRow  = -1;

	colidx = -1;
	rowidx = -1;
	rowfnd = -1;

	GD_COL_WIDTH  = 50;
	GD_ROW_HEIGHT = Draw::GetStdFontCy() + 5;
	GD_HDR_HEIGHT = GD_ROW_HEIGHT + 2;
	GD_IND_WIDTH  = 9;

	дисплей = new GridDisplay();
	дисплей->SetTextAlign(GD::VCENTER);
	orgdisp = дисплей;

	sbx.гориз();
	sby.верт();
	sbx.ПриПромоте = THISBACK(промотай);
	sby.ПриПромоте = THISBACK(промотай);
	sbx.устСтроку(5);
	sby.устСтроку(GridCtrl::GD_ROW_HEIGHT);

	fixed_cols = 1;
	fixed_rows = 1;

	total_cols = 0;
	total_rows = 0;

	minRowSelected = -1;
	maxRowSelected = -1;

	bains = 0;
	coluid = 0;
	rowuid = 0;

	close.устНадпись(t_("закрой"));
	close <<= THISBACK(CloseGrid);

	oldpos.очисть();

	indicator              = false;
	resizing_cols          = true;
	resizing_rows          = true;
	resizing_fixed_cols    = true;
	resizing_fixed_rows    = false;
	resize_paint_mode      = 2;
	resize_col_mode        = 1;
	resize_row_mode        = 0;
	multi_select           = false;
	select_row             = true;
	moving_cols            = false;
	moving_rows            = false;
	dragging               = false;
	horz_grid              = true;
	vert_grid              = true;
	draw_last_horz_line    = true;
	draw_last_vert_line    = true;
	sorting                = false;
	live_cursor            = false;
	row_changing           = true;
	edit_mode              = GE_ROW;
	one_click_edit         = false;
	goto_first_edit        = true; 
	coloring_mode          = 0;
	isedit                 = false;
	genr_ctrls             = 0;
	edit_ctrls             = false;
	sorting                = true;
	sorting_multicol       = true;
	header                 = true;

	cancel_update_cell     = false;
	cancel_update          = false;
	cancel_insert          = false;
	cancel_remove          = false;
	cancel_accept          = false;
	cancel_duplicate       = false;
	cancel_cursor          = false;
	cancel_move            = false;

	inserting              = false;
	appending              = false;
	duplicating            = false;
	removing               = false;
	accepting              = false;
	canceling              = false;
	moving                 = false;
	navigating             = false;
	searching              = false;
	editing                = false;
	edits_in_new_row       = true;
	closing                = false;
	hiding                 = false;
	clipboard              = false;
	extra_paste            = true;
	fixed_paste            = false;
	copy_allowed           = true;
	cut_allowed            = true;
	paste_allowed          = true;
	copy_column_names      = false;
	draw_focus             = false;
	ask_remove             = false;

	search_hide            = true;
	search_highlight       = true;
	search_highlight_first = false;
	search_immediate       = true;
	search_case            = false;
	search_move_cursor     = true;
	search_display         = true;

	row_order              = false;
	row_data               = false;

	reject_null_row        = true;
	tab_changes_row        = true;
	tab_adds_row           = false;
	enter_like_tab         = false;
	keep_last_row          = false;
	remove_hides           = false;
	full_col_resizing      = true;
	full_row_resizing      = false;
	chameleon              = false;
	summary_row            = false;
	update_summary         = true;
	popups                 = true;
	focus_lost_accepting   = false;

	mouse_move             = false;
	row_modified           = 0;

	valid_cursor           = false;

	curpos.x   = curpos.y  = -1;
	oldcur.x   = oldcur.y  = -1;
	curid.x    = curid.y   = -1;
	ctrlid.x   = ctrlid.y  = -1;
	ctrlpos.x  = ctrlpos.y = -1;
	osz.cx     = osz.cy    = -1;
	livecur.x  = livecur.y = -1;
	leftpnt.x  = leftpnt.y = -1;
	
	anchor = Null;

	fixed_width  = 0;
	fixed_height = 0;
	total_width  = 0;
	total_height = 0;
	summary_height = 0;

	ItemRect &ir = vitems.добавь();
	ir.parent = this;
	ir.edits = &edits;
	items.добавь();

	/* add indicator, total_cols = 1 */
	добавьКолонку("", 0);

	recalc_cols = false;
	recalc_rows = false;
	selected_rows = 0;
	selected_items = 0;

	WhenMenuBar = THISBACK(StdMenuBar);
	WhenToolBar = THISBACK(StdToolBar);

	find <<= THISBACK(DoFind);

	StdAppend = THISBACK(DoAppend);
	StdRemove = THISBACK(DoRemove);
	StdInsert = THISBACK(DoInsertBefore);
	StdDuplicate = THISBACK(DoDuplicate);
	StdEdit = THISBACK(DoEdit);

	newrow_inserted = false;
	newrow_appended = false;
	row_removed = false;
	just_clicked = false;

	call_whenchangecol = true;
	call_whenchangerow = true;
	call_whenremoverow = true;
	call_whenupdaterow = true;
	call_wheninsertrow = true;

	sel_begin = false;
	sel_end = false;

	moving_header = false;
	moving_body = false;
	moving_allowed = false;

	join_group = 0;

	curSplitCol = oldSplitCol = -1;
	curSplitRow = oldSplitRow = -1;

	moveCol = moveRow = -1;
	find_offset = 0;

	scrollLeftRight = false;

	fg_focus  = SColorHighlightText;
	bg_focus  = SColorHighlight;
	fg_select = чёрный;
	bg_select = Цвет(217, 198, 251);
	fg_live   = SColorText;
	bg_live   = IsDarkColorFace() ? смешай(SColorHighlight, чёрный, 132) : смешай(SColorHighlight, белый, 132);
	fg_found  = Цвет(0, 0, 0);
	bg_found  = смешай(SColorHighlight, Цвет(189,231,237), 200);
	fg_even   = SColorText;
	fg_odd    = SColorText;
	bg_even   = SColorPaper;
	bg_odd    = SColorPaper;
	fg_grid   = SColorShadow;

	focused_ctrl = NULL;
	focused_ctrl_id = -1;
	focused_col = -1;

	find.NullText(t_("ищи"));
	find.WhenBar = THISBACK(FindOptsBar);

	/* frames added at the very end, otherwise there will be strange crash in optimal mode... */
	sbx.автоСкрой();
	sby.автоСкрой();
	устФрейм(ViewFrame());
	добавьФрейм(sbx);
	добавьФрейм(sby);
	Ктрл::добавь(holder);

	resize_panel_open = false;
	resize_panel.WhenClose = прокси(WhenClose);

	resizing = false;
	is_clipboard = false;
	enabled = true;
	sync_flag = 0;
	paint_flag = 0;	
}

GridCtrl::~GridCtrl()
{
	delete orgdisp;
}

void GridCtrl::StdToolBar(Бар &bar)
{
	bool e = включен_ли();
	bool c = e && курсор_ли();
	bool d = c && IsRowEditable();

	if(appending)
		bar.добавь(e, t_("приставь"), GridImg::приставь(), StdAppend);

	if(inserting)
		bar.добавь(c, t_("вставь "), GridImg::вставь(), StdInsert);

	if(duplicating)
		bar.добавь(d && !isedit, t_("Duplicate"), GridImg::Duplicate(), StdDuplicate);

	if(removing)
		bar.добавь(d && (keep_last_row ? дайСчёт() > 1 : true), t_("Delete "), GridImg::Delete(), StdRemove);

	if(editing)
	{
		bar.добавь(!isedit && d, t_("Edit"), GridImg::Modify(), StdEdit);
		if(accepting)
			bar.добавь(isedit, t_("прими"), GridImg::Commit(), THISBACK(DoEndEdit));
		if(canceling)
			bar.добавь(isedit, t_("Cancel"), GridImg::Cancel(), THISBACK(DoCancelEdit));
	}

	if(searching)
	{
		if(inserting || appending || removing)
			bar.Separator();
		FindBar(bar, 150);
	}

	if(moving)
	{
		if(searching)
			bar.Separator();

		bar.добавь(c, t_("Move up"), GridImg::MoveUp(), THISBACK(DoSwapUp));
		bar.добавь(c, t_("Move down"), GridImg::MoveDn(), THISBACK(DoSwapDown));
	}

	if(navigating)
	{
		if(!closing)
			bar.GapRight();
		else
			bar.Separator();

		NavigatingBar(bar);
	}

	if(closing)
	{
		bar.GapRight();
		bar.добавь(close, 76, 24);
	}
}

void GridCtrl::FindBar(Бар &bar, int cx)
{
	bar.добавь(find, cx);
}

void GridCtrl::InfoBar(Бар &bar, int cx)
{
	bar.добавь(info, cx);
}

void GridCtrl::SetToolBarInfo(Ткст inf)
{
	info.устНадпись(inf);
}

void GridCtrl::NavigatingBar(Бар &bar)
{
	bar.добавь(фмтРяда(t_("First %s")), GridImg::FirstRec(), THISBACK(DoGoBegin));
	bar.добавь(фмтРяда(t_("Previous %s")), GridImg::PrevRec(), THISBACK(DoGoPrev));
	bar.добавь(фмтРяда(t_("следщ %s")), GridImg::NextRec(), THISBACK(DoGoNext));
	bar.добавь(фмтРяда(t_("последний %s")), GridImg::LastRec(), THISBACK(DoGoEnd));
}

GridCtrl& GridCtrl::SetToolBar(bool b, int align, int frame)
{
	удалиФрейм(bar);

	if(!b)
		return *this;

	вставьФрейм(frame, bar.Align(align));
	bar.устСтиль(ToolBar::дефСтиль());

	if(frame == 1)
		switch(align)
		{
			case КтрлБар::BAR_TOP:
				удалиФрейм(фреймВерхнСепаратора());
				вставьФрейм(2, фреймВерхнСепаратора());
				break;
			case КтрлБар::BAR_BOTTOM:
				удалиФрейм(фреймНижнСепаратора());
				вставьФрейм(2, фреймНижнСепаратора());
				break;
			case КтрлБар::BAR_LEFT:
				удалиФрейм(фреймЛевСепаратора());
				вставьФрейм(2, фреймЛевСепаратора());
				break;
			case КтрлБар::BAR_RIGHT:
				удалиФрейм(фреймПравСепаратора());
				вставьФрейм(2, фреймПравСепаратора());
				break;
		}
	WhenToolBar(bar);
	return *this;
}

GridCtrl& GridCtrl::ResizePanel(bool b)
{
	resize_panel_open = b;
	удалиФрейм(resize_panel);
	удалиФрейм(фреймНижнСепаратора());
	if(!b)
		return *this;
	вставьФрейм(1, resize_panel);
	вставьФрейм(2, фреймНижнСепаратора());
	return *this;
}

void GridCtrl::FindOptsBar(Бар &bar)
{
	bar.добавь(t_("Immediate search"), THISBACK1(SetFindOpts, 0)).Check(search_immediate);
	bar.добавь(t_("скрой rows"), THISBACK1(SetFindOpts, 1)).Check(search_hide);
	bar.добавь(t_("Highlight found cells"), THISBACK1(SetFindOpts, 2)).Check(search_highlight);
	bar.добавь(t_("Case sensitive"), THISBACK1(SetFindOpts, 3)).Check(search_case);
}

void GridCtrl::SetFindOpts(int n)
{
	switch(n)
	{
		case 0:
			search_immediate = !search_immediate;
			if(!search_immediate)
			{
				find <<= THISBACK(Nothing);
				find.WhenEnter = THISBACK(DoFind);
			}
			else
			{
				find <<= THISBACK(DoFind);
				find.WhenEnter = THISBACK(Nothing);
			}
			break;
		case 1:
			search_hide = !search_hide;
			if(!Ткст(~find).пустой())
			{
				if(!search_hide)
					ShowRows();
				else
					DoFind();
			}
			break;
		case 2:
			search_highlight = !search_highlight;
			if(!search_highlight)
			{
				ClearFound(false);
				освежи();
			}
			else
				DoFind();
			break;
		case 3:
			search_case = !search_case;
			DoFind();
			break;
	}
}

Ткст GridCtrl::фмтРяда(const char *s)
{
	Ткст row = t_("row");
	return спринтф(s, ~row);
}

void GridCtrl::StdMenuBar(Бар &bar)
{
	bool c = курсор_ли();
	bool e = c && IsRowEditable();
	bool isitem = false;

	if(inserting)
	{
		if(bains == 0)
		{
			bar.добавь(c, t_("вставь "), StdInsert)
			   .Рисунок(GridImg::вставь())
			   .Help(фмтРяда(t_("вставь a new %s into the table.")))
			   .Ключ(K_INSERT);
		}
		else if(bains == 1)
		{
			bar.добавь(c, t_("вставь before"), THISBACK(DoInsertBefore))
			   .Рисунок(GridImg::InsertBefore())
			   .Help(фмтРяда(t_("вставь a new %s into the table before current")))
			   .Ключ(K_INSERT);
			bar.добавь(c, t_("вставь after"), THISBACK(DoInsertAfter))
			   .Рисунок(GridImg::InsertAfter())
			   .Help(фмтРяда(t_("вставь a new %s into the table after current")))
			   .Ключ(K_ALT_INSERT);
		}
		else if(bains == 2)
		{
			bar.добавь(c, t_("вставь after"), THISBACK(DoInsertAfter))
			   .Рисунок(GridImg::InsertAfter())
			   .Help(фмтРяда(t_("вставь a new %s into the table after current")))
			   .Ключ(K_INSERT);
			bar.добавь(c, t_("вставь before"), THISBACK(DoInsertBefore))
			   .Рисунок(GridImg::InsertBefore())
			   .Help(фмтРяда(t_("вставь a new %s into the table before current")))
			   .Ключ(K_ALT_INSERT);
		}
		isitem = true;
	}
	
	if(appending)
	{
		bar.добавь(t_("приставь"), StdAppend)
		   .Рисунок(GridImg::приставь())
		   .Help(фмтРяда(t_("приставь a new %s at the end of the table.")))
		   .Ключ(inserting ? (dword) K_CTRL_INSERT : (dword) K_INSERT);

		isitem = true;
	}
	
	if(duplicating)
	{
		bar.добавь(c, t_("Duplicate"), THISBACK(DoDuplicate))
		   .Рисунок(GridImg::Duplicate())
		   .Help(фмтРяда(t_("Duplicate current table %s.")))
		   .Ключ(K_CTRL_D);

		isitem = true;
	}

	if(editing)
	{
		bar.добавь(!isedit && e, t_("Edit"), StdEdit)
		   .Рисунок(GridImg::Modify())
		   .Help(фмтРяда(t_("Edit active %s.")))
		   .Ключ(K_ENTER);

		isitem = true;
	}

	if(removing)
	{
		RemovingMenu(bar);
		isitem = true;
	}

	if(moving)
	{
		MovingMenu(bar);
		isitem = true;
	}

	if(multi_select)
	{
		SelectMenu(bar);
		isitem = true;
	}

	if(clipboard)
	{
		if(isitem)
			bar.Separator();
		ClipboardMenu(bar);
		isitem = true;
	}

	if(hiding)
	{
		if(isitem)
			bar.Separator();
		менюКолонок(bar);
	}
}

void GridCtrl::RemovingMenu(Бар &bar)
{
	bool c = курсор_ли() && IsRowEditable();
	bar.добавь(c && (keep_last_row ? дайСчёт() > 1 : true), t_("Delete "), StdRemove)
	   .Рисунок(GridImg::Delete())
	   .Help(фмтРяда(t_("Delete active %s.")))
	   .Ключ(K_DELETE);
}

void GridCtrl::MovingMenu(Бар &bar)
{
	bool c = курсор_ли();
	bar.добавь(c && curpos.y > fixed_rows, t_("Move up"), THISBACK(DoSwapUp))
	   .Рисунок(GridImg::MoveUp())
	   .Ключ(K_CTRL_UP);
	bar.добавь(c && curpos.y >= fixed_rows && curpos.y < total_rows - 1, t_("Move down"), THISBACK(DoSwapDown))
	   .Рисунок(GridImg::MoveDn())
	   .Ключ(K_CTRL_DOWN);
}

void GridCtrl::SelectMenu(Бар &bar)
{
	bar.добавь(total_rows > fixed_rows, фмтРяда(t_("выдели all")), THISBACK(выделиВсе))
	   .Рисунок(GridImg::выбериВсе())
	   .Help(t_("выдели all table rows"))
	   .Ключ(K_CTRL_A);
}

void GridCtrl::менюКолонок(Бар &bar)
{
	bar.добавь(t_("Columns"), THISBACK(СписокКолонок));
}

void GridCtrl::СписокКолонок(Бар &bar)
{
	int cnt = 0;
	for(int i = fixed_cols; i < total_cols; i++)
		if(!hitems[i].Индекс && !hitems[i].hidden)
			cnt++;

	for(int i = fixed_cols; i < total_cols; i++)
	{
		if(!hitems[i].Индекс)
			bar.добавь((Ткст) items[0][hitems[i].id].val, THISBACK1(MenuHideColumn, i))
			   .Check(!hitems[i].hidden)
			   .вкл(cnt > 1 || (cnt == 1 && hitems[i].hidden));
	}
}

void GridCtrl::ClipboardMenu(Бар &bar)
{
	bool c = курсор_ли();
	bool s = c || выделение_ли();
	bar.добавь(t_("копируй"), THISBACK(DoCopy)).Рисунок(CtrlImg::copy()).Ключ(K_CTRL_C).вкл(s && copy_allowed);
	if(cut_allowed)
		bar.добавь(t_("вырежь"), THISBACK(Nothing)).Рисунок(CtrlImg::cut()).Ключ(K_CTRL_X).вкл(s && cut_allowed);
	if(paste_allowed)
		bar.добавь(t_("Paste"), THISBACK(DoPaste)).Рисунок(CtrlImg::paste()).Ключ(K_CTRL_V).вкл(c && paste_allowed && IsClipboardAvailable());
	if(extra_paste)
		bar.добавь(t_("Paste as"), THISBACK(PasteAsMenu));
}

void GridCtrl::PasteAsMenu(Бар &bar)
{
	bool c = курсор_ли();
	bool s = IsClipboardAvailable() && !fixed_paste;
	bar.добавь(t_("appended"), THISBACK(DoPasteAppendedRows)).Ключ(K_CTRL_E).вкл(s && paste_allowed);
	bar.добавь(t_("inserted"), THISBACK(DoPasteInsertedRows)).Ключ(K_CTRL_I).вкл(c && paste_allowed && s);
}

bool GridCtrl::IsClipboardAvailable()
{
	return IsClipboardFormatAvailable<GridClipboard>() ||
	       IsClipboardAvailableText();
}

GridClipboard GridCtrl::GetClipboard()
{
	GridClipboard gc = ReadClipboardFormat<GridClipboard>();
	return gc;
}

void GridCtrl::SetClipboard(bool all, bool silent)
{
	if(!clipboard)
		return;

	GridClipboard gc;

	Точка minpos(total_cols, total_rows);
	Точка maxpos(fixed_cols, fixed_rows);

	Ткст body;
	Вектор<int> sc;
	sc.уст(0, -1, total_cols);
	
	int prev_row = -1;

	for(int i = fixed_rows; i < total_rows; i++)
	{
		if(vitems[i].hidden) continue;
		bool row_selected = select_row && выделен(i, false);
		
		for(int j = fixed_cols; j < total_cols; j++)
		{
			if(all || row_selected || выделен(i, j, false))
			{
				if(prev_row < 0)
					prev_row = i;

				GridClipboard::ClipboardData &d = gc.data.добавь();
				d.col = j;
				d.row = i;
				d.v = Get0(i, j);

				if(i < minpos.y) minpos.y = i;
				else if(i > maxpos.y) maxpos.y = i;
				if(j < minpos.x) minpos.x = j;
				else if(j > maxpos.x) maxpos.x = j;

				if(i != prev_row)
				{
					body += "\r\n";
					prev_row = i;
				}
				body += GetStdConvertedColumn(j, d.v).вТкст() + '\t';
				
				sc[j] = 1;
			}
		}
		
		int cnt = body.дайСчёт();
		if(cnt > 0 && body[cnt - 1] == '\t')
			body.удали(cnt - 1);
	}
	
	Ткст header;
	
	if(copy_column_names)
	{
		for(int i = 0; i < sc.дайСчёт(); i++)
			if(sc[i] >= 0)
				header += hitems[i].дайИмя() + '\t';
		
		int cnt = header.дайСчёт();
		if(cnt > 0 && header[cnt - 1] == '\t')
			header.удали(cnt - 1);
		header += "\r\n";
	}
		
	gc.minpos = minpos;
	gc.maxpos = maxpos;

	WriteClipboardFormat(gc);
	AppendClipboardText(header + body);

	if(!silent)
	{
		Цвет c0 = bg_select;
		Цвет c1 = белый;
		Цвет c2 = bg_focus;
	
		for(int i = 0; i < 256; i += 64)
		{
			bg_select = смешай(c0, c1, i);
			bg_focus = смешай(c2, c1, i);
			освежи(); синх();
			спи(1);
		}
	
		for(int i = 0; i < 256; i += 32)
		{
			bg_select = смешай(c1, c0, i);
			bg_focus = смешай(c1, c2, i);
			освежи(); синх();
			спи(1);
		}
	}
}

void GridCtrl::PasteCallbacks(bool new_row)
{
	if(new_row)
	{
		LOG("WhenInsertRow() - paste");
		#ifdef LOG_CALLBACKS
		LGR(2, "WhenInsertRow() - paste");
		#endif
		WhenInsertRow();
	}
	else
	{
		#ifdef LOG_CALLBACKS
		LGR(2, "WhenUpdateRow() - paste");
		#endif
		WhenUpdateRow();
	}
}

void GridCtrl::Paste(int mode)
{
	if(!clipboard)
		return;
	
	GridClipboard gc = GetClipboard();

	bool is_gc = !gc.data.пустой();
	bool is_tc = IsClipboardAvailableText();

	if(!is_gc && !is_tc)
		return;

	if(is_gc && select_row && !gc.shiftmode)
		return;

	Точка cp(curpos);

	if(cp.x < 0 || select_row)
		cp.x = fixed_cols;
	if(cp.y < 0)
		cp.y = fixed_rows;

	Вектор<Ткст> lines;

	Ткст s = изЮникода(ReadClipboardUnicodeText());

	WhenPasting(s);

	if(is_tc && !is_gc)
		lines = РНЦП::разбей(s, '\n');

	if(mode == 1)
	{
		int dy = is_gc ? gc.maxpos.y - gc.minpos.y + 1
		               : lines.дайСчёт();
		вставь0(curpos.y, dy);
		curpos.y += dy;
	}
	else if(mode == 2)
		cp.y = total_rows;

	int lc = -1, lr = -1;

	int tr = total_rows;

	if(!is_gc)
	{
		if(is_tc)
		{
			int pr = 0;
			bool new_row = false;
			
			for(int i = 0; i < lines.дайСчёт(); i++)
			{
				Ткст line = обрежьПраво(lines[i]);
				Вектор<Ткст> cells = РНЦП::разбей(line, '\t', false);
				for(int j = 0; j < cells.дайСчёт(); j++)
				{
					int r = i;
					int c = j;

					if(r > pr)
					{
						PasteCallbacks(new_row);
						pr = r;
					}

					if(cp.x + c < total_cols)
					{
						lc = cp.x + c;
						lr = cp.y + r;

						rowidx = lr;

						new_row = lr >= tr || mode > 0;
						
						if(fixed_paste && new_row)
							break;
						
						Значение v(cells[j]);
						WhenPasteCell(lr - fixed_rows, lc - fixed_cols, v);
						уст0(lr, lc, v, true);
					}

					if(i == lines.дайСчёт() - 1 && j == cells.дайСчёт() - 1)
						PasteCallbacks(new_row);
				}
			}
		}
	}
	else if(!select_row && gc.shiftmode)
	{
		lc = cp.x;
		lr = cp.y;

		for(int i = 0; i < gc.data.дайСчёт(); i++)
		{
			уст0(lr, lc, gc.data[i].v, true);

			bool data_end = i == gc.data.дайСчёт() - 1;
			bool new_row = ++lc > total_cols - 1;
			if(new_row || data_end)
			{
				bool nr = lr + 1 >= tr;
				if(new_row && nr && fixed_paste)
					break;

				PasteCallbacks(new_row && (nr || mode > 0));

				if(!data_end)
				{
					lc = fixed_cols;
					++lr;
					rowidx = lr;
				}
			}
		}
	}
	else
	{
		int pr = gc.data[0].row - gc.minpos.y;
		bool new_row = false;

		for(int i = 0; i < gc.data.дайСчёт(); i++)
		{
			int r = gc.data[i].row - gc.minpos.y;
			int c = gc.data[i].col - gc.minpos.x;

			if(r > pr)
			{
				PasteCallbacks(new_row);
				pr = r;
			}

			if(cp.x + c < total_cols)
			{
				lc = cp.x + c;
				lr = cp.y + r;

				rowidx = lr;

				new_row = lr >= tr || mode > 0;

				if(fixed_paste && new_row)
					break;
				уст0(lr, lc, gc.data[i].v, true);
			}

			if(i == gc.data.дайСчёт() - 1)
				PasteCallbacks(new_row);
		}
	}
	
	if(lc >= 0 && lr >= 0)
	{
		устКурсор0(lc, lr);
		sby.уст(vitems[curpos.y].nBottom() - дайРазм().cy);
	}
	WhenPaste();
	очистьВыделение();
}

void GridCtrl::DoCopy()
{
	SetClipboard();
}

void GridCtrl::DoPaste()
{
	Paste(0);
}

void GridCtrl::DoPasteInsertedRows()
{
	Paste(1);
}

void GridCtrl::DoPasteAppendedRows()
{
	Paste(2);
}

void GridCtrl::SetOrder()
{
	row_order = true;
	WhenChangeOrder();
}

void GridCtrl::Nothing()
{
}

void GridCtrl::DrawLine(bool iniLine, bool delLine)
{
	if((resizeCol || resizeRow) && resize_paint_mode < 2)
	{
		int sx = resize_paint_mode == 1 ? fixed_width : 0;
		int sy = resize_paint_mode == 1 ? fixed_height : 0;
		ViewDraw w(this);
		Размер sz = дайРазм();

		Точка curPnt;
		static Точка oldPnt = curPnt;

		if(resizeCol)
		{
			curPnt.x = hitems[splitCol].nRight(sbx) - 1;
			if(delLine) w.DrawRect(oldPnt.x, sy, 1, sz.cy, InvertColor());
			if(iniLine) w.DrawRect(curPnt.x, sy, 1, sz.cy, InvertColor());
	    }
		if(resizeRow)
		{
			curPnt.y = vitems[splitRow].nBottom(sby) - 1;
			if(delLine) w.DrawRect(sx, oldPnt.y, sz.cx, 1, InvertColor());
			if(iniLine) w.DrawRect(sx, curPnt.y, sz.cx, 1, InvertColor());
		}

		oldPnt = curPnt;
	}
}

Значение GridCtrl::GetItemValue(const Элемент& it, int id, const ItemRect& hi, const ItemRect& vi)
{
	Значение val = hi.IsConvertion() && vi.IsConvertion() 
		? GetConvertedColumn(id, it.val)
		: it.val;
	
	return val;
}

Значение GridCtrl::GetAttrTextVal(const Значение& val)
{
	if(IsType<AttrText>(val))
	{
		const AttrText& t = ValueTo<AttrText>(val);
		return t.text;
	}
	return val;
}

void GridCtrl::GetItemAttrs(const Элемент& it, const Значение& значение, int r, int c, const ItemRect& vi, const ItemRect& hi, dword& style, GridDisplay*& gd, Цвет& fg, Цвет& bg, Шрифт& fnt)
{
	if(!пусто_ли(vi.fg))
		fg = vi.fg;
	else if(!пусто_ли(hi.fg))
		fg = hi.fg;

	if(!пусто_ли(vi.bg))
		bg = vi.bg;
	else if(!пусто_ли(hi.bg))
		bg = hi.bg;

	fnt = StdFont();
	
	if(r < fixed_rows && !пусто_ли(hi.hfnt))
		fnt = hi.hfnt;
	else if(c < fixed_cols && !пусто_ли(vi.hfnt))
		fnt = vi.hfnt;
	else if(!пусто_ли(vi.fnt))
		fnt = vi.fnt;
	else if(!пусто_ли(hi.fnt))
		fnt = hi.fnt;

	GridDisplay * hd = hi.дисплей;
	GridDisplay * vd = vi.дисплей;
	gd = дисплей;
	if(!hi.ignore_display && !vi.ignore_display)
		gd = vd ? vd : (hd ? hd : (it.дисплей ? it.дисплей : дисплей));
	
	gd->SetLeftImage(Null);
	
	const Значение& val = пусто_ли(значение) ? it.val : значение;

	if(IsType<AttrText>(val))
	{
		const AttrText& t = ValueTo<AttrText>(val);
		
		if(!пусто_ли(t.paper)) bg  = t.paper;
		if(!пусто_ли(t.ink))   fg  = t.ink;
		if(!пусто_ли(t.font))  fnt = t.font;
		dword s = 0;
		if(!пусто_ли(t.align))
		{
			if(t.align == ALIGN_LEFT)
				s = GD::LEFT;
			else if(t.align == ALIGN_RIGHT)
				s = GD::RIGHT;
			else if(t.align == ALIGN_CENTER)
				s = GD::HCENTER;
			style &= ~GD::HALIGN;
			style |= s;
		}
		if(!пусто_ли(t.img))
			gd->SetLeftImage(t.img);
	}
	
}

GridCtrl::Элемент& GridCtrl::GetItemSize(int &r, int &c, int &x, int &y, int &cx, int &cy, bool &skip, bool relx, bool rely)
{
	int idx = hitems[c].id;
	int idy = vitems[r].id;

	int dx = 0;
	int dy = 0;

	Элемент *it = &items[idy][idx];

	if(it->isjoined)
	{
		int группа = it->группа;
		it = &items[it->idy][it->idx];
		skip = it->paint_flag == paint_flag;
		it->paint_flag = paint_flag;
		if(skip)
			return *it;

		while(c >= 0 && items[idy][hitems[c].id].группа == группа) --c;
		++c;
		while(r >= 0 && items[vitems[r].id][idx].группа == группа) --r;
		++r;

		dx = it->cx;
		dy = it->cy;
	}
	else
		skip = false;

	x  = hitems[c].nLeft();
	y  = vitems[r].nTop();
	cx = hitems[c + dx].nRight() - x;
	cy = vitems[r + dy].nBottom() - y;

	if(!draw_last_vert_line && c == total_cols - 1 && r >= fixed_rows) cx += 1;
	if(!draw_last_horz_line && r == total_rows - 1 && c >= fixed_cols) cy += 1;

	if(relx) x -= sbx;
	if(rely) y -= sby;

	return *it;
}

void GridCtrl::DrawHorzDragLine(Draw &w, int pos, int cx, int size, Цвет c)
{
	//w.DrawRect(pos, 0, cx / 2, size - 1, Цвет(100, 100, 100, 100);
	DrawFrame(w, pos + 1, 1, cx / 2 - 2, size - 2, c);
	DrawFrame(w, pos, 0, cx / 2, size, c);
//	DrawFatFrame(w, x, 0,ďż˝ int cxďż˝ int cyďż˝ Цвет colorďż˝ int n)
}

void GridCtrl::DrawVertDragLine(Draw &w, int pos, int size, int dx, Цвет c)
{
	w.DrawRect(1 + dx, pos, size - dx - 1, 2, c);
}

Значение GridCtrl::GetStdConvertedValue(const Значение& v) const
{
	if(ткст_ли(v))
	{
		return v;
	}
	else if(IsType<AttrText>(v))
	{
		const AttrText& t = ValueTo<AttrText>(v);
		return t.text;
	}
	else
		return стдПреобр().фмт(v);
}

Значение GridCtrl::GetConvertedColumn(int col, const Значение &v) const
{
	Преобр *conv = edits[col].convert;
	return conv ? conv->фмт(v) : v;
}

Значение GridCtrl::GetStdConvertedColumn(int col, const Значение &v) const
{
	Значение val = GetConvertedColumn(col, v);
	return GetStdConvertedValue(val);
}

Ткст GridCtrl::дайТкст(Ид id) const
{
	int c = aliases.дай(id);
	return GetStdConvertedColumn(c, Get0(rowidx, c));
}

GridCtrl::ItemRect& GridCtrl::InsertColumn(int col, const char *имя, int size, bool idx)
{
	int id;
	
	if(size < 0)
		size = GD_COL_WIDTH;

	if(col < total_cols)
	{
		id = hitems[col].id;
		for(int i = 0; i < total_cols; i++)
		{
			if(hitems[i].id >= id)
				hitems[i].id += 1;			
		}
	}
	else
		id = total_cols;

	ItemRect& ir = hitems.вставь(col);
	ir.parent = this;
	ir.items = &items;
	ir.edits = &edits;
	ir.prop = size;
	ir.id = id;
	ir.uid = coluid++;
	ir.Индекс = idx;

	if(index_as_column && idx)
	{
		size = 70;
		ir.prop = size;
		ir.фиксирован(size);
	}
	
	ir.размер(size);
	aliases.добавь(впроп(имя), id);
	rowbkp.вставь(id);
	edits.вставь(id);
	summary.вставь(id);
	
	for(int i = 0; i < total_rows; i++)
		items[i].вставь(id);
	
	items[0][id].val = имя;

	colidx = col;
	total_cols++;

	UpdateJoins(-1, col, 1);

	firstCol = -1;

	if(ready)
	{
		RecalcCols();
		UpdateSizes();
		UpdateSb();
		Syncсуммаmary();
		SyncCtrls();
		освежи(); //RefreshFromCol??
	}
	else
		recalc_cols = true;

	SetModify();

	return  hitems[col];
}

GridCtrl::ItemRect& GridCtrl::добавьКолонку(const char *имя, int size, bool idx)
{
	ItemRect::aliases = &aliases;
	
	if(size < 0)
		size = GD_COL_WIDTH;

	if(total_rows > 1)
		for(int i = 1; i < total_rows; ++i)
			items[i].добавь();
	else
		total_rows = 1;

	Элемент &it = items[0].добавь();
	it.val = имя;

	ItemRect &ib = hitems.добавь();

	ib.parent = this;
	ib.items  = &items;
	ib.edits  = &edits;
	ib.prop   = size;
	ib.id     = total_cols;
	ib.uid    = coluid++;
	ib.Индекс  = idx;

	if(index_as_column && idx)
	{
		size = 70;
		ib.prop = size;
		ib.фиксирован(size);
	}
	
	ib.размер(size);
	if(!ib.hidden)
	{
		lastVisCol = total_cols;
		if(firstVisCol < 0)
			firstVisCol = lastVisCol;
	}

	if(header && vitems[0].nsize == 0)
	{
		vitems[0].size = vitems[0].nsize = GD_HDR_HEIGHT;
		vitems[0].hidden = false;
	}
	if(!header)
	{
		vitems[0].size = vitems[0].nsize = 0;
		vitems[0].hidden = true;
	}
	
	edits.добавь();
	summary.добавь();
	rowbkp.добавь();
	aliases.добавь(впроп(имя), ib.id);
	total_cols++;

	if(ready && открыт())
	{
		recalc_cols = true;
		освежиВыкладку();
	}

	return ib;
}

GridCtrl::ItemRect& GridCtrl::добавьКолонку(const Ид id, const char *имя, int size, bool idx)
{
	return добавьКолонку(имя ? имя : (const char *) ~id, size, idx).Alias(id);
}

GridCtrl::ItemRect& GridCtrl::добавьКолонку(const Ткст& имя, int size, bool idx)
{
	return добавьКолонку((const char *) имя, size, idx);
}

void GridCtrl::RemoveColumn(int n, int count)
{
	n += fixed_cols;
	if(count < 0)
		count = total_cols - n;
	if(n < fixed_cols || n + count > total_cols)
		return;
	for(int i = 0; i < total_rows; i++)
		items[i].удали(n, count);
	
	Вектор<int> r;
	for(int i = 0; i < count; i++)
	{
		if(edits[hitems[n + i].id].factory)
			--genr_ctrls;
		r.добавь(hitems[n + i].id);
	}

	int id = hitems[n].id;

	РНЦП::сортируй(r);
		
	hitems.удали(n, count);

	rowbkp.удали(r);
	summary.удали(r);
	edits.удали(r);
	total_cols -= count;
	recalc_cols = true;

	for(int i = 0; i < total_cols; i++)
		if(hitems[i].id >= id)
			hitems[i].id -= count;

	valid_cursor = устКурсор0(min(curpos.x, total_cols - 1), curpos.y).пригоден();
	Repaint(true);
}

GridCtrl::ItemRect& GridCtrl::AddRow(int n, int size)
{
	Append0(n, size);
	return GetRow();
}

GridCtrl& GridCtrl::добавьСепаратор(Цвет c)
{
	Append0(1, 3);
	ItemRect& ir = GetRow();
	ir.Bg(c);
	ir.Editable(false);
	ir.кликаем(false);
	ir.пропусти(true);
	return *this;
}

void GridCtrl::SetRowCount(int n, int size)
{
	очисть();
	Append0(n, size);
}

void GridCtrl::SetColCount(int n, int size)
{
	переустанов();
	for(int i = 0; i < n; i++)
		добавьКолонку("", size, false);
}

void GridCtrl::SyncPopup()
{
	if(!IsPopUp() && popups)
	{
		Точка p = GetMouseViewPos();
		
		bool fc = p.x < fixed_width;
		bool fr = p.y < fixed_height;
		
		int c = GetMouseCol(p, !fc, fc);
		int r = GetMouseRow(p, !fr, fr);
				
		bool new_cell = false;
		
		if(r != oldSplitRow)
		{
			oldSplitRow = r;
			new_cell = true;
		}
		if(c != oldSplitCol)
		{
			oldSplitCol = c;
			new_cell = true;
		}
		
		bool valid_pos = c >= 0 && r >= 0;
		
		Ктрл* ctrl = valid_pos ? дайКтрл(r, c, true, false, false, false) : NULL;
		
		bool close = popup.открыт();

		if(valid_pos && !ctrl)
		{
			Элемент& it = дайЭлт(r, c);
			ItemRect& hi = hitems[c];
			ItemRect& vi = vitems[r];
			
			if(it.rcx > 0 || it.rcy > 0)
			{
				close = false;

				Значение val = GetStdConvertedValue(r == 0 ? it.val : GetItemValue(it, hi.id, hi, vi));

				if(new_cell)
				{
					popup.fg = SColorText;
					popup.bg = SColorPaper;
					popup.fnt = StdFont();
					popup.style = 0;
					popup.val = val;

					Точка p0 = дайПозМыши();
					int x = hi.npos + p0.x - p.x - 1 - sbx.дай() * int(!fc);
					int y = vi.npos + p0.y - p.y - 1 - sby.дай() * int(!fr);
					
					GetItemAttrs(it, Null, r, c, vi, hi, popup.style, popup.gd, popup.fg, popup.bg, popup.fnt);
					popup.gd->row = r < fixed_rows ? -1 : r - fixed_rows;
					popup.gd->col = c < fixed_cols ? -1 : c - fixed_cols;
					Прям scr = GetWorkArea();
					int margin = popup.gd->lm + popup.gd->rm;
					int cx = min(600, min((int) (scr.right * 0.4), max(it.rcx + margin + 2, hi.nsize + 1)));
					int lines = popup.gd->GetLinesCount(cx - margin - 2, ШТкст(val), popup.fnt, true);
					int cy = max(lines * Draw::GetStdFontCy() + popup.gd->tm + popup.gd->bm + 2, vi.nsize + 1);
					if(fr && r == 0)
					{
						y++;
						cy--;
					}
					popup.PopUp(this, x, y, cx, cy);
					if(!close)
						popup.освежи();					
					UpdateHighlighting(GS_BORDER, Точка(0, 0));
				}				
				else if(val != popup.val)
				{
					popup.val = val;
					popup.освежи();
				}
			}
		}
		
		if(close)
		{
			popup.закрой();
			oldSplitCol = -1;
			oldSplitRow = -1;
			UpdateHighlighting(GS_BORDER, Точка(0, 0));
		}
	}
}

void GridCtrl::иниц()
{
	bar.уст(WhenToolBar);
	UpdateCols(true);
	/* recalc_rows bo przed otworzeniem grida moglo zostac wywolane setrowheight */
	UpdateRows(resize_row_mode > 0 || recalc_rows);

	UpdateSizes();
	UpdateSb();
	UpdateHolder(true);
	Syncсуммаmary();
	SyncCtrls();
}

void GridCtrl::State(int reason)
{
	if(reason == OPEN)
	{
		иниц();
		ready = true;
		//ready po init - updatesb wola layout() a ten syncctrl
		//(ktory w sumie wola sie 3 razy zanim grid sie wyswietli - niepotrzebnie)
	}
	else if(reason == CLOSE)
	{
		if(live_cursor)
			устКурсор0(-1, -1, CU_HIGHLIGHT);
	}
	else if(reason == ENABLE)
	{
		bool e = включен_ли();
		if(e != enabled)
		{
			RebuildToolBar();
			enabled = e;
		}
	}
}

void GridCtrl::Выкладка()
{
	if(!ready)
		return;

	UpdateCols();
	UpdateRows();
	UpdateSizes();
	UpdateSb();
	UpdateHolder();
	UpdateCtrls(UC_CHECK_VIS | UC_SHOW);
	SyncCtrls();
}

void GridCtrl::ChildAction(Ктрл *child, int event)
{
	if(child != &holder && child->показан_ли())
		popup.закрой();
	
	if(child != focused_ctrl)
	{
		if(event == LEFTDOWN || event == RIGHTDOWN || event == MOUSEWHEEL)
		{
			//LG(2, "got event :%x child: %x", event, child);
			Точка cp = GetCtrlPos(child);
			if(cp.x < 0 || cp.y < 0)
				return;
			
			устКурсор0(cp);
			UpdateCtrls(UC_SHOW);
			WhenCtrlAction();
		}
	}

	if(event == MOUSEMOVE)
	{
		if(live_cursor)
		{
			LG(2, "Child:LiveCursor");
			Точка p = GetMouseViewPos();
			if(IsMouseBody(p))
				устКурсор0(p, CU_MOUSE | CU_HIGHLIGHT);
			else
				устКурсор0(-1, -1, CU_HIGHLIGHT);
		}
	}
	
}

void GridCtrl::ChildMouseEvent(Ктрл *child, int event, Точка p, int zdelta, dword keyflags)
{
	ChildAction(child, event);
	Ктрл::ChildMouseEvent(child, event, p, zdelta, keyflags);
}

void GridCtrl::ChildFrameMouseEvent(Ктрл *child, int event, Точка p, int zdelta, dword keyflags)
{
	ChildAction(child, event);
	Ктрл::ChildFrameMouseEvent(child, event, p, zdelta, keyflags);
}

void GridCtrl::тягИБрос(Точка p, PasteClip& d)
{
	/*
	moving_body = true;
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
	*/
}

Прям GridCtrl::дайПрямЭлта(int r, int c, bool hgrid, bool vgrid, bool hrel, bool vrel)
{
	int dx = sbx + (hrel ? fixed_width : 0);
	int dy = sby + (vrel ? fixed_height : 0);

	int idx = hitems[c].id;
	int idy = vitems[r].id;

	Элемент &it = items[idy][idx];

	int left, top, right, bottom;

	if(it.isjoined)
	{
		int группа = it.группа;

		while(r > fixed_rows && items[vitems[r].id][idx].группа == группа) --r;
		++r;

		top = vitems[r].nTop(dy);
		bottom = vitems[r + it.cy].nBottom(dy);

		while(c > fixed_cols && items[idy][hitems[c].id].группа == группа) --c;
		++c;

		left = hitems[c].nLeft(dx);
		right = hitems[c + it.cx].nRight(dx);
	}
	else
	{
		left = hitems[c].nLeft(dx);
		top = vitems[r].nTop(dy);
		right = hitems[c].nRight(dx);
		bottom = vitems[r].nBottom(dy);
	}

	return Прям(left, top, right - (int) vgrid, bottom - (int) hgrid);
}

Прям& GridCtrl::AlignRect(Прям &r, int i)
{
	Прям c(r);
	int align = hitems[i].calign;
	int sx = hitems[i].sx;
	int sy = hitems[i].sy;

	if(sx > 0)
	{
		if(align & GD::HCENTER)
		{
			int d = (r.устШирину() - sx - 1) / 2;
			r.left += d;
			r.right -= d;
		}
		else if(align & GD::LEFT)
		{
			r.left += hitems[i].sl;
			r.right = r.left + sx;
		}
		else if(align & GD::RIGHT)
		{
			r.right -= hitems[i].sr;
			r.left = r.right - sx;
		}
		else if(align & GD::HPOS)
		{
			r.left += hitems[i].sl;
			r.right -= hitems[i].sr;
		}
	}

	if(sy > 0)
	{
		if(align & GD::VCENTER)
		{
			int d = (r.устВысоту() - sy - 1) / 2;
			r.top += d;
			r.bottom -= d;
		}
		else if(align & GD::TOP)
		{
			r.top += hitems[i].st;
			r.bottom = r.top + sy;
		}
		else if(align & GD::RIGHT)
		{
			r.bottom -= hitems[i].sb;
			r.top = r.bottom - sy;
		}
		else if(align & GD::VPOS)
		{
			r.top += hitems[i].st;
			r.bottom -= hitems[i].sb;
		}
	}

	if(r.left   < c.left)   r.left   = c.left;
	if(r.right  > c.right)  r.right  = c.right;
	if(r.top    < c.top)    r.top    = c.top;
	if(r.bottom > c.bottom) r.bottom = c.bottom;

	return r;
}


void GridCtrl::промотай()
{
	Точка newpos(sbx, sby);
	Размер delta = oldpos - newpos;
	oldpos = newpos;

	if(delta.cx != 0) firstCol = -1;
	if(delta.cy != 0) firstRow = -1;

	if(!doscroll)
		return;

	LG(0, "промотай (%d, %d)", delta.cx, delta.cy);

	SyncCtrls();
	UpdateCtrls(UC_CHECK_VIS | UC_SHOW | UC_SCROLL);

	if(resizeCol || resizeRow)
		return;

	if(!IsFullRefresh())
	{
		Размер sz = дайРазм();
		holder.промотайОбзор(delta);
		if(delta.cx != 0)
		{
			промотайОбзор(Прям(fixed_width, 0, sz.cx, fixed_height), delta.cx, 0);
			промотайОбзор(Прям(fixed_width, sz.cy - summary_height, sz.cx, sz.cy), delta.cx, 0);
			scrollLeftRight = true;
		}
		if(delta.cy != 0)
		{
			промотайОбзор(Прям(0, fixed_height, fixed_width, sz.cy - summary_height), 0, delta.cy);
		}
	}

	if(live_cursor)
		устКурсор0(дайПозМыши() - дайПрямЭкрана().верхЛево(), CU_MOUSE | CU_HIGHLIGHT);

	SyncPopup();
}

void GridCtrl::SetFixedRows(int n)
{
	if(n >= 0 && n <= total_rows)
	{
		LG(0, "SetFixedRows");
		fixed_rows = n;
		firstRow = -1;
		UpdateSizes();
		UpdateHolder();
		UpdateVisColRow(false);
		if(ready)
			SyncCtrls();
		освежи();
	}
}

void GridCtrl::SetFixedCols(int n)
{
	if(n >= 0 && n < total_cols)
	{
		LG(0, "SetFixedCols");
		fixed_cols = n + 1; /* +1 - indicator! */
		firstCol = -1;
		UpdateSizes();
		UpdateHolder();
		UpdateVisColRow(true);
		if(ready)
			SyncCtrls();
		освежи();
	}
}

void GridCtrl::уст0(int r, int c, const Значение &val_, bool paste)
{
	Значение val = val_;
	if(c > total_cols - 1)
		return;
	if(r > total_rows - 1)
		AddRow(r - total_rows + 1);

	vitems[r].operation = GridOperation::UPDATE;
	
	int ri = vitems[r].id;
	Элемент &it = items[ri][c];

	if(it.isjoined) {
		ri = it.idy;
		c  = it.idx;
	}

	Ктрл *ctrl = items[ri][c].ctrl;
	bool  setctrl = true;
	if(!ctrl) {
		ctrl = edits[c].ctrl;
		setctrl = ctrlid.y == ri;
	}
	
	if(ctrl) {
		if(paste && ткст_ли(val)) {
			Преобр *cv = dynamic_cast<Преобр *>(ctrl);
			if(cv)
				val = cv->скан(val);
		}
		if(setctrl)
			ctrl->устДанные(val);
	}

	items[ri][c].val = val;
	освежиЭлт(r, c, false);

	if(paste)
		WhenUpdateCell();

	Syncсуммаmary();
}

void GridCtrl::уст(int r, int c, const Значение &val)
{
	уст0(r + fixed_rows, c + fixed_cols, val);
}

void GridCtrl::уст(int r, Ид id, const Значение &val)
{
	уст0(r + fixed_rows, aliases.дай(id), val);
}

void GridCtrl::уст(int r, const char *s, const Значение &val)
{
	уст0(r + fixed_rows, aliases.дай(s), val);
}

void GridCtrl::уст(int c, const Значение &val)
{
	уст0(rowidx, c + fixed_cols, val);
}

void GridCtrl::уст(Ид id, const Значение &val)
{
	уст0(rowidx, aliases.дай(id), val);
}

void GridCtrl::уст(int r, const Вектор<Значение> &v, int data_offset /* = 0*/, int column_offset /* = 0*/)
{
	r += fixed_rows;
	vitems[r].operation = GridOperation::UPDATE;
	int cnt = min(v.дайСчёт(), total_cols - fixed_cols);
	int r0 = vitems[r].id;
	int c = fixed_cols + column_offset;
	for(int i = data_offset; i < cnt; i++)
		items[r0][c++].val = v[i];

	RefreshRow(r, false, 0);
}

void GridCtrl::уст(const Вектор<Значение> &v, int data_offset /* = 0*/, int column_offset /* = 0*/)
{
	int r = rowidx - fixed_rows;
	уст(r, v, data_offset, column_offset);
}

void GridCtrl::SetAny(int r, int c, const Значение &val)
{
	уст0(r, c, val);
}

void GridCtrl::SetRaw(int r, int c, const Значение &val)
{
	items[r][c].val = val;
}

void GridCtrl::SetIndicator(int r, const Значение &val)
{
	уст0(r, 0, val);
}

void GridCtrl::устКтрл(int r, int c, Ктрл& ctrl)
{
	r += fixed_rows;
	c += fixed_cols;
	дайЭлт(r, c).устКтрл(ctrl, false);
	++genr_ctrls;
	if(ready)
		SyncCtrls(r, c);
}

void GridCtrl::устКтрл(int r, int c, Ктрл* ctrl)
{
	r += fixed_rows;
	c += fixed_cols;
	дайЭлт(r, c).устКтрл(*ctrl, true);
	++genr_ctrls;
	if(ready)	
		SyncCtrls(r, c);
}

void GridCtrl::устКтрл(int c, Ктрл& ctrl)
{
	c += fixed_cols;
	дайЭлт(rowidx, c).устКтрл(ctrl, false);
	++genr_ctrls;
	if(ready)
		SyncCtrls(rowidx, c);
}

void GridCtrl::устКтрл(int c, Ктрл* ctrl)
{
	c += fixed_cols;
	дайЭлт(rowidx, c).устКтрл(*ctrl, true);
	++genr_ctrls;
	if(ready)
		SyncCtrls(rowidx, c);
}

void GridCtrl::ClearCtrl(int r, int c)
{
	GridCtrl::Элемент& элт = дайЭлт(r + fixed_rows, c + fixed_cols);
	if(элт.ctrl)
	{
		элт.ClearCtrl();
		--genr_ctrls;
	}
}

void GridCtrl::устЗначениеКтрл(int r, int c, const Значение &val)
{
	c += fixed_cols;
	int ri = vitems[r].id;
	Ктрл * ctrl = items[ri][c].ctrl;
	if(ctrl)
		ctrl->устДанные(val);
	else
	{
		ctrl = edits[c].ctrl;
		if(ctrl && ctrlid.y == ri)
			ctrl->устДанные(val);
	}
}

void GridCtrl::устЗначениеКтрл(int c, const Значение &val)
{
	устЗначениеКтрл(rowidx, c, val);
}

void GridCtrl::SetLast(int c, const Значение &val)
{
	c += fixed_cols;
	items[vitems[rowidx].id][c].val = val;
	освежиЭлт(rowidx, c, false);
}

void GridCtrl::SetFixed(int r, int c, const Значение &val)
{
	items[r][c + 1].val = val;
	освежи();
}

Значение GridCtrl::GetFixed(int r, int c) const
{
	return items[vitems[r].id][c + fixed_cols].val;
}

Значение GridCtrl::GetFixed(int c) const
{
	return items[0][c + fixed_cols].val;
}

Значение GridCtrl::Get0(int r, int c) const
{
	r = vitems[r].id;
	const Элемент &it = items[r][c];
	if(it.isjoined)
	{
		r = it.idy;
		c = it.idx;
	}

	Ктрл * ctrl = items[r][c].ctrl;

	if(!ctrl && ctrlid.y == r)
		ctrl = edits[c].ctrl;

	return ctrl ? ctrl->дайДанные() : items[r][c].val;
}

Значение GridCtrl::дай(int r, int c) const
{
	return Get0(r + fixed_rows, c + fixed_cols);
}

Значение GridCtrl::дай(int c) const
{
	return Get0(rowidx, c + fixed_cols);
}

Значение GridCtrl::дай(Ид id) const
{
	return Get0(rowidx, aliases.дай(id));
}

Значение GridCtrl::дай(int r, Ид id) const
{
	return Get0(r + fixed_rows, aliases.дай(id));
}

Значение GridCtrl::дай() const
{
	return Get0(curpos.y, curpos.x);
}

Значение GridCtrl::дай(const char * alias) const
{
	return Get0(rowidx, aliases.дай(alias));
}

Значение GridCtrl::дай(int r, const char * alias) const
{
	return Get0(r + fixed_rows, aliases.дай(alias));
}

Значение GridCtrl::дайСырой(int r, int c) const
{
	return items[r][c].val;
}

Значение GridCtrl::дайПерв(int c) const
{
	return Get0(fixed_rows, c + fixed_cols);
}

Значение GridCtrl::дайПоследн(int c) const
{
	return Get0(total_rows - 1, c + fixed_cols);
}

Значение GridCtrl::дайПредш(Ид id) const
{
	return rowbkp[aliases.дай(id)];	
}

Значение GridCtrl::дайПредш(int c) const
{
	return rowbkp[c + fixed_cols];
}

Значение GridCtrl::GetNew(int c) const
{
	return Get0(rowidx, c + fixed_cols);
}

Значение& GridCtrl::operator() (int r, int c)
{
	return items[vitems[r + fixed_rows].id][c + fixed_cols].val;
}

Значение& GridCtrl::operator() (int c)
{
	return items[vitems[rowidx].id][c + fixed_cols].val;
}

Значение& GridCtrl::operator() (Ид id)
{
	return items[vitems[rowidx].id][aliases.дай(id)].val;
}

Значение& GridCtrl::operator() (int r, Ид id)
{
	return items[vitems[r + fixed_rows].id][aliases.дай(id)].val;
}

Значение& GridCtrl::operator() (const char * alias)
{
	return items[vitems[rowidx].id][aliases.дай(alias)].val;
}

Значение& GridCtrl::operator() (int r, const char * alias)
{
	return items[vitems[r + fixed_rows].id][aliases.дай(alias)].val;
}

void GridCtrl::Setсуммаmary(int c, const Значение& val)
{
	summary[c].val = val;
	Refreshсуммаmary();
}

void GridCtrl::Setсуммаmary(Ид id, const Значение& val)
{
	summary[aliases.дай(id)].val = val;
	Refreshсуммаmary();
}

Значение GridCtrl::Getсуммаmary(int c)
{
	return summary[c + fixed_cols].val;
}

Значение GridCtrl::Getсуммаmary(Ид id)
{
	return summary[aliases.дай(id)].val;
}

bool GridCtrl::изменено(int r, int c)
{
	return items[vitems[r + fixed_rows].id][c + fixed_cols].modified;
}

bool GridCtrl::изменено(int c)
{
	return items[vitems[rowidx].id][c + fixed_cols].modified;
}

bool GridCtrl::изменено(int r, Ид id)
{
	return items[vitems[r + fixed_rows].id][aliases.дай(id)].modified;
}

bool GridCtrl::изменено(Ид id)
{
	return items[vitems[rowidx].id][aliases.дай(id)].modified;
}

Вектор<Значение> GridCtrl::ReadCol(int n, int start_row, int end_row) const
{
	Вектор<Значение> v;
	int idx = hitems[n < 0 ? colidx : n + fixed_cols].id;
	
	if(start_row < 0)
		start_row = fixed_rows;
	else
		start_row += fixed_rows;
	
	if(end_row < 0)
		end_row = total_rows - 1;
	else
		end_row += fixed_rows;
	
	for(int i = start_row; i <= end_row; i++)
		v.добавь(items[i][idx].val);
	
	return v;
}

Вектор<Значение> GridCtrl::читайРяд(int n, int start_col, int end_col) const
{
	Вектор<Значение> v;
	int idy = vitems[n < 0 ? rowidx : n + fixed_rows].id;
	
	if(start_col < 0)
		start_col = fixed_cols;
	else
		start_col += fixed_cols;
	
	if(end_col < 0)
		end_col = total_cols - 1;
	else
		end_col += fixed_cols;
	
	for(int i = start_col; i <= end_col; i++)
		v.добавь(items[idy][i].val);
	
	return v;
}

Вектор< Вектор<Значение> > GridCtrl::дайЗначения()
{
	Вектор< Вектор<Значение> > v;
	
	int rows_cnt = total_rows - fixed_rows;
	int cols_cnt = total_cols - fixed_cols;
	
	v.устСчёт(rows_cnt);
	
	for(int i = 0; i < rows_cnt; i++)
	{
		v[i].устСчёт(cols_cnt);
		
		for(int j = 0; j < cols_cnt; j++)
		{
			const Значение &val = items[i + fixed_rows][j + fixed_cols].val;
			if(IsType<AttrText>(val))
			{
				const AttrText& t = ValueTo<AttrText>(val);
				v[i][j] = t.text;
			}
			else 
				v[i][j] = val;
		}
	}
	
	return v;
}

void GridCtrl::SetValues(const Вектор< Вектор<Значение> >& v)
{
	int rows_cnt = v.дайСчёт();
	
	if(rows_cnt <= 0)
		return;
	
	int cols_cnt = v[0].дайСчёт();
	
	int tc = total_cols - fixed_cols;
	if(cols_cnt > tc)
		cols_cnt = tc;
	
	SetRowCount(rows_cnt);
	
	for(int i = 0; i < rows_cnt; i++)
		for(int j = 0; j < cols_cnt; j++)
		{
			int r = i + fixed_rows;
			int c = j + fixed_cols;
			Ктрл * ctrl = items[r][c].ctrl;
			if(ctrl)
				ctrl->устДанные(v[i][j]);
			items[r][c].val = v[i][j];
		}

	SyncCtrls();
	Syncсуммаmary();
	освежи();
}

GridCtrl& GridCtrl::добавь(const Вектор<Значение> &v, int offset, int count, bool hidden)
{
	Append0(1, hidden ? 0 : GD_ROW_HEIGHT);

	int cnt = min(count < 0 ? v.дайСчёт() : count,
	              total_cols - fixed_cols);

	int r0 = total_rows - 1;
	int r = vitems[r0].id;
	for(int i = offset; i < cnt; i++)
		items[r][i + fixed_cols].val = v[i];

	RefreshRow(r0, 0, 0);

	return *this;
}

bool GridCtrl::IsColumn(const Ид& id)
{
	return valid_cursor ? hitems[curpos.x].id == aliases.дай(id) : false;
}

GridCtrl::ItemRect& GridCtrl::дайКолонку(int n)
{
	return hitems[GetIdCol(n + fixed_cols)];
}

GridCtrl::ItemRect& GridCtrl::дайКолонку()
{
	return hitems[curpos.x];
}

GridCtrl::ItemRect& GridCtrl::GetRow(int n)
{
	return vitems[n + fixed_rows];
}

GridCtrl::ItemRect& GridCtrl::GetRow()
{
	return vitems[rowidx];
}

int GridCtrl::GetCurrentRow() const
{
	return rowidx - fixed_rows;
}

bool GridCtrl::IsCurrentRow() const
{
	return rowidx == curpos.y;
}

void GridCtrl::RestoreCurrentRow()
{
	rowidx = curpos.y;
}

GridCtrl::Элемент& GridCtrl::GetCell(int n, int m)
{
	return items[vitems[n + fixed_rows].id][hitems[m + fixed_cols].id];
}

GridCtrl::Элемент& GridCtrl::GetCell(int n, Ид id)
{
	return items[vitems[n + fixed_rows].id][hitems[aliases.дай(id)].id];
}

void GridCtrl::MouseAccel(const Точка &p, bool horz, bool vert, dword keyflags)
{
	Размер sz = дайРазм();
	int speedx = 0, speedy = 0;
	const int bound = 5;

	if(horz)
	{
		if(p.x > sz.cx - bound)
			speedx = p.x - (sz.cx - bound);
		else if(p.x < fixed_width + bound)
			speedx = -(bound - p.x + fixed_width);
	}

	if(vert)
	{
		if(p.y > sz.cy - bound)
			speedy = p.y - (sz.cy - bound);
		else if(p.y < fixed_height + bound)
			speedy = -(bound - p.y + fixed_height);
	}

	if(speedx) sbx.уст(sbx + speedx);
	if(speedy) sby.уст(sby + speedy);

	if(speedx || speedy)
	{
		LG(0, "speedx %d, speedy %d", speedx, speedy);
		двигМыши(p, keyflags);
	}

}

Рисунок GridCtrl::HorzPosImage()
{
	#ifdef PLATFORM_X11
		return Рисунок::SizeHorz();
	#else
		return GridImg::HorzPos();
	#endif 
}

Рисунок GridCtrl::VertPosImage()
{
	#ifdef PLATFORM_X11
		return Рисунок::SizeVert();
	#else
		return GridImg::VertPos();
	#endif 
}

Рисунок GridCtrl::рисКурсора(Точка p, dword keyflags)
{
	if(!moving_header && !moving_body && HasCapture())
	{
		if(resizing_cols && curSplitCol >= 0)
			return HorzPosImage();
		if(resizing_rows && curSplitRow >= 0)
			return VertPosImage();
		else
			return Рисунок::Arrow();
	}

	if(moving_header)
	{
		curSplitCol = GetSplitCol(p, -1);
		curSplitRow = GetSplitRow(p, -1);

		if(resize_col_mode == 0 || resize_row_mode == 0)
			MouseAccel(p, fixed_top_click, fixed_left_click, keyflags);

		return Рисунок::Arrow();
	}
	else if(moving_body)
	{
		curSplitRow = GetSplitRow(Точка(0, p.y), -1);
		return Рисунок::Arrow();
	}
	else if(mouse_move)
	{
		curSplitCol = GetSplitCol(p);
		curSplitRow = GetSplitRow(p);
		mouse_move = false;
	}

	curResizeCol = curResizeRow = false;

	if(resizing_cols && curSplitCol >= 0 || resizeCol)
	{
		if(curSplitCol >= 0 && hitems[curSplitCol].join > 0)
		{
			int idy = GetMouseRow(p, true, p.y < fixed_height, true);
			if(idy >= 0)
			{
				Элемент &it = items[vitems[idy].id][hitems[curSplitCol].id];
				if(it.isjoined && it.idx + it.cx != curSplitCol)
					return Рисунок::Arrow();
			}
		}
		curResizeCol = true;
		return HorzPosImage();
	}
	else if(resizing_rows && curSplitRow >= 0 || resizeRow)
	{
		if(curSplitRow >= 0 && vitems[curSplitRow].join > 0)
		{
			int idx = GetMouseCol(p, true, p.x < fixed_width, true);
			if(idx >= 0)
			{
				Элемент &it = items[vitems[curSplitRow].id][hitems[idx].id];
				if(it.isjoined && it.idy + it.cy != curSplitRow)
					return Рисунок::Arrow();
			}
		}
		curResizeRow = true;
		return VertPosImage();
	}
	return Рисунок::Arrow();
}


void GridCtrl::UpdateHolder(bool force)
{
	if(size_changed || force)
	{
		holder.SetOffset(Точка(fixed_width, fixed_height));
		holder.HSizePos(fixed_width, 0).VSizePos(fixed_height, summary_height);
		size_changed = false;
	}
}

GridCtrl::CurState GridCtrl::устКурсор0(int x, int y, int opt, int dirx, int diry)
{
	return устКурсор0(Точка(x, y), opt, dirx, diry);
}

GridCtrl::CurState GridCtrl::устКурсор0(Точка p, int opt, int dirx, int diry)
{
	CurState cs;
	if(!row_changing)
	{
		cs.valid = false;
		return cs;
	}
		
	bool mouse = opt & CU_MOUSE;
	bool highlight = opt & CU_HIGHLIGHT;
	bool ctrlmode = opt & CU_CTRLMODE;
	bool hidectrls = opt & CU_HIDECTRLS;

	Точка tmpcur;

	bool mouse_valid = true;

	if(mouse)
	{
		tmpcur.x = GetMouseCol(p, true, false);
		tmpcur.y = GetMouseRow(p, true, false);
		if(tmpcur.x < 0 || tmpcur.y < 0)
			mouse_valid = false;
	}
	else
		tmpcur = p;
	
	if(!highlight && dirx == 0 && diry == 0 && !IsValidCursor(tmpcur))
	{
		cs.valid = false;
		return cs;
	}
	
	Точка oldcur = highlight ? livecur : curpos;

	bool oldvalid = IsValidCursorAll(oldcur);
	bool newvalid = false;
	Элемент *nit = NULL;

	if(!highlight && mouse_valid)
	{
		bool quit = false;

		int fc = max(fixed_cols, firstVisCol);
		int lc = lastVisCol;
		int fr = max(fixed_rows, firstVisRow);
		int lr = lastVisRow;

		Элемент *oit = oldvalid ? &дайЭлт(oldcur) : NULL;

		while(true)
		{
			bool cur = IsValidCursor(tmpcur, fc, lc, fr, lr);

			bool hidden = true;
			bool clickable = true;
			bool группа = true;

			if(cur)
			{
				ItemRect& h = hitems[tmpcur.x];
				ItemRect& v = vitems[tmpcur.y];
				bool hx   = dirx != 0 ? h.hidden : false;
				bool hy   = diry != 0 ? v.hidden : false;
				hidden    = hx || hy;
				clickable = h.clickable && v.clickable;
				if(oit && oit->группа >= 0 && !select_row)
				{
					nit = &дайЭлт(tmpcur);
					группа = nit->группа != oit->группа;
				}
			}

			newvalid = cur && !hidden && clickable && группа;

			if(newvalid)
			{
				int idx = hitems[tmpcur.x].id;
				int idy = vitems[tmpcur.y].id;

				Элемент &it = items[idy][idx];
				
				newvalid = it.clickable;

				if(newvalid && ctrlmode) 
				{
					Ктрл * ctrl = it.ctrl;
					if(!ctrl && isedit)
						ctrl = edits[idx].ctrl;
	
					if(ctrl && it.editable && ctrl->включен_ли())
						break;
				}
			}

			if(newvalid && !ctrlmode)
				break;

			if(quit || (dirx == 0 && diry == 0))
				return cs;

			if(dirx != 0)
			{
				tmpcur.x += dirx;

				if(tmpcur.x > lc)
				{
					if(tab_changes_row && diry == 0)
					{
						tmpcur.y += 1;
						if(tmpcur.y > lr)
						{
							tmpcur.y = lr;
							tmpcur.x = lc;
							quit = true;
						}
						else
							tmpcur.x = fc;
					}
					else
						quit = true;
				}
				else if(tmpcur.x < fc)
				{
					if(tab_changes_row && diry == 0)
					{
						tmpcur.y -= 1;
						if(tmpcur.y < fr)
						{
							tmpcur.y = fr;
							tmpcur.x = fc;
							quit = true;
						}
						else
							tmpcur.x = lc;
					}
					else
						quit = true;
				}
				continue;
			}

			if(diry != 0)
			{
				tmpcur.y += diry;

				if(tmpcur.y < fr)
				{
					tmpcur.y = fr;
					quit = true;
				}
				else if(tmpcur.y > lr)
				{
					tmpcur.y = lr;
					quit = true;
				}
			}
		}
	}
	else
	{
		newvalid = IsValidCursor(tmpcur);
		if(newvalid && highlight)
		{
			if(!vitems[tmpcur.y].clickable || !hitems[tmpcur.x].clickable)
				newvalid = false;
		}
	}

	bool isnewcol = oldcur.x != tmpcur.x;
	bool isnewrow = oldcur.y != tmpcur.y;

	if(isnewcol || isnewrow)
		this->oldcur = oldcur;

	cs.valid = newvalid;

	if(!highlight)
	{
		if(!GetCtrlsData(!isnewrow))
		{
			cs.accepted = false;
			return cs;
		}
		else
		{
			cs.accepted = true;
			if(hidectrls && (edit_mode == GE_CELL || (edit_mode == GE_ROW && (isnewrow || !newvalid))))
			{
				UpdateCtrls(UC_HIDE | UC_CTRLS | UC_OLDCUR);
				if(!one_click_edit || !newvalid)
					WhenEndEdit();
			}
		}

		oldvalid = IsValidCursorAll(oldcur);
	}

	if(tmpcur == oldcur)
		return cs;

	if(highlight)
	{
		livecur = tmpcur;

		if(oldvalid)
		{
			SetItemCursor(oldcur, false, true);
			RefreshRow(oldcur.y, 0);
		}
		if(newvalid)
		{
			SetItemCursor(tmpcur, true, true);
			RefreshRow(tmpcur.y, 0);
		}
		return cs;
	}

	if(!newvalid)
		return cs;

	if(isnewrow)
		WhenBeforeChangeRow();
	
	if(isnewcol)
		WhenBeforeChangeCol();
	
	cs.newx = isnewcol;
	cs.newy = isnewrow;

	Точка t_curpos = curpos;
	Точка t_curid = curid;
	int t_colidx = colidx;
	int t_rowidx = rowidx;
	bool t_valid_cursor = valid_cursor;

	valid_cursor = true;
	curpos = tmpcur;
	colidx = curpos.x;
	rowidx = curpos.y;

	curid.x = hitems[curpos.x].id;
	curid.y = vitems[curpos.y].id;

	WhenCursor();
	
	if(cancel_cursor)
	{
		cancel_cursor = false;
		curpos = t_curpos;
		curid = t_curid;
		colidx = t_colidx;
		rowidx = t_rowidx;
		valid_cursor = t_valid_cursor;
		cs.очисть();
		return cs;
	}

	if(oldvalid)
	{
		SetItemCursor(oldcur, false, highlight);
		RefreshRow(oldcur.y, 0);
	}

	SetItemCursor(tmpcur, true, false);
	if(isnewrow || (!select_row && isnewcol))
		RefreshRow(tmpcur.y, 0);
	
	if(call_whenchangerow && isnewrow)
	{
		#ifdef LOG_CALLBACKS
		LGR(2, "WhenChangeRow()");
		LGR(2, фмт("[row: %d]", rowidx));
		#endif
		WhenChangeRow();
	}

	if(call_whenchangecol && isnewcol)
	{
		#ifdef LOG_CALLBACKS
		LGR(2, "WhenChangeCol()");
		LGR(2, фмт("[col: %d]",colidx));
		#endif
		WhenChangeCol();
	}

	if(isnewrow)
		SetCtrlsData();
	
	LG(0, "cur(%d, %d)", curpos.x, curpos.y);

	return cs;
}

int GridCtrl::дайШирину(int n)
{
	if(n < 0) n = total_cols;
	if(n == 0) return 0;
	return hitems[n - 1].nRight();
}

int GridCtrl::дайВысоту(int n)
{
	if(n < 0) n = total_rows;
	if(n == 0) return 0;
	return vitems[n - 1].nBottom();
}

int GridCtrl::GetFixedWidth()
{
	return дайШирину(fixed_cols);
}

int GridCtrl::GetFixedHeight()
{
	return дайВысоту(fixed_rows);
}

int GridCtrl::GetFirst0(Вектор<ItemRect> &its, int total, int sb, int p)
{
	int l = 0;
	int r = total - 1;

	while(l <= r)
	{
		int i = (l + r) / 2;

		int p0 = its[i].nLeft(sb);
		int p1 = its[i].nRight(sb);

		if(p0 <= p && p1 >= p)
		{
			if(!its[i].hidden)
			{
				return i;
			}
			else
			{
				for(int j = i + 1; j < total; j++)
					if(!its[j].hidden)
						return j;
				for(int j = i - 1; j > 0; j--)
					if(!its[j].hidden)
						return j;

				return -1;
			}
		}

		if(p1 < p)
			l = i + 1;
		else
			r = i - 1;
	}
	return -1;
}

int GridCtrl::GetFirstVisCol(int p)
{
	return total_cols <= 2 ? fixed_cols : GetFirst0(hitems, total_cols, sbx, p);
}

int GridCtrl::GetFirstVisRow(int p)
{
	return total_rows <= 1 ? fixed_rows : GetFirst0(vitems, total_rows, sby, p);
}

GridCtrl& GridCtrl::SetColWidth(int n, int width, bool recalc /* = true */)
{
	if(resize_col_mode > 0 && n >= fixed_cols)
		return *this;

	hitems[n].устШирину(width);
	Repaint(true, false);

	return *this;
}

GridCtrl& GridCtrl::SetRowHeight(int n, int height, bool recalc)
{
	LG(0, "SetRowHeight %d %d", n, height);

	if(resize_row_mode > 0 && n >= fixed_rows)
		return *this;

	vitems[n].устВысоту(height);
	Repaint(false, true);

	return *this;
}

bool GridCtrl::SetDiffItemSize(bool horizontal, RectItems &its, int n, int diff, bool newsize)
{
	if(diff == 0)
		return false;

	if(diff < 0 && its[n].IsMin())
		return false;

	if(diff > 0 && its[n].IsMax())
		return false;

	int resize_mode = horizontal ? resize_col_mode : resize_row_mode;

	if(resize_mode > 0 && diff > 0)
	{
		bool ismin = true;
		for(int i = n + 1; i < (horizontal ? total_cols : total_rows); i++)
			if(!its[i].IsMin())
			{
				ismin = false;
				break;
			}
		if(ismin)
			return false;
	}

	double size = its[n].size + diff;

	if(size <= its[n].min)
	{
		size = its[n].min;
		its[n].ismin = true;
	}
	else if(size >= its[n].max)
	{
		size = its[n].max;
		its[n].ismax = true;
	}
	else
	{
		its[n].ismin = false;
		its[n].ismax = false;
	}

	double ddiff = size - its[n].size;

	if(ddiff != 0)
	{
		Recalc(horizontal, its, n, size, ddiff);
		return true;
	}
	return false;
}

void GridCtrl::Recalc(bool horizontal, RectItems &its, int n, double size, double diff)
{
	its[n].size = size;

	Размер sz = дайРазм();
	int cnt = horizontal ? total_cols : total_rows;
	int maxsize = horizontal ? sz.cx : sz.cy;
	int tcnt = cnt;

	int resize_mode = horizontal ? resize_col_mode : resize_row_mode;

	if(resize_mode == 0)
	{
		for(int i = n + 1; i < cnt; i++)
			its[i].pos += diff;
	}
	else if(resize_mode == 1)
	{
		double imaxsize = 1.0 / (double) maxsize;
		double ms = maxsize;

		loop:
			double sumprop = 0;

			for(int i = cnt - 1; i >= n + 1; --i)
			{
				if(its[i].hidden) continue;

				bool prop = (diff > 0 && its[i].IsMin() || diff < 0 && its[i].IsMax());
				if(!prop)
					sumprop += its[i].prop;
			}

			double cps = sumprop != 0 ? -diff / sumprop : 0;

			for(int i = cnt - 1; i >= n + 1; --i)
			{
				if(its[i].hidden)
				{
					its[i].pos = ms;
					continue;
				}

				if(!(diff > 0 && its[i].IsMin() || diff < 0 && its[i].IsMax()))
				{
					double size = its[i].size + its[i].prop * cps;

					bool minsize = (diff > 0 && size < its[i].min);
					bool maxsize = (diff < 0 && size > its[i].max);

					its[i].ismin = minsize;
					its[i].ismax = maxsize;

					if(minsize || maxsize)
					{
						diff += size - its[i].size;
						double ns = minsize ? its[i].min : its[i].max;
						its[i].size = ns;
						its[i].prop = ns * imaxsize;
						cnt = i + 1;
						goto loop;
					}
					its[i].size = size;
					its[i].prop = size * imaxsize;
				}
				ms -= its[i].size;
				its[i].pos = ms;
			}

			its[n].size -= its[n].pos + its[n].size - ms;
			its[n].prop = its[n].size * imaxsize;
	}

	CalcIntPos(its, n, maxsize, tcnt - 1, resize_mode, false);
}

void GridCtrl::CalcIntPos(RectItems &its, int n, int maxsize, int cnt, int resize_mode, bool renumber)
{
	its[0].npos = 0;

	int last_vis = 1;
	int hidden = 0;

	for(int i = (renumber ? 1 : max(1, n)); i <= cnt ; i++)
	{
		its[i].npos = Round(its[i].лево());
		its[i - 1].nsize = its[i].npos - its[i - 1].npos;

		if(renumber)
			its[i].n = hidden;
		
		if(its[i].hidden)
			hidden++;
		else
			last_vis = i;
	}

	last_vis = cnt;
	if(resize_mode > 0)
	{
		int size = maxsize - its[last_vis].npos;
		its[last_vis].nsize = size ;//>= its[cnt].min && size <= its[cnt].max ? size : Round(its[cnt].size);
	}
	else
		its[last_vis].nsize = Round(its[last_vis].size);
}

bool GridCtrl::UpdateSizes()
{
	total_width  = total_cols ? hitems[total_cols - 1].nRight() : 0;
	total_height = total_rows ? vitems[total_rows - 1].nRight() : 0;

	int prev_summary_height = summary_height;
	
	summary_height = summary_row ? GD_HDR_HEIGHT : 0;
	
	size_changed = prev_summary_height != summary_height;

	int new_fixed_width  = fixed_cols ? hitems[fixed_cols - 1].nRight() : 0;
	int new_fixed_height = fixed_rows ? vitems[fixed_rows - 1].nRight() : 0;

	Размер sz = дайРазм();

	if(resize_col_mode > 0 && new_fixed_width >= sz.cx)
		new_fixed_width = GridCtrl::GD_IND_WIDTH;

	if(resize_row_mode > 0 && new_fixed_height >= sz.cy)
		new_fixed_height = GridCtrl::GD_HDR_HEIGHT;

	if(fixed_cols == 1 && !indicator)
		new_fixed_width = 0;

	if(new_fixed_width != fixed_width)
	{
		fixed_width = new_fixed_width;
		size_changed = true;
	}

	if(new_fixed_height != fixed_height)
	{
		fixed_height = new_fixed_height;
		size_changed = true;
	}

	return size_changed;
}

bool GridCtrl::UpdateCols(bool force)
{
	Размер sz = дайРазм();
	bool change = false;;

	if((osz.cx != sz.cx && resize_col_mode > 0) || force || recalc_cols)
	{
		RecalcCols(-1);
		recalc_cols = false;
		change = true;
	}

	osz.cx = sz.cx;
	return change;
}

bool GridCtrl::UpdateRows(bool force)
{
	Размер sz = дайРазм();
	bool change = false;;

	if((osz.cy != sz.cy && resize_row_mode > 0) || force || recalc_rows)
	{
		RecalcRows(-1);
		recalc_rows = false;
		change = true;
	}
	osz.cy = sz.cy;
	return change;
}

bool GridCtrl::Recalc(bool horizontal, RectItems &its, int resize_mode)
{
	Размер sz = дайРазм();

	if(resize_mode < 0)
		resize_mode = horizontal ? resize_col_mode : resize_row_mode;

	int fixed = horizontal ? fixed_cols : fixed_rows;
	int cnt = horizontal ? total_cols : total_rows;
	int tcnt = cnt;

	its[0].pos = 0;
	
	if(!horizontal && !header)
		its[0].size = 0;

	if(resize_mode == 0)
	{
		for(int i = 1; i < cnt; i++)
			its[i].pos = its[i - 1].pos + its[i - 1].size;
	}
	else if(resize_mode == 1)
	{
		int cs = horizontal ? sz.cx - fixed_width
		                    : sz.cy - fixed_height;

		//int cs = horizontal ? sz.cx : sz.cy;

		if(cs <= 0)
			return false;

		double imaxsize = 1.0 / cs;

		for(int i = fixed; i < cnt; i++)
		{
			its[i].ismin = false;
			its[i].ismax = false;
		}

		double sumprop = 0;
		for(int i = fixed; i < cnt; i++)
			if(!its[i].hidden)
				sumprop += its[i].prop;

		double ics = sumprop <= 0 ? 0 : cs / sumprop;
		sumprop = 0;

		for(int i = fixed; i < cnt; i++)
		{
			if(its[i].hidden)
				continue;

			its[i].size = its[i].prop * ics;

			if(its[i].size < its[i].min)
			{
				cs -= its[i].min;
				its[i].size = its[i].min;
				its[i].ismin = true;
				its[i].prop = its[i].min * imaxsize;
			}
			else if(its[i].size > its[i].max)
			{
				cs -= its[i].max;
				its[i].size = its[i].max;
				its[i].ismax = true;
				its[i].prop = its[i].max * imaxsize;
			}
			else
				sumprop += its[i].prop;
		}

		ics = sumprop <= 0 ? 0 : cs / sumprop;

		for(int i = fixed; i < cnt; i++)
		{
			its[i].pos = i == 0 ? 0 : its[i - 1].право();
			if(its[i].hidden)
			{
				its[i].size = 0;
				//its[i].prop = 0;
				continue;
			}

			if(!its[i].ismin && !its[i].ismax)
				its[i].size = its[i].prop * ics;
			its[i].prop = its[i].size * imaxsize;
		}
	}

	CalcIntPos(its, 0, horizontal ? sz.cx : sz.cy, tcnt - 1, resize_mode, true);

	UpdateVisColRow(horizontal);
	oldpos.x = sbx;
	oldpos.y = sby;

	return true;
}

bool GridCtrl::RecalcCols(int mode)
{
	return Recalc(true, hitems, mode);
}

bool GridCtrl::RecalcRows(int mode)
{
	return Recalc(false, vitems, mode);
}

int GridCtrl::GetSplitCol(const Точка &p, int splitSize, bool full)
{
	if(total_cols < 2)
		return -1;

	int diff = 0;
	if(p.x > fixed_width || moving_body || moving_header)
	{
		if(!full && !full_col_resizing && p.y >= fixed_height)
			return -1;
		diff = sbx;
	}
	else if(p.y < fixed_height - splitSize)
		return -1;

	int tc = splitSize >= 0 ? (resize_col_mode == 0 ? 0 : 1) : 0;

	int fc = lastVisCol - tc;
	int lc = resizing_fixed_cols ? 1 : firstVisCol;

	if(splitSize >= 0)
	{
		for(int i = fc; i >= lc; i--)
		{
			if(hitems[i].hidden) continue;
			int x = hitems[i].nRight(diff);
			if(p.x >= x - splitSize &&
			   p.x <= x + splitSize)
				return i;
		}
	}
	else
	{
		int c = fc;
		for(int i = fc; i >= lc; i--)
		{
			if(!hitems[i].hidden) c = i;
			int x = hitems[c].nLeft(diff) + hitems[c].nWidth() / 2;
			if(p.x >= x)
				return c < fixed_cols ? firstVisCol - 1 : c;
			else if(i == lc)
				return c - 1;
		}
	}

	return -1;
}

int GridCtrl::GetSplitRow(const Точка &p, int splitSize, bool full)
{
	if(total_rows < 2)
		return -1;

	int diff = 0;
	if(p.y > fixed_height || moving_body || moving_header)
	{
		if(!full && !moving_header && !moving_body && !full_row_resizing && p.x >= fixed_width)
			return -1;
		diff = sby;
	}
	else if(p.x < fixed_width)
		return -1;

	int tr = splitSize >= 0 ? (resize_row_mode == 0 ? 0 : 1) : 0;

	int fr = lastVisRow - tr;
	int lr = p.y < fixed_height && resizing_fixed_rows ? 0 : firstVisRow;

	if(splitSize >= 0)
	{
		for(int i = fr; i >= lr; i--)
		{
			if(vitems[i].hidden)
				continue;
			int y = vitems[i].nBottom(diff);
			if(p.y >= y - splitSize &&
			   p.y <= y + splitSize)
				return i;
		}
	}
	else
	{
		int c = fr;
		for(int i = fr; i >= lr; i--)
		{
			if(!vitems[i].hidden) c = i;
			int y = vitems[c].nTop(diff) + vitems[c].nHeight() / 2;
			if(p.y >= y)
				return c < fixed_rows ? firstVisRow - 1 : c;
			else if(i == lr)
				return c - 1;
		}
	}

	return -1;
}

bool GridCtrl::IsValidCursor(const Точка &p, int fc, int lc, int fr, int lr) const
{
	return p.x >= fc && p.x <= lc &&
		   p.y >= fr && p.y <= lr;
}

bool GridCtrl::IsValidCursorVis(const Точка &p) const
{
	return p.x >= firstVisCol && p.x <= lastVisCol &&
	       p.y >= firstVisRow && p.y <= lastVisRow;
}

bool GridCtrl::IsValidCursorAll(const Точка &p) const
{
	return p.x >= fixed_cols && p.x < total_cols &&
	       p.y >= fixed_rows && p.y < total_rows;
}

bool GridCtrl::IsValidCursor(const Точка &p) const
{
	return ready ? IsValidCursorVis(p) : IsValidCursorAll(p);
}

bool GridCtrl::IsValidCursor(int c) const
{
	c += fixed_rows;
	return c >= fixed_rows && c < total_rows;
}

bool GridCtrl::IsRowEditable(int r)
{
	if(r < 0)
		r = curpos.y;
	else
		r += fixed_rows;
	
	return vitems[r].editable && hitems[curpos.x].editable;
}

bool GridCtrl::IsRowClickable(int r /* = -1*/)
{
	if(r < 0)
		r = curpos.y;
	else
		r += fixed_rows;
	
	return vitems[r].clickable &&  hitems[curpos.x].clickable;
}

void GridCtrl::SetItemCursor(Точка p, bool b, bool highlight)
{
	if(highlight)
	{
		hitems[p.x].Live(b);
		vitems[p.y].Live(b);
		дайЭлт(p).Live(b);
	}
	else
	{
		hitems[p.x].Cursor(b);
		vitems[p.y].Cursor(b);
		дайЭлт(p).Cursor(b);
	}
}

GridCtrl& GridCtrl::Indicator(bool b, int size)
{
	if(size < 0)
		size = GD_IND_WIDTH;
	indicator = b;
	fixed_width += size * (b ? 1 : -1);
	SetColWidth(0, b ? size : 0);
	return *this;
}

void GridCtrl::RefreshRow(int n, bool relative, bool fixed)
{
	if(!ready)
		return;
	if(n < 0) { n = rowidx; relative = false; }
	if(relative) n += fixed_rows;
	if(vitems[n].hidden) return;
	int dy = fixed ? 0 : sby;
	int join = vitems[n].join;
	if(join > 0)
	{
		int s = n;
		while(s >= 0 && vitems[s].join > 0) s--;
		s++;
		int e = n;
		while(e < total_rows && vitems[e].join > 0) e++;
		e--;
		освежи(Прям(0, vitems[s].nTop(dy), дайРазм().cx, vitems[e].nBottom(dy)));
	}
	else
		освежи(Прям(0, vitems[n].nTop(dy), дайРазм().cx, vitems[n].nBottom(dy)));
}

void GridCtrl::RefreshCol(int n, bool relative, bool fixed)
{
	if(!ready)
		return;
	if(n < 0) { n = curpos.x; relative = false; }
	if(relative) n += fixed_cols;
	if(hitems[n].hidden) return;
	int dx = fixed ? 0 : sbx;
	освежи(Прям(hitems[n].nLeft(dx), 0, hitems[n].nRight(dx), дайРазм().cy));
}

void GridCtrl::RefreshRows(int from, int to, bool relative, bool fixed)
{
	if(!ready)
		return;
	if(relative)
	{
		from += fixed_rows;
		to += fixed_rows;
	}
	if(vitems.дайСчёт())
		освежи(Прям(0, vitems[clamp(from, 0, vitems.дайСчёт() - 1)].nTop(sby), дайРазм().cx,
		                vitems[clamp(to, 0, vitems.дайСчёт() - 1)].nBottom(sby)));
}

void GridCtrl::RefreshFrom(int from)
{
	Размер sz = дайРазм();
	int y = 0;
	if(resize_row_mode == 0)
		if(from > 2 && from <= total_rows)
			y = vitems[from - 1].nBottom(sby);
	освежи(Прям(0, y, sz.cx, sz.cy));
}

void GridCtrl::освежиЭлт(int r, int c, bool relative)
{
	if(!ready)
		return;
	if(relative)
	{
		c += fixed_cols;
		r += fixed_rows;
	}
	освежи(дайПрямЭлта(r, c));
}

void GridCtrl::RefreshNewRow()
{
	RefreshRow(rowidx, 0);
}

void GridCtrl::RefreshTop()
{
	освежи(0, 0, дайРазм().cx, fixed_height);
}

void GridCtrl::RefreshLeft()
{
	освежи(0, fixed_height, fixed_width, дайРазм().cy - fixed_height);
}

void GridCtrl::Refreshсуммаmary()
{
	Размер sz = дайРазм();
	освежи(0, sz.cy - GD_HDR_HEIGHT, sz.cx, GD_HDR_HEIGHT);
}

bool GridCtrl::IsMouseBody(Точка &p)
{
	return p.x >= fixed_width && p.x < total_width && p.y >= fixed_height && p.y < total_height;
}

int GridCtrl::GetIdCol(int id, bool checkall) const
{
	for(int i = checkall ? 1 : fixed_cols; i < total_cols; i++)
	{
		if(id == hitems[i].id)
			return i;
	}
	return -1;
}

int GridCtrl::GetIdRow(int id, bool checkall) const
{
	for(int i = checkall ? 0 : fixed_rows; i < total_rows; i++)
	{
		if(id == vitems[i].id)
			return i;
	}
	return -1;
}

int GridCtrl::GetNextRow(int n)
{
	n += fixed_rows;
	for(int i = n + 1; i < total_rows; i++)
		if(!vitems[i].hidden)
			return i - fixed_rows;
	return -1;
}

int GridCtrl::GetPrevRow(int n)
{
	n += fixed_rows;
	for(int i = n - 1; i >= fixed_rows; i--)
		if(!vitems[i].hidden)
			return i - fixed_rows;
	return -1;
}

void GridCtrl::UpdateCursor()
{
	curpos.x = GetIdCol(curid.x);
	curpos.y = GetIdRow(curid.y);
	rowidx = curpos.y;
	ctrlid.y = curpos.y < 0 ? -1 : curid.y;
	ctrlpos.y = curpos.y;
	rowfnd = curpos.y;
}

int GridCtrl::найди(const Значение &v, int col, int start_from, int opt) const
{
	for(int i = fixed_rows + start_from; i < total_rows; i++)
	{
		if(opt & GF::SKIP_CURRENT_ROW && i == rowidx)
			continue;
		if(opt & GF::SKIP_HIDDEN && vitems[i].hidden)
			continue;
		if(!vitems[i].skip && items[vitems[i].id][col + fixed_cols].val == v)
			return i - fixed_rows;
	}
	return -1;
}

int GridCtrl::найди(const Значение &v, Ид id, int opt) const
{
	return найди(v, aliases.дай(id) - fixed_cols, 0, opt);
}

int GridCtrl::FindInRow(const Значение& v, int row, int start_from) const
{
	for(int i = fixed_cols + start_from; i < total_cols; i++)
	{
		if(!hitems[i].skip && items[row + fixed_rows][hitems[i].id].val == v)
			return i - fixed_cols;
	}
	return -1;
}

int GridCtrl::FindCurrent(Ид id, int opt) const
{
	int col = aliases.дай(id) - fixed_cols;
	return найди(дай(col), col, 0, opt);
}

int GridCtrl::найди(const Значение &v0, Ид ид0, const Значение&v1, Ид id1, int opt) const
{
	int col0 = aliases.дай(ид0);
	int col1 = aliases.дай(id1);
	
	for(int i = fixed_rows; i < total_rows; i++)
	{
		if(opt & GF::SKIP_CURRENT_ROW && i == rowidx)
			continue;
		if(opt & GF::SKIP_HIDDEN && vitems[i].hidden)
			continue;
		if(!vitems[i].skip &&
		   items[vitems[i].id][col0].val == v0 &&
		   items[vitems[i].id][col1].val == v1)
			return i - fixed_rows;
	}
	return -1;
}

int GridCtrl::FindCurrent(Ид ид0, Ид id1, int opt) const
{
	int col0 = aliases.дай(ид0);
	int col1 = aliases.дай(id1);

	const Значение& val0 = items[vitems[rowidx].id][col0].val;
	const Значение& val1 = items[vitems[rowidx].id][col1].val;

	return найди(val0, ид0, val1, id1, opt);
}

void GridCtrl::UpdateDefaults(int ri)
{
	for(int i = 1; i < total_cols; i++)
		if(!пусто_ли(hitems[i].defval))
			items[ri][hitems[i].id].val = hitems[i].defval;
}

void GridCtrl::SetCtrlsData()
{
	if(!valid_cursor)
		return;

	for(int i = 1; i < total_cols; i++)
	{
		int idx = hitems[i].id;
		Элемент &it = items[curid.y][idx];
		Ктрл * ctrl = it.ctrl;
		if(!ctrl)
			ctrl = edits[idx].ctrl;
		if(ctrl)
		{
			ctrl->устДанные(it.val);
			rowbkp[idx] = it.val;
		}
	}
}

bool GridCtrl::GetCtrlsData(bool samerow, bool doall, bool updates)
{
	if(!valid_cursor || !HasCtrls())
		return true;

	bool newrow = newrow_inserted || newrow_appended;

	if(focused_ctrl)
	{
		Элемент &it = items[curid.y][focused_ctrl_id];

		if(updates && edit_mode == GE_CELL && !focused_ctrl->прими())
			return false;

		Значение v = focused_ctrl->дайДанные();

		if(v.ошибка_ли())
			v = Null;

		bool was_modified = it.modified;

		it.modified = edit_mode == GE_CELL 
			? it.val != v 
			: rowbkp[focused_ctrl_id] != v;
			
		it.val = v;
		
		if(it.modified)
		{
			if(!was_modified)
				row_modified++;

			//it.val = v;

			if(updates)
			{
				#ifdef LOG_CALLBACKS
				LGR(2, "WhenUpdateCell()");
				LGR(2, фмт("[row: %d, colid: %d]", curid.y, focused_ctrl_id));
				LGR(2, фмт("[oldval : %s]", какТкст(rowbkp[focused_ctrl_id])));
				LGR(2, фмт("[newval : %s]", какТкст(v)));
				#endif
				WhenUpdateCell();

				if(cancel_update_cell)
				{
					it.val = rowbkp[focused_ctrl_id];
					it.modified = false;
					if(edit_mode == GE_CELL)
						rowbkp[focused_ctrl_id] = it.val;
					cancel_update_cell = false;
					row_modified--;
				}
			}
		}
	}

	if(!updates)
		return false;

	if(!samerow || doall)
	{
		if(edit_mode == GE_ROW)
		{
			for(int i = fixed_cols; i < total_cols; ++i)
			{
				int idx = hitems[i].id;
				Ктрл * ctrl = дайКтрл(rowidx, i, true, false, false);
				if(ctrl && !ctrl->прими())
				{
					focused_ctrl = ctrl;
					focused_ctrl_id = idx;
					ctrl->устФокус();
					curpos.x = i;
					return false;
				}
			}
		}
		
		if(row_modified)
			vitems[curid.y].operation = GridOperation::UPDATE;

		WhenAcceptRow();
		if(cancel_accept)
		{
			cancel_accept = false;
			return false;
		}

		bool removed = false;
		if(newrow)
		{
			#ifdef LOG_CALLBACKS
			LGR(2, фмт("WhenInsertRow()", curid.y));
			LGR(2, фмт("[row: %d]", curid.y));
			#endif
			removed = !WhenInsertRow0();

		}
		else if(row_modified)
		{
			row_data = true;
			SetModify();

			#ifdef LOG_CALLBACKS
			LGR(2, фмт("WhenUpdateRow()", curid.y));
			LGR(2, фмт("[row: %d]", curid.y));
			#endif
			WhenUpdateRow();
		}

		if(!removed)
		{
			if(!cancel_accept && row_modified)
			{
				WhenAcceptedRow();
				/*
				if(auto_sorting)
				{
					GSort();
					UpdateCursor();
					Repaint(false, true);
				}
				*/
			}

			if(cancel_accept)
			{
				cancel_accept = false;
				return false;
			}

			if(cancel_update)
				CancelCtrlsData(true);
			else
				ClearModified();
		}
	}

	if(newrow && (!samerow || doall))
	{
		newrow_inserted = false;
		newrow_appended = false;
	}

	return true;
}

bool GridCtrl::CancelCtrlsData(bool all)
{
	cancel_update = false;

	int ie = total_cols;
	int is = 1;

	if(!all && edit_mode == GE_CELL)
	{
		is = curpos.x;
		ie = is + 1;
	}

	for(int i = is; i < ie; i++)
	{
		int id = hitems[i].id;
		Элемент &it = items[curid.y][id];

		Ктрл * ctrl = it.ctrl;
		if(!ctrl)
			ctrl = edits[id].ctrl;
		if(ctrl)
		{
			ctrl->отклони();
			ctrl->устДанные(rowbkp[id]);
			
			if(it.modified)
			{
				it.modified = false;
				row_modified--;
				it.val = rowbkp[id];
			}
		}
	}

	if(!row_modified)
		vitems[curid.y].operation = GridOperation::NONE;

	return true;
}

void GridCtrl::UpdateCtrls(int opt /*= UC_CHECK_VIS | UC_SHOW | UC_CURSOR */)
{
	if(!valid_cursor)
		return;

	if((opt & UC_CHECK_VIS) && !HasCtrls())
		return;

	Точка cp(opt & UC_OLDCUR ? oldcur : curpos);

	bool show = opt & UC_SHOW;

	ctrlid.y  = show ? (cp.y < 0 ? -1 : vitems[cp.y].id) : -1;
	ctrlpos.y = show ? cp.y : -1;

	if(cp.y < 0)
		return;

	Размер sz = дайРазм();

	bool gofirst = (opt & UC_GOFIRST) && !ктрл_ли(cp, false);

	edit_ctrls = false;

	bool nofocus = opt & UC_NOFOCUS;
	
	focused_ctrl = NULL;
	focused_ctrl_id = -1;
	focused_col = -1;

	for(int i = 1; i < total_cols; i++)
	{
		if(hitems[i].hidden)
			continue;

		Ктрл* ctrl = дайКтрл(cp.y, i, show == false);
		
		if(!ctrl)
			continue;

		if(show)
		{
			if(newrow_appended || newrow_inserted)
			{
				if(!hitems[i].edit_insert)
					continue;
			}
			else
			{
				if(!hitems[i].edit_update)
					continue;
			}
		}

		int id = hitems[i].id;

		bool sync_ctrl = items[vitems[cp.y].id][id].ctrl;
		bool dorect = false;
		bool dorf = i == curpos.x;
		
		if(gofirst)
		{
			dorf = true;
			gofirst = false;
		}
		
		bool dofocus = !nofocus && !(opt & UC_HIDE) && dorf;

		if(show)
		{
			dorect = edit_mode == GE_CELL ? dorf : (isedit || (opt & UC_CTRLS));
			dorect = dorect && дайЭлт(cp.y, i).editable;
		}
		
		if(!sync_ctrl)
		{
			if(dorect)
			{
				Прям r = дайПрямЭлта(ctrlpos.y, i, horz_grid, vert_grid, true, true);
	
				if(!r.пересекается(sz))
					r.уст(0, 0, 0, 0);
	
				ctrl->устПрям(AlignRect(r, i));
				ctrl->покажи();
				edit_ctrls = true;
			}
			else
			{
				ctrl->устПрям(0, 0, 0, 0);
				ctrl->скрой();
			}
		}

		if(dofocus && ctrl->показан_ли())
		{
			ctrl->устФокус();
			focused_ctrl = ctrl;
			focused_ctrl_id = id;
			focused_ctrl_val = hitems[i].defval;
			focused_col = i;

			if(opt & UC_CURSOR)
				устКурсор0(i, cp.y);
		}
	}

	if(!nofocus && !focused_ctrl)
		устФокус();

	if(opt & UC_CTRLS)
		isedit = edit_ctrls;
	
	if(opt & UC_CTRLS_OFF)
		isedit = false;

	if(!(opt & UC_SCROLL))
		RebuildToolBar();
		
	if(isedit)
		popup.закрой();
}

void GridCtrl::SyncCtrls(int row, int col)
{
	if(!genr_ctrls)
		return;
	
	Размер sz = дайРазм();

	int js = row < 0 ? 0 : row;
	int je = row < 0 ? total_rows : row + 1;
	
	int is = col < 0 ? 1 : col;
	int ie = col < 0 ? total_cols : col + 1;

	for(int j = js; j < je; j++)
	{
		int idy = vitems[j].id;
		bool fixed_row = j < fixed_rows;
		bool create_row = !fixed_row && vitems[j].editable;

		for(int i = is; i < ie; i++)
		{
			bool fixed_col = i < fixed_cols;
			bool create_col = !fixed_col && hitems[i].editable;
						
			int idx = hitems[i].id;

			Элемент *it = &items[idy][idx];
			
			if(it->isjoined)
			{
				it = &items[it->idy][it->idx];
				idx = it->idx;
			}

			if(!it->ctrl && create_row && create_col && it->editable && edits[idx].factory)
			{
				Один<Ктрл> newctrl;
				edits[idx].factory(newctrl);
				it->ctrl = newctrl.открепи();
				it->ctrl_flag = IC_FACTORY | IC_INIT | IC_OWNED;
			}
			
			if(it->ctrl && (it->ctrl_flag & IC_INIT))
			{
				it->ctrl->устДанные(it->val);
				it->ctrl->WhenAction << прокси(WhenCtrlsAction);
				holder.добавьОтпрыск(it->ctrl);				
				it->ctrl_flag &= ~IC_INIT;
			}

			if(it->ctrl)
			{
				if(it->isjoined && it->sync_flag == sync_flag)
					continue;

				Прям r = дайПрямЭлта(j, i, horz_grid, vert_grid, true, true);
				AlignRect(r, i);
				
				if(r.пересекается(sz) && !fixed_col && !fixed_row && !vitems[j].hidden && !hitems[i].hidden)
				{
					it->ctrl->устПрям(r);
					it->ctrl->покажи();
				}
				else if(it->ctrl->показан_ли())
				{
					it->ctrl->устПрям(0, 0, 0, 0);
					it->ctrl->скрой();
				}
			}

			if(it->isjoined)
				it->sync_flag = sync_flag;
		}
	}
	sync_flag = 1 - sync_flag;
}

void GridCtrl::Syncсуммаmary()
{
	if(!summary)
		return;
	
	if(WhenUpdateсуммаmary)
	{
		WhenUpdateсуммаmary();
	}
	else
	{
		for(int i = fixed_cols; i < total_cols; i++)
		{
			Значение t = 0;
			
			int idx = hitems[i].id;
			
			int sop = hitems[i].sop;
			
			if(sop == SOP_NONE)
				continue;
				
			int n = 0;
			
			for(int j = fixed_rows; j < total_rows; j++)
			{
				if(vitems[j].скрыт_ли())
					continue;
				
				if(sop == SOP_CNT)
				{
					++n;
					continue;
				}

				int idy = vitems[j].id;
					
				Значение v = items[idy][idx].val;
				
				if(пусто_ли(v))
					continue;
									
				ProcessсуммаmaryValue(v);
				
				if(n == 0 && (sop == SOP_MIN || sop == SOP_MAX))
					t = v;
				
				if(число_ли(v))
				{
					switch(sop)
					{
						case SOP_MIN:
							if(double(v) < double(t))
								t = v;
							break;
						case SOP_MAX:
							if(double(v) > double(t))
								t = v;
							break;
						case SOP_SUM:
						case SOP_AVG:				
							t = double(t) + double(v);
					}
				}
				else if(IsType<Дата>(v))
				{
					switch(sop)
					{
						case SOP_MIN:
							if((Дата) v < (Дата) t)
								t = v;
							break;
						case SOP_MAX:
							if((Дата) v > (Дата) t)
								t = v;
							break;
						case SOP_SUM:
						case SOP_AVG:
							t = v;
							break;
					}
				}				
			}

			if(sop == SOP_AVG)
			{
				if(число_ли(t))
					t = double(t) / double(n);						
			}
			else if(sop == SOP_CNT)
			{
				t = n;
			}
			
			summary[idx].val = t;
		}
	}
	
	if(summary_row)
		Refreshсуммаmary();
}

void GridCtrl::Updateсуммаmary(bool b)
{
	update_summary = b;
	if(b)
		Syncсуммаmary();
}

bool GridCtrl::HasCtrls()
{
	return edit_ctrls || genr_ctrls;
}

void GridCtrl::SetCtrlFocus(int col)
{
	oldcur.x = curpos.x;
	Ктрл * ctrl = дайКтрл(col + fixed_cols, rowidx, false, false);
	focused_ctrl = ctrl;
	focused_ctrl_id = hitems[col + fixed_cols].id;
	ctrl->устФокус();
	curpos.x = col + fixed_cols;
}

void GridCtrl::SetCtrlFocus(Ид id)
{
	SetCtrlFocus(aliases.дай(id));
}

bool GridCtrl::прими()
{
	if(!EndEdit())
		return false;
	return Ктрл::прими();
}

void GridCtrl::отклони()
{
	CancelEdit();
	Ктрл::отклони();
}

void GridCtrl::RestoreFocus()
{
	if(focused_ctrl && !focused_ctrl->HasFocusDeep())
		focused_ctrl->устФокус();
}

bool GridCtrl::ShowNextCtrl()
{
	if(GoRight(1, 1))
	{
		UpdateCtrls(UC_CHECK_VIS | UC_SHOW);
		return true;
	}
	return false;
}

bool GridCtrl::ShowPrevCtrl()
{
	if(GoLeft(1, 1))
	{
		UpdateCtrls(UC_CHECK_VIS | UC_SHOW);
		return true;
	}
	return false;
}

int GridCtrl::GetFocusedCtrlIndex()
{
	for(int i = 1; i < total_cols; i++)
	{
		int id = hitems[i].id;

		Ктрл * ctrl = items[0][id].ctrl;
		if(ctrl && ctrl->HasFocusDeep())
			return i;
	}
	return -1;
}

Точка GridCtrl::GetCtrlPos(Ктрл * ctrl)
{
	for(int i = fixed_rows; i < total_rows; i++)
	{
		int idy = vitems[i].id;
		for(int j = fixed_cols; j < total_cols; j++)
		{
			int idx = hitems[j].id;
			Ктрл * ci = items[idy][idx].ctrl;
			bool isedit = false;
			if(!ci)
			{
				ci = edits[idx].ctrl;
				isedit = true;
			}
			if(ci == ctrl || ci->HasChildDeep(ctrl))
				return Точка(j, isedit ? ctrlpos.y : i);
		}
	}
	return Точка(-1, -1);
}

void GridCtrl::разбей(int state, bool sync)
{
	if(resize_paint_mode < 2)
	{
		if(resize_paint_mode > 0 && state != GS_DOWN)
		{
			if(resizeCol) RefreshTop();
			if(resizeRow) RefreshLeft();
		}

		if(state == GS_DOWN)
			DrawLine(true, false);
		else if(state == GS_MOVE)
			DrawLine(true, true);
		else
			DrawLine(false, true);
	}

	if(state == GS_DOWN)
	{
		firstCol = firstRow = -1;
		return;
	}

	if(state != GS_DOWN && (resize_paint_mode == 2 || state == GS_UP))
	{
		UpdateSizes();
		UpdateHolder();
		UpdateSb();
		освежи();
	}

	if((resize_paint_mode > 1 && state > GS_UP) || state == GS_UP)
	{
		SyncCtrls();
		UpdateCtrls(UC_CHECK_VIS | UC_SHOW);
	}

	if(sync)
		синх();
}

bool GridCtrl::TabKey(bool enter_mode)
{
	if(!естьФокус() && !holder.HasFocusDeep())
		return false;
	
	bool has_ctrls = HasCtrls();

	if(has_ctrls)
	{
		bool isnext = ShowNextCtrl();
		if(tab_adds_row && !isnext && curpos.y == lastVisRow)
		{
			DoAppend();
			return true;
		}
		else
			return focused_ctrl ? true : (genr_ctrls > 0 && !edit_ctrls) ? true : isnext;
	}

	if(tab_changes_row && ((enter_mode && has_ctrls) || (!enter_mode && !has_ctrls)))
	{
		bool isnext = false;
		if(select_row)
		{
			isnext = идиСледщ();
			if(!isnext && tab_adds_row)
				DoAppendNoEdit();
		}
		else
		{
			isnext = GoRight();
			if(!isnext && tab_adds_row)
				DoAppendNoEdit();
		}
		очистьВыделение();

		if(isnext)
			return true;
	}
	else if(!enter_mode)
		return false;

	if(enter_mode && !has_ctrls)
	{
		SwitchEdit();
		return true;
	}

	return false;
}

bool GridCtrl::ищи(dword ключ)
{
	//if(ключ & K_UP)
	//	return false;

	if(ключ >= 32 && ключ < 65536)
	{
		search_string += (wchar) ключ;
		if(!ShowMatchedRows(search_string) && search_string.дайСчёт() > 0)
			search_string.удали(search_string.дайСчёт() - 1);
		else
			find <<= search_string;

		return true;
	}
	return false;
}

int GridCtrl::GetResizePanelHeight() const
{
	return (resize_panel.дайВысоту() + 2) * resize_panel_open;
}

Ткст GridCtrl::GetColumnName(int n) const
{
	return hitems[GetIdCol(n + fixed_cols)].дайИмя();
}

Ид GridCtrl::GetColumnId(int n) const
{
	return aliases.дайКлюч(n + fixed_cols);
}

void GridCtrl::SwapCols(int n, int m)
{
	if(m == n ||
	   n < fixed_cols || n > total_cols - 1 ||
	   m < fixed_cols || m > total_cols - 1)
		return;

	разверни(hitems[m], hitems[n]);
	UpdateCursor();
	Repaint(true, false);
}

bool GridCtrl::CanMoveCol(int n, int m)
{
	if(m == n || m == n - 1 ||
	   n < 0 || n > total_cols - 1 ||
	   m < 0 || m > total_cols - 1)
		return false;
	else
	{
		if(hitems[n].join > 0)
		{
			LG(2, "n=%d(%d) m=%d(%d)", n, hitems[n].join, m, hitems[m].join);
			if(m == n - 2 && hitems[n].join == hitems[n - 1].join)
				return true;

			if(hitems[m].join != hitems[n].join)
				return false;
		}
		//tu sprawdzic join
		return true;
	}
}

void GridCtrl::MoveCol(int n, int m)
{
	LG(0, "%d->%d", n, m);

	if(!CanMoveCol(n, m))
	{
		Repaint();
		return;
	}

	ItemRect ir = hitems[n];
	if(m > total_cols)
		hitems.добавь(ir);
	else
		hitems.вставь(m + 1, ir);
	if(m > n)
		hitems.удали(n);
	else
		hitems.удали(n + 1);

	UpdateCursor();
	Repaint(true, false);
}

bool GridCtrl::MoveRow(int n, int m, bool repaint)
{
	LG(0, "%d->%d", n, m);

	if(m == n || m == n - 1 ||
	   n < 0 || n > total_rows - 1 ||
	   m < -1 || m > total_rows - 1)
	{
		Repaint();
		return false;
	}
	
	WhenMoveRow(n, m);
	
	if(cancel_move)
	{
		cancel_move = false;
		return false;
	}

	ItemRect ir = vitems[n];
	if(m > total_rows)
		vitems.добавь(ir);
	else
		vitems.вставь(m + 1, ir);
	if(m > n)
		vitems.удали(n);
	else
		vitems.удали(n + 1);

	if(repaint)
	{
		UpdateCursor();
		Repaint(false, true);
	}

	SetOrder();
	SetModify();

	return true;
}

void GridCtrl::MoveRows(int n, bool onerow)
{
	if(selected_rows && !onerow)
	{
		Вектор<ItemRect> vi;
		vi.резервируй(selected_rows);
		for(int i = fixed_rows; i < total_rows; i++)
			if(vitems[i].IsSelect())
			{
				WhenMoveRow(i, n);
				if(cancel_move)
				{
					cancel_move = false;
					return;
				}
				vi.добавь(vitems[i]);
			}

		int cnt = 0;

		for(int i = total_rows - 1; i >= fixed_rows; i--)
			if(vitems[i].IsSelect())
			{
				vitems.удали(i);
				if(i < n)
					cnt++;
			}

		vitems.вставь(n - cnt, vi);

		SetOrder();
		SetModify();

		UpdateCursor();
		Repaint(false, true);
	}
	else
	{
		MoveRow(curpos.y, n - 1);
	}
}

bool GridCtrl::SwapRows(int n, int m, bool repaint)
{
	if(isedit || m == n ||
	   n < fixed_rows || n > total_rows - 1 ||
	   m < fixed_rows || m > total_rows - 1)
		return false;
	
	WhenMoveRow(n, m);
	
	if(cancel_move)
	{
		cancel_move = false;
		return false;
	}

	разверни(vitems[m], vitems[n]);
	if(repaint)
	{
		UpdateCursor();
		Repaint(false, true);
	}
	SetOrder();
	SetModify();
	return true;
}

void GridCtrl::SwapUp(int cnt)
{
	int yp = 0;
	bool first = false;
	bool repaint = false;

	if(selected_rows == 0)
	{
		if(SwapRows(curpos.y, curpos.y - cnt))
			yp = vitems[curpos.y + cnt].nTop(sby + fixed_height);
		else
			return;
	}
	else
	{
		for(int i = fixed_rows; i < total_rows; i++)
		{
			if(vitems[i].IsSelect())
			{
				if(!SwapRows(i, i - cnt, false))
					return;
				if(!first)
				{
					yp = vitems[i].nTop(sby + fixed_height);
					first = true;
				}
			}
		}
		repaint = true;
	}

	if(resize_row_mode == 0 && yp < 0)
		sby.уст(sby + yp);

	if(repaint)
	{
		UpdateCursor();
		Repaint(false, true);
	}
}

void GridCtrl::SwapDown(int cnt)
{
	int yp = 0;
	bool first = false;
	bool repaint = false;

	if(selected_rows == 0)
	{
		if(SwapRows(curpos.y, curpos.y + cnt))
			yp = vitems[curpos.y - cnt].nBottom(sby);
		else
			return;
	}
	else
	{
		for(int i = total_rows - 1; i >= fixed_rows; i--)
		{
			if(vitems[i].IsSelect())
			{
				if(!SwapRows(i, i + cnt, false))
					return;
				if(!first)
				{
					yp = vitems[i].nBottom(sby);
					first = true;
				}
			}
		}
		repaint = true;
	}

	int cy = дайРазм().cy - bar.дайРазм().cy;
	if(resize_row_mode == 0 && yp > cy)
		sby.уст(sby + yp - cy);

	if(repaint)
	{
		UpdateCursor();
		Repaint(false, true);
	}
}

GridCtrl& GridCtrl::GridColor(Цвет fg)
{
	fg_grid = fg;
	return *this;
}

GridCtrl& GridCtrl::FocusColor(Цвет fg, Цвет bg)
{
	fg_focus = fg;
	bg_focus = bg;
	return *this;
}

GridCtrl& GridCtrl::LiveColor(Цвет fg, Цвет bg)
{
	fg_live = fg;
	bg_live = bg;
	return *this;
}

GridCtrl& GridCtrl::OddColor(Цвет fg, Цвет bg)
{
	fg_odd = fg;
	bg_odd = bg;
	return *this;
}

GridCtrl& GridCtrl::EvenColor(Цвет fg, Цвет bg)
{
	fg_even = fg;
	bg_even = bg;
	return *this;
}

GridCtrl& GridCtrl::ColoringMode(int m)
{
	coloring_mode = m;
	return *this;
}

void GridCtrl::ClearCursor(bool remove)
{
	if(!remove && valid_cursor)
	{
		SetItemCursor(curpos, false, false);
		RefreshRow(curpos.y, 0);
	}

	curpos.x = curpos.y = -1;
	curid.x = curid.y = -1;
	rowidx = -1;
	valid_cursor = false;
}

void GridCtrl::ClearRow(int r, int column_offset)
{
	if(r < 0)
		r = rowidx;
	else
		r -= fixed_rows;
	for(int i = fixed_cols + column_offset; i < total_cols; i++)
		items[vitems[r].id][hitems[i].id].val = Null;

	SetCtrlsData();
	RefreshRow(rowidx, 0);
}

void GridCtrl::очисть(bool columns)
{
	doscroll = false;
	
	anchor = Null;

	UpdateCtrls(UC_HIDE | UC_CTRLS);

	int nrows = columns ? 1 : fixed_rows;
	items.удали(nrows, items.дайСчёт() - nrows);
	vitems.удали(nrows, vitems.дайСчёт() - nrows);

	total_rows = nrows;
	fixed_rows = nrows;

	if(columns)
	{
		hitems.удали(1, hitems.дайСчёт() - 1);
		items[0].удали(1, items[0].дайСчёт() - 1);
		rowbkp.удали(1, rowbkp.дайСчёт() - 1);
		edits.удали(1, edits.дайСчёт() - 1);
		sortOrder.очисть();
		total_cols = 1;
		total_width = 0;
		total_height = 0;
		firstCol = -1;
		lastCol = -1;
		fixed_cols = 1;
		coluid = 0;
		hcol = -1;
		sortCol = -1;
		genr_ctrls = 0;
		firstVisCol = fixed_cols;
		lastVisCol = total_cols - 1;
	}
	else
	{
		total_height = fixed_height;
	}

	firstVisRow = fixed_rows;
	lastVisRow = total_rows - 1;
	
	focused_ctrl = NULL;

	valid_cursor = false;

	firstRow = -1;
	lastRow = -1;

	curpos.x = curpos.y = -1;
	curid.x  = curid.y  = -1;

	hrow = -1;

	rowidx = -1;
	rowuid = 0;

	row_modified = 0;

	UpdateSizes();
	UpdateSb();
	
	if(ready)
	{
		UpdateHolder();
	
		oldpos.x = sbx;
		oldpos.y = sby;
	
		RebuildToolBar();
		освежи();
	}

	WhenEmpty();
	WhenCursor();
	
	doscroll = true;
}

void GridCtrl::переустанов()
{
	очисть(true);
}

void GridCtrl::ClearOperations()
{
	for(int i = fixed_rows; i < total_rows; i++)
		vitems[i].operation.очисть();
}

void GridCtrl::ClearVersions()
{
	for(int i = fixed_rows; i < total_rows; i++)
		vitems[i].operation.ClearVersion();
}

void GridCtrl::старт()
{
	bkp_rowidx = rowidx;
	rowidx = fixed_rows;
}

void GridCtrl::стоп()
{
	rowidx = total_rows - 1;
}

bool GridCtrl::конец_ли()
{
	if(rowidx < total_rows)
		return true;
	else
	{
		rowidx = bkp_rowidx;
		return false;
	}
}

void GridCtrl::следщ()
{
	++rowidx;
}

void GridCtrl::Prev()
{
	--rowidx;
}

void GridCtrl::Move(int r)
{
	rowidx = r + fixed_rows;
}

bool GridCtrl::IsNext()
{
	return rowidx < total_rows - 1;
}

bool GridCtrl::IsPrev()
{
	return rowidx > fixed_rows;
}

bool GridCtrl::IsFirst()
{
	return rowidx == fixed_rows;
}

bool GridCtrl::IsLast()
{
	return rowidx == total_rows - 1;
}

int GridCtrl::устКурсор0(int n)
{
	int t = curpos.y;
	устКурсор0(curpos.x < 0 ? firstVisCol : curpos.x, n);
	return t;
}

int GridCtrl::устКурсор(int n)
{
	return устКурсор0(n + fixed_rows) - fixed_rows;
}

void GridCtrl::устКурсор(const Точка& p)
{
	устКурсор0(Точка(p.x + fixed_cols, p.y + fixed_rows), false);
}

int GridCtrl::SetCursorId(int id)
{
	id += fixed_rows;
	for(int i = fixed_rows; i < total_rows; i++)
	{
		if(vitems[i].id == id)
			return устКурсор(i - fixed_rows);
	}
	return -1;
}

int GridCtrl::дайКурсор(bool rel) const
{
	if(rel)
		return valid_cursor ? vitems[curpos.y].id - fixed_rows : -1;
	else
		return valid_cursor ? curpos.y - fixed_rows : -1;
}

int GridCtrl::GetPrevCursor(bool rel) const
{
	if(rel)
		return IsValidCursor(oldcur) ? vitems[oldcur.y].id - fixed_rows : -1;
	else
		return IsValidCursor(oldcur) ? oldcur.y - fixed_rows : -1;
}

int GridCtrl::дайКурсор(int uid) const
{
	for(int i = fixed_rows; i < total_rows; i++)
		if(vitems[i].uid == uid)
			return i - fixed_rows;
	return -1;
}

Точка GridCtrl::GetCursorPos() const
{
	return valid_cursor ? Точка(curpos.x - fixed_cols, curpos.y - fixed_rows) : Точка(-1, -1);
}

int GridCtrl::GetRowId() const
{
	return valid_cursor ? vitems[curpos.y].id - fixed_rows : -1;
}

int GridCtrl::GetColId() const
{
	return valid_cursor ? hitems[curpos.x].id - fixed_cols: -1;
}

int GridCtrl::GetRowId(int n) const { return vitems[n + fixed_rows].id - fixed_rows; }
int GridCtrl::GetColId(int n) const { return hitems[n + fixed_cols].id - fixed_cols; }

int GridCtrl::GetColUId() const
{
	return valid_cursor ? hitems[curpos.x].uid : -1;
}

int GridCtrl::GetRowUId() const
{
	return valid_cursor ? vitems[curpos.y].uid : -1;
}

int GridCtrl::FindCol(int id) const
{
	for(int i = fixed_cols; i < total_cols; i++)
		if(hitems[i].id == id)
			return i - fixed_cols;
	return -1;
}

int GridCtrl::FindCol(const Ид& id) const
{
	for(int i = fixed_cols; i < total_cols; i++)
		if(aliases.дайКлюч(i) == id)
			return i - fixed_cols;
	return -1;
}

int GridCtrl::FindCol(const Ткст& s) const
{
	for(int i = fixed_cols; i < total_cols; i++)
		if(hitems[i].дайИмя() == s)
			return i - fixed_cols;
	return -1;
}

int GridCtrl::FindRow(int id) const
{
	for(int i = fixed_rows; i < total_rows; i++)
		if(vitems[i].id == id)
			return i - fixed_rows;
	return -1;
}

int GridCtrl::GetNewRowPos()
{
	return rowidx > 0 ? rowidx - fixed_rows : -1;
}

int GridCtrl::GetNewRowId()
{
	return rowidx > 0 ? vitems[rowidx].id - fixed_rows : -1;
}

int GridCtrl::GetRemovedRowPos()
{
	return rowidx > 0 ? rowidx - fixed_rows : -1;
}

void GridCtrl::курсорПоЦентру()
{
	if(пустой() || !курсор_ли())
		return;

	sbx.уст(hitems[curpos.x].nLeft() - дайРазм().cx / 2);
	sby.уст(vitems[curpos.y].nTop() - дайРазм().cy / 2);
}

bool GridCtrl::Go0(int jump, bool scroll, bool goleft, bool ctrlmode)
{
	if(пустой())
		return false;

	if(!ready)
	{
		UpdateSizes();
		UpdateSb();
	}

	if(jump == GO_LEFT || jump == GO_RIGHT)
	{
		if(select_row && !ctrlmode && !draw_focus)
		{
			if(jump == GO_LEFT)
				sbx.уст(sbx.дай() - 5);
			else
				sbx.уст(sbx.дай() + 5);
			return false;
		}
	}

	if(jump == GO_PREV)
		if(curpos.y >= 0 && curpos.y <= firstVisRow)
			return false;

	if(jump == GO_NEXT)
		if(curpos.y >= 0 && curpos.y >= lastVisRow)
			return false;

	if(jump == GO_PAGEUP || jump == GO_PAGEDN)
	{
		if(jump == GO_PAGEDN && curpos.y == lastVisRow)
			return false;

		if(jump == GO_PAGEUP && curpos.y == firstVisRow)
			return false;

		if(!valid_cursor)
		{
			GoFirstVisible();
			return true;
		}
	}

	Размер sz = дайРазм();
	int sy = -1;
	
	int opt = /*ctrls*/ ctrlmode ? CU_CTRLMODE : 0;

	switch(jump)
	{
		case GO_BEGIN:
		{
			if(!устКурсор0(curpos.x < 0 ? firstVisCol : curpos.x, firstVisRow, opt, 0, 1))
				return false;
			sy = 0;

			break;
		}
		case GO_END:
		{
			if(!устКурсор0((curpos.x < 0 || goleft) ? firstVisCol : curpos.x, lastVisRow, opt, 0, -1))
				return false;
			if(goleft)
				GoCursorLeftRight();
			else
				sy = total_height - fixed_height - summary_height;

			break;
		}
		case GO_NEXT:
		{
			if(!устКурсор0(curpos.x < 0 ? firstVisCol : curpos.x,
			               curpos.y < 0 ? firstVisRow : curpos.y + 1,
					       opt, 0, 1))
				return false;

			int b = vitems[curpos.y].nBottom(sby);
			int r = sz.cy - summary_height;

			if(b > r)
				sy = sby + b - r;

			break;
		}
		case GO_PREV:
		{
			if(!устКурсор0(curpos.x < 0 ? firstVisCol : curpos.x,
			               curpos.y < 0 ? firstVisRow : curpos.y - 1,
			               opt, 0, -1))
				return false;

			int t = vitems[curpos.y].nTop(sby + fixed_height);

			if(t < 0)
				sy = sby + t;

			break;
		}
		case GO_LEFT:
		{
			if(!устКурсор0(curpos.x < 0 ? firstVisCol : curpos.x - 1,
						   curpos.y < 0 ? firstVisRow : curpos.y,
						   opt, -1, 0))
				return false;

			break;
		}
		case GO_RIGHT:
		{
			if(!устКурсор0(curpos.x < 0 ? firstVisCol : curpos.x + 1,
						   curpos.y < 0 ? firstVisRow : curpos.y,
						   ctrlmode ? CU_CTRLMODE : 0, 1, 0))
				return false;

			break;
		}
		case GO_PAGEUP:
		{
			int cp = curpos.y;
			int c = cp;

			int yn = vitems[c].nTop() - sz.cy;
			int ya = vitems[c].nTop(sby);

			bool found = false;
			int i;
			for(i = c - 1; i >= fixed_rows; i--)
				if(yn >= vitems[i].nTop() && yn < vitems[i].nBottom())
				{
					found = true;
					break;
				}

			c = found ? i : firstVisRow;

			if(!устКурсор0(curpos.x < 0 ? firstVisCol : curpos.x, c, opt, 0, 1))
				return false;
			
			c = curpos.y;

			if(scroll && resize_row_mode == 0)
			{
				int yc = vitems[c].nTop();
				int yt = vitems[cp].nTop(sby);
				int yb = vitems[cp].nBottom(sby);

				if(yt < 0 || yb > sz.cy - 1)
					sby.уст(yc - sz.cy + vitems[c].nHeight());
				else
					sby.уст(yc - ya);
			}

			break;
		}
		case GO_PAGEDN:
		{
			int cp = curpos.y;
			int c = cp;

			int yn = vitems[c].nTop() + sz.cy;
			int ya = vitems[c].nTop(sby);

			bool found = false;
			int i;
			for(i = c + 1; i < total_rows; i++)
				if(yn >= vitems[i].nTop() && yn < vitems[i].nBottom())
				{
					found = true;
					break;
				}

			c = found ? i : lastVisRow;

			if(!устКурсор0(curpos.x < 0 ? firstVisCol : curpos.x, c, opt, 0, -1))
				return false;

			c = curpos.y;

			if(scroll && resize_row_mode == 0)
			{
				int yc = vitems[c].nTop();
				int yt = vitems[cp].nTop(sby);
				int yb = vitems[cp].nBottom(sby);

				if(yt < 0 || yb > sz.cy - 1)
					sby.уст(yc);
				else
					sby.уст(yc - ya);
			}

			break;
		}
	}

	if(jump == GO_LEFT || jump == GO_RIGHT)
	{
		if(scroll)
			GoCursorLeftRight();
	}
	else
	{
		if(scroll && resize_row_mode == 0 && sy >= 0)
			sby.уст(sy);
	}

	opt = UC_CHECK_VIS;
	if(isedit)
		opt |= UC_SHOW;
	UpdateCtrls(opt);

	return true;
}

void GridCtrl::GoCursorLeftRight()
{
	if(resize_col_mode == 0)
	{
		int l = hitems[curpos.x].nLeft(sbx + fixed_width);
		int r = hitems[curpos.x].nRight(sbx);
		int w = дайРазм().cx;

		if(l < 0)
			sbx.уст(sbx + l);
		else if(r > w)
			sbx.уст(sbx + r - w);
	}

	if(resize_row_mode == 0)
	{
		int t = vitems[curpos.y].nTop(sby + fixed_height);
		int b = vitems[curpos.y].nBottom(sby);
		int h = дайРазм().cy - summary_height;

		if(t < 0)
			sby.уст(sby + t);
		else if(b > h)
			sby.уст(sby + b - h);
	}
}

bool GridCtrl::GoFirstVisible(bool scroll)
{
	if(пустой())
		return false;

	устКурсор0(curpos.x < 0 ? firstVisCol : curpos.x, max(firstVisRow, firstRow));
	if(scroll && resize_row_mode == 0)
		sby.уст(vitems[firstRow].nTop(/*fixed_height*/));
	if(isedit)
		UpdateCtrls();

	return true;
}

bool GridCtrl::идиВНач(bool scroll)                { return Go0(GO_BEGIN, scroll);                  }
bool GridCtrl::идиВКон(bool scroll, bool goleft)     { return Go0(GO_END, scroll, goleft);            }
bool GridCtrl::идиСледщ(bool scroll)                 { return Go0(GO_NEXT, scroll);                   }
bool GridCtrl::идиПредш(bool scroll)                 { return Go0(GO_PREV, scroll);                   }
bool GridCtrl::GoLeft(bool scroll, bool ctrlmode)  { return Go0(GO_LEFT, scroll, false, ctrlmode);  }
bool GridCtrl::GoRight(bool scroll, bool ctrlmode) { return Go0(GO_RIGHT, scroll, false, ctrlmode); }
bool GridCtrl::GoPageUp(bool scroll)               { return Go0(GO_PAGEUP, scroll);                 }
bool GridCtrl::GoPageDn(bool scroll)               { return Go0(GO_PAGEDN, scroll);                 }

Ктрл * GridCtrl::дайКтрл(const Точка &p, bool check_visibility, bool hrel, bool vrel, bool check_edits)
{
	return дайКтрл(p.y, p.x, check_visibility, hrel, vrel, check_edits);
}

Ктрл * GridCtrl::дайКтрл(int r, int c, bool check_visibility, bool hrel, bool vrel, bool check_edits)
{
	int idx = hrel ? fixed_cols + c : hitems[c].id;
	int idy = vrel ? fixed_rows + r : vitems[r].id;
	Ктрл * ctrl = items[idy][idx].ctrl;
	if(check_edits && !ctrl)
		ctrl = edits[idx].ctrl;
	if(check_visibility && ctrl && !ctrl->показан_ли())
		ctrl = NULL;
	return ctrl;
}

Ктрл * GridCtrl::дайКтрл(int r, int c)
{
	return дайКтрл(r + fixed_rows, c, true, true, false);
}

Ктрл * GridCtrl::GetCtrlAt(int r, int c)
{
	return дайКтрл(r + fixed_rows, c, false, true, false);
}

Ктрл * GridCtrl::дайКтрл(int c)
{
	return дайКтрл(rowidx, c, true, true, false);
}

bool GridCtrl::ктрл_ли(Точка &p, bool check_visibility)
{
	return дайКтрл(p, check_visibility, false, false) != NULL;
}

void GridCtrl::GoTo(int r, bool setcursor, bool scroll)
{
	r += fixed_rows;

	if(setcursor)
		if(!устКурсор0(r))
			return;

	if(scroll)
	{
		Размер sz = дайРазм();
		sby.уст(vitems[r].nTop() + vitems[r].nHeight() / 2 - sz.cy / 2);
	}
}

void GridCtrl::GoTo(int r, int c, bool setcursor, bool scroll)
{
	c += fixed_cols;
	r += fixed_rows;

	if(setcursor)
		if(!устКурсор0(c, r))
			return;

	if(scroll)
	{
		Размер sz = дайРазм();
		sbx.уст(hitems[c].nLeft() + hitems[c].nWidth() / 2 - sz.cx / 2);
		sby.уст(vitems[r].nTop() + vitems[r].nHeight() / 2 - sz.cy / 2);
	}
}


int GridCtrl::дайСчёт() const      { return total_rows - fixed_rows; }
int GridCtrl::GetFixedCount() const { return fixed_rows;              }
int GridCtrl::GetTotalCount() const { return total_rows;              }

int GridCtrl::GetVisibleCount() const
{
	int cnt = 0;
	for(int i = fixed_rows; i < total_rows; i++)
		if(!vitems[i].hidden)
			++cnt;
	return cnt;
}

GridCtrl& GridCtrl::SetColsMin(int size)
{
	for(int i = 1; i < total_cols; i++)
		hitems[i].min = size;

	return *this;
}

GridCtrl& GridCtrl::SetColsMax(int size)
{
	for(int i = 0; i < total_rows; i++)
		hitems[i].max = size;

	return *this;
}

void GridCtrl::сфокусирован()
{
	LG(3, "сфокусирован");
	RestoreFocus();
	if(valid_cursor)
		RefreshRow(curpos.y, 0, 0);
}

void GridCtrl::расфокусирован()
{
	LG(3, "расфокусирован");
	if(valid_cursor)
		RefreshRow(curpos.y, 0, 0);
	popup.закрой();
}

void GridCtrl::отпрыскФок()
{
	LG(3, "отпрыскФок");
	if(valid_cursor)
		RefreshRow(curpos.y, 0, 0);
	Ктрл::отпрыскФок();
}

void GridCtrl::отпрыскРасфок()
{
	LG(3, "отпрыскРасфок");
	if(valid_cursor)
	{
		//if(focus_lost_accepting && !HasFocusDeep())
		//	EndEdit();
		RefreshRow(curpos.y, 0, 0);
	}
	Ктрл::отпрыскРасфок();
}

void GridCtrl::Repaint(bool do_recalc_cols /* = false*/, bool do_recalc_rows /* = false*/, int opt)
{
	if(do_recalc_cols)
	{
		if(ready)
		{
			UpdateCols(true);
			firstCol = fixed_cols;
		}
		else
			recalc_cols = true;
	}

	if(do_recalc_rows)
	{
		if(ready)
		{
			UpdateRows(true);
			firstRow = fixed_rows;
		}
		else
			recalc_rows = true;
	}

	if(ready)
	{
		doscroll = false;
		UpdateSizes();
		UpdateSb();
		UpdateHolder();
		Updateсуммаmary();
		if(opt & RP_UPDCTRLS)
			UpdateCtrls();
		SyncCtrls();
		if(opt & RP_TOOLBAR)
			RebuildToolBar();
		doscroll = true;
		освежи();
	}
}

void GridCtrl::Ready(bool b)
{
	ready = b;
	if(b)
		Repaint(true);
}

GridCtrl& GridCtrl::ResizeColMode(int m)
{
	resize_col_mode = m;
	recalc_cols = true;
	освежиВыкладку();
	return *this;
}

GridCtrl& GridCtrl::ResizeRowMode(int m)
{
	resize_row_mode = m;
	recalc_rows = true;
	освежиВыкладку();
	return *this;
}

void GridCtrl::UpdateSb(bool horz, bool vert)
{
	scrollbox.устШирину(размПромотБара());

	if(horz)
	{
		sbx.устВсего(resize_col_mode == 0 ? total_width - fixed_width : 0);
		sbx.устСтраницу(дайРазм().cx - fixed_width);
	}

	if(vert)
	{
		sby.устВсего(resize_row_mode == 0 ? total_height - fixed_height + summary_height : 0);
		sby.устСтраницу(дайРазм().cy - fixed_height);
	}

	sbx.устФрейм(resize_row_mode == 0 && sby.дайВсего() > дайРазм().cy - fixed_height ? scrollbox : фреймПусто());
}

bool GridCtrl::SwitchEdit()
{
	if(!valid_cursor)
		return false;

	Ктрл * ctrl = items[curid.y][curid.x].ctrl;
	if(ctrl)
	{
		if(ctrl->HasFocusDeep())
			EndEdit(true, true);
		устФокус();
		rowbkp[curid.x] = ctrl->дайДанные();
		//items[curid.y][curid.x].val = ctrl->дайДанные();
		focused_ctrl = ctrl;
		focused_ctrl_id = curid.x;
		focused_ctrl_val = ctrl->дайДанные();
		ctrl->устФокус();
	}
	else
	{
		if(isedit)
			EndEdit(true, true);
		else
			StartEdit();
	}
	return true;
}

bool GridCtrl::StartEdit()
{
	if(!valid_cursor || !IsRowEditable())
		return false;

	WhenStartEdit();
	
	SetCtrlsData();
	UpdateCtrls(UC_SHOW | UC_CURSOR | UC_CTRLS | (goto_first_edit ? UC_GOFIRST : 0));
	return true;
}

bool GridCtrl::EndEdit(bool accept, bool doall, bool remove_row)
{
	if(!valid_cursor)
		return true;

	if(accept && !GetCtrlsData(false, doall, accept))
		return false;

	UpdateCtrls(UC_CTRLS);

	if(!accept)
	{
		CancelCtrlsData();
		if(newrow_inserted || newrow_appended)
		{
			newrow_inserted = false;
			newrow_appended = false;
			if(remove_row)
			{
				WhenCancelNewRow();
				удали0(curpos.y, 1, true, true, false);
			}
		}
	}
	WhenEndEdit();
	Syncсуммаmary();
	return true;
}

void GridCtrl::вставь0(int row, int cnt /* = 1*/, bool recalc /* = true*/, bool refresh /* = true*/, int size /* = GD_ROW_HEIGHT*/)
{
	int id;
	
	if(size < 0)
		size = GD_ROW_HEIGHT;

	if(row < total_rows)
	{
		id = vitems[row].id;
		for(int i = 0; i < total_rows; i++)
		{
			if(vitems[i].id >= id)
				vitems[i].id += cnt;			
		}
	}
	else
		id = total_rows;

	ItemRect ir;
	ir.size = size;
	vitems.вставь(row, ir, cnt);
	items.вставьН(id, cnt);

	for(int i = 0; i < cnt; i++)
	{
		int nid = id + i;
		int r = row + i;
		vitems[r].id = nid;
		vitems[r].uid = rowuid++;
		vitems[r].items = &items;
		vitems[r].operation = ready ? GridOperation::INSERT : GridOperation::NONE;
		items[nid].устСчёт(total_cols);
		UpdateDefaults(nid);
		rowidx = r;
		total_rows++;
		WhenCreateRow();
	}

	UpdateJoins(row, -1, cnt);

	firstRow = -1;

	if(recalc)
	{
		if(ready)
		{
			RecalcRows();
			UpdateSizes();
	
			if(refresh)
			{
				UpdateSb();
				Syncсуммаmary();
				SyncCtrls();
				RefreshFrom(row);
			}
		}
		else
			recalc_rows = true;
	}

	SetOrder();
	SetModify();
}

bool GridCtrl::удали0(int row, int cnt /* = 1*/, bool recalc /* = true*/, bool refresh /* = true*/, bool whens /* = true*/)
{
	if(cnt < 0)
		return false;

	bool cancel = true;
	int x = -1;
	int y = -1;

	for(int	i = 0; i < cnt; i++)
	{
		int rid = row + i;
		rowidx = remove_hides ? rid : row;

		if(vitems[rowidx].locked)
			continue;

		int id = vitems[rowidx].id;
		int op = vitems[rowidx].operation;

		vitems[rowidx].operation = GridOperation::REMOVE;
		
		if(remove_hides)
		{
			vitems[rowidx].hidden = true;
			vitems[rowidx].tsize = vitems[rowidx].size;
			vitems[rowidx].size = 0;
			vitems[rowidx].nsize = 0;
			//for this row in hitems selected flag should be cleared too
		}

		if(whens)
		{
			if(call_whenremoverow)
			{
				#ifdef LOG_CALLBACKS
				LGR(2, "WhenRemoveRow()");
				LGR(2, фмт("[row: %d]", rowidx));
				#endif
				WhenRemoveRow();
			}

			if(cancel_remove)
			{
				vitems[rowidx].operation = op;
				cancel_remove = false;
				cancel = false;
				if(i == cnt - 1)
					return cancel;
				else
					continue;
			}
			if(call_whenremoverow)
				WhenAcceptRow();
		}

		if(vitems[rowidx].IsSelect())
		{
			int si = 0;
			for(int j = fixed_cols; j < total_cols; j++)
				if(дайЭлт(rowidx, j).IsSelect())
					si++;

			selected_items -= si;
			selected_rows -= 1;
		}

		if(!remove_hides)
		{
			for(int j = 0; j < total_rows; j++)
				if(vitems[j].id > id)
					vitems[j].id--;
		}
		else
			vitems[rowidx].style = 0; //bo IsSelect korzysta z pola style


		total_height -= vitems[rowidx].nHeight();

		if(rid == ctrlid.y)
			UpdateCtrls(UC_HIDE | UC_CTRLS);

		bool removed = false;
		
		if(!remove_hides)
		{
			total_rows--;
			vitems.удали(rowidx);
			items.удали(id);
			removed = true;
		}

		if(rid == curpos.y)
		{
			x = curpos.x;
			y = remove_hides ? curpos.y + cnt : curpos.y;
			ClearCursor(true);
		}

		if(rid == oldcur.y)
		{
			oldcur.x = oldcur.y = -1;
		}

		if(whens && removed)
			WhenRemovedRow();
	}

	if(recalc)
	{
		if(ready)
		{
			RecalcRows();
			UpdateSizes();
	
			if(refresh)
			{
				UpdateSb();
				Syncсуммаmary();
				SyncCtrls();
				RefreshFrom(row);
	
				if(x >= 0 && y >= 0)
					устКурсор0(x, max(fixed_rows, min(total_rows - 1, y)), 0, 0, -1);
	
				if(!valid_cursor)
					RebuildToolBar();
			}
		}
		else
		{
			UpdateVisColRow(false);
			recalc_rows = true;
		}
	}
		
	firstRow = -1;

	if(пустой())
	{
		WhenEmpty();
		WhenCursor();
	}

	SetOrder();
	SetModify();
	return cancel;
}

int GridCtrl::Append0(int cnt, int size, bool refresh)
{
	if(size < 0)
		size = GD_ROW_HEIGHT;
	
	vitems.добавьН(cnt);
	items.добавьН(cnt);

	int j = total_rows;
	int k = j;
	int n = j > 0 ? vitems[j - 1].n + (int) vitems[j - 1].hidden : 0;
	for(int i = 0; i < cnt; i++)
	{
		ItemRect &ir = vitems[j];
		ir.parent = this;
		ir.items = &items;
		ir.size = ir.nsize = size;
		ir.operation = ready ? GridOperation::INSERT : GridOperation::NONE;

		if(total_rows > 0)
		{
			ir.pos = ir.npos = vitems[j - 1].nBottom();
			ir.n = n;
		}

		if(size == 0)
			ir.hidden = true;
		else
		{
			lastVisRow = j;
			if(firstVisRow < 0)
				firstVisRow = lastVisRow;
		}

		items[j].устСчёт(total_cols);
		ir.id = j++;
		ir.uid = rowuid++;
		UpdateDefaults(ir.id);
		rowidx = j - 1;
		total_rows = j;
		WhenCreateRow();
	}

	total_height += size * cnt;

	if(refresh && ready)
	{
		if(resize_row_mode > 0)
			UpdateRows(true);
		UpdateSb();
		Syncсуммаmary();
		SyncCtrls();
		RefreshFrom(k);
	}

	SetOrder();
	SetModify();
	return total_rows - fixed_rows;
}

bool GridCtrl::Duplicate0(int row, int cnt, bool recalc, bool refresh)
{
	int id;
	int nrow = row + cnt;

	if(nrow < total_rows)
	{
		id = vitems[nrow].id;
		for(int i = 0; i < total_rows; i++)
		{
			if(vitems[i].id >= id)
				vitems[i].id += cnt;
		}
	}
	else
		id = total_rows;

	ItemRect ir;
	vitems.вставь(nrow, ir, cnt);
	items.вставьН(id, cnt);
	
	int duplicated = 0;

	for(int i = 0; i < cnt; i++)
	{
		int nid = id + i;
		int r = nrow + i;
		vitems[r].id = nid;
		vitems[r].uid = rowuid++;
		vitems[r].items = &items;
		vitems[r].size = vitems[row + i].size;
		items[nid].устСчёт(total_cols);

		int oid = vitems[row + i].id;
		for(int j = 1; j < total_cols; j++)
			items[nid][j].val = items[oid][j].val;

		rowidx = r;
		total_rows++;
		WhenCreateRow();
		
		duplicated++;
		WhenDuplicateRow();		
		if(cancel_duplicate)
		{
			duplicated--;
			удали0(r, 1, false, false, false);
			cancel_duplicate = false;
		}
		
	}

	firstRow = -1;

	if(recalc)
	{
		if(ready)
		{
			RecalcRows();
			UpdateSizes();
	
			if(refresh)
			{
				UpdateSb();
				Syncсуммаmary();
				SyncCtrls();
				RefreshFrom(nrow);
			}
		}
		else
			recalc_rows = true;
	}

	if(duplicated > 0)
	{
		SetOrder();
		SetModify();
	}
	
	return duplicated > 0;
}

int GridCtrl::приставь(int cnt, bool refresh, int height)
{
	return Append0(cnt, height, refresh);
}

void GridCtrl::вставь(int i, int cnt)
{
	вставь0(fixed_rows + i, cnt);
}

void GridCtrl::удали(int i, int cnt)
{
	удали0(i < 0 ? rowidx : fixed_rows + i, cnt);
}

void GridCtrl::RemoveFirst(int cnt /* = 1*/)
{
	удали0(fixed_rows, min(total_rows - fixed_rows, cnt));
}

void GridCtrl::RemoveLast(int cnt /* = 1*/)
{
	удали0(total_rows - cnt, min(total_rows - fixed_rows, cnt));
}

void GridCtrl::Duplicate(int i, int cnt)
{
	Duplicate0(fixed_rows + i, cnt);
}

void GridCtrl::DoInsert0(bool edit, bool after)
{
	if(!valid_cursor)
		return;

	if(!EndEdit())
		return;

	SetItemCursor(curpos, false, false);
	RefreshRow(curpos.y, false);
	curpos.y += int(after);
	вставь0(curpos.y, 1, true, true, GD_ROW_HEIGHT);
	int y = curpos.y;
	curpos.y = -1;
	valid_cursor = false;
	call_whenchangecol = false;
	call_whenchangerow = false;
	устКурсор0(curpos.x, y > total_rows - 1 ? total_rows - 1 : y);
	call_whenchangecol = true;
	call_whenchangerow = true;

	newrow_inserted = true;

	if(edit)
		StartEdit();

	if(!isedit)
		WhenInsertRow0();

	WhenNewRow();

	if(!edit)
		newrow_inserted = false;
}

void GridCtrl::DoInsertBefore0(bool edit)
{
	DoInsert0(edit, false);
}

void GridCtrl::DoInsertAfter0(bool edit)
{
	DoInsert0(edit, true);
}

void GridCtrl::DoDuplicate0()
{
	int cy = 0;
	if(selected_rows == 0)
	{
		if(!valid_cursor)
			return;
		cy = curpos.y + 1;		
		if(!Duplicate0(curpos.y, 1, false, false))
			cy = 0;
	}
	else if(!multi_select)
	{
		cy = GetMinRowSelected() + selected_rows * 2 - 1;
		if(!Duplicate0(GetMinRowSelected(), selected_rows, false, false))
			cy = 0;
	}
	
	if(cy > 0)
	{
		Repaint(false, true);
		устКурсор0(curpos.x < 0 ? firstVisCol : curpos.x, max(fixed_rows, min(total_rows - 1, cy)));
	}
}

void GridCtrl::DoRemove()
{
	if(keep_last_row && дайСчёт() == 1)
		return;

	if(!valid_cursor && selected_rows == 0)
		return;

	if(ask_remove)
	{
		if(!PromptYesNo(фмт(t_("Do you really want to delete selected %s ?"), selected_rows > 1 ? t_("rows") : t_("row"))))
			return;
	}

	bool newrow = IsNewRow();
	CancelEdit(false);

	int y = curpos.y;
	int ocy = curpos.y;

	if(selected_rows == 0)
	{
		//do not call WhenRemoveRow when not new (not inserted) row
		удали0(curpos.y, 1, true, true, !newrow);
	}
	else
	{
		int not_removed = 0;

		minRowSelected = GetMinRowSelected();
		maxRowSelected = GetMaxRowSelected();

		if(keep_last_row && (maxRowSelected - minRowSelected + 1) == дайСчёт())
			maxRowSelected--;

		LG(0, "мин:%d, макс:%d", minRowSelected, maxRowSelected);

		for(int i = minRowSelected; i <= maxRowSelected; i++)
		{
			int rid = remove_hides ? i : minRowSelected + not_removed;
			if(vitems[rid].IsSelect())
			{
				sel_begin = i == minRowSelected;
				sel_end = i == maxRowSelected;

				if(удали0(rid, 1, false, false))
				{
					/* curpos.y tez sie zmienia bo gdy w whenromoverow jest woloanie innego okna to
					   grid traci fokus i wola sie lostfoucs, ktory wymaga poprawnego curpos.y */
					if(i == ocy)
						y = curpos.y = rid - 1;
				}
				else
					not_removed++;
			}
			else
				not_removed++;
		}
		RecalcRows();
		//UpdateSizes();
		SyncCtrls();
		UpdateSb();
		освежи();
		curpos.y = -1;
		valid_cursor = false;
		устКурсор0(curpos.x < 0 ? firstVisCol : curpos.x, max(fixed_rows, min(total_rows - 1, y)), 0, 0, -1);
	}
}

void GridCtrl::DoAppend0(bool edit)
{
	if(!EndEdit())
		return; //powinno byc zakomentowane ale wtedy goend wola endedit ale juz dla rowidx wiekszego o 1..

	Append0(1, GD_ROW_HEIGHT, true);

	call_whenchangecol = false;
	call_whenchangerow = false;
	идиВКон(true, true);
	call_whenchangecol = true;
	call_whenchangerow = true;

	newrow_appended = true;

	if(edit)
	{
		StartEdit();
		GoCursorLeftRight();
	}

	if(!isedit)
		WhenInsertRow0();

	WhenNewRow();

	if(!edit)
		newrow_appended = false;

}

void GridCtrl::DoAppend()             { DoAppend0(edits_in_new_row); }
void GridCtrl::DoAppendNoEdit()       { DoAppend0(0);                }
void GridCtrl::DoInsertBefore()       { DoInsertBefore0(1);          }
void GridCtrl::DoInsertBeforeNoEdit() {	DoInsertBefore0(0);          }
void GridCtrl::DoInsertAfter()        { DoInsertAfter0(1);           }
void GridCtrl::DoInsertAfterNoEdit()  { DoInsertAfter0(0);           }
void GridCtrl::DoDuplicate()          { DoDuplicate0();              }
void GridCtrl::DoEdit()               { StartEdit();                 }
void GridCtrl::DoEndEdit()            { EndEdit();                   }
void GridCtrl::DoCancelEdit()         { EndEdit(false);              }
void GridCtrl::DoSwapUp()             { SwapUp();                    }
void GridCtrl::DoSwapDown()           { SwapDown();                  }
void GridCtrl::DoGoBegin()            { идиВНач();                   }
void GridCtrl::DoGoEnd()              { идиВКон();                     }
void GridCtrl::DoGoNext()             { идиСледщ();                    }
void GridCtrl::DoGoPrev()             { идиПредш();                    }
void GridCtrl::DoGoLeft()             { GoLeft();                    }
void GridCtrl::DoGoRight()            { GoRight();                   }
void GridCtrl::DoGoPageUp()           { GoPageUp();                  }
void GridCtrl::DoGoPageDn()           { GoPageDn();                  }

void GridCtrl::выделиВсе()
{
	SelectCount(fixed_rows, total_rows - fixed_rows);
}

GridCtrl& GridCtrl::ShowRow(int n, bool refresh)
{
	if(!vitems[n].hidden)
		return *this;
	vitems[n].hidden = false;
	vitems[n].size = vitems[n].tsize;
	if(refresh)
		Repaint(false, true);

	return *this;
}

GridCtrl& GridCtrl::HideRow(int n, bool refresh)
{
	if(n < 0)
		n = rowidx;
	
	if(vitems[n].hidden)
		return *this;
	vitems[n].hidden = true;
	vitems[n].tsize = vitems[n].size;
	vitems[n].size = 0;
	if(refresh)
		Repaint(false, true);

	return *this;
}

GridCtrl& GridCtrl::ShowColumn(int n, bool refresh)
{
	if(!hitems[n].hidden)
		return *this;
	hitems[n].hidden = false;
	hitems[n].size = hitems[n].tsize;
	if(refresh)
		Repaint(true, false);

	return *this;
}

GridCtrl& GridCtrl::HideColumn(int n, bool refresh)
{
	if(hitems[n].hidden)
		return *this;
	hitems[n].hidden = true;
	hitems[n].tsize = hitems[n].size;
	if(refresh)
		Repaint(true, false);

	return *this;
}

void GridCtrl::HideRows(bool repaint)
{
	bool change = false;
	for(int i = fixed_rows; i < total_rows; i++)
		if(!vitems[i].hidden)
		{
			change = true;
			vitems[i].hidden = true;
			vitems[i].tsize = vitems[i].size;
			vitems[i].size = 0;
		}
	if(change || repaint)
		Repaint(false, true);
}

void GridCtrl::ShowRows(bool repaint)
{
	bool change = false;
	for(int i = fixed_rows; i < total_rows; i++)
		if(vitems[i].hidden)
		{
			change = true;
			vitems[i].hidden = false;
			vitems[i].size = vitems[i].tsize;
		}
	if(change || repaint)
		Repaint(false, true, UC_SCROLL);
}

void GridCtrl::MenuHideColumn(int n)
{
	if(hitems[n].hidden)
		ShowColumn(n);
	else
		HideColumn(n);
}

int GridCtrl::ShowMatchedRows(const ШТкст &f)
{
	if(f.пустой())
	{
		if(search_highlight)
			ClearFound();
		else
			ShowRows(true);
		return 0;
	}
	
	if(!search_immediate && search_highlight)
	{
		ClearFound(true, false);
		//search_string = f;
	}

	int first_matched_row = -1;

	int s, e;
	for(int i = fixed_rows; i < total_rows; i++)
	{
		if(!vitems[i].clickable)
			continue;
		
		int idv = vitems[i].id;
		
		for(int j = fixed_cols; j < total_cols; j++)
		{
			int idh = hitems[j].id;
			Элемент &it = items[idv][idh];
			it.Found(false);
			it.fs = it.fe = 0;

			if(hitems[j].hidden || !hitems[j].clickable)
				continue;

			if(сверь(f, (ШТкст) GetStdConvertedColumn(idh, it.val), s, e))
			{
				first_matched_row = i;
				rowfnd = i;
				goto found_first_matched_row;
			}
		}
	}

	if(first_matched_row < 0)
		return 0;

	found_first_matched_row:

	bool change = false;

	int rows = 0;
	for(int i = fixed_rows; i < total_rows; i++)
	{
		if(!vitems[i].clickable)
			continue;

		bool match = false;
		int idv = vitems[i].id;
		for(int j = fixed_cols; j < total_cols; j++)
		{
			int idh = hitems[j].id;
			Элемент &it = items[idv][idh];
			it.Found(false);
			it.fs = it.fe = 0;

			if(hitems[j].hidden || !hitems[j].clickable)
				continue;

			if(сверь(f, (ШТкст) GetStdConvertedColumn(idh, it.val), s, e))
			{
				match = true;
				it.Found(search_highlight);
				it.fs = s;
				it.fe = e;

				if(!search_highlight)
					break;
			}
		}

		if(match)
		{
			rows++;
			if(search_hide && vitems[i].hidden)
			{
				vitems[i].hidden = false;
				vitems[i].size = vitems[i].tsize;
				change = true;
			}

			if(search_highlight_first)
				break;
		}
		else if(search_hide && !vitems[i].hidden)
		{
			vitems[i].hidden = true;
			vitems[i].tsize = vitems[i].size;
			vitems[i].size = 0;
			change = true;
		}
		vitems[i].found = match;
	}

	if(rows > 0)
	{
		if(change || search_highlight)
		{
			LG(0, "Repaint %d", search_hide);
			Repaint(false, search_hide, 0);
		}
	}
	else if(search_hide)
	{
		for(int i = fixed_rows; i < total_rows; i++)
		{
			vitems[i].hidden = false;
			vitems[i].size = vitems[i].tsize;
		}
	}

	if(search_move_cursor && first_matched_row >= 0)
	{
		устКурсор0(first_matched_row);
		курсорПоЦентру();
		WhenSearchCursor();
	}

	LG(0, "Matched rows %d", rows);
	return rows;
}

void GridCtrl::ClearFound(bool showrows, bool clear_string)
{
	for(int i = 0; i < total_rows; i++)
	{
		vitems[i].found = false;
		for(int j = 0; j < total_cols; j++)
		{
			items[i][j].Found(false);
			items[i][j].fs = items[i][j].fe = 0;
		}
	}
	if(showrows)
		ShowRows(true);

	if(clear_string)
		search_string.очисть();
	
	WhenSearchCursor();
}

bool GridCtrl::сверь(const ШТкст &f, const ШТкст &s, int &fs, int &fe)
{
	int i = 0;

	int fl = f.дайДлину();
	int sl = s.дайДлину();

	if(fl > sl)
		return false;

	for(int j = find_offset; j < sl; j++)
	{
		bool match;
		if(search_case)
			match = f[i] == s[j];
		else
			match = взаг(f[i]) == взаг(s[j]);

		if(match)
		{
			if(++i == fl)
			{
				fs = j + 1 - fl;
				fe = j;
				return true;
			}
		}
		else
			i = 0;
	}
	return false;
}

void GridCtrl::DoFind()
{
	UpdateCtrls(UC_CHECK_VIS | UC_HIDE | UC_CTRLS | UC_SCROLL | UC_NOFOCUS);
	ShowMatchedRows((ШТкст) ~find);
}

bool GridCtrl::WhenInsertRow0()
{
	WhenInsertRow();
	if(cancel_insert)
	{
		WhenCancelNewRow();
		удали0(curpos.y, 1, true, true, false);
		cancel_insert = false;
		return false;
	}
	return true;
}

int GridCtrl::GetMinRowSelected()
{
	int i = fixed_rows;
	while(i < total_rows && !vitems[i].IsSelect()) i++;
	return i > total_rows - 1 ? -1 : i;
}

int GridCtrl::GetMaxRowSelected()
{
	int i = total_rows - 1;
	while(i >= fixed_rows && !vitems[i].IsSelect()) i--;
	return i < fixed_rows ? -1 : i;
}

void GridCtrl::CloseGrid()
{
	дайТопОкно()->закрой();
}

void GridCtrl::UpdateVisColRow(bool col)
{
	if(col)
	{
		firstVisCol = fixed_cols;
		lastVisCol = total_cols - 1;

		for(int i = fixed_cols; i < total_cols; i++)
			if(!hitems[i].hidden)
			{
				firstVisCol = i;
				break;
			}

		for(int i = total_cols - 1; i >= fixed_cols; i--)
			if(!hitems[i].hidden)
			{
				lastVisCol = i;
				break;
			}
	}
	else
	{
		firstVisRow = fixed_rows;
		lastVisRow = total_rows - 1;

		for(int i = fixed_rows; i < total_rows; i++)
			if(!vitems[i].hidden)
			{
				firstVisRow = i;
				break;
			}

		for(int i = total_rows - 1; i >= fixed_rows; i--)
			if(!vitems[i].hidden)
			{
				lastVisRow = i;
				break;
			}
	}
}

Точка GridCtrl::GetBarOffset()
{
	Размер bsz = bar.дайРазм();
	return Точка(bar.дайЛин() == КтрлБар::BAR_LEFT ? bsz.cx : 0,
	             bar.дайЛин() == КтрлБар::BAR_TOP ? bsz.cy : 0);
}

void GridCtrl::ClearModified()
{
	row_modified = 0;
	for(int i = 0; i < total_cols; ++i)
		items[curid.y][i].modified = false;
}

void GridCtrl::Debug(int n)
{
	if(n == 0)
	{
		LG("---- DEBUG 0 ----------");
		LG("firstVisCol    %d", firstVisCol);
		LG("lastVisCol     %d", lastVisCol);
		LG("firstVisRow    %d", firstVisRow);
		LG("lastVisRow     %d", lastVisRow);
		LG("firstCol       %d", firstCol);
		LG("firstRow       %d", firstRow);
		LG("lastCol        %d", lastCol);
		LG("lastRow        %d", lastRow);
		LG("total_cols     %d", total_cols);
		LG("total_rows     %d", total_rows);
		LG("curpos         %d, %d", curpos.x, curpos.y);
		LG("sbPos          %d, %d", sbx.дай(), sby.дай());
		LG("sbTotal        %d, %d", sbx.дайВсего(), sby.дайВсего());
		LG("sbPage         %d, %d", sbx.дайСтраницу(), sby.дайСтраницу());
		LG("Размер           %d, %d", дайРазм().cx, дайРазм().cy);
		LG("fixed_width    %d", fixed_width);
		LG("fixed_height   %d", fixed_height);
		LG("total_width    %d", total_width);
		LG("total_height   %d", total_height);
		LG("row_modified   %d", row_modified);
		LG("selected_rows  %d", selected_rows);
		LG("selected_items %d", selected_items);
		LG("---- END --------------");
	}
	if(n == 1)
	{
		LG("---- DEBUG 1 ----------");
		for(int i = 0; i < total_cols; i++)
		{
			LG("Col %d h:%d p:%d s:%d", i, hitems[i].hidden, hitems[i].npos, hitems[i].nsize);
			//LG("ismin %d ismax %d", hitems[i].ismin, hitems[i].ismax);
		}
		LG("---- END --------------");
	}
	if(n == 2)
	{
		LG("---- DEBUG 2 ----------");
		for(int i = 0; i < total_rows; i++)
		{
			LG("Row %d p:%d s:%d", i, vitems[i].npos, vitems[i].nsize);
		}
		LG("---- END --------------");
	}
	if(n == 3)
	{
		Точка p = GetCtrlPos(focused_ctrl);
		LG(2, "Focused %x (%d, %d)", focused_ctrl, p.x, p.y);
	}
}

void GridCtrl::сериализуй(Поток &s)
{
	if(s.сохраняется())
	{
		s % total_cols;
		for(int i = 1; i < total_cols; i++)
		{
			int id = hitems[i].id;
			//s % Ткст(aliases[id]);
			s % id;
			s % hitems[id];
		}
	}
	else
	{
		int tc;
		s % tc;
		for(int i = 1; i < tc; i++)
		{
			//Ткст alias;
			//s % alias;
			//int n = aliases.найди(alias);
			int id;
			s % id;
			//if(id >= 0 && id < total_cols)
			s % hitems[id];
		}
	}
}

void GridCtrl::JoinCells(int left, int top, int right, int bottom, bool relative)
{
	int fc = relative ? fixed_cols : 0;
	int fr = relative ? fixed_rows : 0;

	left   += fc;
	top    += fr;
	right  += fc;
	bottom += fr;

	int idx = hitems[left].id;
	int idy = vitems[top].id;
	int cx = right - left;
	int cy = bottom - top;
	
	for(int i = top; i <= bottom; ++i)
	{
		vitems[i].join++;

		for(int j = left; j <= right; ++j)
		{
			Элемент &it = items[i][j];

			it.idx = idx;
			it.idy = idy;
			it.cx  = cx;
			it.cy  = cy;
			it.группа = join_group;
			it.isjoined = true;

			if(i == top)
				hitems[j].join++;
		}
	}
	
	JoinRect& jr = joins.добавь();
	jr.r.уст(left, top, right, bottom);
	jr.группа = join_group;
	jr.idx = idx;
	jr.idy = idy;
	
	join_group++;
}

void GridCtrl::JoinFixedCells(int left, int top, int right, int bottom)
{
	JoinCells(left, top, right, bottom, false);
}

void GridCtrl::JoinRow(int n, int left, int right)
{
	if(n < 0)
		n = rowidx;

	if(left < 0)
		left = fixed_cols;
	else
		left += fixed_cols;

	if(right < 0)
		right = total_cols - fixed_cols;
	else
		right = total_cols - fixed_cols - right;

	JoinCells(left, n, right, n, false);
}

void GridCtrl::JoinRow(int left, int right)
{
	JoinRow(-1, left, right);
}

/*

jr.r.top = 2;
jr.r.bottom = 6

2 -----------------------
3 ----------------------- 
  +++++++++++++++++++++++  //insert at 4
  +++++++++++++++++++++++
4 -----------------------
5 -----------------------
6 -----------------------

*/

void GridCtrl::UpdateJoins(int row, int col, int cnt)
{
	if(row >= 0)
	{
		for(int i = 0; i < joins.дайСчёт(); i++)
		{
			JoinRect& jr = joins[i];
			
			int top = jr.r.top;
			int bottom = jr.r.bottom;

			if(row > top && row < bottom) // new row is inside cell rect, idy doesn't change but cy does
			{
				for(int r = jr.r.top; r <= jr.r.bottom; r++)
				{
					for(int c = jr.r.left; c <= jr.r.right; c++)
					{
						if(r < row || r > row)
						{
							Элемент& it = дайЭлт(r, c);
							it.cy += cnt;
						}
						else if(r == row)
						{
							for(int n = 0; n < cnt; n++)
							{
								Элемент& it = дайЭлт(r + n, c);
								it.группа = jr.группа;
								it.idx = jr.idx;
								it.idy = jr.idy;
								it.cx = jr.r.устШирину();
								it.cy = jr.r.устВысоту() + cnt;
								it.isjoined = true;
			
								if(c == jr.r.left)
									vitems[r + n].join++;
							}
						}
					}				
				}
								
				jr.r.bottom += cnt;
			}
			else if(row <= top) // idy changes
			{
				jr.idy += cnt;
				
				for(int r = jr.r.top; r <= jr.r.bottom; r++)
				{
					for(int c = jr.r.left; c <= jr.r.right; c++)
					{
						Элемент& it = дайЭлт(r + cnt, c);
						it.idy = jr.idy;
					}
				}

				jr.r.top += cnt;
				jr.r.bottom += cnt;
			}
		}	
	}
	
	if(col >= 0)
	{
		for(int i = 0; i < joins.дайСчёт(); i++)
		{
			JoinRect& jr = joins[i];
			
			int left = jr.r.left;
			int right = jr.r.right;

			if(col > left && col < right)
			{
				for(int c = jr.r.left; c <= jr.r.right; c++)
				{
					for(int r = jr.r.top; r <= jr.r.bottom; r++)
					{
						if(c < col || c > col)
						{
							Элемент& it = дайЭлт(r, c);
							it.cx += cnt;
						}
						else if(c == col)
						{
							for(int n = 0; n < cnt; n++)
							{
								Элемент& it = дайЭлт(r, c + n);
								it.группа = jr.группа;
								it.idx = jr.idx;
								it.idy = jr.idy;
								it.cx = jr.r.устШирину() + cnt;
								it.cy = jr.r.устВысоту();
								it.isjoined = true;
			
								if(r == jr.r.top)
									hitems[c + n].join++;
							}
						}
					}				
				}
								
				jr.r.right += cnt;
			}
			else if(col <= left)
			{
				jr.idx += cnt;
				
				for(int c = jr.r.left; c <= jr.r.right; c++)
				{
					for(int r = jr.r.top; r <= jr.r.bottom; r++)
					{
						Элемент& it = дайЭлт(r, c + cnt);
						it.idx = jr.idx;
					}
				}

				jr.r.left += cnt;
				jr.r.right += cnt;
			}
		}	
	}
}

/*----------------------------------------------------------------------------------------*/

void GridPopUp::рисуй(Draw &w)
{
	Размер sz = дайРазм();
	if(gd->row < 0 || gd->col < 0)
		gd->PaintFixed(w, false, false, 1, 1, sz.cx - 2, sz.cy - 2, val, style | GD::WRAP | GD::VCENTER, fnt);
	else
		gd->рисуй(w, 1, 1, sz.cx - 2, sz.cy - 2, val, style | GD::WRAP | GD::VCENTER, fg, bg, fnt);
	DrawBorder(w, sz, BlackBorder);
}

Точка GridPopUp::смещение(Точка p)
{
	return p + GetScreenView().верхЛево() - ctrl->GetScreenView().верхЛево();
}

void GridPopUp::леваяВнизу(Точка p, dword flags)
{
	ctrl->леваяВнизу(смещение(p), flags);
}

void GridPopUp::леваяТяг(Точка p, dword flags)
{
	закрой();
	ctrl->леваяТяг(смещение(p), flags);
}

void GridPopUp::леваяДКлик(Точка p, dword flags)
{
	ctrl->леваяДКлик(смещение(p), flags);
}

void GridPopUp::праваяВнизу(Точка p, dword flags)
{
	ctrl->праваяВнизу(смещение(p), flags);
}

void GridPopUp::леваяВверху(Точка p, dword flags)
{
	ctrl->леваяВверху(смещение(p), flags);
}

void GridPopUp::колесоМыши(Точка p, int zdelta, dword flags)
{
	ctrl->колесоМыши(смещение(p), zdelta, flags);
}

void GridPopUp::выходМыши()
{
	ctrl->выходМыши();
	закрой();
}

void GridPopUp::входМыши(Точка p, dword flags)
{
	ctrl->входМыши(смещение(p), flags);
}

void GridPopUp::двигМыши(Точка p, dword flags)
{
	ctrl->двигМыши(смещение(p), flags);
}

Рисунок GridPopUp::рисКурсора(Точка p, dword flags)
{
	return ctrl->рисКурсора(смещение(p), flags);
}

void GridPopUp::расфокусирован()
{
	закрой();
}

void GridPopUp::PopUp(Ктрл *owner, int x, int y, int width, int height)
{
	Прям r(x, y, x + width, y + height);
	if(r != дайПрям())
		устПрям(r);
	
	if(!open)
	{
		ctrl = owner;
		open = true;
		Ктрл::PopUp(owner, true, false, GUI_DropShadows());
		SetAlpha(230);
	}
}

void GridPopUp::закрой()
{
	open = false;
	Ктрл::закрой();
}

void GridCtrl::UpdateHighlighting(int mode, Точка p)
{
	if(resize_paint_mode < 1 && (resizeCol || resizeRow))
		return;

	bool refresh = false;
	
	if(hcol >= hitems.дайСчёт())
		hcol = -1;

	if(mode == GS_UP)
	{
		if((p.x < 0 || p.x > дайРазм().cx - 1) && hcol >= 0 && hitems[hcol].IsHighlight())
		{
			hitems[hcol].Highlight(0);
			refresh = true;
			hcol = -1;
		}
	}
	else if(mode == GS_MOVE)
	{
		int col = GetMouseCol(p, true, true, false);
		int row = hrow = GetMouseRow(p, false, true, true);

		if(hcol >= 0 && (hcol != col || row != 0) && hitems[hcol].IsHighlight())
		{
			hitems[hcol].Highlight(0);
			refresh = true;
		}
		if(col >= 0 && row == 0 && !hitems[col].IsHighlight())
		{
			hitems[col].Highlight(1);
			hcol = col;
			refresh = true;
		}
	}
	else if(mode == GS_POPUP)
	{
		if(hcol >= 0)
			hitems[hcol].Highlight(0);
		refresh = true;
		hcol = moveCol;
	}
	else if(mode == GS_BORDER)
	{
		if(hcol >= 0 && hitems[hcol].IsHighlight())
		{
			hitems[hcol].Highlight(0);
			refresh = true;
		}
	}
	if(refresh)
		RefreshRow(0, 0, 1);
}

Ткст GridCtrl::GetColumnWidths()
{
	Ткст s;
	for(int i = fixed_cols; i < total_cols; i++)
	{
		s += какТкст(hitems[i].nsize);
		if(i < total_cols - 1)
			s += " ";
	}
	return s;
}

void GridCtrl::ColumnWidths(const char* s)
{
	Вектор<Ткст> w = РНЦП::разбей(s, ' ');
	for(int i = 0; i < min(w.дайСчёт(), дайСчётКолонок()); i++)
		hitems[i + fixed_cols].устШирину(atoi(w[i]));
}

void GridCtrl::устДисплей(int r, int c, GridDisplay& gd)
{
	дайЭлт(r + fixed_rows, c + fixed_cols).устДисплей(gd);
}

#ifdef flagGRIDSQL
void GridCtrl::FieldLayout(FieldOperator& f)
{
	for(int i = 1; i < total_cols; i++)
	{
		if(hitems[i].hidden)
			continue;
		int id = hitems[i].id;
		const Ид& ключ = aliases.дайКлюч(id);
		f(ключ, items[vitems[rowidx].id][id].val);
	}
}

SqlSet GridCtrl::GetColumnList(bool skip_hidden) const
{
	SqlSet s;
	for(int i = 1; i < total_cols; i++)
	{
		if(skip_hidden && hitems[i].hidden)
			continue;
		int id = hitems[i].id;
		s.конкат(SqlId(aliases.дайКлюч(id)));
	}
	return s;
}

#endif


/*----------------------------------------------------------------------------------------*/
GridFind::GridFind()
{
	MultiButton::SubButton& btn = button.AddButton().Main();
	btn.WhenPush = THISBACK(сунь);
	btn.SetMonoImage(CtrlImg::smallright());

	button.устСтиль(button.StyleFrame());
	button.NoDisplay();
	button.добавьК(*this);
}

bool GridFind::Ключ(dword ключ, int count)
{
	if(ключ == K_ENTER && WhenEnter)
	{
		WhenEnter();
		return true;
	}

	return EditString::Ключ(ключ, count);
}

Размер GridFind::дайМинРазм() const
{
	return button.дайМинРазм();
}

void GridFind::сунь()
{
	БарМеню::выполни(WhenBar, дайПрямЭкрана().верхПраво());
}

/*----------------------------------------------------------------------------------------*/
void GridPopUpHeader::рисуй(Draw &w)
{
	Размер sz = дайРазм();
	dword style = chameleon ? GD::CHAMELEON : 0;
	Шрифт stdfont(StdFont());
	дисплей->PaintFixed(w, 1, 1, 0, 0, sz.cx, sz.cy,
		                val, style, stdfont, false, true,
		                sortmode, sortcol, sortcnt, true);
}

void GridPopUpHeader::PopUp(Ктрл *owner, int x, int y, int width, int height)
{
	устПрям(Прям(x, y, x + width, y + height));
	if(open)
		return;
	Ктрл::PopUp(owner, true, true, GUI_DropShadows());
	SetAlpha(200);
	open = true;
}

void GridPopUpHeader::закрой()
{
	open = false;
	Ктрл::закрой();
}

/*----------------------------------------------------------------------------------------*/
GridButton::GridButton()
{
	n = 0;
	img = 0;
}

void GridButton::рисуй(Draw& w)
{
	static Рисунок (*vimg[])() = {
		&GridImg::Btn0N, &GridImg::Btn0H, &GridImg::Btn0P,
		&GridImg::Btn1N, &GridImg::Btn1H, &GridImg::Btn1P
	};
	
	Размер sz = дайРазм();
	w.DrawImage(0, 0, sz.cx, sz.cy, vimg[img + n * 3]);
}

void GridButton::леваяВнизу(Точка p, dword flags)
{
	img = 2;
	освежи();
}

void GridButton::леваяВверху(Точка p, dword flags)
{
	img = 1;
	освежи();
	Action();
}

void GridButton::входМыши(Точка p, dword flags)
{
	img = flags & K_MOUSELEFT ? 2 : 1;
	освежи();
}

void GridButton::выходМыши()
{
	img = 0;
	освежи();
}

void GridButton::State(int reason)
{
	if(reason == CLOSE)
		img = 0;
}

Размер GridButton::дайСтдРазм() const
{
	return n > 0 ? Размер(14, 11) : Размер(17, 17); //FIXME
}

void GridButton::SetButton(int b)
{
	n = b;
}

GridResizePanel::GridResizePanel()
{
	int h = max(16, Draw::GetStdFontCy()) + 5;
	устВысоту(h);
	Размер csz = close.дайСтдРазм();
	добавь(close.LeftPos(1, csz.cx).TopPos((h - csz.cy) / 2, csz.cy));
	minsize.очисть();
	close.WhenAction = прокси(WhenClose);
}

void GridResizePanel::рисуй(Draw& w)
{
	Размер sz = дайРазм();
	w.DrawRect(sz, SColorFace);
	Размер isz = CtrlsImg::SizeGrip().дайРазм();
	w.DrawImage(sz.cx - isz.cx, sz.cy - isz.cy, CtrlsImg::SizeGrip());
}

bool GridResizePanel::MouseOnGrip(const Точка &p)
{
	Размер isz = CtrlsImg::SizeGrip().дайРазм();
	Размер sz = дайРазм();
	return (p.x > sz.cx - isz.cx && p.y > sz.cy - isz.cy);
}

void GridResizePanel::устМинРазм(Размер sz)
{
	minsize = sz;
}

Рисунок GridResizePanel::рисКурсора(Точка p, dword flags)
{
	if(MouseOnGrip(p) || HasCapture())
		return Рисунок::SizeBottomRight();
	return Рисунок::Arrow();
}

void GridResizePanel::леваяВнизу(Точка p, dword flags)
{
	if(MouseOnGrip(p))
	{
		r = дайРодителя()->дайПрямЭкрана();
		pos = дайПозМыши();
		SetCapture();
	}
}

void GridResizePanel::леваяВверху(Точка p, dword flags)
{
	ReleaseCapture();
}

void GridResizePanel::двигМыши(Точка p, dword flags)
{
	if(HasCapture())
	{
		Точка curpos = дайПозМыши();
		Точка diff = curpos - pos;
		Прям r(this->r);
		r.right += diff.x;
		r.bottom += diff.y;
		bool setmin = false;

		if(!minsize.пустой())
		{
			if(r.right < r.left + minsize.cx)
			{
				r.right = r.left + minsize.cx;
				setmin = true;
			}
			if(r.bottom < r.top + minsize.cy)
			{
				r.bottom = r.top + minsize.cy;
				setmin = true;
			}
		}
		if(this->r != r || setmin)
		{
			дайРодителя()->устПрям(r);
			дайРодителя()->синх();
		}
	}
}

/*----------------------------------------------------------------------------------------*/

template<> void вРяр(РярВВ& xml, GridCtrl& g) {
	Вектор< Вектор<Значение> > v;
	
	if(xml.грузится()) {
		xml("data", v);
		g.SetValues(v);
	} else {
		v = g.дайЗначения();
		xml("data", v);
	}
}

template<> void вДжейсон(ДжейсонВВ& json, GridCtrl& g) {
	Вектор< Вектор<Значение> > v;
	
	if(json.грузится()) {
		json("data", v);
		g.SetValues(v);
	} else {
		v = g.дайЗначения();
		json("data", v);
	}	
}

/* after this assist++ sees nothing */
//$-
#define E__Addv0(I)    уст(q, I - 1, p##I)
#define E__AddF0(I) \
GridCtrl& GridCtrl::добавь(__List##I(E__Value)) { \
	int q = дайСчёт(); \
	__List##I(E__Addv0); \
	return *this; \
}
__Expand(E__AddF0)

#define E__Addv1(I)    уст(q, I - 1, p##I)
#define E__AddF1(I) \
GridCtrl::ItemRect& GridCtrl::AddRow(__List##I(E__Value)) { \
	int q = дайСчёт(); \
	ItemRect& ir = AddRow(); \
	__List##I(E__Addv1); \
	return ir; \
}
__Expand(E__AddF1)

}
