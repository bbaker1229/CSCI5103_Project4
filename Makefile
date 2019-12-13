all:
	(cd Buffer; make)
	(cd test; make)

clean:
	(cd Buffer; make clean)
	(cd test; make clean)
