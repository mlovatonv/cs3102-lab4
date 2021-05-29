run: compile
	@./main $(IMAGE_NAME)

compile:
	@g++ main.cpp -o main

clean:
	@rm main
