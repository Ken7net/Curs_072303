set fromDir=e:\Max\Repositories\Curs_Kornilova_072303
set toDir=e:\Max\CPP\Curs
set cp=4131
set akelpad=c:\TotalCmd\Plugins\exe\AkelPad.exe


xcopy %fromDir%\CursSrv\*.cpp %toDir%\CursSrv\ /Y
xcopy %fromDir%\CursSrv\*.h %toDir%\CursSrv\ /Y
xcopy %fromDir%\CursCln\*.cpp %toDir%\CursCln\ /Y
xcopy %fromDir%\CursCln\*.h %toDir%\CursCln\ /Y
xcopy %fromDir%\Utils\*.h %toDir%\Utils\ /Y


%akelpad% /OpenFile("%toDir%\CursCln\Cln.cpp") /Command(%cp%) /Command(4109)
%akelpad% /OpenFile("%toDir%\CursCln\Client.h") /Command(%cp%) /Command(4109)
%akelpad% /OpenFile("%toDir%\CursCln\MenuCln.h") /Command(%cp%) /Command(4109)

%akelpad% /OpenFile("%toDir%\CursSrv\Srv.cpp") /Command(%cp%) /Command(4109)
%akelpad% /OpenFile("%toDir%\CursSrv\Company.h") /Command(%cp%) /Command(4109)
%akelpad% /OpenFile("%toDir%\CursSrv\Date.h") /Command(%cp%) /Command(4109)
%akelpad% /OpenFile("%toDir%\CursSrv\DBWork.h") /Command(%cp%) /Command(4109)
%akelpad% /OpenFile("%toDir%\CursSrv\Mark.h") /Command(%cp%) /Command(4109)
%akelpad% /OpenFile("%toDir%\CursSrv\MenuSrv.h") /Command(%cp%) /Command(4109)
%akelpad% /OpenFile("%toDir%\CursSrv\Project.h") /Command(%cp%) /Command(4109)
%akelpad% /OpenFile("%toDir%\CursSrv\Rating.h") /Command(%cp%) /Command(4109)
%akelpad% /OpenFile("%toDir%\CursSrv\Server.h") /Command(%cp%) /Command(4109)
%akelpad% /OpenFile("%toDir%\CursSrv\User.h") /Command(%cp%) /Command(4109)
%akelpad% /OpenFile("%toDir%\Utils\stdafx.h") /Command(%cp%) /Command(4109)
%akelpad% /OpenFile("%toDir%\Utils\Connection.h") /Command(%cp%) /Command(4109)