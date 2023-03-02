#ifndef _DinrusLLVM_DinrusLLVM_h_
#define _DinrusLLVM_DinrusLLVM_h_

#include <DinrusC/Core.h>
#include "Import.h"

typedef РНЦП::Ткст ткст;

//===---------------------------------------------------------------------------
/// Класс ИнфОПроходке(PassInfo) - его экземпляр существует для каждой проходки, известной системе,
/// его можно получить из реальной Проходки(Pass), вызвав её метод getPassInfo().
/// Эти объекты устанавливаются по шпблону RegisterPass<>.
///
/*&
class ИнфОПроходке {
public:
  using NormalCtor_t = Pass* (*)();

private:
  StringRef PassName;     // Nice name for Pass
  StringRef PassArgument; // Command Line argument to run this pass
  const void *PassID;
  const bool IsCFGOnlyPass = false;      // Pass only looks at the CFG.
  const bool IsAnalysis;                 // True if an analysis pass.
  const bool IsAnalysisGroup;            // True if an analysis group.
  std::vector<const PassInfo *> ItfImpl; // Interfaces implemented by this pass
  NormalCtor_t NormalCtor = nullptr;

public:
  /// PassInfo ctor - Do not call this directly, this should only be invoked
  /// through RegisterPass.
  PassInfo(StringRef name, StringRef arg, const void *pi, NormalCtor_t normal,
           bool isCFGOnly, bool is_analysis)
      : PassName(name), PassArgument(arg), PassID(pi), IsCFGOnlyPass(isCFGOnly),
        IsAnalysis(is_analysis), IsAnalysisGroup(false), NormalCtor(normal) {}

  /// PassInfo ctor - Do not call this directly, this should only be invoked
  /// through RegisterPass. This version is for use by analysis groups; it
  /// does not auto-register the pass.
  PassInfo(StringRef name, const void *pi)
      : PassName(name), PassID(pi), IsAnalysis(false), IsAnalysisGroup(true) {}

  PassInfo(const PassInfo &) = delete;
  PassInfo &operator=(const PassInfo &) = delete;

  /// getPassName - Return the friendly name for the pass, never returns null
  StringRef getPassName() const { return PassName; }

  /// getPassArgument - Return the command line option that may be passed to
  /// 'opt' that will cause this pass to be run.  This will return null if there
  /// is no argument.
  StringRef getPassArgument() const { return PassArgument; }

  /// getTypeInfo - Return the id object for the pass...
  /// TODO : Rename
  const void *getTypeInfo() const { return PassID; }

  /// Return true if this PassID implements the specified ID pointer.
  bool isPassID(const void *IDPtr) const { return PassID == IDPtr; }

  /// isAnalysisGroup - Return true if this is an analysis group, not a normal
  /// pass.
  bool isAnalysisGroup() const { return IsAnalysisGroup; }
  bool isAnalysis() const { return IsAnalysis; }

  /// isCFGOnlyPass - return true if this pass only looks at the CFG for the
  /// function.
  bool isCFGOnlyPass() const { return IsCFGOnlyPass; }

  /// getNormalCtor - Return a pointer to a function, that when called, creates
  /// an instance of the pass and returns it.  This pointer may be null if there
  /// is no default constructor for the pass.
  NormalCtor_t getNormalCtor() const {
    return NormalCtor;
  }
  void setNormalCtor(NormalCtor_t Ctor) {
    NormalCtor = Ctor;
  }

  /// createPass() - Use this method to create an instance of this pass.
  Pass *createPass() const {
    assert((!isAnalysisGroup() || NormalCtor) &&
           "No default implementation found for analysis group!");
    assert(NormalCtor &&
           "Cannot call createPass on PassInfo without default ctor!");
    return NormalCtor();
  }

  /// addInterfaceImplemented - This method is called when this pass is
  /// registered as a member of an analysis group with the RegisterAnalysisGroup
  /// template.
  void addInterfaceImplemented(const PassInfo *ItfPI) {
    ItfImpl.push_back(ItfPI);
  }

  /// getInterfacesImplemented - Return a list of all of the analysis group
  /// interfaces implemented by this pass.
  const std::vector<const PassInfo*> &getInterfacesImplemented() const {
    return ItfImpl;
  }
};
*/
///////////////////////////////////
ПИ_РНЦП
///////////////////////////////////
struct Контекст
{
	public:
    Контекст();
};
/////////////////////////
struct Тип{
    public:
	Тип();
};
////////////////////////
struct Модуль
    {
        private:
         ЛЛМодуль м_сам;

        public:
        Модуль(ткст идМодуля);
        Модуль(ткст идМодуля, Контекст контекст);

        static проц линкуйМодули(Модуль куда, Модуль исток);
		ЛЛМодуль дай();
        Модуль(ЛЛМодуль m);
        ~Модуль();
        ткст раскладкаДанных();
        проц раскладкаДанных( ткст ткт);
        Контекст контекст();
        Модуль клонируйМодуль();
        проц дамп();
        бул выведиВФайл(ткст имяф, ткст0 ошСооб);
        ткст выведиВТкст();
        проц устИнлайнАсм(ткст asm_);
        //ГлобПеременная дайИменГлоб(ткст имя);
        Тип дайТипПоИмени(ткст имя);
        бцел дайЧлоОперандовИМД(ткст имя);
        //Значение[] дайОперандыИМД(ткст имя);
        проц добавьОперандИМД(ткст имя, Значение val);
        //Функция добавьФункцию(ткст имя, Тип типФн);
        //Функция дайИменФункцию(ткст имя);
        //Функция первФункц();
        //Функция последнФункц();
        //ГлобЗначение добавьГлоб(Тип тип, ткст имя);
        //ГлобЗначение добавьГлобВАдрПростр(Тип тип, ткст имя, бцел адреснПрострво);
        //ГлобЗначение дайИменованЗнач(ткст имя);
        //ГлобЗначение первГлоб();
        //ГлобЗначение последнГлоб();
        //ГлобЗначение добавьНик(Тип тип, Значение aliasee, ткст имя);
        бцел дайИдТипаМД(ткст имя);
        //МодульПровайдер создайМПДляМодуля();
        //МенеджерПроходок создайМПФДляМодуля();
        проц Verify();
        бул TryVerify(ткст message);
        цел пишиВФайл(ткст путь);
        цел пишиВФД(цел fd, бул закрыть_ли, бул небуферировать_ли);
        цел пишиВФук(цел handle);
        //БуфПам пишиВБуфПам();
        бул operator =(Модуль other);
       // override bool Equals(object obj);
        бул operator ==(Модуль op1, Модуль op2);
        бул operator !=(Модуль op1, Модуль op2);
      //  override цел GetHashCode();
        ткст цель();

    };
////////////////////////////
struct Значение;
//////////////////////////////////
struct РеестрПроходок
{
private:
   ЛЛРеестрПроходок рп_сам;
   ук датчик_сам;

public:
	РеестрПроходок();
	РеестрПроходок(ЛЛРеестрПроходок рп);
	~РеестрПроходок();
	ЛЛРеестрПроходок дай();
	проц перечисли_c( ук перечислитель);
	проц добавьДатчикРегистаций(ук датчик);
	проц удалиДатчикРегистаций();
	проц иницЯдро();
    проц иницТрансформУтилз();
    проц иницСкалярОпц();
    проц иницОбджСиАРЦ();
    проц иницВекторизацию();
    проц иницИнстКомб();
    проц иницАгрИнстКомб();
    проц иницМПО();
    проц иницИнстр();
    проц иницАнализ();
    проц иницМПА();
    проц иницКодген();
    проц иницЦель();

};
//////////////////////////////////////////////////////
class ГлобРеестрПроходок : public РеестрПроходок
{
	   ГлобРеестрПроходок(): РеестрПроходок(ЛЛДайГлобРеестрПроходок()){}
};
//////////////////////////////////////////////////////
struct Анализ
{
public:
	/*
typedef enum{
  ПрервиПроцесс, // верификатор выведет в стдош и abort()
  ВыведиСооб, // верификатор выведет в стдош и вернёт 1
  ВерниСтатус  // верификатор просто вернёт 1
} Действие;
*/
	 Анализ(РНЦП::РеестрПроходок *рп);
     бул проверьФункцию(РНЦП::Значение зн, ПДействиеПриОш д);
     бул  проверьМодуль(РНЦП::Модуль мод, ПДействиеПриОш д, ткст0 * сооб);
     проц покажиКонфигФн(РНЦП::Значение зн);
     проц покажиТолькоКонфигФн(РНЦП::Значение зн);
};


} //конец неймспейса РНЦП
#endif
