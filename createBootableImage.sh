#remove existing floppy (seems to bug if you don't)
rm kernel.flp
rm kernel.iso
rm cdcontent/*

#build primary bootloader
cd bootloader1
sh build.sh
cd ..

#build secondary bootloader
cd bootloader2
sh build.sh
cd ..

#build kernel
#NOT HERE YET
#cd kernel
#sh build.sh
#cd ..

#create new floppy
dd status=noxfer conv=notrunc if=bootloader1/bin/bootloader1.bin of=kernel.flp
dd status=noxfer conv=notrunc if=bootloader2/bin/bootloader2.bin of=kernel.flp seek=1
dd status=noxfer conv=notrunc if=kernel/bin/kernel.bin of=kernel.flp seek=4
cp kernel.flp cdcontent

mkisofs -o kernel.iso -V CynkkerOS -b kernel.flp  cdcontent/