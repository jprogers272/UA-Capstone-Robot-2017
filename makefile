FLAGS = -Wall -c
ODIR = obj
SDIR = src

analog.o:	$(SDIR)/analog.cpp $(SDIR)/analog.hpp
	g++ $(SDIR)/analog.cpp -o $(ODIR)/analog.o $(FLAGS)
	
DCmotor.o:	$(SDIR)/DCmotor.cpp $(SDIR)/DCmotor.hpp $(SDIR)/gpio.hpp $(SDIR)/pwm.hpp
	g++ $(SDIR)/DCmotor.cpp -o $(ODIR)/DCmotor.o $(FLAGS)
	
gpio.o:		$(SDIR)/gpio.cpp $(SDIR)/gpio.hpp
	g++ $(SDIR)/gpio.cpp -o $(ODIR)/gpio.o $(FLAGS)
	
i2cbus.o:	$(SDIR)/i2cbus.cpp $(SDIR)/i2cbus.hpp
	g++ $(SDIR)/i2cbus.cpp -o $(ODIR)/i2cbus.o  $(FLAGS)
	
ir.o:		$(SDIR)/ir.cpp $(SDIR)/ir.hpp $(SDIR)/gpio.hpp
	g++ $(SDIR)/ir.cpp -o $(ODIR)/ir.o $(FLAGS)
	
pwm.o:		$(SDIR)/pwm.cpp $(SDIR)/pwm.hpp
	g++ $(SDIR)/pwm.cpp -o $(ODIR)/pwm.o $(FLAGS)
	
all_obj:	$(SDIR)/*
	make analog.o DCmotor.o gpio.o i2cbus.o ir.o pwm.o
	
clean:
	rm obj/*.o