src=$1
name=TerraOS

mkdir -p isodir/boot/grub
cp $src isodir/boot/
echo -e "menuentry "$name" {\nmultiboot /boot/$name\n}" >> isodir/boot/grub/grub.cfg
grub-mkrescue -o $src.iso isodir
rm -rf isodir
