@if "%CFG%"=="" set CFG=Debug
@call vcvars32.bat
@cd ..
@md lib
msdev src/cppunit/cppunit.dsp /MAKE "cppunit - Win32 %CFG%"
msdev srcex/cppunit/cppunit.dsp /MAKE "cppunit - Win32 %CFG%"
@cd ..\..
msdev kfc/develop/rununit/rununit.dsp /MAKE "rununit - Win32 %CFG%"
@pause
