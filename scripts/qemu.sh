name=TerraOS
src=$(git rev-parse --show-toplevel)

qemu-system-x86_64 $src/bin/$name.iso -serial stdio
