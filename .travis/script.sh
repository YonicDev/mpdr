#!/bin/bash
cd randomprime
cargo build --release
cd ../build
if [ "$TRAVIS_OS_NAME" = "osx" ]; then
    cmake .. -DCMAKE_BUILD_TYPE=MinSizeRel -DCMAKE_PREFIX_PATH=/usr/local
    make DESTDIR=./mpdr-qt.app install
    /usr/local/opt/qt/bin/macdeployqt mpdr-qt.app -dmg -fs=APFS
else
    cmake .. -DCMAKE_BUILD_TYPE=MinSizeRel -DCMAKE_C_COMPILER=gcc-7 -DCMAKE_CXX_COMPILER=g++-7 -DCMAKE_INSTALL_PREFIX=/usr -DCMAKE_INSTALL_LIBDIR=/usr/lib
    cmake --build . --target all -j $(nproc)
    make DESTDIR=appdir -j$(nproc) install ; find appdir/
    find $TRAVIS_BUILD_DIR \( -name "moc_*" -or -name "*.o" -or -name "qrc_*" -or -name "Makefile*" -or -name "*.a" \) -exec rm {} \
    wget -c -nv "https://github.com/probonopd/linuxdeployqt/releases/download/continuous/linuxdeployqt-continuous-x86_64.AppImage"
    chmod a+x linuxdeployqt-continuous-x86_64.AppImage
    ./linuxdeployqt-continuous-x86_64.AppImage appdir/usr/share/applications/*.desktop -appimage -verbose=2 -bundle-non-qt-libs -qmake=/opt/qt514/bin/qmake -extra-plugins=iconengines/libqsvgicon.so,platformthemes/libqgtk3.so
fi
