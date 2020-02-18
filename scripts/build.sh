name=TerraOS
src=$(git rev-parse --show-toplevel)

cd $src
cmake -S . -B build -DCMAKE_BUILD_TYPE=$1
cmake --build build -j4
bash scripts/iso.sh $src/bin/$1/$name

# if [ -n "$1" ]; then
#   	cmake --build build -j4 --config $1
#   	bash scripts/iso.sh $src/bin/$1/$name
# else
# 	cmake --build build -j4 --config Release
# 	bash scripts/iso.sh $src/bin/Release/$name
# fi