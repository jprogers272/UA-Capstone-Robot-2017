FLAGS = -Wall -c
ODIR = obj
SDIR = src

analog.o:	$(SDIR)/analog.cpp $(SDIR)/analog.hpp
	g++ $(SDIR)/analog.cpp -o $(ODIR)/analog.o $(FLAGS)
DCmotor.o:	$(SDIR)/DCmotor.cpp $(SDIR)/DCmotor.hpp
	g++ $(SDIR)/DCmotor.cpp -o $(ODIR)/DCmotor.o $(FLAGS)
gpio.o:		$(SDIR)/gpio.cpp $(SDIR)/gpio.hpp
	g++ $(SDIR)/gpio.cpp -o $(ODIR)/gpio.o $(FLAGS)
pwm.o:		$(SDIR)/pwm.cpp $(SDIR)/pwm.hpp
	g++ $(SDIR)/pwm.cpp -o $(ODIR)/pwm.o $(FLAGS)
clean:
	rm obj/*.o