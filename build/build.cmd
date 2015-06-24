set PATH=C:\Qt\5.3\mingw482_32\bin\;.;%PATH%
cmake -G "CodeBlocks - MinGW Makefiles" -DCMAKE_PREFIX_PATH=C:\Qt\5.3\mingw482_32\lib\cmake\Qt5 -DCMAKE_INSTALL_PREFIX=..\bin\ ..\registry\
make
make install
copy launch.cmd ..\bin
@pause