#This file was developed for Dinrus Programming Language by Vitaliy Kulich
# Copyright is by Dinrus Group.
# back
# Setting environment variables
export this=/e/dinrus/dinrus64/dev/dinrus/baseD2
export R=/e/dinrus/dinrus64/imp/D2
export LIBS=/e/dinrus/dinrus64/lib/d2/x64/mingw
export LDIR=/e/dinrus/dinrus64/lib/d2/x64
export DMD=/e/dinrus/dinrus64/bin/ldmd2.exe
export CC=clang
export LS=ls

::goto a

# Deleting previous objects
rm -R $LDIR/DinrusX64.a
rm -R $LDIR/DinrusX64.bak
rm -R $this/*.rsp
rm -R $this/*.o
rm -R $this/*.map
rm -R $this/*.dll
rm -R $this/src/rt/*.o
rm -R $this/*.a

# Files with staff that must be same in imports and base-making
#  just copied to imports immediately? without manual processing

# cp $this/src/sys/WinConsts.d $this/export/sys/WinConsts.d 
# cp $this/src/sys/WinStructs.d $this/export/sys/WinStructs.d 
# cp $this/src/sys/WinTypes.d $this/export/sys/WinTypes.d 
# cp $this/src/sys/WinIfaces.d $this/export/sys/WinIfaces.d 
# cp $this/src/base.d $this/export/base.d

# Making dirs for di files in /imp/dinrus/
#  and copying imports from ./export folder to them

if not exist $R mkdir $R
cp $this/export/*.d  $R/*.di 

rm -R $R/std
mkdir $R/std
cp $this/export/std/*.d  $R/std/*.di 

rm -R $R/tpl
mkdir $R/tpl
cp $this/export/tpl/*.d  $R/tpl/*.di

rm -R $R/st
mkdir $R/st
cp $this/export/st/*.d  $R/st/*.di

rm -R $R/mesh
mkdir $R/mesh
cp $this/export/mesh/*.d  $R/mesh/*.di

rm -R $R/win32
mkdir $R/win32
mkdir $R/win32/directx
cp $this/export/win32/*.di  $R/win32/*.di
cp $this/export/win32/directx/*.di  $R/win32/directx/*.di

::mkdir $R/def
::cp $this/../win32/directx/*.def  $R/def/*.def

rm -R $R/sys
mkdir $R/sys
mkdir $R/sys/com
cp $this/export/sys/*.d  $R/sys/*.di
::cp $this/export/sys/inc/*.d  $R/sys/inc/*.di
cp $this/export/sys/com/*.d  $R/sys/com/*.di

rm -R $R/a
mkdir $R/a
cp $this/export/lib/*.d  $R/lib/*.di

rm -R $R/time
mkdir $R/time
mkdir $R/time/chrono
cp $this/export/time/*.d  $R/time/*.di
cp $this/export/time/chrono/*.d  $R/time/chrono/*.di

rm -R $R/col
mkdir $R/col
mkdir $R/col/model
mkdir $R/col/impl
mkdir $R/col/iterator
cp $this/export/col/*.d  $R/col/*.di
cp $this/export/col/model/*.d  $R/col/model/*.di
cp $this/export/col/impl/*.d $R/col/impl/*.d
cp $this/export/col/iterator/*.d $R/col/iterator/*.d

rm -R $R/geom
mkdir $R/geom
cp $this/export/geom/*.d  $R/geom/*.di

rm -R $R/math
mkdir $R/math
cp $this/export/math/*.d  $R/math/*.di
mkdir $R/math/internal
cp $this/export/math/internal/*.d  $R/math/internal/*.di
mkdir $R/math/random
cp $this/export/math/random/*.d  $R/math/random/*.di
mkdir $R/math/random/engines
cp $this/export/math/random/engines/*.d  $R/math/random/engines/*.di
mkdir $R/math/linalg
cp $this/export/math/linalg/*.d  $R/math/linalg/*.di

rm -R $R/util
mkdir $R/util
cp $this/export/util/*.d  $R/util/*.di

rm -R $R/crypto
mkdir $R/crypto
mkdir $R/crypto/cipher
mkdir $R/crypto/digest
cp $this/export/crypto/*.d $R/crypto/*.di
cp $this/export/crypto/cipher/*.d $R/crypto/cipher/*.di
cp $this/export/crypto/digest/*.d $R/crypto/digest/*.di

rm -R $R/text
mkdir $R/text
mkdir $R/text/convert
mkdir $R/text/json
mkdir $R/text/locale
mkdir $R/text/xml
cp $this/export/text/*.d $R/text/*.di
cp $this/export/text/json/*.d $R/text/json/*.di
cp $this/export/text/digest/*.d $R/text/digest/*.di
cp $this/export/text/locale/*.d $R/text/locale/*.di
cp $this/export/text/xml/*.d $R/text/xml/*.di
cp $this/export/text/convert/*.d $R/text/convert/*.di

rm -R  $R/io
mkdir $R/io
mkdir $R/io/device
mkdir $R/io/stream
mkdir $R/io/protocol
mkdir $R/io/log
mkdir $R/io/log/model
::mkdir $R/io/selector
::mkdir $R/io/vfs
cp $this/export/io/*.d  $R/io/*.di
cp $this/export/io/device/*.d  $R/io/device/*.di
cp $this/export/io/stream/*.d  $R/io/stream/*.di
cp $this/export/io/protocol/*.d  $R/io/protocol/*.di
cp $this/export/io/log/*.d  $R/io/log/*.di
cp $this/export/io/log/model/*.d  $R/io/log/model/*.di
::cp $this/export/io/selector/*.d  $R/io/selector/*.di
::cp $this/export/io/vfs/*.d  $R/io/vfs/*.di

rm -R  $R/net
mkdir $R/net
mkdir $R/net/device
mkdir $R/net/ftp
mkdir $R/net/htpp
mkdir $R/net/model
mkdir $R/net/http/model

cp $this/export/net/*.d  $R/net/*.di
cp $this/export/net/device/*.d  $R/net/device/*.di
cp $this/export/net/ftp/*.d  $R/net/ftp/*.di
cp $this/export/net/htpp/*.d  $R/net/htpp/*.di
cp $this/export/net/model/*.d  $R/net/model/*.di
cp $this/export/net/http/model/*.d  $R/net/http/model/*.di

# Compiling C code

$CC-c -o$this/complex.o $this/src/rt/complex.c
$CC-c  -o$this/critical.o $this/src/rt/critical.c
$CC-c  -o$this/deh.o $this/src/rt/deh.c
$CC-c  -o$this/monitor.o $this/src/rt/monitor.c

$DMD -lib -of$this/Cdinr.a $this/complex.o $this/critical.o $this/deh.o $this/monitor.o


# Base
# Creating respond file
$LS -d $this/src/std/*.d $this/src/*.d $this/src/lib/*.d $this/src/tpl/*.d $this/src/rt/*.d $this/src/sys/*.d $this/src/sys/com/*.d $this/src/math/*.d $this/src/math/random/*.d $this/src/time/*.d $this/src/time/chrono/*.d $this/src/crypto/*.d $this/src/crypto/digest/*.d $this/src/crypto/cipher/*.d $this/src/text/*.d $this/src/text/convert/*.d $this/src/text/locale/*.d $this/src/io/*.d $this/src/io/device/*.d $this/src/io/stream/*.d $this/src/io/protocol/*.d $this/src/io/log/*.d $this/src/io/log/model/*.d $this/src/col/*.d $this/src/net/*.d $this/src/net/device/*.d $this/src/net/ftp/*.d $this/src/net/http/*.d $this/src/net/http/model/*.d $this/src/net/model/*.d>>$this/objs.rsp

# Make DinrusBaseX64.dll

# if exist /e/dinrus/dinrus64/bin/setD2.exe /e/dinrus/dinrus64/bin/setD2.exe

$DMD -g -O -debug -of$this/DinrusBaseX64.dll @$this/objs.rsp $this/res/base.def $this/res/base.res $LDIR/minit.o $LDIR/phobos64.a $this/Cdinr.a
pause
cls
@if not exist $this/DinrusBaseX64.dll pause
cls
@if exist $this/DinrusBaseX64.dll goto nextStep
rm -R $this/objs.rsp
@goto Base


# nextStep
# Make its export a
%IMPa% /system /pagesiz/e256 $this/DinrusX64.a $this/DinrusBaseX64.dll
cp $this/DinrusX64.a $LDIR/DinrusBaseX64DLL.a 
cp $this/DinrusX64.a $this/DinrusBaseX64DLL.a 
::cp $this/DinrusBaseX64.dll /e/dinrus/dinrus64/bin
::cp $this/DinrusBaseX64.dll c:/Windows/system32

# To compress
:%PACK% $this/DinrusBaseX64.dll

# Clean
rm -R $this/*.o

# Compiling imports into static part of dinrus.a

$DMD  -c -O  -g -of$this/cidrus.o $this/export/cidrus.d -I$R
$DMD  -c -O  -g -of$this/stdrus.o $this/export/stdrus.d -I$R
$DMD  -c -O  -g -of$this/stdrusex.o $this/export/stdrusex.d -I$R
$DMD  -c -O  -g -of$this/runtime.o $this/export/runtime.d -I$R
$DMD  -c -O  -g -of$this/runtimetraits.o $this/export/runtimetraits.d -I$R
$DMD  -c -O  -g -of$this/object.o $this/export/object.d -I$R

$DMD  -c -O  -g -of$this/gc.o $this/export/gc.d -I$R
$DMD  -c -O  -g -of$this/thread.o $this/export/thread.d -I$R
$DMD  -c -O  -g -of$this/sync.o $this/export/sync.d -I$R
:$DMD  -c -O  -g $this/export/tracer.d
$DMD  -c -O  -g -of$this/ini.o $this/static/ini.d -I$R
$DMD  -c -O  -g -of$this/stringz.o $this/export/stringz.d -I$R

$DMD  -c -O  -g -of$this/wincom.o $this/export/com.d -I$R
$DMD  -c -O  -g -of$this/dinrus.o $this/export/dinrus.d -I$R
$DMD  -c -O  -g -of$this/exception.o $this/export/exception.d -I$R
:::$DMD  -c -O  -g $this/export/openrj.d

$DMD  -c -O  -g -of$this/rotozoom.o $this/static/lib/rotozoom.d -I$R
$DMD  -c -O  -g -of$this/msscript.o $this/export/sys/com/msscript.d
$DMD  -c -O  -g -of$this/activex.o $this/static/sys/com/activex.d
$DMD  -c -O  -g -of$this/json.o $this/static/json.d -I$R

::Special configuration items
$DMD  -c -O  -g -of$this/base.o $this/export/base.d -I$R
::$DMD  -c -O  -g -of$this/exeMain.o $this/static/exeMain.d -I$R




:$DMD  -c -O  -g exef.d

$DMD  -c -O  -g -of$this/winapi.o $this/export/winapi.d -I$R
$DMD  -c -O  -g -of$this/usergdi.o $this/export/usergdi.d -I$R
$DMD  -c -O  -g -of$this/global.o $this/export/global.d -I$R

$DMD  -c -O  -g -of$this/all.o $this/export/tpl/all.d -I$R
$DMD  -c -O  -g -of$this/alloc.o $this/export/tpl/alloc.d -I$R
$DMD  -c -O  -g -of$this/bind.o $this/export/tpl/bind.d -I$R
$DMD  -c -O  -g -of$this/box.o $this/export/tpl/box.d -I$R
$DMD  -c -O  -g -of$this/collection.o $this/export/tpl/collection.d -I$R
$DMD  -c -O  -g -of$this/metastrings.o $this/export/tpl/metastrings.d -I$R
$DMD  -c -O  -g -of$this/minmax.o $this/export/tpl/minmax.d -I$R
$DMD  -c -O  -g -of$this/signal.o $this/export/tpl/signal.d -I$R
$DMD  -c -O  -g -of$this/args.o $this/export/tpl/args.d -I$R
$DMD  -c -O  -g -of$this/traits.o $this/export/tpl/traits.d -I$R
$DMD  -c -O  -g -of$this/typetuple.o $this/export/tpl/typetuple.d -I$R
$DMD  -c -O  -g -of$this/stream.o $this/export/tpl/stream.d  -I$R
$DMD  -c -O  -g -of$this/singleton.o $this/export/tpl/singleton.d  -I$R
$DMD  -c -O  -g -of$this/comtpl.o $this/export/tpl/com.d -I$R
$DMD  -c -O  -g  -of$this/std.o $this/export/tpl/std.d -I$R
$DMD  -c -O  -g  -of$this/weakref.o $this/export/tpl/weakref.d -I$R
$DMD  -c -O  -g  -of$this/tplarray.o $this/export/tpl/array.d -I$R
$DMD  -c -O  -g  -of$this/tplsigstruct.o $this/export/tpl/sigstruct.d -I$R
$DMD  -c -O  -g  -of$this/tpltuple.o $this/export/tpl/tuple.d -I$R
$DMD  -c -O  -g  -of$this/tplthreadpool.o $this/export/tpl/threadpool.d -I$R
$DMD  -c -O  -g  -of$this/tplvariant.o $this/export/tpl/variant.d -I$R
# pause
$DMD  -c -O  -g -of$this/WinStructs.o $this/export/sys/WinStructs.d -I$R

$DMD  -c -O  -g -of$this/WinIfaces.o $this/export/sys/WinIfaces.d -I$R
$DMD  -c -O  -g -of$this/WinConsts.o $this/export/sys/WinConsts.d -I$R
$DMD  -c -O  -g -of$this/WinFuncs.o $this/export/sys/WinFuncs.d -I$R
$DMD  -c -O  -g -of$this/WinProcess.o $this/export/sys/WinProcess.d -I$R
$DMD  -c -O  -g -of$this/registry.o $this/export/sys/registry.d -I$R
$DMD  -c -O  -g -of$this/sysCommon.o $this/export/sys/Common.d -I$R

:$DMD  -c -O  -g $this/export/sys/en.d
$DMD  -c -O  -g -of$this/memory.o $this/export/sys/memory.d -I$R
$DMD  -c -O  -g -of$this/uuid.o $this/export/sys/uuid.d -I$R
$DMD  -c -O  -g -of$this/comsys.o $this/static/sys/com/com.d -I$R


$DMD  -c -O  -g -of$this/shell32.o $this/export/sys/com/shell32.d -I$R
$DMD  -c -O  -g -of$this/scomall.o $this/export/sys/com/all.d -I$R

:$DMD  -c -O  -g  $this/static/lib/mesa.d mesa.a

:$DMD  -c -O  -g $this/export/stddinrus/base64.d

:$DMD  -c -O  -g -ofrt.o @dobjs.rsp


# Making arary with static content
# dinrus2

$DMD -lib -of$this/dinrus2.a $this/base.o  $this/object.o  $this/cidrus.o  $this/stdrus.o  $this/dinrus.o $this/runtime.o $this/runtimetraits.o  $this/gc.o  $this/thread.o  $this/sync.o  $this/stringz.o   $this/all.o  $this/bind.o  $this/box.o  $this/metastrings.o  $this/minmax.o  $this/signal.o  $this/args.o  $this/typetuple.o  $this/traits.o $this/tpltuple.o $this/tplthreadpool.o  $this/exception.o $LDIR/minit.o  $this/WinStructs.o  $this/WinIfaces.o  $this/WinConsts.o  $this/WinFuncs.o  $this/WinProcess.o  $this/comtpl.o  $this/wincom.o  $this/shell32.o  $this/stream.o  $this/memory.o $this/activex.o  $this/winapi.o  $this/singleton.o  $this/alloc.o  $this/collection.o  $this/ini.o  $this/std.o  $this/uuid.o  $this/comsys.o  $this/rotozoom.o  $this/scomall.o  $this/global.o  $this/weakref.o $this/registry.o $this/stdrusex.o $this/usergdi.o $this/msscript.o $this/sysCommon.o $this/tplarray.o $this/tplsigstruct.o $this/tplvariant.o $this/Cdinr.a
@if exist $this/dinrus2.a  goto Join
@if not exist $this/dinrus2.a pause
cls
@goto NextStep
# Ading static araries to DinrusX64.a
# Join
%a% -p256   $this/DinrusX64.a  $this/dinrus2.a

# Compiling codes from ./static folder

# a

$LS -d $this/static/lib/*.d >>$this/a.rsp
$DMD -lib -of$this/da.a @$this/a.rsp 
@if exist $this/da.a del $this/a.rsp
%a% -p256  $this/DinrusX64.a $this/da.a
@if exist $this/da.a goto Col
@if not exist $this/da.a pause
rm -R $this/col.rsp
cls
@goto a
pause

# Col
$LS -d $this/export/col/*.d  $this/export/col/model/*.d $this/export/col/impl/*.d $this/export/col/iterator/*.d>>$this/col.rsp
$DMD -lib -of$this/col.a @$this/col.rsp 
@if exist $this/col.a del $this/col.rsp
%a% -p256  $this/DinrusX64.a $this/col.a
@if exist $this/col.a goto Util
@if not exist $this/col.a pause
rm -R $this/col.rsp
cls
@goto Col

# Util
$LS -d $this/static/util/*.d $this/static/util/uuid/*.d>>$this/ut.rsp
$DMD -lib -of$this/util.a @$this/ut.rsp 
@if exist $this/util.a del $this/ut.rsp
%a% -p256  $this/DinrusX64.a $this/util.a
@if exist $this/util.a goto Mesh
@if not exist $this/util.a pause
rm -R $this/ut.rsp
cls
@goto Util

# Mesh
$LS -d $this/static/mesh/*.d>>$this/mesh.rsp
$DMD -lib -of$this/mesh.a @$this/mesh.rsp 
@if exist $this/mesh.a del $this/mesh.rsp
%a% -p256  $this/DinrusX64.a $this/mesh.a
@if exist $this/mesh.a goto St
@if not exist $this/mesh.a pause
rm -R $this/mesh.rsp
cls
@goto Mesh

# St
$LS -d $this/static/st/*.d>>$this/st.rsp
$DMD -lib -of$this/st.a @$this/st.rsp 
@if exist $this/st.a del $this/st.rsp
%a% -p256  $this/DinrusX64.a $this/st.a
@if exist $this/st.a goto Geom
@if not exist $this/st.a pause
rm -R $this/st.rsp
cls
@goto St

# Geom
$LS -d $this/static/geom/*.d>>$this/geom.rsp
$DMD -lib -of$this/geom.a @$this/geom.rsp 
@if exist $this/geom.a del $this/geom.rsp
%a% -p256  $this/DinrusX64.a $this/geom.a
@if exist $this/geom.a goto Math
@if not exist $this/geom.a pause
rm -R $this/geom.rsp
cls
@goto Geom

# Math
$LS -d $this/static/math/*.d $this/static/math/linalg/*.d $this/static/math/internal/*.d $this/static/math/random/*.d $this/static/math/random/engines/*.d>>$this/math.rsp
$DMD -lib -of$this/math.a @$this/math.rsp 
@if exist $this/math.a del $this/math.rsp
%a% -p256  $this/DinrusX64.a $this/math.a
@if exist $this/math.a goto Crypto
@if not exist $this/math.a pause
rm -R $this/math.rsp
cls
@goto Math

# Crypto

$LS -d $this/export/crypto/*.d $this/export/crypto/cipher/*.d $this/export/crypto/digest/*.d>>$this/crypto.rsp
$DMD -lib -of$this/crypto.a @$this/crypto.rsp 
@if exist $this/crypto.a del $this/crypto.rsp
%a% -p256  $this/DinrusX64.a $this/crypto.a
@if exist $this/crypto.a goto Text
@if not exist $this/crypto.a pause
rm -R $this/crypto.rsp
cls
@goto Crypto


# Text
$LS -d $this/export/text/*.d $this/export/text/convert/*.d $this/export/text/json/*.d $this/export/text/digest/*.d $this/export/text/locale/*.d $this/export/text/xml/*.d>>$this/text.rsp
$DMD -lib -of$this/text.a @$this/text.rsp 
@if exist $this/text.a del $this/text.rsp
%a% -p256  $this/DinrusX64.a $this/text.a
@if exist $this/text.a goto Time
@if not exist $this/text.a pause
rm -R $this/text.rsp
cls
@goto Text

# Time
$LS -d $this/export/time/*.d  $this/export/time/chrono/*.d>>$this/time.rsp
$DMD -lib -of$this/time.a @$this/time.rsp 
@if exist $this/time.a del $this/time.rsp
%a% -p256  $this/DinrusX64.a $this/time.a
@if exist $this/time.a goto IO
@if not exist $this/time.a pause
rm -R $this/time.rsp
cls
@goto Time

# IO

::$this/static/io/selector/*.d $this/static/io/vfs/*.d
$LS -d $this/export/io/*.d $this/export/io/device/*.d $this/export/io/stream/*.d $this/export/io/protocol/*.d $this/export/io/log/*.d $this/export/io/log/model/*.d>>$this/io.rsp
$DMD -lib -of$this/io.a @$this/io.rsp 
@if exist $this/io.a del $this/io.rsp
%a% -p256  $this/DinrusX64.a $this/io.a
@if exist $this/io.a goto NET
@if not exist $this/io.a pause
rm -R $this/io.rsp
cls
@goto IO

# NET

::$this/static/io/selector/*.d $this/static/io/vfs/*.d
$LS -d $this/export/net/*.d $this/export/net/device/*.d $this/export/net/model/*.d $this/export/net/http/model/*.d $this/export/net/ftp/*.d $this/export/net/http/*.d>>$this/net.rsp
$DMD -lib -of$this/net.a @$this/net.rsp 
@if exist $this/net.a del $this/net.rsp
%a% -p256  $this/DinrusX64.a $this/net.a
@if exist $this/net.a goto finish
@if not exist $this/net.a pause
rm -R $this/net.rsp
cls
@goto NET

# DRwin32
# Makin Dinrus_win32.a
# if exist $LDIR/DinrusWin32.a goto skip
$LS -d $this/../win32/*.d $this/../win32/directx/*.d $this/../win32/directx/*.def>>$this/win32.rsp
$DMD -O -release -version=Unicode -lib -of$this/DinrusWin32.a @$this/win32.rsp
if exist $this/win32.rsp del $this/win32.rsp
if not exist $this/DinrusWin32.a pause
cp $this/DinrusWin32.a /b  $LDIR/DinrusWin32.a /b

::skip
goto finish

# Dinrus.Arc.dll
# Making Dinrus.Arc.dll
cd %ARCDIR%
/e/dinrus/dinrus64/bin/rulada
$DMD -of%ARCDIR%/Dinrus.Arc.dll %ARCDIR%/dll.d %ARCDIR%/arc.d %ARCDIR%/arcus.def %ARCDIR%/arcus.res derelict.a arc.a
%IMPa% /system %ARCDIR%/DinrusArcDLL.a %ARCDIR%/Dinrus.Arc.dll
cp %ARCDIR%/DinrusArcDLL.a $LDIR
%PACK% %ARCDIR%/Dinrus.Arc.dll
cp %ARCDIR%/Dinrus.Arc.dll /e/dinrus/dinrus64/bin
del %ARCDIR%/*.dll %ARCDIR%/*.o %ARCDIR%/*.rsp %ARCDIR%/*.map

# Dinrus.Minid.dll
cd %MINIDDIR%
$LS -d %MINIDDIR%/*.d >>%MINIDDIR%/objs.rsp
$DMD -g -O -cov -of%MINIDDIR%/Dinrus.Minid.dll @%MINIDDIR%/objs.rsp %MINIDDIR%/minid.def %MINIDDIR%/minid.res 
%IMPa% /system %MINIDDIR%/DinrusMinidDLL.a %MINIDDIR%/Dinrus.Minid.dll
%PACK% %MINIDDIR%/Dinrus.Minid.dll
cp %MINIDDIR%/DinrusMinidDLL.a $LDIR
cp %MINIDDIR%/Dinrus.Minid.dll /e/dinrus/dinrus64/bin
del %MINIDDIR%/*.dll %MINIDDIR%/*.o %MINIDDIR%/*.rsp %MINIDDIR%/*.map
cd $this
/e/dinrus/dinrus64/bin/dinrus

# finish

::%a% -p256  $this/DinrusX64.a $this/da.a
::%a% -p256  $this/DinrusX64.a $this/col.a
::%a% -p256  $this/DinrusX64.a $this/util.a
::%a% -p256  $this/DinrusX64.a $this/geom.a
::%a% -p256  $this/DinrusX64.a $this/mesh.a
::%a% -p256  $this/DinrusX64.a $this/st.a
::%a% -p256  $this/DinrusX64.a $this/math.a
::%a% -p256  $this/DinrusX64.a $this/crypto.a
::%a% -p256  $this/DinrusX64.a $this/time.a
::%a% -p256  $this/DinrusX64.a $this/text.a
::%a% -p256  $this/DinrusX64.a $this/io.a
::%a% -p256  $this/DinrusX64.a %ARCDIR%/arc2.a
::%a% -p256  $this/DinrusX64.a %MINIDDIR%/rminid.a

# Adding system imports
::::%a% -p256  DinrusX64.a $LDIR/export.a

# Copying DinrusX64.a to main DinrusX64 a folder

::%a% -p256  DinrusX64.a $LDIR/DImport.a
cp $this/DinrusX64.a $LDIR
cp $this/DinrusX64.a $LDIR/DinrusX64_dbg.a
cp $this/DinrusX64.a $this/DinrusX64_dbg.a

::%PACK% $this/DinrusBaseX64.dll
cp $this/DinrusBaseX64.dll /e/dinrus/dinrus64/bin


$DMD -O -lib -of$this/DinrusX64DllMain.a $this/static/dllMain.d $LDIR/minit.o
cp $this/DinrusX64DllMain.a  $LDIR

$DMD -O -lib -of$this/DinrusX64ExeMain.a $this/static/exeMain.d $LDIR/minit.o
cp $this/DinrusX64ExeMain.a  $LDIR


goto Cleaning

# Making DinrusStd.a
cd ./std/mk
del *.o
/e/dinrus/dinrus64/bin/dmd -run compile.d
if exist DinrusStd.a cp DinrusStd.a  $LDIR
if not exist DinrusStd.a pause
if not exist $R/std mkdir $R/std
cp ../*.d  $R/std/*.di 
del *.exe *.map
cd $this
# Cleaning
# Cleaning
$DMD $this/clean.d $this/DinrusX64.a $this/DinrusX64ExeMain.a
chcp 866
$this/clean.py
#  same with the Dll - to bin folder

# exes

::del *.a *.dll
cd $this/Exe
mk.bat
pause
exit

