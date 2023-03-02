:::This file was developed for Dinrus Programming Language by Vitaliy Kulich
:::Copyright is by Dinrus Group.
chcp 65001
:back
:::Setting environment variables
@set this=e:\dinrus\dinrus64\dev\dinrus\base
@set IMPDIR=e:\dinrus\dinrus64\imp\DEV
@set LIBS=e:\dinrus\dinrus64\lib\d2\x64\mingw
@set LDIR=e:\dinrus\dinrus64\lib\d2\x64
@set DMD=e:\dinrus\dinrus64\bin\ldmd2.exe
@set CC=clang.exe
@set LIB=e:\dinrus\dinrus64\bin\dmlib.exe
@set IMPLIB=e:\dinrus\dinrus64\bin\implib.exe
@set LS=e:\dinrus\dinrus32\bin\ls2.exe
@set PACK=e:\dinrus\dinrus32\bin\upx.exe

::goto Lib

:::Deleting previous objects
@del %LDIR%\DinrusX64.lib
@del %LDIR%\DinrusX64.bak
@del %this%\*.rsp
@del %this%\*.obj
@del %this%\*.map
@del %this%\*.dll
@del %this%\*.lib
@del %this%\*.exe

:::Files with staff that must be same in imports and base-making
::: just copied to imports immediately? without manual processing

:copy %this%\src\sys\WinConsts.d %this%\export\sys\WinConsts.d 
:copy %this%\src\sys\WinStructs.d %this%\export\sys\WinStructs.d 
:copy %this%\src\sys\WinTypes.d %this%\export\sys\WinTypes.d 
:copy %this%\src\sys\WinIfaces.d %this%\export\sys\WinIfaces.d 
:copy %this%\src\base.d %this%\export\base.d

:::Making dirs for di files in \imp\dinrus\
::: and copying imports from .\export folder to them

if not exist %IMPDIR% mkdir %IMPDIR%
copy %this%\export\*.d  %IMPDIR%\*.di 

rm -IMPDIR %IMPDIR%\std
mkdir %IMPDIR%\std
copy %this%\export\std\*.d  %IMPDIR%\std\*.di 

rm -IMPDIR %IMPDIR%\tpl
mkdir %IMPDIR%\tpl
copy %this%\export\tpl\*.d  %IMPDIR%\tpl\*.di

rm -IMPDIR %IMPDIR%\st
mkdir %IMPDIR%\st
copy %this%\export\st\*.d  %IMPDIR%\st\*.di

rm -IMPDIR %IMPDIR%\mesh
mkdir %IMPDIR%\mesh
copy %this%\export\mesh\*.d  %IMPDIR%\mesh\*.di

rm -IMPDIR %IMPDIR%\win32
mkdir %IMPDIR%\win32
mkdir %IMPDIR%\win32\directx
copy %this%\export\win32\*.di  %IMPDIR%\win32\*.di
copy %this%\export\win32\directx\*.di  %IMPDIR%\win32\directx\*.di

::mkdir %IMPDIR%\def
::copy %this%\..\win32\directx\*.def  %IMPDIR%\def\*.def

rm -IMPDIR %IMPDIR%\sys
mkdir %IMPDIR%\sys
mkdir %IMPDIR%\sys\com
copy %this%\export\sys\*.d  %IMPDIR%\sys\*.di
::copy %this%\export\sys\inc\*.d  %IMPDIR%\sys\inc\*.di
copy %this%\export\sys\com\*.d  %IMPDIR%\sys\com\*.di

rm -IMPDIR %IMPDIR%\lib
mkdir %IMPDIR%\lib
copy %this%\export\lib\*.d  %IMPDIR%\lib\*.di

rm -IMPDIR %IMPDIR%\time
mkdir %IMPDIR%\time
mkdir %IMPDIR%\time\chrono
copy %this%\export\time\*.d  %IMPDIR%\time\*.di
copy %this%\export\time\chrono\*.d  %IMPDIR%\time\chrono\*.di

rm -IMPDIR %IMPDIR%\col
mkdir %IMPDIR%\col
mkdir %IMPDIR%\col\model
mkdir %IMPDIR%\col\impl
mkdir %IMPDIR%\col\iterator
copy %this%\export\col\*.d  %IMPDIR%\col\*.di
copy %this%\export\col\model\*.d  %IMPDIR%\col\model\*.di
copy %this%\export\col\impl\*.d %IMPDIR%\col\impl\*.d
copy %this%\export\col\iterator\*.d %IMPDIR%\col\iterator\*.d

rm -IMPDIR %IMPDIR%\geom
mkdir %IMPDIR%\geom
copy %this%\export\geom\*.d  %IMPDIR%\geom\*.di

rm -IMPDIR %IMPDIR%\math
mkdir %IMPDIR%\math
copy %this%\export\math\*.d  %IMPDIR%\math\*.di
mkdir %IMPDIR%\math\internal
copy %this%\export\math\internal\*.d  %IMPDIR%\math\internal\*.di
mkdir %IMPDIR%\math\random
copy %this%\export\math\random\*.d  %IMPDIR%\math\random\*.di
mkdir %IMPDIR%\math\random\engines
copy %this%\export\math\random\engines\*.d  %IMPDIR%\math\random\engines\*.di
mkdir %IMPDIR%\math\linalg
copy %this%\export\math\linalg\*.d  %IMPDIR%\math\linalg\*.di

rm -IMPDIR %IMPDIR%\util
mkdir %IMPDIR%\util
copy %this%\export\util\*.d  %IMPDIR%\util\*.di

rm -IMPDIR %IMPDIR%\crypto
mkdir %IMPDIR%\crypto
mkdir %IMPDIR%\crypto\cipher
mkdir %IMPDIR%\crypto\digest
copy %this%\export\crypto\*.d %IMPDIR%\crypto\*.di
copy %this%\export\crypto\cipher\*.d %IMPDIR%\crypto\cipher\*.di
copy %this%\export\crypto\digest\*.d %IMPDIR%\crypto\digest\*.di

rm -IMPDIR %IMPDIR%\text
mkdir %IMPDIR%\text
mkdir %IMPDIR%\text\convert
mkdir %IMPDIR%\text\json
mkdir %IMPDIR%\text\locale
mkdir %IMPDIR%\text\xml
copy %this%\export\text\*.d %IMPDIR%\text\*.di
copy %this%\export\text\json\*.d %IMPDIR%\text\json\*.di
copy %this%\export\text\digest\*.d %IMPDIR%\text\digest\*.di
copy %this%\export\text\locale\*.d %IMPDIR%\text\locale\*.di
copy %this%\export\text\xml\*.d %IMPDIR%\text\xml\*.di
copy %this%\export\text\convert\*.d %IMPDIR%\text\convert\*.di

rm -IMPDIR  %IMPDIR%\io
mkdir %IMPDIR%\io
mkdir %IMPDIR%\io\device
mkdir %IMPDIR%\io\stream
mkdir %IMPDIR%\io\protocol
mkdir %IMPDIR%\io\log
mkdir %IMPDIR%\io\log\model
::mkdir %IMPDIR%\io\selector
::mkdir %IMPDIR%\io\vfs
copy %this%\export\io\*.d  %IMPDIR%\io\*.di
copy %this%\export\io\device\*.d  %IMPDIR%\io\device\*.di
copy %this%\export\io\stream\*.d  %IMPDIR%\io\stream\*.di
copy %this%\export\io\protocol\*.d  %IMPDIR%\io\protocol\*.di
copy %this%\export\io\log\*.d  %IMPDIR%\io\log\*.di
copy %this%\export\io\log\model\*.d  %IMPDIR%\io\log\model\*.di
::copy %this%\export\io\selector\*.d  %IMPDIR%\io\selector\*.di
::copy %this%\export\io\vfs\*.d  %IMPDIR%\io\vfs\*.di

rm -IMPDIR  %IMPDIR%\net
mkdir %IMPDIR%\net
mkdir %IMPDIR%\net\device
mkdir %IMPDIR%\net\ftp
mkdir %IMPDIR%\net\http
mkdir %IMPDIR%\net\model
mkdir %IMPDIR%\net\http\model

copy %this%\export\net\*.d  %IMPDIR%\net\*.di
copy %this%\export\net\device\*.d  %IMPDIR%\net\device\*.di
copy %this%\export\net\ftp\*.d  %IMPDIR%\net\ftp\*.di
copy %this%\export\net\http\*.d  %IMPDIR%\net\http\*.di
copy %this%\export\net\model\*.d  %IMPDIR%\net\model\*.di
copy %this%\export\net\http\model\*.d  %IMPDIR%\net\http\model\*.di

:Base
:::Creating respond file
%LS% -d %this%\src\std\*.d %this%\src\*.d %this%\src\lib\*.d %this%\src\tpl\*.d %this%\src\sys\*.d %this%\src\sys\com\*.d %this%\src\math\*.d %this%\src\math\random\*.d %this%\src\time\*.d %this%\src\time\chrono\*.d %this%\src\crypto\*.d %this%\src\crypto\digest\*.d %this%\src\crypto\cipher\*.d %this%\src\text\*.d %this%\src\text\convert\*.d %this%\src\text\locale\*.d %this%\src\io\*.d %this%\src\io\device\*.d %this%\src\io\stream\*.d %this%\src\io\protocol\*.d %this%\src\io\log\*.d %this%\src\io\log\model\*.d %this%\src\col\*.d %this%\src\net\*.d %this%\src\net\device\*.d %this%\src\net\ftp\*.d %this%\src\net\http\*.d %this%\src\net\http\model\*.d %this%\src\net\model\*.d>>%this%\objs.rsp

:::Make DinrusBaseX64.dll

%DMD% -g -O -debug -of%this%\DinrusBaseX64.dll @%this%\objs.rsp %this%\res\base.def %this%\res\base.res %LDIR%\phobos64.lib
pause
cls
@if not exist %this%\DinrusBaseX64.dll pause
cls
@if exist %this%\DinrusBaseX64.dll goto nextStep
@del %this%\objs.rsp
@goto Base


:nextStep
:::Make its export lib
%IMPLIB% /system /pagesize:256 %this%\DinrusX64.lib %this%\DinrusBaseX64.dll
copy %this%\DinrusX64.lib %LDIR%\DinrusBaseX64DLL.lib 
copy %this%\DinrusX64.lib %this%\DinrusBaseX64DLL.lib 
::copy %this%\DinrusBaseX64.dll e:\dinrus\dinrus64\bin
::copy %this%\DinrusBaseX64.dll c:\Windows\system32

:::To compress
:%PACK% %this%\DinrusBaseX64.dll

:::Clean
@del %this%\*.obj

:::Compiling imports into static part of dinrus.lib

%DMD%  -c -O  -g -of%this%\cidrus.obj %this%\export\cidrus.d -I%IMPDIR%
%DMD%  -c -O  -g -of%this%\stdrus.obj %this%\export\stdrus.d -I%IMPDIR%
%DMD%  -c -O  -g -of%this%\stdrusex.obj %this%\export\stdrusex.d -I%IMPDIR%
%DMD%  -c -O  -g -of%this%\runtime.obj %this%\export\runtime.d -I%IMPDIR%
%DMD%  -c -O  -g -of%this%\runtimetraits.obj %this%\export\runtimetraits.d -I%IMPDIR%
%DMD%  -c -O  -g -of%this%\object.obj %this%\export\object.d -I%IMPDIR%

%DMD%  -c -O  -g -of%this%\gc.obj %this%\export\gc.d -I%IMPDIR%
%DMD%  -c -O  -g -of%this%\thread.obj %this%\export\thread.d -I%IMPDIR%
%DMD%  -c -O  -g -of%this%\sync.obj %this%\export\sync.d -I%IMPDIR%
:%DMD%  -c -O  -g %this%\export\tracer.d
%DMD%  -c -O  -g -of%this%\ini.obj %this%\static\ini.d -I%IMPDIR%
%DMD%  -c -O  -g -of%this%\stringz.obj %this%\export\stringz.d -I%IMPDIR%

%DMD%  -c -O  -g -of%this%\wincom.obj %this%\export\com.d -I%IMPDIR%
%DMD%  -c -O  -g -of%this%\dinrus.obj %this%\export\dinrus.d -I%IMPDIR%
%DMD%  -c -O  -g -of%this%\exception.obj %this%\export\exception.d -I%IMPDIR%
:::%DMD%  -c -O  -g %this%\export\openrj.d

%DMD%  -c -O  -g -of%this%\rotozoom.obj %this%\static\lib\rotozoom.d -I%IMPDIR%
%DMD%  -c -O  -g -of%this%\msscript.obj %this%\export\sys\com\msscript.d
%DMD%  -c -O  -g -of%this%\activex.obj %this%\static\sys\com\activex.d
%DMD%  -c -O  -g -of%this%\json.obj %this%\static\json.d -I%IMPDIR%

::Special configuration items
%DMD%  -c -O  -g -of%this%\base.obj %this%\export\base.d -I%IMPDIR%
::%DMD%  -c -O  -g -of%this%\exeMain.obj %this%\static\exeMain.d -I%IMPDIR%




:%DMD%  -c -O  -g exef.d

%DMD%  -c -O  -g -of%this%\winapi.obj %this%\export\winapi.d -I%IMPDIR%
%DMD%  -c -O  -g -of%this%\usergdi.obj %this%\export\usergdi.d -I%IMPDIR%
%DMD%  -c -O  -g -of%this%\global.obj %this%\export\global.d -I%IMPDIR%

%DMD%  -c -O  -g -of%this%\all.obj %this%\export\tpl\all.d -I%IMPDIR%
%DMD%  -c -O  -g -of%this%\alloc.obj %this%\export\tpl\alloc.d -I%IMPDIR%
%DMD%  -c -O  -g -of%this%\bind.obj %this%\export\tpl\bind.d -I%IMPDIR%
%DMD%  -c -O  -g -of%this%\box.obj %this%\export\tpl\box.d -I%IMPDIR%
%DMD%  -c -O  -g -of%this%\collection.obj %this%\export\tpl\collection.d -I%IMPDIR%
%DMD%  -c -O  -g -of%this%\metastrings.obj %this%\export\tpl\metastrings.d -I%IMPDIR%
%DMD%  -c -O  -g -of%this%\minmax.obj %this%\export\tpl\minmax.d -I%IMPDIR%
%DMD%  -c -O  -g -of%this%\signal.obj %this%\export\tpl\signal.d -I%IMPDIR%
%DMD%  -c -O  -g -of%this%\args.obj %this%\export\tpl\args.d -I%IMPDIR%
%DMD%  -c -O  -g -of%this%\traits.obj %this%\export\tpl\traits.d -I%IMPDIR%
%DMD%  -c -O  -g -of%this%\typetuple.obj %this%\export\tpl\typetuple.d -I%IMPDIR%
%DMD%  -c -O  -g -of%this%\stream.obj %this%\export\tpl\stream.d  -I%IMPDIR%
%DMD%  -c -O  -g -of%this%\singleton.obj %this%\export\tpl\singleton.d  -I%IMPDIR%
%DMD%  -c -O  -g -of%this%\comtpl.obj %this%\export\tpl\com.d -I%IMPDIR%
%DMD%  -c -O  -g  -of%this%\std.obj %this%\export\tpl\std.d -I%IMPDIR%
%DMD%  -c -O  -g  -of%this%\weakref.obj %this%\export\tpl\weakref.d -I%IMPDIR%
%DMD%  -c -O  -g  -of%this%\tplarray.obj %this%\export\tpl\array.d -I%IMPDIR%
%DMD%  -c -O  -g  -of%this%\tplsigstruct.obj %this%\export\tpl\sigstruct.d -I%IMPDIR%
%DMD%  -c -O  -g  -of%this%\tpltuple.obj %this%\export\tpl\tuple.d -I%IMPDIR%
%DMD%  -c -O  -g  -of%this%\tplthreadpool.obj %this%\export\tpl\threadpool.d -I%IMPDIR%
%DMD%  -c -O  -g  -of%this%\tplvariant.obj %this%\export\tpl\variant.d -I%IMPDIR%
:pause
%DMD%  -c -O  -g -of%this%\WinStructs.obj %this%\export\sys\WinStructs.d -I%IMPDIR%

%DMD%  -c -O  -g -of%this%\WinIfaces.obj %this%\export\sys\WinIfaces.d -I%IMPDIR%
%DMD%  -c -O  -g -of%this%\WinConsts.obj %this%\export\sys\WinConsts.d -I%IMPDIR%
%DMD%  -c -O  -g -of%this%\WinFuncs.obj %this%\export\sys\WinFuncs.d -I%IMPDIR%
%DMD%  -c -O  -g -of%this%\WinProcess.obj %this%\export\sys\WinProcess.d -I%IMPDIR%
%DMD%  -c -O  -g -of%this%\registry.obj %this%\export\sys\registry.d -I%IMPDIR%
%DMD%  -c -O  -g -of%this%\sysCommon.obj %this%\export\sys\Common.d -I%IMPDIR%

:%DMD%  -c -O  -g %this%\export\sys\en.d
%DMD%  -c -O  -g -of%this%\memory.obj %this%\export\sys\memory.d -I%IMPDIR%
%DMD%  -c -O  -g -of%this%\uuid.obj %this%\export\sys\uuid.d -I%IMPDIR%
%DMD%  -c -O  -g -of%this%\comsys.obj %this%\static\sys\com\com.d -I%IMPDIR%


%DMD%  -c -O  -g -of%this%\shell32.obj %this%\export\sys\com\shell32.d -I%IMPDIR%
%DMD%  -c -O  -g -of%this%\scomall.obj %this%\export\sys\com\all.d -I%IMPDIR%

:%DMD%  -c -O  -g  %this%\static\lib\mesa.d mesa.lib

:%DMD%  -c -O  -g %this%\export\stddinrus\base64.d

:%DMD%  -c -O  -g -ofrt.obj @dobjs.rsp


:::Making library with static content
:dinrus2

%DMD% -lib -of%this%\dinrus2.lib %this%\base.obj  %this%\object.obj  %this%\cidrus.obj  %this%\stdrus.obj  %this%\dinrus.obj %this%\runtime.obj %this%\runtimetraits.obj  %this%\gc.obj  %this%\thread.obj  %this%\sync.obj  %this%\stringz.obj   %this%\all.obj  %this%\bind.obj  %this%\box.obj  %this%\metastrings.obj  %this%\minmax.obj  %this%\signal.obj  %this%\args.obj  %this%\typetuple.obj  %this%\traits.obj %this%\tpltuple.obj %this%\tplthreadpool.obj  %this%\exception.obj %LDIR%\minit.obj  %this%\WinStructs.obj  %this%\WinIfaces.obj  %this%\WinConsts.obj  %this%\WinFuncs.obj  %this%\WinProcess.obj  %this%\comtpl.obj  %this%\wincom.obj  %this%\shell32.obj  %this%\stream.obj  %this%\memory.obj %this%\activex.obj  %this%\winapi.obj  %this%\singleton.obj  %this%\alloc.obj  %this%\collection.obj  %this%\ini.obj  %this%\std.obj  %this%\uuid.obj  %this%\comsys.obj  %this%\rotozoom.obj  %this%\scomall.obj  %this%\global.obj  %this%\weakref.obj %this%\registry.obj %this%\stdrusex.obj %this%\usergdi.obj %this%\msscript.obj %this%\sysCommon.obj %this%\tplarray.obj %this%\tplsigstruct.obj %this%\tplvariant.obj %this%\Cdinr.lib
@if exist %this%\dinrus2.lib  goto Join
@if not exist %this%\dinrus2.lib pause
cls
@goto NextStep
:::Ading static libraries to DinrusX64.lib
:Join
%LIB% -p256   %this%\DinrusX64.lib  %this%\dinrus2.lib

:::Compiling codes from .\static folder

:Lib

%LS% -d %this%\static\lib\*.d >>%this%\lib.rsp
%DMD% -lib -of%this%\dlib.lib @%this%\lib.rsp 
@if exist %this%\dlib.lib del %this%\lib.rsp
%LIB% -p256  %this%\DinrusX64.lib %this%\dlib.lib
@if exist %this%\dlib.lib goto Col
@if not exist %this%\dlib.lib pause
@del %this%\col.rsp
cls
@goto Lib
pause

:Col
%LS% -d %this%\export\col\*.d  %this%\export\col\model\*.d %this%\export\col\impl\*.d %this%\export\col\iterator\*.d>>%this%\col.rsp
%DMD% -lib -of%this%\col.lib @%this%\col.rsp 
@if exist %this%\col.lib del %this%\col.rsp
%LIB% -p256  %this%\DinrusX64.lib %this%\col.lib
@if exist %this%\col.lib goto Util
@if not exist %this%\col.lib pause
@del %this%\col.rsp
cls
@goto Col

:Util
%LS% -d %this%\static\util\*.d %this%\static\util\uuid\*.d>>%this%\ut.rsp
%DMD% -lib -of%this%\util.lib @%this%\ut.rsp 
@if exist %this%\util.lib del %this%\ut.rsp
%LIB% -p256  %this%\DinrusX64.lib %this%\util.lib
@if exist %this%\util.lib goto Mesh
@if not exist %this%\util.lib pause
@del %this%\ut.rsp
cls
@goto Util

:Mesh
%LS% -d %this%\static\mesh\*.d>>%this%\mesh.rsp
%DMD% -lib -of%this%\mesh.lib @%this%\mesh.rsp 
@if exist %this%\mesh.lib del %this%\mesh.rsp
%LIB% -p256  %this%\DinrusX64.lib %this%\mesh.lib
@if exist %this%\mesh.lib goto St
@if not exist %this%\mesh.lib pause
@del %this%\mesh.rsp
cls
@goto Mesh

:St
%LS% -d %this%\static\st\*.d>>%this%\st.rsp
%DMD% -lib -of%this%\st.lib @%this%\st.rsp 
@if exist %this%\st.lib del %this%\st.rsp
%LIB% -p256  %this%\DinrusX64.lib %this%\st.lib
@if exist %this%\st.lib goto Geom
@if not exist %this%\st.lib pause
@del %this%\st.rsp
cls
@goto St

:Geom
%LS% -d %this%\static\geom\*.d>>%this%\geom.rsp
%DMD% -lib -of%this%\geom.lib @%this%\geom.rsp 
@if exist %this%\geom.lib del %this%\geom.rsp
%LIB% -p256  %this%\DinrusX64.lib %this%\geom.lib
@if exist %this%\geom.lib goto Math
@if not exist %this%\geom.lib pause
@del %this%\geom.rsp
cls
@goto Geom

:Math
%LS% -d %this%\static\math\*.d %this%\static\math\linalg\*.d %this%\static\math\internal\*.d %this%\static\math\random\*.d %this%\static\math\random\engines\*.d>>%this%\math.rsp
%DMD% -lib -of%this%\math.lib @%this%\math.rsp 
@if exist %this%\math.lib del %this%\math.rsp
%LIB% -p256  %this%\DinrusX64.lib %this%\math.lib
@if exist %this%\math.lib goto Crypto
@if not exist %this%\math.lib pause
@del %this%\math.rsp
cls
@goto Math

:Crypto

%LS% -d %this%\export\crypto\*.d %this%\export\crypto\cipher\*.d %this%\export\crypto\digest\*.d>>%this%\crypto.rsp
%DMD% -lib -of%this%\crypto.lib @%this%\crypto.rsp 
@if exist %this%\crypto.lib del %this%\crypto.rsp
%LIB% -p256  %this%\DinrusX64.lib %this%\crypto.lib
@if exist %this%\crypto.lib goto Text
@if not exist %this%\crypto.lib pause
@del %this%\crypto.rsp
cls
@goto Crypto


:Text
%LS% -d %this%\export\text\*.d %this%\export\text\convert\*.d %this%\export\text\json\*.d %this%\export\text\digest\*.d %this%\export\text\locale\*.d %this%\export\text\xml\*.d>>%this%\text.rsp
%DMD% -lib -of%this%\text.lib @%this%\text.rsp 
@if exist %this%\text.lib del %this%\text.rsp
%LIB% -p256  %this%\DinrusX64.lib %this%\text.lib
@if exist %this%\text.lib goto Time
@if not exist %this%\text.lib pause
@del %this%\text.rsp
cls
@goto Text

:Time
%LS% -d %this%\export\time\*.d  %this%\export\time\chrono\*.d>>%this%\time.rsp
%DMD% -lib -of%this%\time.lib @%this%\time.rsp 
@if exist %this%\time.lib del %this%\time.rsp
%LIB% -p256  %this%\DinrusX64.lib %this%\time.lib
@if exist %this%\time.lib goto IO
@if not exist %this%\time.lib pause
@del %this%\time.rsp
cls
@goto Time

:IO

::%this%\static\io\selector\*.d %this%\static\io\vfs\*.d
%LS% -d %this%\export\io\*.d %this%\export\io\device\*.d %this%\export\io\stream\*.d %this%\export\io\protocol\*.d %this%\export\io\log\*.d %this%\export\io\log\model\*.d>>%this%\io.rsp
%DMD% -lib -of%this%\io.lib @%this%\io.rsp 
@if exist %this%\io.lib del %this%\io.rsp
%LIB% -p256  %this%\DinrusX64.lib %this%\io.lib
@if exist %this%\io.lib goto NET
@if not exist %this%\io.lib pause
@del %this%\io.rsp
cls
@goto IO

:NET

::%this%\static\io\selector\*.d %this%\static\io\vfs\*.d
%LS% -d %this%\export\net\*.d %this%\export\net\device\*.d %this%\export\net\model\*.d %this%\export\net\http\model\*.d %this%\export\net\ftp\*.d %this%\export\net\http\*.d>>%this%\net.rsp
%DMD% -lib -of%this%\net.lib @%this%\net.rsp 
@if exist %this%\net.lib del %this%\net.rsp
%LIB% -p256  %this%\DinrusX64.lib %this%\net.lib
@if exist %this%\net.lib goto finish
@if not exist %this%\net.lib pause
@del %this%\net.rsp
cls
@goto NET

:DRwin32
:::Makin Dinrus_win32.lib
:if exist %LDIR%\DinrusWin32.lib goto skip
%LS% -d %this%\..\win32\*.d %this%\..\win32\directx\*.d %this%\..\win32\directx\*.def>>%this%\win32.rsp
%DMD% -O -release -version=Unicode -lib -of%this%\DinrusWin32.lib @%this%\win32.rsp
if exist %this%\win32.rsp del %this%\win32.rsp
if not exist %this%\DinrusWin32.lib pause
copy %this%\DinrusWin32.lib /b  %LDIR%\DinrusWin32.lib /b

::skip
goto finish

:Dinrus.Arc.dll
:::Making Dinrus.Arc.dll
cd %ARCDIR%
e:\dinrus\dinrus64\bin\rulada
%DMD% -of%ARCDIR%\Dinrus.Arc.dll %ARCDIR%\dll.d %ARCDIR%\arc.d %ARCDIR%\arcus.def %ARCDIR%\arcus.res derelict.lib arc.lib
%IMPLIB% /system %ARCDIR%\DinrusArcDLL.lib %ARCDIR%\Dinrus.Arc.dll
copy %ARCDIR%\DinrusArcDLL.lib %LDIR%
%PACK% %ARCDIR%\Dinrus.Arc.dll
copy %ARCDIR%\Dinrus.Arc.dll e:\dinrus\dinrus64\bin
del %ARCDIR%\*.dll %ARCDIR%\*.obj %ARCDIR%\*.rsp %ARCDIR%\*.map

:Dinrus.Minid.dll
cd %MINIDDIR%
%LS% -d %MINIDDIR%\*.d >>%MINIDDIR%\objs.rsp
%DMD% -g -O -cov -of%MINIDDIR%\Dinrus.Minid.dll @%MINIDDIR%\objs.rsp %MINIDDIR%\minid.def %MINIDDIR%\minid.res 
%IMPLIB% /system %MINIDDIR%\DinrusMinidDLL.lib %MINIDDIR%\Dinrus.Minid.dll
%PACK% %MINIDDIR%\Dinrus.Minid.dll
copy %MINIDDIR%\DinrusMinidDLL.lib %LDIR%
copy %MINIDDIR%\Dinrus.Minid.dll e:\dinrus\dinrus64\bin
del %MINIDDIR%\*.dll %MINIDDIR%\*.obj %MINIDDIR%\*.rsp %MINIDDIR%\*.map
cd %this%
e:\dinrus\dinrus64\bin\dinrus

:finish

::%LIB% -p256  %this%\DinrusX64.lib %this%\dlib.lib
::%LIB% -p256  %this%\DinrusX64.lib %this%\col.lib
::%LIB% -p256  %this%\DinrusX64.lib %this%\util.lib
::%LIB% -p256  %this%\DinrusX64.lib %this%\geom.lib
::%LIB% -p256  %this%\DinrusX64.lib %this%\mesh.lib
::%LIB% -p256  %this%\DinrusX64.lib %this%\st.lib
::%LIB% -p256  %this%\DinrusX64.lib %this%\math.lib
::%LIB% -p256  %this%\DinrusX64.lib %this%\crypto.lib
::%LIB% -p256  %this%\DinrusX64.lib %this%\time.lib
::%LIB% -p256  %this%\DinrusX64.lib %this%\text.lib
::%LIB% -p256  %this%\DinrusX64.lib %this%\io.lib
::%LIB% -p256  %this%\DinrusX64.lib %ARCDIR%\arc2.lib
::%LIB% -p256  %this%\DinrusX64.lib %MINIDDIR%\rminid.lib

:::Adding system imports
::::%LIB% -p256  DinrusX64.lib %LDIR%\export.lib

:::Copying DinrusX64.lib to main DinrusX64 lib folder

::%LIB% -p256  DinrusX64.lib %LDIR%\DImport.lib
copy %this%\DinrusX64.lib %LDIR%
copy %this%\DinrusX64.lib %LDIR%\DinrusX64_dbg.lib
copy %this%\DinrusX64.lib %this%\DinrusX64_dbg.lib

::%PACK% %this%\DinrusBaseX64.dll
copy %this%\DinrusBaseX64.dll e:\dinrus\dinrus64\bin


%DMD% -O -lib -of%this%\DinrusX64DllMain.lib %this%\static\dllMain.d %LDIR%\minit.obj
copy %this%\DinrusX64DllMain.lib  %LDIR%

%DMD% -O -lib -of%this%\DinrusX64ExeMain.lib %this%\static\exeMain.d %LDIR%\minit.obj
copy %this%\DinrusX64ExeMain.lib  %LDIR%


goto Cleaning

:::Making DinrusStd.lib
cd .\std\mk
del *.obj
e:\dinrus\dinrus64\bin\dmd -run compile.d
if exist DinrusStd.lib copy DinrusStd.lib  %LDIR%
if not exist DinrusStd.lib pause
if not exist %IMPDIR%\std mkdir %IMPDIR%\std
copy ..\*.d  %IMPDIR%\std\*.di 
del *.exe *.map
cd %this%
:Cleaning
:::Cleaning
%DMD% %this%\clean.d %this%\DinrusX64.lib %this%\DinrusX64ExeMain.lib
chcp 866
%this%\clean.py
::: same with the Dll - to bin folder

:exes

::del *.lib *.dll
cd %this%\Exe
mk.bat
pause
exit

