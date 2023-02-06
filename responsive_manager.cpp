#include "responsive_manager.h"

responsive_manager::responsive_manager(){
	//allocate list of hooks
	this->hooks = std::list<managed_hook*>();

}

responsive_manager::~responsive_manager(){
	//free list of hooks
	this->hooks.clear();
}
void responsive_manager::remove_hook(managed_hook* hook){
	this->hooks.remove(hook);
}
void responsive_manager::add_hook(managed_hook* hook){
	//remove all hooks with the same address(prevents duplicates, i didnt itterate to keep the code shorter)
	this->remove_hook(hook);
	this->hooks.push_back(hook);
}
void responsive_manager::on_resize(){
	//call on_resize for all hooks
	for (managed_hook* hook : this->hooks){
		hook->on_resize();
	}
	

}