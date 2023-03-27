#include <PowerEditor/ScintillaComponent/ScintillaComponent.h>

Upp::Ctrl* ScintillaCtrls::createSintilla(Upp::Ctrl* hParent)
{
	_hParent = hParent;
	ScintillaEditView *scint = new ScintillaEditView;
	scint->init(_hInst, _hParent);
	_scintVector.push_back(scint);
	return scint->getHSelf();
}

int ScintillaCtrls::getIndexFrom(Upp::Ctrl* handle2Find)
{
	for (size_t i = 0, len = _scintVector.size(); i < len ; ++i)
	{
		if (_scintVector[i]->getHSelf() == handle2Find)
		{
			return static_cast<int32_t>(i);
		}
	}
	return -1;
}

ScintillaEditView * ScintillaCtrls::getScintillaEditViewFrom(Upp::Ctrl* handle2Find)
{
	int i = getIndexFrom(handle2Find);
	if (i == -1 || size_t(i) >= _scintVector.size())
		return Null;
	return _scintVector[i];
}

/*
bool ScintillaCtrls::destroyScintilla(Upp::Ctrl* handle2Destroy)
{
	int i = getIndexFrom(handle2Destroy);
	if (i == -1)
		return false;

	_scintVector[i]->destroy();
	delete _scintVector[i];

	std::vector<ScintillaEditView *>::iterator it2delete = _scintVector.begin()+ i;
	_scintVector.erase(it2delete);
	return true;
}
*/

void ScintillaCtrls::destroy()
{
	for (size_t i = 0, len = _scintVector.size(); i < len ; ++i)
	{
		_scintVector[i]->destroy();
		delete _scintVector[i];
	}
}
