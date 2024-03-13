@echo off

rem Set the range of input sizes
set start_size=64
set end_size=16384
set /a size=%start_size%

rem Set the number of runs for each input size
set runs=10

rem Set the output file name
set output_file=results.csv

rem Create the header in the output file
echo "S.no,File,Input Size,Time Taken,Average Time" > %output_file%

:run_programs
if %size% gtr %end_size% goto end_programs

echo Running programs for input size %size%...

rem Run each program for the current input size
set "program=mv.exe"
call :run_program %size% "%program%"

set "program=openMp.exe"
call :run_program %size% "%program%"

set "program=mpi.exe"
call :run_program %size% "%program%"

set "program=openMpTiling.exe"
call :run_program %size% "%program%"

set "program=mpiTiling.exe"
call :run_program %size% "%program%"

rem Double the size for the next iteration
set /a size*=2

rem Move to the next iteration
goto run_programs

:run_program
setlocal
set "timesum=0"
set /a run=0

:run_once
if %run% geq %runs% goto end_run_once

rem Run the program and get the execution time
for /f "tokens=3" %%i in ('(time /t)') do set "start_time=%%i"
"%~2" %1 %1
for /f "tokens=3" %%i in ('(time /t)') do set "end_time=%%i"

rem Calculate the execution time
set /a run+=1
set "start_time=%start_time::=%"
set "end_time=%end_time::=%"
set /a diff_time=(1%end_time:~0,2%-1%start_time:~0,2%)*3600+(1%end_time:~2,2%-1%start_time:~2,2%)*60+(1%end_time:~4,2%-1%start_time:~4,2%)
set /a timesum+=diff_time

rem Repeat the run if needed
goto run_once

:end_run_once
rem Calculate the average time
set /a avg_time=%timesum%/%runs%

rem Output the result to the CSV file
echo %run%,%~2,%1,%timesum% secs,%avg_time% secs >> %output_file%

endlocal
exit /b
:end_programs

echo All programs executed successfully.
pause
