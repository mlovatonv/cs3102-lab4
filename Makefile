run: compile
	@./main

compile:
	@g++ main.cpp -o main

clean:
	@rm main
