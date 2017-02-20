username=$1
cd /home/$username

#install opencv  for workstation
apt-get -qq -y install build-essential
apt-get -qq -y install cmake git libgtk2.0-dev pkg-config libavcodec-dev libavformat-dev libswscale-dev
apt-get -qq -y install python-dev python-numpy libtbb2 libtbb-dev libjpeg-dev libpng-dev libtiff-dev libjasper-dev libdc1394-22-dev
mkdir opencvPC
mkdir opencvCrossBBB
cd opencvPC
git clone https://github.com/Itseez/opencv.git
cp opencv/ ../opencvCrossBBB/opencv
cd opencv
mkdir release
cd release
cmake -D CMAKE_BUILD_TYPE=RELEASE -D CMAKE_INSTALL_PREFIX=/usr/local ..
make install

#install opencv for crosscompilation
#cd ../../../opencvCrossBBB

