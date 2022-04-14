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
 

 
CHECKBAZEL="$(dpkg-query -W -f='${Status}' bazel)"
 
 
 
if [ "$CHECKBAZEL" = "$INSTALL" ]
then
   echo "BAZEL is intalled already\n"
else
   echo "Trying to install BAZEL\n"
  sudo apt install apt-transport-https curl gnupg
curl -fsSL https://bazel.build/bazel-release.pub.gpg | gpg --dearmor > bazel.gpg
sudo mv bazel.gpg /etc/apt/trusted.gpg.d/
echo "deb [arch=amd64] https://storage.googleapis.com/bazel-apt stable jdk1.8" | sudo tee /etc/apt/sources.list.d/bazel.list
 
sudo apt update && sudo apt install bazel
sudo apt update && sudo apt full-upgrade
fi
 
 
echo "IPC program required packages installed successfully"
 
chmod +x ./build_and_test.sh

