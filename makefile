G = gcc -g
O = -o
D = -lglut -lGLU -lGL -lm
names = main sellers chefs bakers supply_chain customer drawer

files:
	$(G) main.c $(O) main
	$(G) sellers.c $(O) sellers
	$(G) chefs.c $(O) chefs
	$(G) bakers.c $(O) bakers
	$(G) supply_chain.c $(O) supply_chain
	$(G) customer.c $(O) customer
	$(G) drawer.c $(O) drawer $(D)

clean:
	rm -f $(names)

run:
	./main