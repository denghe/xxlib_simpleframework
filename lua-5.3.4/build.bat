echo 生成lua的库文件
cd src 
cl /O2 /W3 /c /DLUA_BUILD_AS_DLL l*.c 
del lua.obj luac.obj 
link /DLL /out:lua53.dll l*.obj 
echo 生成解释器
cl /O2 /W3 /c /DLUA_BUILD_AS_DLL lua.c luac.c 
link /out:lua.exe lua.obj lua53.lib 
echo 生成编译器
del lua.obj 
link /out:luac.exe l*.obj 
echo 删除中间
del *.obj

pause
