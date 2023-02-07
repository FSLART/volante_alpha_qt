#ifndef RESP_LABEL_H
#define RESP_LABEL_H
#include "responsive_manager.h"
#include <QLabel>
#include <QCoreApplication>
#include <qsize.h>
class resp_label : public managed_hook  {
    public:
		QSize dirtySize;
		bool isVertical=false;
        resp_label(bool isVertical=false, QSize size=QSize(0,0));

        void on_resize(QSize t) override;
        QLabel* ref=nullptr;
	
};

#endif // RESP_LABEL_H
