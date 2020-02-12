name=TerraOS
src=$(git rev-parse --show-toplevel)

cmake $src --build $src/build
bash $src/scripts/iso.sh