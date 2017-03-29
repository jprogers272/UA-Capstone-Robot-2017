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
		int testGPIOs[6];		
	//member functions
	public:
		Stage1(void);
		void energizeComponent(void);
		void deEnergizeComponent(void);
		void energizeCommon(void);
		void identifyComponent(void);
		void checkCapacitorDiode(int);
		int detectProblems(void);
		void zeroComponentArray(void);
		
	private:
		int detectShort(int,int);
};

#endif
