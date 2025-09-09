default:
	echo "Specify platform (windows/linux)"

linux:
	gcc -o main main.c

windows:
	gcc -o main.exe main.c

quicklinux:
	gcc -o main main.c
	./main compile helloworld.bf
	gcc -o helloworld helloworld.c
	./helloworld

quickwindows:
	gcc -o main.exe main.c
	./main.exe compile helloworld.bf
	gcc -o helloworld.exe helloworld.c
	./helloworld.exe