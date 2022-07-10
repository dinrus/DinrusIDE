class PythonSyntax : public EditorSyntax {
public:
	virtual void Highlight(const wchar *start, const wchar *end, HighlightOutput& hls,
	                       РедакторКода *editor, int line, int64 pos);
	virtual void IndentInsert(РедакторКода& e, int chr, int count);

private:
	struct Identation {
		enum Type {
			Вкладка = 0,
			Space,
			Unknown
		};
	};

	bool             LineHasColon(const ШТкст& line);
	int              CalculateLineIndetations(const ШТкст& line, Identation::Type тип);
	int              CalculateSpaceIndetationSize(РедакторКода& editor);
	Identation::Type FindIdentationType(РедакторКода& editor, const ШТкст& line);
	char             GetIdentationByType(Identation::Type тип);
};
