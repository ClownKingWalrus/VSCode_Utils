#!/bin/bash
# Compiler yea its g++ get over it
CXX=g++

# All flags included (this is for errors and linking when it compiles) -w disables loud errors from -Wall and -Wextra
CXXFLAGS="-Wall -Wextra -w -std=c++17 -I ./hdr -I ./src -L./lib -I ./Include"

# This is * all cpp files its called a wildcard character
SRCPP=./src/*.cpp
SRC=./src/*.c

# Link Libs
LIBS="-lglfw3 -lgdi32 -lopengl32 -luser32 -lshell32 -lstdc++fs"

# This is what the binarary it produces gets outputted
OUT=./bin/my_program

#ensure the bin for the binarary exist IE the program we make
mkdir -p ./bin

# attempt to comple
echo "Attempting Build..."

# Arguments goes as follows btw this is a litteraly command in the terminal line as shown below (bin\bash, Compiler, Flags, ObjFiles, -o ProgramFileOutput)
# "g++ -Wall -Wextra -std=c++17 -I./hdr -I./src ./src/*.cpp -o ./bin/my_program" 
$CXX $CXXFLAGS $SRCPP $SRC $LIBS -o $OUT

# if it completes then this will echo
echo "Build complete: $OUT"
