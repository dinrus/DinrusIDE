class LinearSolver
{
public:
	LinearSolver(int count, double tolerance = 1e-10);

	double        *Row(int r)                        { return &matrix[r * col1]; }
	const double  *Row(int r) const                  { return &matrix[r * col1]; }
	double&        лево(int r, int c)                { return matrix[r * col1 + c]; }
	double         лево(int r, int c) const          { return matrix[r * col1 + c]; }
	double&        право(int r)                      { return matrix[(r + 1) * col1 - 1]; }
	double         право(int r) const                { return matrix[(r + 1) * col1 - 1]; }
	double&        operator () (int r, int c)        { return лево(r, c); }
	double         operator () (int r, int c) const  { return лево(r, c); }
	double&        operator () (int r)               { return право(r); }
	double         operator () (int r) const         { return право(r); }

	void           AddLSI(const double *bases, double значение);

	Вектор<double> Solve();

	static void    SelfTest();

private:
	int            rows;
	int            col1;
	Вектор<double> matrix;
	Вектор<Точка>  pivots;
	double         tolerance;
	Вектор<int>    left_rows, left_cols;
};
