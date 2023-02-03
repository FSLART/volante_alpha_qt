#include "store.h"



store::store(char a){
	this->a=a; 
	
}

int store::fetchmessage(){
	//read from serial port
	int file_descriptor= open(device, O_RDWR | O_NOCTTY | O_NDELAY);
	if (file_descriptor == -1)
	{
		perror("open_port: Unable to open /dev/ttyS0 - ");
	}
	else{
		read(file_descriptor, &a, 1);
		std::cout << "a: " << a << std::endl;
	}
	return close(file_descriptor);

}