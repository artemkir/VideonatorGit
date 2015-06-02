
rd /q /s build_vc9
md build_vc9
cd build_vc9

rem for /r /d %%i in (build*) do ( rd /s /q %%i )
 
cmake -G"Visual Studio 9 2008" ..

pause
