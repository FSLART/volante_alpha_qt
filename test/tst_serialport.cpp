#include "tst_serialport.h"
#include <cstdio>
#include <iostream>
#include <regex>
#include <sys/types.h>
#include <signal.h>
#include <unistd.h>
#include <fstream>

using namespace std;
void Tst_serialport::sanityCheck(){
	
	 pid= fork();
	
	if(pid == 0){
        launchSocat();
		
	}else if(pid < 0){
		QFAIL("Failed to fork");
    }else{
		// read /tmp/socatVolante and regex /dev/pts/[0-9]+ the first match equals banana
		// the second match equals tango
		//wait 2 secs
		sleep(2);
		ifstream fp;
        //TODO CAN BE HEAVILY OPTIMIZED BUT I JUST SPENT LIKE AN ENTIRE DAY DUE TO FUCKERY THEY DID WITH C++
		std::string s;
		fp.open("/tmp/socatVolante");

		std::getline(fp, s);

        std::regex r(".*(\\/dev\\/pts\\/\\d+)(.*).*");
		std::smatch m;
		if(std::regex_search(s, m, r)){
            banana = QString::fromStdString(m[1]);
			
		}	
		getline(fp, s);
		if(std::regex_search(s, m, r)){
			tango = QString::fromStdString(m[1]);
		}
		fp.close();
		//convert banana QString to char*
		QByteArray ba = banana.toLocal8Bit();
		char *cstr = ba.data();

        store * _store = new store(cstr);

        QCOMPARE(_store->dev, cstr);

	}
}
void Tst_serialport::launchSocat(){

    char socatCommand[]= "socat -d -d pty,raw,echo=0,b115200 pty,raw,echo=0,b115200 2> /tmp/socatVolante";
	system(socatCommand);

	
}

void Tst_serialport::closeHandle(){
	//close pid
    kill(pid, SIGKILL);

}
QTEST_MAIN(Tst_serialport)
