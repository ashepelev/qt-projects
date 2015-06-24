cmake -G "CodeBlocks - MinGW Makefiles" -DCMAKE_INSTALL_PREFIX=..\bin\ ..\Canny\
mingw32-make
mingw32-make install
copy launch.cmd ..\bin\
@pause