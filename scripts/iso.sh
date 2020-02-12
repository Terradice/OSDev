name=TerraOS
src=$(git rev-parse --show-toplevel)

mkdir -p isodir/boot/grub
cp $src/bin/$name isodir/boot/
echo -e "menuentry "$name" {\nmultiboot /boot/$name\n}" >> isodir/boot/grub/grub.cfg
grub-mkrescue -o $src/bin/$name.iso isodir
rm -rf isodir
