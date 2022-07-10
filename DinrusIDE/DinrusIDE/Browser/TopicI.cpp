#include "Browser.h"

bool IsTopicFile(const char *path)
{
	return впроп(дайРасшф(path)) == ".tpp" && файлЕсть(path);
}

bool IsTopicGroup(const char *path)
{
	return впроп(дайРасшф(path)) == ".tpp" && (!полнпуть_ли(path) || !файлЕсть(path));
}

struct TopicModule : public МодульИСР {
	virtual Ткст       GetID() { return "TopicModule"; }

	virtual Рисунок FileIcon(const char *path) {
		return IsTopicFile(path) ? TopicImg::Topic() : IsTopicGroup(path) ? TopicImg::Group() : Null;
	}
	virtual IdeDesigner *CreateDesigner(const char *path, byte cs) {
		if(IsTopicGroup(path)) {
			TopicEditor *d = new TopicEditor;
			d->PersistentFindReplace(TheIde()->IsPersistentFindReplace());
			d->открой(path);
			return d;
		}
		if(IsTopicFile(path)) {
			TopicEditor *d = new TopicEditor;
			d->PersistentFindReplace(TheIde()->IsPersistentFindReplace());
			d->OpenFile(path);
			return d;
		}
		return NULL;
	}
	virtual void сериализуй(Поток& s) {
		TopicEditor::SerializeEditPos(s);
	}
};

void InitializeTopicModule()
{
	регМодульИСР(Single<TopicModule>());
}