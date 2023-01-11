GCC= C:\MinGW\bin\gcc
SRC= controller
TEST= controller_test


CUnitAutomated-Results.xml : $(TEST).exe
	./$(TEST).exe
	rm -f *.o *.exe

$(SRC).o : $(SRC).c
	$(GCC) -Wall -c $(SRC).c -o $(SRC).o

$(TEST).exe : $(TEST).c $(SRC).o 
	$(GCC) -Wall -LC:/CUnit-2.1-3/CUnit/Sources/.libs -IC:/CUnit-2.1-3/CUnit/Headers -o $(TEST) $(TEST).c $(SRC).o -lcunit

clean :
	rm -f *.o *.exe *.xml