all: generate run

generate:
		@gcc cal.c -o cal
		@gcc date.c -o date
		@gcc uptime.c -o uptime

run:
	gcc main.c -o main 
	./main

clean:
	@rm -f cal,date,time,main