
#sudo apt-get install libboost-date-time-dev \
#  libboost-thread-dev nvidia-cg-toolkit \
#  zlib1g-dev libcppunit-dev doxygen \
#  libxt-dev libxaw7-dev libxxf86vm-dev libxrandr-dev libglu-dev

#sudo apt-get install libxt-dev libxaw7-dev

#rm -rf build

# Configure with CMake
#mkdir -p build
cd build

#cmake -DCMAKE_CXX_FLAGS="-Wl,--no-undefined" -DCMAKE_CXX_COMPILER=/usr/bin/clang++ -DCMAKE_C_COMPILER=/usr/bin/clang -G "CodeBlocks - Unix Makefiles" ..

echo Done!

make

rm -rf /home/jetdogs/Projects/TheoraPluginCMake-unity/linux-build/test_Data/Plugins/x86/libvideonator.so
cp libTHEORA_PLUGIN.so /home/jetdogs/Projects/TheoraPluginCMake-unity/linux-build/test_Data/Plugins/x86/libvideonator.so

cd /home/jetdogs/Projects/TheoraPluginCMake-unity/linux-build/
./test.x86

cat /home/jetdogs/.config/unity3d/DefaultCompany/VideoTest/Player.log