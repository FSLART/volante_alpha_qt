#ifndef STORE_H
#define STORE_H
#include <iostream>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <fcntl.h>
#include <errno.h>
#include <bits/types/FILE.h>
#include <cstdio>
class store
{
public:
	char a;
	char device[50]="/dev/ttyS0"; 
    store(char a);
	int fetchmessage();
};



#endif // STORE_H
