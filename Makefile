run: compile
	@./main $(IMAGE_NAME)

compile:
	@g++ -DDBG_MACRO_NO_WARNING main.cpp -o main

clean:
	@rm main
