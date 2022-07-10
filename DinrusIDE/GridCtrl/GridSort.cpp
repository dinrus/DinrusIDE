#include "GridCtrl.h"

namespace РНЦП {

/*
bool GridCtrl::operator<(const Вектор<Значение>& a, const Вектор<Значение>& b)
{
	for(int i = 0; i < SortOrder.дайСчёт(); i++)
	{
		if(sortMode)
			return (сравниСтдЗнач((*a.items)[a.id][sortCol].val, (*b.items)[b.id][sortCol].val, 0) < 0);
		else
			return a.id < b.id;

	}
}
*/

void GridCtrl::GSort(int scol)
{
	int col;

	int cnt = sortOrder.дайСчёт();

	if(cnt == 0)
		return;

	if(scol < 0)
		scol = cnt - 1;

	col = sortOrder[scol];

	//int order = 1;
	int order = hitems[col].sortmode;

	/* dla trybu multisort nie mozna w ten sposob
	   sortowac dla pierwszej kolumny */

	if(cnt > 1 && scol != 0)
	{
		int match = 1;
		int is = fixed_rows;

		for(int i = fixed_rows + 1; i < total_rows; i++)
		{
			int n = vitems[i].id;
			int m = vitems[is].id;

			bool found = true;
			for(int j = 0; j < scol; j++)
			{
				int k = sortOrder[j];
				if(items[n][k].val != items[m][k].val)
				{
					found = false;
					break;
				}
			}

			if(found)
			{
				match++;
				continue;
			}
			else
			{
				if(match > 1)
					GSort(col, order, is, match);

				match = 1;
				is = i;
			}
		}
		if(match > 1)
			GSort(col, order, is, match);
	}
	else
		GSort(col, order, fixed_rows);

}

void GridCtrl::GSort(int col, int order, int from, int count)
{
	if(count == 0)
		return;

	ItemRect::sortCol = col;
	ItemRect::sortMode = (order != 0);

	VItems::Обходчик its, ite;

	its = vitems.старт() + from;

	if(count < 0)
		ite = vitems.стоп();
	else
		ite = its + count;

	if(order < 2)
		РНЦП::сортируй(СубДиапазон(its, ite).пиши(), StdLess<ItemRect>());
	else
		РНЦП::сортируй(СубДиапазон(its, ite).пиши(), StdGreater<ItemRect>());
}


void GridCtrl::Multisort()
{
	GSort(-1, 0, fixed_rows);
	for(int i = 0; i < sortOrder.дайСчёт(); i++)
		GSort(i);
}

int GridCtrl::InMultisort(int col)
{
	for(int i = 0; i < sortOrder.дайСчёт(); i++)
		if(col == sortOrder[i])
			return i;

	return -1;
}

void GridCtrl::ClearMultisort(int n)
{
	for(int i = n; i < sortOrder.дайСчёт(); i++)
	{
		int c = GetIdCol(sortOrder[i], true);
		hitems[c].sortmode = 0;
		hitems[c].sortcol = 0;
	}
	sortOrder.очисть();
}

bool GridCtrl::IsSorted()
{
	return sortOrder.дайСчёт() > 0;
}

void GridCtrl::MarkSort(int col, int sort_mode, bool refresh)
{
	int mcol = InMultisort(col);

	sortCol = col;
	hitems[col].sortmode = sort_mode;
	
	if(mcol < 0)
	{
		sortOrder.добавь(col);
		mcol = sortOrder.дайСчёт() - 1;
	}

	hitems[col].sortcol = mcol;
	
	if(refresh)
		RefreshTop();
}

void GridCtrl::MarkSort(int col, int sort_mode)
{
	MarkSort(fixed_cols + col, sort_mode, true);
}

void GridCtrl::MarkSort(Ид id, int sort_mode)
{
	MarkSort(aliases.дай(id), sort_mode, true);
}

GridCtrl& GridCtrl::сортируй(int sort_col, int sort_mode, bool multisort, bool repaint)
{
	int col = GetIdCol(sort_col + fixed_cols);
	if(col < 0)
		return *this;

	if(!multisort)
		ClearMultisort();
	
	MarkSort(col, sort_mode, false);
	GSort();

	UpdateCursor();

	if(repaint)
		Repaint(false, true);
	return *this;
}

GridCtrl& GridCtrl::сортируй(Ид id, int sort_mode, bool multisort, bool repaint)
{
	return сортируй(aliases.дай(id) - fixed_cols, sort_mode, multisort, repaint);
}

GridCtrl& GridCtrl::MultiSort(int sort_col, int sort_mode)
{
	return сортируй(sort_col, sort_mode, true);
}

GridCtrl& GridCtrl::MultiSort(Ид id, int sort_mode)
{
	return сортируй(aliases.дай(id) - fixed_cols, sort_mode, true);
}

void GridCtrl::ClearSort()
{
	сортируй(0, SORT_ID);
}

void GridCtrl::ReSort()
{
	Multisort();
	Repaint(false, true);
}

Вектор<Ид> GridCtrl::GetSortOrderId() const
{
	Вектор<Ид> v;
	for(int i = 0; i < sortOrder.дайСчёт(); i++)
		v.добавь(aliases.дайКлюч(sortOrder[i]));
	return v;
}

Вектор<GridCtrl::SortOrder> GridCtrl::GetSortOrder() const
{
	Вектор<SortOrder> v;

	for(int i = 0; i < sortOrder.дайСчёт(); i++)
	{
		int c = sortOrder[i];
		SortOrder& s = v.добавь();
		s.id = c;
		s.имя = aliases.дайКлюч(c);
		s.ascending = hitems[c].IsSortAsc();
		s.descending = hitems[c].IsSortDsc();
	}

	return v;
}

}
