#ifndef I2C_ROBOT_HPP
#define I2C_ROBOT_HPP

class I2Cbus {
	//members
	private:
		int file_i2c;
		int bus;
		int device_address;
		
	//member functions
	public:
		I2Cbus(int);
		~I2Cbus(void);
		int openFile(void);
		int setDevice(int);
		unsigned char readRegisterSingle(unsigned int);
		unsigned char *readRegisters(unsigned int,unsigned int);
		int setRegisterAddress(unsigned int);
		int writeRegister(unsigned int,unsigned char);
};

#endif
