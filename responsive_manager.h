#ifndef RESPONSIVE_MANAGER_H
#define RESPONSIVE_MANAGER_H
#include <list>
#include <QCoreApplication>
#include <qobject.h>
class managed_hook {
	public:
        virtual void on_resize(QSize size)=0;
};

class responsive_manager{
public:	
	void add_hook(managed_hook* hook);
	void remove_hook(managed_hook* hook);
	void on_resize(QSize size);
	
    responsive_manager();
	~responsive_manager();
protected:
	std::list<managed_hook*> hooks;
};



#endif // RESPONSIVE_MANAGER_H
