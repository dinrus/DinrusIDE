#include "GridCtrl.h"

namespace РНЦП {

void GridCtrl::рисуй(Draw &w)
{
	static int paintcnt = 0;

	Шрифт stdfont(StdFont());

	Размер sz = дайРазм();
	Прям rc = Прям(sz);  //w.GetClip() - it always returns view rect now. bug??
	
	if(!ready)
	{
		w.DrawRect(rc, SColorPaper);
		return;
	}
	
	int i, j, cx, cy, x, y;
	bool skip;
	Прям r;

	LG(0, "---- рисуй(%d)", ++paintcnt);

	if(total_cols <= 1 || total_rows == 0)
	{
		LG(0, "---- рисуй(%d) Empty.", paintcnt);
		w.DrawRect(sz, SColorPaper);
		return;
	}

	if(UpdateCols() || UpdateRows())
		UpdateSizes();

	if(firstCol < 0) firstCol = GetFirstVisCol(fixed_width);
	if(firstRow < 0) firstRow = GetFirstVisRow(fixed_height);

	LG(0, "firstCol %d", firstCol);
	LG(0, "firstRow %d", firstRow);

	int en = IsShowEnabled() ? 0 : GD::READONLY;

	//---------------------------------------------------------------------------------------

	if(fixed_width > 0 && fixed_height > 0)
	{
		w.Clip(0, 0, fixed_width, fixed_height);
		dword style = chameleon ? GD::CHAMELEON : 0;

		дисплей->PaintFixed(w, 1, 1, 0, 0, fixed_width, fixed_height,
							Значение(""),
							style, stdfont, false, false,
							0, -1, 0,
							true);
		w.стоп();
	}

	r.уст(fixed_width, 0, sz.cx, summary_row ? sz.cy : fixed_height);

	if(w.IsPainting(r) && total_cols > 1)
	{
		LG(0, "верх header");
		w.Clip(r);

		x = hitems[total_cols - 1].nRight(sbx);
		int rx = x;

		int firstcol = indicator ? 0 : (fixed_cols > 1 ? 1 : firstVisCol);
		if(firstCol < 0) firstCol = 0;
		int jfc = chameleon ? 0 : firstCol;

		for(i = 0; i < fixed_rows; i++)
		{
			for(j = jfc; j < total_cols; j++)
			{
				ItemRect& hi = hitems[j];
				
				if(hi.hidden)
					continue;

				int jj = j;
				Элемент &it = GetItemSize(i, j, x, y, cx, cy, skip, true, false);
				if(skip)
					continue;

				if(x >= rc.right)
					break;

				if(w.IsPainting(x, y, cx, cy))
				{
					GridDisplay * gd = it.дисплей ? it.дисплей : дисплей;

					dword style = hi.style | hi.halign;
					if(i > 0) style &= ~GD::HIGHLIGHT;
					if(chameleon)
						style |= GD::CHAMELEON;

					Шрифт fnt(stdfont);
					gd->SetLeftImage(hi.img);
					gd->ReverseSortIcon(reverse_sort_icon);
					gd->PaintFixed(w, jj == firstcol, i == 0, x, y, cx, cy,
								i == 0 ? it.val : GetConvertedColumn(hi.id, it.val),
								style | en, пусто_ли(hi.hfnt) ? fnt : hi.hfnt, false, false,
								i == 0 ? hi.sortmode : 0,
								hi.sortcol,
								sortOrder.дайСчёт(),
								true);
					
					it.rcx = gd->real_size.cx;
					it.rcy = gd->real_size.cy;
				}
				
				if(summary_row && i == 0)
				{
					cy = GD_HDR_HEIGHT;
					y = sz.cy - cy;
				
					if(w.IsPainting(x, y, cx, cy))
					{
						Элемент &it = summary[hi.id];
						GridDisplay * gd = it.дисплей ? it.дисплей : дисплей;
	
						dword style = en | hi.halign;
						if(chameleon)
							style |= GD::CHAMELEON;
						
						Ткст s;
						if(hi.sop != SOP_NONE && !hi.sopfrm.пустой() && !пусто_ли(it.val))
							s = фмт(hi.sopfrm, it.val);
						else
							s = ткст_ли(it.val) ? it.val : стдПреобр().фмт(it.val);
						gd->SetLeftImage(Null);
						//gd->PaintFixed(w, jj == firstcol, i == 0, x, y, cx, cy, s,
						//			   style | en, stdfont, false, false, 0, -1, 0, true);
						Цвет fg = чёрный;
						Цвет bg = смешай(синий, белый, 240);
						w.DrawRect(x, y, cx, 1, серый);
						Шрифт fnt(stdfont);

						if(style & GD::READONLY)
						{
							bg = смешай(bg, SGray(), 40);
							fg = смешай(fg, SGray(), 200);
						}
						
						gd->рисуй(w, x, y + 1, cx, cy - 1, s, style, fg, bg, fnt.Bold(), false, 0, 0);
						
						it.rcx = gd->real_size.cx;
						it.rcy = gd->real_size.cy;
					}
				}
			}
		}
		if(rx < sz.cx || chameleon)
		{
			int cx = sz.cx - rx + 1;
			dword style = 0;
			if(chameleon)
			{
				cx = max(10, cx);
				style = GD::CHAMELEON;
			}
			дисплей->PaintFixed(w, 0, 1, rx, 0, cx, fixed_height,
								Значение(""),
								style, stdfont, false, false,
								0, -1, 0,
								true);
			if(summary_row)
			{
				Цвет fg = чёрный;
				Цвет bg = смешай(синий, белый, 240);
				w.DrawRect(rx, y, cx, 1, серый);
				
				if(style & GD::READONLY)
				{
					bg = смешай(bg, SGray(), 40);
					fg = смешай(fg, SGray(), 200);
				}
				дисплей->рисуй(w, rx, y + 1, cx, GD_HDR_HEIGHT - 1, Значение(""), style, fg, bg, stdfont, false, 0, 0);
			}
		}

		w.стоп();
	}
	//---------------------------------------------------------------------------------------

	bool can_paint = firstCol >= 0 && firstRow >= 0;
	r.уст(0, fixed_height, fixed_width, sz.cy);

	if(can_paint && w.IsPainting(r))
	{
		LG(0, "лево header");
		w.Clip(r);
		y = vitems[total_rows - 1].nBottom(sby);

		if(y < sz.cy)
			w.DrawRect(Прям(0, y, fixed_width, sz.cy - summary_height), SColorPaper);

		for(i = 0; i < fixed_cols; i++)
		{
			bool firstx = (i == !indicator);
			bool indicator = (i == 0);
			int id = hitems[i].id;

			for(j = firstRow; j < total_rows; j++)
			{
				ItemRect& vi = vitems[j];
				
				if(vi.hidden)
					continue;

				Элемент &it = GetItemSize(j, i, x, y, cx, cy, skip, false, true);

				if(skip)
					continue;

				if(y >= rc.bottom)
					break;

				if(w.IsPainting(x, y, cx, cy))
				{
					GridDisplay * gd = it.дисплей ? it.дисплей : дисплей;

					dword style = vi.style;
					if(i > 0) style &= ~GD::HIGHLIGHT;
					if(chameleon)
						style |= GD::CHAMELEON;

					Шрифт fnt(stdfont);
					gd->PaintFixed(w, firstx, j == 0, x, y, cx, cy,
									GetConvertedColumn(id, it.val),
									style | en, пусто_ли(vi.hfnt) ? fnt : vi.hfnt,
									indicator, false, 0, -1, 0, false);
					
					it.rcx = gd->real_size.cx;
					it.rcy = gd->real_size.cy;
				}
			}
			
			if(summary_row)
			{
				j = 0;
				cy = GD_HDR_HEIGHT;
				y = sz.cy - cy;
				if(w.IsPainting(x, y, cx, cy))
				{
					Элемент& it = summary[hitems[i].id];
					GridDisplay * gd = it.дисплей ? it.дисплей : дисплей;

					dword style = vitems[j].style;
					if(chameleon)
						style |= GD::CHAMELEON;

//					gd->PaintFixed(w, firstx, true, x, y, cx, cy,
//									GetConvertedColumn(id, it.val),
//									style | en, stdfont,
//									false, false, 0, -1, 0, false);
//
					Цвет fg = чёрный;
					Цвет bg = смешай(синий, белый, 240);
					w.DrawRect(x, y, cx, 1, серый);
					Шрифт fnt(stdfont);
					gd->рисуй(w, x, y + 1, cx, cy - 1, GetConvertedColumn(id, it.val), style | en, fg, bg, fnt.Bold(), false, 0, 0);

				}
			}
		}

		w.стоп();
	}

	//---------------------------------------------------------------------------------------
	// Draw the "active" (non-fixed) cells:

	r.уст(fixed_width, fixed_height, sz.cx, sz.cy - summary_height);

	if(can_paint && w.IsPainting(r))
	{
		LG(0, "Body");
		w.Clip(r);

		x = hitems[total_cols - 1].nRight(sbx);
		y = vitems[total_rows - 1].nBottom(sby);

		if(x < sz.cx) w.DrawRect(Прям(max(x, rc.left), max(fixed_height, rc.top), sz.cx, sz.cy), SColorPaper);
		if(y < sz.cy) w.DrawRect(Прям(max(fixed_width, rc.left), max(y, rc.top), sz.cx, sz.cy), SColorPaper);

		bool hasfocus = естьФокус() || holder.HasFocusDeep();

		for(i = max(firstRow, fixed_rows); i < total_rows; i++)
		{
			ItemRect& vi = vitems[i];
			if(vi.hidden) continue;

			bool even = coloring_mode == 2 ? (i - vi.n - fixed_rows) & 1 : false;

			for(j = max(firstCol, fixed_cols); j < total_cols; j++)
			{
				const ItemRect& hi = hitems[j];

				if(hi.hidden)
					continue;

				Элемент &it = GetItemSize(i, j, x, y, cx, cy, skip);
				if(skip)
					continue;

				if(y >= rc.bottom)
					goto end_paint;

				if(x >= rc.right)
					break;

				if(!w.IsPainting(0, y, sz.cx, cy))
					break;

				if(w.IsPainting(x, y, cx, cy))
				{
					bool iscur = draw_focus ? (i == curpos.y && j == curpos.x) : false;

					if(coloring_mode == 1)
						even = (j - hi.n - fixed_cols) & 1;

					int id = hi.id;

					dword style = en | (select_row ? vi.style : it.style) | hi.balign;
					dword istyle = it.style;

					if(hitems[j].wrap)
						style |= GD::WRAP;
					if(ctrlpos.y == i && edits[id].ctrl && edits[id].ctrl->показан_ли())
						style |= GD::NOTEXT;
					if(it.ctrl)
						style |= GD::NOTEXT;

					if(coloring_mode > 0)
						style |= (even ? GD::EVEN : GD::ODD);
					if(hasfocus)
						style |= GD::FOCUS;
					
					if(IsValidCursor(anchor) && anchor != curpos && multi_select) { // mouse selection in progress
						Прям r(anchor, curpos);
						r.нормализуй();
						r.устРазм(r.дайРазм() + 1);
						if(select_row) {
							r.left = 0;
							r.right = INT_MAX;
						}
						if(r.содержит(Точка(j, i)))
							style |= GD::SELECT;
					}

					Цвет cfg;
					Цвет cbg;

					Шрифт fnt = StdFont();
					GridDisplay* gd;
					Значение val = GetItemValue(it, id, hi, vi);
					GetItemAttrs(it, val, i, j, vi, hi, style, gd, cfg, cbg, fnt);

					Цвет fg = SColorText;
					Цвет bg = SColorPaper;

					bool custom = true;

					if(style & GD::CURSOR)
					{
						if(style & GD::FOCUS)
						{
							fg = iscur ? смешай(bg_focus, чёрный, 230) : fg_focus;
							bg = iscur ? смешай(bg_focus, белый, 230) : bg_focus;
						}
						else
						{
							bg = смешай(SColorDisabled, bg);
						}
						custom = false;
					}
					else if(style & GD::LIVE)
					{
						fg = fg_live;
						bg = bg_live;
						custom = false;
					}
					else if(istyle & GD::FOUND)
					{
						fg = fg_found;
						bg = bg_found;
						custom = false;
					}
					else if(style & GD::SELECT)
					{
						fg = fg_select;
						bg = bg_select;
						custom = false;
					}
					else if(style & GD::EVEN)
					{
						fg = fg_even;
						bg = bg_even;
					}
					else if(style & GD::ODD)
					{
						fg = fg_odd;
						bg = bg_odd;
					}

					if(custom)
					{
						if(!пусто_ли(cfg)) fg = cfg;
						if(!пусто_ли(cbg)) bg = cbg;
					}
					
					if(style & GD::READONLY)
					{
						bg = смешай(bg, SGray(), 40);
						fg = смешай(fg, SGray(), 200);
					}

					gd->SetBgImage(vi.img);
					gd->col = j - fixed_rows;
					gd->row = i - fixed_cols;
					gd->parent = this;

					val = GetAttrTextVal(val);
					
					gd->рисуй(w, x, y, cx, cy,
					          val, style,
					          fg, bg, fnt, it.style & GD::FOUND, it.fs, it.fe);
					
					it.rcx = gd->real_size.cx;
					it.rcy = gd->real_size.cy;

					if(vert_grid)
					{
						bool skip = false;
						if(!draw_last_vert_line && j == total_cols - 1)
							skip = true;

						if(!skip)
							w.DrawRect(x + cx - 1, y, 1, cy, fg_grid);
					}
					if(horz_grid)
					{
						bool skip = false;
						if(!draw_last_horz_line && i == total_rows - 1)
							skip = true;

						if(!skip)
							w.DrawRect(x, y + cy - 1, cx, 1, fg_grid);
					}

					if(iscur && draw_focus)
						РНЦП::DrawFocus(w, x, y, cx, cy);
				}
			}
		}

	end_paint:

		w.стоп();

		lastCol = j - 1;
		lastRow = i - 1;
	}


	if(moving_header && fixed_top_click && curSplitCol >= 0)
	{
		r.уст(fixed_width, 0, sz.cx, fixed_height);
		w.Clip(r);
		bool lastcol = curSplitCol == lastVisCol;

		int cp = curSplitCol;
		if(!lastcol)
			while(++cp < total_cols && hitems[cp].hidden);

		int cx = hitems[cp].nWidth() / 2;
		int x = lastcol ? hitems[curSplitCol].nLeft(sbx) + cx
		                : hitems[curSplitCol].nRight(sbx) - 1;
		DrawFatFrame(w, x, 0, cx, vitems[fixed_rows - 1].nBottom(), moving_allowed ? светлоСиний : красный, 2);
		w.стоп();
	}

	if(moving_header && fixed_left_click)
	{
		int dy = curSplitRow == lastVisRow ? -2 : -1;
		int y = curSplitRow >= 0 ? vitems[curSplitRow].nBottom(sby) + dy : 0;
		if(y >= fixed_height - 1)
			DrawVertDragLine(w, y, hitems[fixed_cols - 1].nRight(), 0, moving_allowed ? светлоСиний : красный);
	}

	if(moving_body)
	{
		int dy = curSplitRow == lastVisRow ? -2 : -1;
		int y = curSplitRow >= 0 ? vitems[curSplitRow].nBottom(sby) + dy : 0;
		if(y >= fixed_height - 1)
			DrawVertDragLine(w, y, sz.cx, fixed_width - 1, светлоСиний);
	}

	if(search_display && !search_string.пустой())
	{
		Размер tsz = дайРазмТекста(search_string, StdFont());
		w.DrawRect(Прям(0, sz.cy - tsz.cy - 4, tsz.cx + 4, sz.cy), SRed);
		w.DrawText(2, sz.cy - tsz.cy - 2, search_string, StdFont(), SYellow);
	}

	if(!can_paint)
		w.DrawRect(Прям(0, total_cols > 1 ? fixed_height : 0, sz.cx, sz.cy), SColorPaper);
	if(++paint_flag > 100)
		paint_flag = 0;

	LG(0, "---- рисуй(%d).", paintcnt);
}

};