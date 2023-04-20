
ProjectConfigAtlps.dll: dlldata.obj ProjectConfigAtl_p.obj ProjectConfigAtl_i.obj
	link /dll /out:ProjectConfigAtlps.dll /def:ProjectConfigAtlps.def /entry:DllMain dlldata.obj ProjectConfigAtl_p.obj ProjectConfigAtl_i.obj \
		kernel32.lib rpcndr.lib rpcns4.lib rpcrt4.lib oleaut32.lib uuid.lib \

.c.obj:
	cl /c /Ox /DWIN32 /D_WIN32_WINNT=0x0400 /DREGISTER_PROXY_DLL \
		$<

clean:
	@del ProjectConfigAtlps.dll
	@del ProjectConfigAtlps.lib
	@del ProjectConfigAtlps.exp
	@del dlldata.obj
	@del ProjectConfigAtl_p.obj
	@del ProjectConfigAtl_i.obj
