dinrus
set this=%DINRUS%\..\dev\dinrus\Base\Exe
%DINRUS%\drc -release %this%\dinrus.d %this%\scConfig.d %this%\Resources\dinrus.res DinrusX86.lib DinrusX86ExeMain.lib
%DINRUS%\upx dinrus.exe
%DINRUS%\drc -release %this%\dinrus2.d %this%\scConfig.d %this%\Resources\dinrus.res DinrusX86.lib DinrusX86ExeMain.lib
%DINRUS%\upx dinrus2.exe
%DINRUS%\drc -release %this%\dinrusex.d %this%\scConfig.d %this%\Resources\dinrus.res DinrusX86.lib DinrusX86ExeMain.lib
%DINRUS%\upx dinrusex.exe
%DINRUS%\drc -release %this%\dgui.d %this%\scConfig.d %this%\Resources\dinrus.res DinrusX86.lib DinrusX86ExeMain.lib
%DINRUS%\upx dgui.exe
%DINRUS%\drc -release %this%\rulada.d  %this%\scConfig.d %this%\Resources\dinrus.res DinrusX86.lib DinrusX86ExeMain.lib
%DINRUS%\upx rulada.exe
%DINRUS%\drc -release %this%\ruladaex.d %this%\scConfig.d %this%\Resources\dinrus.res DinrusX86.lib DinrusX86ExeMain.lib
%DINRUS%\upx ruladaex.exe
%DINRUS%\drc -release %this%\rgui.d %this%\scConfig.d %this%\Resources\dinrus.res DinrusX86.lib DinrusX86ExeMain.lib
%DINRUS%\upx rgui.exe
%DINRUS%\drc -release %this%\libproc.d %this%\Resources\dinrus.res DinrusX86.lib DinrusX86ExeMain.lib
%DINRUS%\upx libproc.exe
%DINRUS%\drc -release %this%\setd2.d %this%\scConfig.d %this%\Resources\dinrus.res DinrusX86.lib DinrusX86ExeMain.lib
%DINRUS%\upx setd2.exe
copy %this%\*.exe %DINRUS%
del %this%\*.map %this%\*.obj %this%\*.exe