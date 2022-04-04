#!/usr/bin/sh
[ "$(whoami)" != "root" ] && exec sudo -- "$0" "$@"
set -x



INSTALL="install ok installed"

CHECKGNU="$(dpkg-query -W -f='${Status}' g++)"

if [ "$CHECKGNU" = "$INSTALL" ]
then
   echo "G++ is intalled already\n"
else
   echo "Trying to install G++\n"
   sudo apt install g++
fi

CHECKGIT="$(dpkg-query -W -f='${Status}' git)"


if [ "$CHECKGIT" = "$INSTALL" ]
then
   echo "GIT is intalled already\n"
else
   echo "Trying to install GIT\n"
   sudo apt-get update
   sudo apt-get install git
fi



CHECKNODEJS="$(dpkg-query -W -f='${Status}' nodejs)"



if [ "$CHECKNODEJS" = "$INSTALL" ]
then
   echo "NODEJS is intalled already\n"
else
   echo "Trying to install NODEJS\n"
   curl -sL https://deb.nodesource.com/setup_14.x | sudo -E bash -
   sudo apt update
   sudo apt install nodejs
fi

CHECKBAZEL="$(dpkg-query -W -f='${Status}' bazel)"



if [ "$CHECKBAZEL" = "$INSTALL" ]
then
   echo "BAZEL is intalled already\n"
else
   echo "Trying to install BAZEL\n"
   npm install -g @bazel/bazelisk
   npm install --save-dev @bazel/ibazel
fi


echo "IPC program required packages installed successfully"

chmod +x ./build_and_test.sh 