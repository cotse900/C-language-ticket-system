@echo off

REM Step 1: Copy the files to replace the worked-on files
copy /Y textbackup\accounts.txt accounts.txt
copy /Y textbackup\tickets.txt tickets.txt
copy /Y textbackup\accounts_arc.txt accounts_arc.txt
copy /Y textbackup\tickets_arc.txt tickets_arc.txt

echo Batch file execution completed.
pause
