#ifndef _ide_Methods_h_
#define _ide_Methods_h_

#include "DinrusIDE.h"

class DirTable : public КтрлМассив {
public:
	virtual void   устДанные(const Значение& data);
	virtual Значение  дайДанные() const;

protected:
	void Modify()  { Update(); }

	EditString      edit;
	SelectDirButton edit_dir;

	void иниц(const char *имя = NULL);

public:
	DirTable();
	DirTable(const char *имя);
};

class DirMap : public КтрлМассив {
public:
	virtual void   устДанные(const Значение& data);
	virtual Значение  дайДанные() const;

protected:
	void Modify()  { Update(); }

	EditString      localpath, remotepath;
	SelectDirButton edit_dir;

public:
	DirMap();
};

class BuilderSetup;

class BuilderSetupInterface : public КтрлРодитель {
public:
	virtual ~BuilderSetupInterface() {}

	virtual void нов(const Ткст& builder) = 0;
	virtual void OnLoad() = 0;
	virtual void OnCtrlLoad(const Ткст& ctrlKey, const Ткст& значение) = 0;
	virtual void OnShow() = 0;
	virtual void InitSetupCtrlsMap(ВекторМап<Ид, Ктрл*>& map) = 0;
	
	virtual void InitBuilderSetup(BuilderSetup& bs);
};

class AndroidBuilderSetup : public WithBuildMethodsAndroidBuilderSetupLayout<BuilderSetupInterface> {
public:
	AndroidBuilderSetup();
	
public: /* BuilderSetupInterface */
	void нов(const Ткст& builder) override;
	void OnLoad() override;
	void OnCtrlLoad(const Ткст& ctrlKey, const Ткст& значение) override;
	void OnShow() override;
	
	void InitSetupCtrlsMap(ВекторМап<Ид, Ктрл*>& map) override;

private:
	void OnSdkShow();
	void OnSdkPathInsert();
	void OnSdkPathChange();
	void OnSdkPathChange0(const Ткст& sdkPath);
	void OnNdkShow();
	void OnNdkPathInsert();
	void OnNdkPathChange();
	void OnNdkPathChange0(const Ткст& ndkPath);

private:
	void LoadPlatforms(const AndroidSDK& sdk);
	void LoadBuildTools(const AndroidSDK& sdk);
	void LoadToolchains(const AndroidNDK& ndk);
	void LoadCppRuntimes(const AndroidNDK& ndk);
	void LoadDropList(СписокБроса& dropList, const Вектор<Ткст>& values, const Ткст& defaultKey = "");
	
private:
	void EnableSdkCtrls(bool enable = true);
	void DisableSdkCtrls();
	void ClearSdkCtrls();
	
	void EnableNdkCtrls(bool enable = true);
	void DisableNdkCtrls();
	void ClearNdkCtrls();
};

class BuilderSetup {
public:
	BuilderSetupInterface* setupCtrl;
	ВекторМап<Ид, Ктрл*> setupCtrlsMap;
};

class DefaultBuilderSetup : public WithBuildMethodsDefaultBuilderSetupLayout<BuilderSetupInterface> {
public:
	DirTable   path;
	DirTable   include;
	DirTable   lib;
	
public: /* BuilderSetupInterface */
	DefaultBuilderSetup();
	
	void нов(const Ткст& builder) override;
	void OnLoad() override {}
	void OnCtrlLoad(const Ткст& ctrlKey, const Ткст& значение) override {}
	void OnShow() override {}
	
	void InitSetupCtrlsMap(ВекторМап<Ид, Ктрл*>& map) override;
};

class BuildMethods : public WithBuildMethodsLayout<ТопОкно> {
public:
	OpenFileButton open_script;
	TextOption linkmode_lock;
	
	EditStringNotNull имя;
	Индекс<Ткст>     origfile;
	Ткст            default_method;
	
	МассивМап<Ткст, BuilderSetup> setups;
	
	AndroidBuilderSetup androidSetup;
	DefaultBuilderSetup defaultSetup;
	
	void грузи();
	bool сохрани();

	void NewBuilder();
	void ShowDefault();
	void устДефолт();
	void ChangeMethod();
	void Import();

	void MethodMenu(Бар& bar);
	
	Ткст GetSetupPrefix(const Ткст& setupKey) const;
	Ткст GetSetupPrefix(const Индекс<Ткст>& buildersGroup) const;
	void   InitSetups();
	void   SwitchSetupView();
	ВекторМап<Ткст, Ткст> SieveBuilderVars(const ВекторМап<Ткст, Ткст>& map);
	ВекторМап<Ткст, Ткст> MapBuilderVars(const ВекторМап<Ткст, Ткст>& map);
	
	typedef BuildMethods ИМЯ_КЛАССА;

	BuildMethods();
	
private:
	void          SieveBuilders(Индекс<Ткст>& sievedBuilders, const Индекс<Ткст>& builders) const;
	Ткст        BuildersToString(const Индекс<Ткст>& builders) const;
	Индекс<Ткст> StringToBuilders(const Ткст& str) const;
	Индекс<Ткст> GetBuilders() const;
};

#endif
