#include "resp_label.h"
#include <qsize.h>

resp_label::resp_label(bool isVertical, QSize size){
	this->dirtySize = size;
	this->isVertical = isVertical;

    //on_resize();
}

void resp_label::on_resize(QSize size){
	float ratio=0.0f;
	if(this->isVertical){
		ratio = (float)size.height()/(float)this->dirtySize.height();
	}else{
		ratio = (float)size.width()/(float)this->dirtySize.width();
	}
	
	//set new font size
    QFont font = this->ref->font();
	
	font.setPointSizeF((qreal)font.pointSizeF()*ratio);
	//make shure the font does not go over bounding box
	//undo if it goes over
	

	this->ref->setFont(font);
}

