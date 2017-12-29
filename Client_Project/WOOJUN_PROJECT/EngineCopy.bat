cd..
cd..

xcopy .\WOOJUN_ENGINE\Include\*.h .\Engine\Include\ /d /y /s
xcopy .\WOOJUN_ENGINE\Include\*.inl .\Engine\Include\ /d /y /s
xcopy .\WOOJUN_ENGINE\Bin\*.* .\Engine\Bin\ /d /y /s /exclude:Exclude.txt
xcopy .\Engine\Bin\*.* .\WOOJUN_CLIENT\Bin\ /d /y /s
xcopy .\Engine\Bin\*.* .\WOOJUN_TOOL\Bin\ /d /y /s