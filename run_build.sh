g++-10 src/main.cpp -I./include -std=c++20 -o build/main.run -DDEBUG_BUILD
g++-10 test/test.cpp -I./include -L/usr/local/lib/googletest -lgtest -pthread -std=c++20 -o build/test.run -DDEBUG_BUILD
