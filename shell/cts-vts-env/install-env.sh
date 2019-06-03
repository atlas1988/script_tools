sudo apt-get update
sudo chmod 777 .
sudo apt-get -y install git
sudo apt-get -y install gnupg
sudo apt-get -y install flex
sudo apt-get -y install gperf
sudo apt-get -y install build-essential
sudo apt-get -y install curl
sudo apt-get -y install zlib1g-dev
sudo apt-get -y install libc6-dev
sudo apt-get -y install lib32ncurses5-dev
sudo apt-get -y install ia32-libs
sudo apt-get -y install x11proto-core-dev
sudo apt-get -y install libx11-dev
##lib32readline-gplv2-dev
sudo apt-get -y install lib32readline5-dev
sudo apt-get -y install lib32z-dev
sudo apt-get -y install libgl1-mesa-dev
sudo apt-get -y install g++-multilib
sudo apt-get -y install mingw32
sudo apt-get -y install tofrodos
sudo apt-get -y install bison
sudo apt-get -y install zip
sudo apt-get -y install libxml2-utils
sudo apt-get -y install git-core gitk git-gui ssh vim
sudo apt-get -y install cutecom
sudo apt-get -y install openjdk-7-jdk
sudo add-apt-repository ppa:openjdk-r/ppa
sudo apt-get update
sudo apt-get -y install openjdk-8-jdk
sudo apt-get -y install dos2unix
####vts
##install Python develpment kit
sudo apt-get -y install python-dev
##install Protocol buff tools(for python)
sudo apt-get -y install python-protobuf
sudo apt-get -y install protobuf-compiler
##install python virtual environment-related tools
sudo apt-get -y install python-virtualenv
sudo apt-get -y install python-pip
###adb
sudo chmod 777 51-android.rules
sudo cp 51-android.rules /etc/udev/rules.d/51-android.rules
unzip platform-tools-1-40.zip -d ./
sudo mv platform-tools /
mkdir temp
cp ~/.bashrc temp/
echo $PATH
sh -c 'echo "export PATH=/platform-tools:\$PATH" >> ~/.bashrc'
####repo
sudo chmod 777 repo
sudo cp repo /usr/local/bin/


