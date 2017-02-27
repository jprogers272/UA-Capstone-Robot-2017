//Author(s): John Rogers

#ifndef STAGE1_HPP
#define STAGE1_HPP

#include "robotDefinitions.hpp"

class Stage1 {
	//members:
	public:
		int currentComponent;
		int components[5];
		
	private:
		const int testGPIOs[6] = {
			STAGE1_GPIO1,
			STAGE1_GPIO2,
			STAGE1_GPIO3,
			STAGE1_GPIO4,
			STAGE1_GPIO5,
			STAGE1_GPIO_COM};
		
	//member functions
	public:
		Stage1(void);
		void energizeComponent(void);
		void energizeCommon(void);
		void identifyComponent(void);
		void checkCapacitorDiode(void);
		int detectProblems(void);
		
	private:
		int detectShort(int,int);
};

#endif