build:
	echo "Building Link,all errors are place inside error.txt!"
	g++ -std=c++17 src/*.cpp -I./include -lraylib -lGL -lm -lpthread -ldl -lrt -lX11 -o linklang 2> error.txt

