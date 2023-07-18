// This file is part of Notepad++ project
// Copyright (C)2021 Don HO <don.h@free.fr>

// This program is free software: you can redistribute it and/or modify
// it under the terms of the GNU General Public License as published by
// the Free Software Foundation, either version 3 of the License, or
// at your option any later version.
//
// This program is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
// GNU General Public License for more details.
//
// You should have received a copy of the GNU General Public License
// along with this program.  If not, see <https://www.gnu.org/licenses/>.

#pragma once

class ScintillaEditView;
class TiXmlDocument;
class TiXmlNode;

struct foundInfo final
{
	String _data;
	String _data2;
	intptr_t _pos = -1;
	intptr_t _pos2 = -1;
};

class FunctionParser
{
friend class FunctionParsersManager;
public:
	FunctionParser(const char *id, const char *displayName, const char *commentExpr, const char* functionExpr, const Upp::Vector<String>& functionNameExprArray, const Upp::Vector<String>& classNameExprArray):
	  _id(id), _displayName(displayName), _commentExpr(commentExpr?commentExpr:TEXT("")), _functionExpr(functionExpr), _functionNameExprArray(functionNameExprArray), _classNameExprArray(classNameExprArray){};

	virtual void parse(std::vector<foundInfo> & foundInfos, size_t begin, size_t end, ScintillaEditView **ppEditView, String classStructName = TEXT("")) = 0;
	void funcParse(std::vector<foundInfo> & foundInfos, size_t begin, size_t end, ScintillaEditView **ppEditView, String classStructName = TEXT(""), const std::vector< std::pair<size_t, size_t> > * commentZones = nullptr);
	bool isInZones(size_t pos2Test, const std::vector< std::pair<size_t, size_t> > & zones);
	virtual ~FunctionParser() = default;

protected:
	String _id;
	String _displayName;
	String _commentExpr;
	String _functionExpr;
	Upp::Vector<String> _functionNameExprArray;
	Upp::Vector<String> _classNameExprArray;
	void getCommentZones(std::vector< std::pair<size_t, size_t> > & commentZone, size_t begin, size_t end, ScintillaEditView **ppEditView);
	void getInvertZones(std::vector< std::pair<size_t, size_t> > & destZones, std::vector< std::pair<size_t, size_t> > & sourceZones, size_t begin, size_t end);
	String parseSubLevel(size_t begin, size_t end, std::vector< String > dataToSearch, intptr_t & foundPos, ScintillaEditView **ppEditView);
};


class FunctionZoneParser : public FunctionParser
{
public:
	FunctionZoneParser() = delete;
	FunctionZoneParser(const char *id, const char *displayName, const char *commentExpr, const char* rangeExpr, const char* openSymbole, const char* closeSymbole,
		const Upp::Vector<String>& classNameExprArray, const char* functionExpr, const Upp::Vector<String>& functionNameExprArray):
		FunctionParser(id, displayName, commentExpr, functionExpr, functionNameExprArray, classNameExprArray), _rangeExpr(rangeExpr), _openSymbole(openSymbole), _closeSymbole(closeSymbole) {};

	void parse(std::vector<foundInfo> & foundInfos, size_t begin, size_t end, ScintillaEditView **ppEditView, String classStructName = TEXT(""));
	
protected:
	void classParse(std::vector<foundInfo> & foundInfos, std::vector< std::pair<size_t, size_t> > & scannedZones, const std::vector< std::pair<size_t, size_t> > & commentZones, size_t begin, size_t end, ScintillaEditView **ppEditView, String classStructName = TEXT(""));

private:
	String _rangeExpr;
	String _openSymbole;
	String _closeSymbole;

	size_t getBodyClosePos(size_t begin, const char *bodyOpenSymbol, const char *bodyCloseSymbol, const std::vector< std::pair<size_t, size_t> > & commentZones, ScintillaEditView **ppEditView);
};



class FunctionUnitParser : public FunctionParser
{
public:
	FunctionUnitParser(const char *id, const char *displayName, const char *commentExpr,
		const char* mainExpr, const Upp::Vector<String>& functionNameExprArray,
		const Upp::Vector<String>& classNameExprArray): FunctionParser(id, displayName, commentExpr, mainExpr, functionNameExprArray, classNameExprArray)
	{}

	void parse(std::vector<foundInfo> & foundInfos, size_t begin, size_t end, ScintillaEditView **ppEditView, String classStructName = TEXT(""));
};

class FunctionMixParser : public FunctionZoneParser
{
public:
	FunctionMixParser(const char *id, const char *displayName, const char *commentExpr, const char* rangeExpr, const char* openSymbole, const char* closeSymbole,
		const Upp::Vector<String>& classNameExprArray, const char* functionExpr, const Upp::Vector<String>& functionNameExprArray, FunctionUnitParser *funcUnitPaser):
		FunctionZoneParser(id, displayName, commentExpr, rangeExpr,	openSymbole, closeSymbole, classNameExprArray, functionExpr, functionNameExprArray), _funcUnitPaser(funcUnitPaser){};
		
	~FunctionMixParser()
	{
		delete _funcUnitPaser;
	}

	void parse(std::vector<foundInfo> & foundInfos, size_t begin, size_t end, ScintillaEditView **ppEditView, String classStructName = TEXT(""));

private:
	FunctionUnitParser* _funcUnitPaser = nullptr;
};


struct AssociationInfo final
{
	int _id;
	int _langID;
	String _ext;
	String _userDefinedLangName;

	AssociationInfo(int id, int langID, const char *ext, const char *userDefinedLangName)
		: _id(id), _langID(langID)
	{
		if (ext)
			_ext = ext;
		else
			_ext.Clear();

		if (userDefinedLangName)
			_userDefinedLangName = userDefinedLangName;
		else
			_userDefinedLangName.Clear();
	};
};

const int nbMaxUserDefined = 25;

struct ParserInfo
{
	String _id; // xml parser rule file name - if empty, then we use default name. Mandatory if _userDefinedLangName is not empty
	FunctionParser* _parser = nullptr;
	String _userDefinedLangName;

	ParserInfo() {};
	ParserInfo(const char* id): _id(id) {};
	ParserInfo(const char* id, const char* userDefinedLangName): _id(id), _userDefinedLangName(userDefinedLangName) {};
	~ParserInfo() { if (_parser) delete _parser; }
};

class FunctionParsersManager final
{
public:
	~FunctionParsersManager();

	bool init(const char* xmlPath, const char* xmlInstalledPath, ScintillaEditView ** ppEditView);
	bool parse(std::vector<foundInfo> & foundInfos, const AssociationInfo & assoInfo);
	

private:
	ScintillaEditView **_ppEditView = nullptr;
	String _xmlDirPath; // The 1st place to load function list files. Usually it's "%APPDATA%\Notepad++\functionList\"
	String _xmlDirInstalledPath; // Where Notepad++ is installed. The 2nd place to load function list files. Usually it's "%PROGRAMFILES%\Notepad++\functionList\" 

	ParserInfo* _parsers[L_EXTERNAL + nbMaxUserDefined] = {nullptr};
	int _currentUDIndex = L_EXTERNAL;

	bool getOverrideMapFromXmlTree(String& xmlDirPath);
	bool loadFuncListFromXmlTree(String& xmlDirPath, LangType lType, const char* overrideId, int udlIndex = -1);
	bool getZonePaserParameters(TiXmlNode *classRangeParser, String&mainExprStr, String&openSymboleStr, String&closeSymboleStr, Upp::Vector<String> &classNameExprArray, String&functionExprStr, Upp::Vector<String> &functionNameExprArray);
	bool getUnitPaserParameters(TiXmlNode *functionParser, String&mainExprStr, Upp::Vector<String> &functionNameExprArray, Upp::Vector<String> &classNameExprArray);
	FunctionParser * getParser(const AssociationInfo & assoInfo);
};
