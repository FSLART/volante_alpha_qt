#include "resp_label.h"

resp_label::resp_label(bool isVertical){
	this->isVertical = isVertical;
	onResize();
}

void resp_label::onResize(){
	float ratio=0.0f;
	// TOOPTIMIZE: im kinda lazy, this is not the best way of doing it. It will fail miserably if theres a layout with more than one widget
	if(!isVertical){
		//get maximum width available
		int max_width = this->parentWidget()->width();
		//get current width
		int current_width = this->width();
		ratio = (float)max_width/(float)current_width;

	}else{
		int max_height = this->parentWidget()->height();
		int current_height = this->height();
		ratio = (float)max_height/(float)current_height;

	}
	//set new font size
	QFont font = this->font();
	font.setPointSizeF((qreal)font.pointSizeF()*ratio);
	this->setFont(font);	
}

