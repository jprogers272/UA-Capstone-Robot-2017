FLAGS = -Wall -c
ODIR = obj
SDIR = src
OBJ = $(ODIR)/analog.o $(ODIR)/compass.o $(ODIR)/DCmotor.o $(ODIR)/gpio.o $(ODIR)/i2cbus.o $(ODIR)/imu.o $(ODIR)/ir.o $(ODIR)/pwm.o
BIN_DEP = analog.o compass.o DCmotor.o gpio.o i2cbus.o imu.o ir.o pwm.o
HEADERS = $(SDIR)/analog.hpp $(SDIR)/compass.hpp $(SDIR)/DCmotor.hpp $(SDIR)/gpio.hpp $(SDIR)/i2cbus.hpp $(SDIR)/imu.hpp $(SDIR)/ir.hpp $(SDIR)/pwm.hpp
	
example: example.o $(BIN_DEP)
	g++ -Wall -g $(ODIR)/example.o $(OBJ) -o bin/example 

example.o: $(SRC_ALL)
	g++ $(SDIR)/example.cpp -o $(ODIR)/example.o $(FLAGS)

analog.o:	$(SDIR)/analog.cpp $(HEADERS)
	g++ $(SDIR)/analog.cpp -o $(ODIR)/analog.o $(FLAGS)
	
compass.o:	$(SDIR)/compass.cpp $(HEADERS)
	g++ $(SDIR)/compass.cpp -o $(ODIR)/compass.o $(FLAGS)

DCmotor.o:	$(SDIR)/DCmotor.cpp $(HEADERS)
	g++ $(SDIR)/DCmotor.cpp -o $(ODIR)/DCmotor.o $(FLAGS)
	
gpio.o:		$(SDIR)/gpio.cpp $(HEADERS)
	g++ $(SDIR)/gpio.cpp -o $(ODIR)/gpio.o $(FLAGS)
	
i2cbus.o:	$(SDIR)/i2cbus.cpp $(HEADERS)
	g++ $(SDIR)/i2cbus.cpp -o $(ODIR)/i2cbus.o  $(FLAGS)
	
imu.o:		$(SDIR)/imu.cpp $(HEADERS)
	g++ $(SDIR)/imu.cpp -o $(ODIR)/imu.o $(FLAGS)
	
ir.o:		$(SDIR)/ir.cpp $(HEADERS)
	g++ $(SDIR)/ir.cpp -o $(ODIR)/ir.o $(FLAGS)
	
pwm.o:		$(SDIR)/pwm.cpp $(HEADERS)
	g++ $(SDIR)/pwm.cpp -o $(ODIR)/pwm.o $(FLAGS)
	
all_obj:	$(SDIR)/*
	make example.o analog.o compass.o DCmotor.o gpio.o i2cbus.o imu.o ir.o pwm.o
	
clean:
	rm obj/*.o
