@echo off
setlocal

for /f "tokens=*" %%i in ('d:\cygwin\bin\cygpath %*') do set VAR=%%i

d:\cygwin\bin\bash --login -c "cd \"%CD%\";  \"%var%\""

endlocal