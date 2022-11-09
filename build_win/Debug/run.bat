@echo off
echo "-----------------------"
echo %cd%
echo "-----------------------"
echo input path:  %1%
echo "-----------------------"
echo output path: %2%
echo "-----------------------"
if [%1] == [] goto end
if [%2] == [] goto onlyOne

cmd /k %cd%/obj23mf.exe %1% %2%
goto end

:onlyOne
cmd /k %cd%/obj23mf.exe %1%

:end
echo "finished exectuion"