username=$1
cd /home/$username

#copy sources.list
echo 'Copying sources list to /etc/apt/sources.list'
cat /home/$username/install_pc/sources.list > /etc/apt/sources.list
echo 'first update attempt'
apt-get -qq -y update
echo 'install multimedia keyring'
apt-get -qq --force-yes install deb-multimedia-keyring
echo 'second update attempt'
apt-get -qq -y update

#install i3
echo 'install i3'
apt-get -qq -y install i3

#install chromiu
echo  'install chromium'
apt-get install chromium

#install linux headers
echo 'install linux headers'
aptitude -r install linux-headers-$(uname -r)

#install video 4 linux
echo 'install v4l'
apt-get -qq -y install v4l-utils

#install joystick
echo 'install joystick'
apt-get -qq -y install joystick

#cross compilación
echo 'deb http://emdebian.org/tools/debian jessie main' > /etc/apt/sources.list.d/crosstools.list
apt-get -qq -y install curl
curl http://emdebian.org/tools/debian/emdebian-toolchain-archive.key | apt-key-add -
dpkg --add-architecture armhf
dpkg --print-architecture
dpkg --print-foreign-architectures
apt-get -qq -y update
apt-get -qq --force-yes install crossbuild-essential-armhf

#qemu emulator
apt-get -qq -y install qemu-user-static

#install opencv  for workstation
#apt-get -qq -y install build-essential
#apt-get -qq -y install cmake git libgtk2.0-dev pkg-config libavcodec-dev libavformat-dev libswscale-dev
#apt-get -qq -y install python-dev python-numpy libtbb2 libtbb-dev libjpeg-dev libpng-dev libtiff-dev libjasper-dev libdc1394-22-dev
#mkdir opencvPC
#mkdir opencvCrossBBB
#cd opencvPC
#git clone https://github.com/Itseez/opencv.git
#cp opencv/ ../opencvCrossBBB/opencv
#cd opencv
#mkdir release
#cd release
#cmake -D CMAKE_BUILD_TYPE=RELEASE -D CMAKE_INSTALL_PREFIX=/usr/local ..
#make install

#install opencv for crosscompilation
#cd ../../../opencvCrossBBB

