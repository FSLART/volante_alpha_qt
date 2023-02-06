#ifndef RESPONSIVE_MANAGER_H
#define RESPONSIVE_MANAGER_H
#include <list>
class managed_hook{
	public:
		virtual void on_resize();
		managed_hook();
		~managed_hook();
};

class responsive_manager{
public:	
	void add_hook(managed_hook* hook);
	void remove_hook(managed_hook* hook);
	void on_resize();
	
    responsive_manager();
	~responsive_manager();
protected:
	std::list<managed_hook*> hooks;
};



#endif // RESPONSIVE_MANAGER_H
