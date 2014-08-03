@REM Register COM server
@echo OFF
echo PWD=%cd%
echo Register server %1
regsvr32 /s /u "%1" && echo Unregistred
regsvr32 /s    "%1" && echo Registered
echo OK>%2
