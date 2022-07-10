class LogSyntax : public EditorSyntax {
public:
	virtual void            Highlight(const wchar *s, const wchar *end, HighlightOutput& hls,
	                                  РедакторКода *editor, int line, int64 pos);
};
