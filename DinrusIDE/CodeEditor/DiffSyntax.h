class DiffSyntax : public EditorSyntax {
public:
	DiffSyntax();

	virtual void Highlight(const wchar *start, const wchar *end, HighlightOutput& hls,
	                       РедакторКода *editor, int line, int64 pos);
	
protected:
	void помести(int ink, int n = 1, int paper = PAPER_NORMAL);
	void Do(const wchar *s, const wchar *end, РедакторКода *editor, int line, int tabsize, int64 pos);
	
	int  FindTheNumberOfCharsToLineEnd(const wchar *current, const wchar *end) const;
	bool IsPattern(const wchar *current, const wchar *end, Ткст pattern) const;
	
private:
	HighlightOutput *hout;
};
