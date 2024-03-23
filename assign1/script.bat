@echo off
echo Batch script started.

rem Define input sizes
set "SIZES=64 128 256 512 1024 2048 4096 8192 16384 32768"

rem Define number of iterations
set "ITERATIONS=10"

rem Create a CSV file and add headers
echo S.no,File,Input Size,Time Taken> results.csv

setlocal enabledelayedexpansion

rem Run sequential matrix-vector multiplication program
echo Running sequential matrix-vector multiplication program...
for %%s in (%SIZES%) do (
    echo Input size: %%s
    for /l %%i in (1,1,%ITERATIONS%) do (
        echo Iteration %%i
        seq_mXv.exe %%s > temp.txt
        
        REM Extracting the third index from the output
        set "string="
        set /p string=<temp.txt
        set "index=0"
        for %%j in (!string!) do (
            set /a index+=1
            if !index! equ 3 (
                set "timeTaken=%%j"
            )
        )
        echo %%i,seq_mXv.exe,%%s,!timeTaken!

        REM Appending to CSV with delayed expansion for timeTaken
        echo %%i,seq_mXv.exe,%%s,!timeTaken! >> results.csv

        del temp.txt
    )
)

rem Run OpenMP naive matrix-vector multiplication program
echo Running OpenMP naive matrix-vector multiplication program...
for %%s in (%SIZES%) do (
    echo Input size: %%s
    for /l %%i in (1,1,%ITERATIONS%) do (
        echo Iteration %%i
        omp_mXv.exe %%s > temp.txt
        
        REM Extracting the third index from the output
        set "string="
        set /p string=<temp.txt
        set "index=0"
        for %%j in (!string!) do (
            set /a index+=1
            if !index! equ 3 (
                set "timeTaken=%%j"
            )
        )
        echo %%i,omp_mXv.exe,%%s,!timeTaken!

        REM Appending to CSV with delayed expansion for timeTaken
        echo %%i,omp_mXv.exe,%%s,!timeTaken! >> results.csv

        del temp.txt
    )
)

rem Run MPI naive matrix-vector multiplication program
echo Running MPI naive matrix-vector multiplication program...
for %%s in (%SIZES%) do (
    echo Input size: %%s
    for /l %%i in (1,1,%ITERATIONS%) do (
        echo Iteration %%i
        mpi_mXv.exe %%s > temp.txt
        
        REM Extracting the third index from the output
        set "string="
        set /p string=<temp.txt
        set "index=0"
        for %%j in (!string!) do (
            set /a index+=1
            if !index! equ 3 (
                set "timeTaken=%%j"
            )
        )
        echo %%i,mpi_mXv.exe,%%s,!timeTaken!

        REM Appending to CSV with delayed expansion for timeTaken
        echo %%i,mpi_mXv.exe,%%s,!timeTaken! >> results.csv

        del temp.txt
    )
)

rem Run OpenMP matrix-vector multiplication program with tiling
echo Running OpenMP matrix-vector multiplication program with tiling...
for %%s in (%SIZES%) do (
    echo Input size: %%s
    for /l %%i in (1,1,%ITERATIONS%) do (
        echo Iteration %%i
        omp_tiled_mXv.exe %%s > temp.txt
        
        REM Extracting the third index from the output
        set "string="
        set /p string=<temp.txt
        set "index=0"
        for %%j in (!string!) do (
            set /a index+=1
            if !index! equ 3 (
                set "timeTaken=%%j"
            )
        )
        echo %%i,omp_tiled_mXv.exe,%%s,!timeTaken!

        REM Appending to CSV with delayed expansion for timeTaken
        echo %%i,omp_tiled_mXv.exe,%%s,!timeTaken! >> results.csv

        del temp.txt
    )
)

rem Run MPI matrix-vector multiplication program with tiling
echo Running MPI matrix-vector multiplication program with tiling...
for %%s in (%SIZES%) do (
    echo Input size: %%s
    for /l %%i in (1,1,%ITERATIONS%) do (
        echo Iteration %%i
        mpi_tiled_mXv.exe %%s > temp.txt
        
        REM Extracting the third index from the output
        set "string="
        set /p string=<temp.txt
        set "index=0"
        for %%j in (!string!) do (
            set /a index+=1
            if !index! equ 3 (
                set "timeTaken=%%j"
            )
        )
        echo %%i,mpi_tiled_mXv.exe,%%s,!timeTaken!

        REM Appending to CSV with delayed expansion for timeTaken
        echo %%i,mpi_tiled_mXv.exe,%%s,!timeTaken! >> results.csv

        del temp.txt
    )
)

endlocal

echo All programs completed.
