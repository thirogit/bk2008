echo "copying C:\CATTLEBASE\CATTLEBASE\UPDATERCFG\PRJ\UPDATERCFG\LIB lib and dll to main lib directory"
copy "C:\cattlebase\cattlebase\updaterCfg\prj\UpdaterCfg\lib\UpdaterCfg_RU.dll" ..\..\lib\UpdaterCfg_RU.dll
copy "C:\cattlebase\cattlebase\updaterCfg\prj\UpdaterCfg\lib\UpdaterCfg_RU.lib" ..\..\lib\UpdaterCfg_RU.lib

echo "copying dll to test project directory"
copy "C:\cattlebase\cattlebase\updaterCfg\prj\UpdaterCfg\lib\UpdaterCfg_RU.dll" "..\testProject\UpdaterCfg_RU.dll"

