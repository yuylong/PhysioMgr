# PhysioMgr

How to compile, install, and use
---------------------------------
0. Sorry, for simple I currently do not write the automatically deploying script. You need to compile, install and use this software following the instructions below.

1. Goto PhysioMgr sub-folder/directory.

   1.1  Comile the main program with qmake or QtCreator.
   
   1.2  Copy the output executable file to a separate folder.
   
   1.3  Use 'windeployqt' to add the enssetial library files.
   
   1.4  Copy 'PsmDbConfig.xml' to the folder of executable files.
   
2. Install MySQL.

   2.1  For standalone installation or server, you need to install the MySQL server, and import the SQL script in 'database' sub-folder.
   
   2.2  Install the MySQL connector/C, and copy libmysql.dll to the folder of executable files.
   
   2.3  If you would like to enable the data export and import functions, you need to copy 'mysql' and 'mysqldump' executable files to the folder of executable files.
   
3. Configuration

   3.1  Update the content of 'PsmDbConfig.xml' according to your environment.

4. Run and Enjoy!


CAUTIONS
---------
1. ONLY Simplified Chinese version currently.
