#!/bin/bash
set -x
mkdir bin
mkdir bin/release
mkdir build
mkdir build/release
qmake -o build/release/Makefile calc_ui.pro -spec linux-g++ CONFIG+=qtquickcompiler CONFIG+=release && make qmake_all
cd build/release
sed 's/$(LINK) $(LFLAGS) -o $(TARGET) $(OBJECTS) $(OBJCOMP) $(LIBS)/$(LINK) $(OBJECTS) $(OBJCOMP) $(LIBS) $(LFLAGS) -o $(TARGET)/1' Makefile >> makefile
rm Makefile
make -j$(nproc)
