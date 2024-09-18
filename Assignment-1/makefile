src = q4.c

out = q4

all = $(out)

$(out) : $(src)
	@gcc $(src) -o $(out)

run: $(out)
	@echo "4\nP1\n0\n5\nP2\n2\n3\nP3\n1\n8\nP4\n3\n6\n2" |4./$(out)

clean:
	@rm -f $(out)

q1:
	@gcc q1.c -o q1
	@./q1

q2:
	@gcc q2.c -o q2
	@./q2