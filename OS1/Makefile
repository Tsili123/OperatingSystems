all: main1 main2
CC = g++
source = source/input_handling/input_handling.cpp source/shared_memory_operators/shared_memory_operators.cpp source/semaphore_operators/semaphore_operators.cpp
args = -lcrypto -o 

main1:
	$(CC) main1.cpp $(source) $(args) main1
main2:
	$(CC) main2.cpp $(source) $(args) main2

clean:
	if [ -f main2 ]; then rm main2; fi;
	if [ -f main1 ]; then rm main1; fi;