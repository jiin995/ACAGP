Example from: https://gist.github.com/mikesart/6832418

1) Download raw file and save as sse.cpp
2) Run: c++ -D_FILE_OFFSET_BITS=64 -D_LARGEFILE_SOURCE -D_LARGE_FILES -march=native -g -O0 -std=c++0x -g -o sse.cpp.o -c sse.cpp
3) Run: c++ -march=native -g -O0 -std=c++0x -g sse.cpp.o -o sse -rdynamic -ldl -lpthread
4) Run code: ./sse