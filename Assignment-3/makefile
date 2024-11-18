all : q1 q2 q3 q4 

q1: 
	@gcc q1.c -lpthread -o q1.out
q2:
	@gcc q2.c -lpthread -o q2.out
q3:
	@gcc q3.c -lpthread -o q3.out
q4:
	@gcc q4.c -lpthread -o q4.out

run_q1:
	@./q1.out
run_q2:
	@./q2.out
run_q3:
	@./q3.out
run_q4:
	@./q4.out
	
	  	
clean:
	  @echo "Cleaning up executables..."
	  @rm *.out
	  @echo "Done."
