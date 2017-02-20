username=$1
cd /home/$username

#install opencv  for crosscompilation in workstation
cd opencvCrossBBB
cd opencv/platforms/linux/
mkdir -p build_hardfp
cd build_hardfp
cmake -DCMAKE_TOOLCHAIN_FILE=../arm-gnueabi.toolchain.cmake ../../..
make install 



