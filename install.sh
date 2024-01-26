#!/usr/bin/bash
set  -v
mkdir ~/.twcalc
install --mode=ugo+x bin/release/twcalc /usr/bin
cp icons/icon_64.png /usr/share/pixmaps/twcalc_64.png
install twcalc.desktop /usr/share/applications
