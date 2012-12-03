sudo mount /dev/mapper/loop0p1/ /media/bootfs
sudo cp kernel/kernel.bin /media/bootfs
sudo cp tasks/bin/*.bin /media/bootfs
sudo umount /media/bootfs
