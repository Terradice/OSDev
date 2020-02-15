name=TerraOS
src=$(git rev-parse --show-toplevel)

tar czf $src/bin/$name.tar.gz --directory=$src/bin $name.iso
