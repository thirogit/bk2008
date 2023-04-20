echo "copying C:\CATTLEBASE\CATTLEBASE\UPDATERCFG\PRJ\UPDATERCFG\LIB lib and dll to main lib directory"
copy "C:\cattlebase\cattlebase\updaterCfg\prj\UpdaterCfg\lib\UpdaterCfg_DU.dll" ..\..\lib\UpdaterCfg_DU.dll
copy "C:\cattlebase\cattlebase\updaterCfg\prj\UpdaterCfg\lib\UpdaterCfg_DU.lib" ..\..\lib\UpdaterCfg_DU.lib

echo "copying dll to test project directory"
copy "C:\cattlebase\cattlebase\updaterCfg\prj\UpdaterCfg\lib\UpdaterCfg_DU.dll" "..\testProject\UpdaterCfg_DU.dll"

