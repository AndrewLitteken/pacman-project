CMP = g++
CLASS2= ghost
CLASS3 = pacman
CLASS4 = board
CLASS = game
MAIN = gamemain
EXEC = game
FLAGS= -std=c++11

$(EXEC): $(CLASS).o $(CLASS2).o $(CLASS3).o $(CLASS4).o $(MAIN).o
	$(CMP) $(FLAGS) $(CLASS).o $(CLASS2).o $(CLASS3).o $(CLASS4).o gfxnew_mac.o -lX11 -I/opt/X11/include/ -L/opt/X11/lib/ $(MAIN).o -o $(EXEC)

$(CLASS).o: $(CLASS).cpp $(CLASS).h
	$(CMP) $(FLAGS) -c $(CLASS).cpp -I/opt/X11/include/ -o $(CLASS).o

$(CLASS2).o: $(CLASS2).cpp $(CLASS2).h
	$(CMP) $(FLAGS) -c $(CLASS2).cpp -I/opt/X11/include/ -o $(CLASS2).o

$(CLASS3).o: $(CLASS3).cpp $(CLASS3).h
	$(CMP) $(FLAGS) -c $(CLASS3).cpp -I/opt/X11/include/ -o $(CLASS3).o

$(CLASS4).o: $(CLASS4).cpp $(CLASS4).h
	$(CMP) $(FLAGS) -c $(CLASS4).cpp -I/opt/X11/include/ -o $(CLASS4).o

$(MAIN).o: $(MAIN).cpp $(CLASS).h
	$(CMP) $(FLAGS) -c $(MAIN).cpp -I/opt/X11/include/ -o $(MAIN).o

clean:
	rm $(CLASS4).o
	rm $(CLASS3).o
	rm $(CLASS2).o
	rm $(CLASS).o
	rm $(MAIN).o
	rm $(EXEC)
