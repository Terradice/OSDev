name=TerraOS
src=$(git rev-parse --show-toplevel)

qemu-system-x86_64 -cdrom $src/bin/$name.iso -serial stdio -m 512M
