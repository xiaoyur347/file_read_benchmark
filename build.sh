SOURCE="src/util/file.cpp src/util/limit_bench.cpp src/main.cpp src/single_thread_read_test.cpp src/multi_thread_read_test.cpp src/limit_read_test.cpp"
g++ -m32 -g -O0 -std=c++0x ${SOURCE} -lpthread -o file_read_benchmark
cp file_read_benchmark /opt/wwwroot/