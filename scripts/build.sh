name=TerraOS
src=$(git rev-parse --show-toplevel)

cd $src
if [ ! -d directory ]; then
    cmake -S . -B build
fi

if [ -n "$1" ]; then
	cmake --build build $@
else
	cmake --build build -j4
fi

bash scripts/iso.sh
