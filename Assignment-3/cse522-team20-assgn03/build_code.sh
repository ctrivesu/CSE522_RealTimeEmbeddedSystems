#SD_CARD='/home/sushant/Desktop/SDCard'
SD_CARD='/media/sushant/S'
PROGRAM_LOCATION='/home/sushant/Desktop/zephyr_1.10/samples/measure_20'
ZEPHYR_BASE='/home/sushant/Desktop/zephyr_1.10'
#Setup Program Environment.
export ZEPHYR_TOOLCHAIN_VARIANT=zephyr
export ZEPHYR_SDK_INSTALL_DIR=/opt/zephyr-sdk/
cd $ZEPHYR_BASE
source zephyr-env.sh
#Build Your Code
cd $PROGRAM_LOCATION
rm -r build
mkdir build && cd build && mkdir galileo && cd galileo	
cmake -DBOARD=galileo ../.. 
make -Wall
cd $SD_CARD
mkdir efi
mkdir kernel
mkdir boot
cd $SD_CARD/efi
mkdir boot
cd $SD_CARD/boot
mkdir grub
cp $PROGRAM_LOCATION/build/galileo/zephyr/zephyr.strip $SD_CARD/kernel/zephyr.strip
cp $ZEPHYR_BASE/boards/x86/galileo/support/grub/bin/grub.efi $SD_CARD/efi/boot/grub.efi
cd $SD_CARD/efi/boot
echo -e "set default=0" > grub.cfg
echo -e "set timeout=10" >> grub.cfg
echo -e "menuentry \"Zephyr Kernel\" {" >> grub.cfg
echo "multiboot /kernel/zephyr.strip }" >> grub.cfg
cd $SD_CARD/boot/grub
echo -e "default=0" > grub.conf
echo -e "timeout=5" >> grub.conf
echo -e "title Zephyr" >> grub.conf
echo -e "rootnoverify (hd0,0)" >> grub.conf
echo "chainloader /efi/boot/grub.efi" >> grub.conf
