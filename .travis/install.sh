#!/bin/bash
if [ "$TRAVIS_OS_NAME" = "osx" ]; then
    wget https://github.com/PostgresApp/PostgresApp/releases/download/v2.3.3e/Postgres-2.3.3e-9.4-9.5-9.6-10-11-12.dmg
    sudo hdiutil attach Postgres-2.3.3e-9.4-9.5-9.6-10-11-12.dmg
    sudo cp -R "/Volumes/Postgres-2.3.3e-9.4-9.5-9.6-10-11-12/Postgres.app" /Applications
    sudo hdiutil detach "/Volumes/Postgres-2.3.3e-9.4-9.5-9.6-10-11-12"
else
    sudo apt-get -y install qt514base qt514webengine qt514tools qt514svg qt514translations libgl1-mesa-dev
    source /opt/qt*/bin/qt*-env.sh
fi
