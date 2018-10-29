if [ ! -d "build" ]; then
  mkdir bin
fi
cd bin
cmake ..
make
echo build complete on ./bin/src/WebRadar!
echo run with ./run