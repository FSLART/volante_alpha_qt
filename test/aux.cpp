
#include "aux.h"

AuxSingleton& AuxSingleton::getInstance() {
	static AuxSingleton instance;
    return instance;
}

int AuxSingleton::randomInt(int offset=0, int n=255){
	QRandomGenerator a = QRandomGenerator::securelySeeded();
	return a.bounded(n)+offset;
}