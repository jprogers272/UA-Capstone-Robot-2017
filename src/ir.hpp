#ifndef IR_HPP
#define IR_HPP

class IR {
	//members
	private:
		int channelGPIO;
	//member functions
	public:
		IR(int);
		int getData(void);
};

#endif
