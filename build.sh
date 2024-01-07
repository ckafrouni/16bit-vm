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
    echo -e "\033[0;31mbuild.sh: Build the project...\033[0m"

    mkdir -p $build_dir
    cd $build_dir
    cmake ..
    # cmake -DCMAKE_VERBOSE_MAKEFILE:BOOL=ON ..
    if [ $? -ne 0 ]; then
        code="$?"
        echo -e "\033[0;31mbuild.sh: CMake failed with code $code\033[0m"
        exit 1
    fi
    make -j
    # exit with the same code as make
    code="$?"
    if [ $code -ne 0 ]; then
        echo -e "\033[0;31mbuild.sh: Make failed with code $code\033[0m"
        exit $code
    fi
    exit $code
fi

if [ "$1" == "clean" ]; then
    echo -e "\033[0;31mbuild.sh: Clean the build directory...\033[0m"
    rm -rf $build_dir
    exit 0
fi

if [ "$1" == "run" ]; then
    shift 1 # remove the first argument

    ./build.sh build

    echo -e "\033[0;31mbuild.sh: Run the project...\033[0m"
    echo -e "\033[0;31mbuild.sh: Arguments: ( $@ )\033[0m"

    ./$build_dir/$EXECUTABLE "$@"
    exit 0
fi

if [ "$1" == "clean-run" ]; then
    shift 1 # remove the first argument

    ./build.sh clean
    clear
    ./build.sh run "$@"
    exit 0
fi

echo -e "\033[0;31mbuild.sh: Invalid mode: $mode\033[0m"
