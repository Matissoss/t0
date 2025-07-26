default:

build:
	@cc src/*.c -o target/t0
	@chmod +x target/t0
run: build
	@./target/t0
