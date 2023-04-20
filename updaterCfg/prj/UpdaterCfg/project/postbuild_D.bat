echo "copying C:\CATTLEBASE\CATTLEBASE\UPDATERCFG\PRJ\UPDATERCFG\LIB lib and dll to main lib directory"
copy "C:\cattlebase\cattlebase\updaterCfg\prj\UpdaterCfg\lib\UpdaterCfg_D.dll" ..\..\lib\UpdaterCfg_D.dll
copy "C:\cattlebase\cattlebase\updaterCfg\prj\UpdaterCfg\lib\UpdaterCfg_D.lib" ..\..\lib\UpdaterCfg_D.lib

echo "copying dll to test project directory"
copy "C:\cattlebase\cattlebase\updaterCfg\prj\UpdaterCfg\lib\UpdaterCfg_D.dll" "..\testProject\UpdaterCfg_D.dll"

