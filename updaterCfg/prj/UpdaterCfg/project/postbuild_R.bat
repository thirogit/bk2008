echo "copying C:\CATTLEBASE\CATTLEBASE\UPDATERCFG\PRJ\UPDATERCFG\LIB lib and dll to main lib directory"
copy "C:\cattlebase\cattlebase\updaterCfg\prj\UpdaterCfg\lib\UpdaterCfg_R.dll" ..\..\lib\UpdaterCfg_R.dll
copy "C:\cattlebase\cattlebase\updaterCfg\prj\UpdaterCfg\lib\UpdaterCfg_R.lib" ..\..\lib\UpdaterCfg_R.lib

echo "copying dll to test project directory"
copy "C:\cattlebase\cattlebase\updaterCfg\prj\UpdaterCfg\lib\UpdaterCfg_R.dll" "..\testProject\UpdaterCfg_R.dll"

