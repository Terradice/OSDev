name=TerraOS
src=$(git rev-parse --show-toplevel)

cd $src
if [ ! -d directory ]; then
    cmake -S . -B build
fi

cmake --build build
bash scripts/iso.sh
