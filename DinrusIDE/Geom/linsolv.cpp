#include "Geom.h"

namespace РНЦП {

LinearSolver::LinearSolver(int count, double tolerance)
: rows(count)
, col1(count + 1)
, tolerance(tolerance)
{
	ПРОВЕРЬ(tolerance >= 0);
	matrix.устСчёт(count * (count + 1), 0);
	left_rows.устСчёт(rows);
	left_cols.устСчёт(rows);
	for(int i = 0; i < rows; i++)
		left_rows[i] = left_cols[i] = i;
}

void LinearSolver::AddLSI(const double *bases, double значение)
{
	double *row = matrix.старт();
	const double *bi = bases;
	for(int i = 0; i < rows; i++) {
		const double *bj = bases;
		for(int j = 0; j < rows; j++)
			*row++ += *bi * *bj++;
		*row++ += *bi++ * значение;
	}
}

Вектор<double> LinearSolver::Solve()
{
	while(!left_rows.пустой())
	{
		int er = -1, ec = -1;
		double best = tolerance;
		for(int pr = 0; pr < left_rows.дайСчёт(); pr++)
		{
			const double *p = Row(left_rows[pr]);
			for(int pc = 0; pc < left_cols.дайСчёт(); pc++)
			{
				double v = fabs(p[left_cols[pc]]);
				if(v > best)
				{
					best = v;
					ec = pc;
					er = pr;
				}
			}
		}

		if(er < 0 || best <= 0) // just to be sure
		{ // no more fixed variables
			for(int i = 0; i < left_rows.дайСчёт(); i++)
				if(fabs(право(left_rows[i])) > tolerance)
					return Вектор<double>(); // Ошибка
			break;
		}

		int cr = left_rows[er], cc = left_cols[ec];

		int p = left_rows.вынь();
		if(er < left_rows.дайСчёт()) left_rows[er] = p;
		p = left_cols.вынь();
		if(ec < left_cols.дайСчёт()) left_cols[ec] = p;

		const double *src = Row(cr);
		const int *xb = left_cols.старт(), *xe = left_cols.стоп();
		for(int i = 0; i < left_rows.дайСчёт(); i++)
		{
			double *dest = Row(left_rows[i]);
			double r = -dest[cc] / src[cc];
			for(const int *xp = xb; xp < xe; xp++)
				dest[*xp] += src[*xp] * r;
			dest[rows] += src[rows] * r;
		}

		pivots.добавь(Точка(cc, cr));
	}

	Вектор<double> result;
	result.устСчёт(rows, Null);
	for(int r = pivots.дайСчёт(); --r >= 0;)
	{
		Точка pivot = pivots[r];
		const double *row = Row(pivot.y);
//		int px = pivot.x;
		double out = row[rows]; // right side
		for(int s = r; ++s < pivots.дайСчёт();)
		{
			int c = pivots[s].x;
			out -= row[c] * result[c];
		}
		result[pivot.x] = out / row[pivot.x];
	}

	return result;
}

void LinearSolver::SelfTest()
{
	for(int i = 0; i < 1000; i++)
	{
		int ord = rand() % 10 + 1;
		Вектор<double> res;
		while(res.дайСчёт() < ord)
			res.добавь(rand());
		LinearSolver ls(ord);
		for(int r = 0; r < ord; r++)
		{
			double rs = 0;
			for(int c = 0; c < ord; c++)
				rs += res[c] * (ls(r, c) = rand());
			ls(r) = rs;
		}
		Вектор<double> out = ls.Solve();
		if(!out.пустой())
		{
			ПРОВЕРЬ(out.дайСчёт() == ord);
			for(int c = 0; c < out.дайСчёт(); c++)
				if(!пусто_ли(out[c]))
				{
					double d = out[c] - res[c];
					ПРОВЕРЬ(fabs(d) <= 1e-3);
				}
		}
	}
}

}
