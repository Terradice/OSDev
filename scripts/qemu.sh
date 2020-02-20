name=TerraOS
src=$(git rev-parse --show-toplevel)

qemu-system-x86_64 -cdrom $src/bin/$1/$name.iso -serial stdio -m 4G -soundhw pcspk
