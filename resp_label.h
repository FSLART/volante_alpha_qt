#ifndef RESP_LABEL_H
#define RESP_LABEL_H
#include "responsive_manager.h"

class resp_label : public managed_hook {
	public:
		bool isVertical=false;
		resp_label(bool isVertical);
		void onResize(); 
	
};

#endif // RESP_LABEL_H
