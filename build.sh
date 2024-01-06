# ./build.sh [MODE] [ARGUMENTS]
#
# Modes:
#   build: (default) Builds the project
#   clean: Removes the build directory
#   run: Runs the project

mode=$1

EXECUTABLE="vm"

[ -z "$mode" ] && mode="build"

build_dir="build"

if [ "$mode" == "build" ]; then
    echo -e "\033[0;31mBuild the project...\033[0m"7

    mkdir -p $build_dir
    cd $build_dir
    cmake -DCMAKE_VERBOSE_MAKEFILE:BOOL=ON ..
    make -j
    cd ..
    exit 0
fi

if [ "$1" == "clean" ]; then
    echo -e "\033[0;31mClean the build directory...\033[0m"
    rm -rf $build_dir
    exit 0
fi

if [ "$1" == "run" ]; then
    shift 1 # remove the first argument

    ./build.sh build

    echo -e "\033[0;31mRun the project...\033[0m"
    echo -e "\033[0;31mArguments: $@\033[0m"

    ./$build_dir/$EXECUTABLE "$@"
    exit 0
fi

echo -e "\033[0;31mInvalid mode: $mode\033[0m"
