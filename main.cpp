#include <iostream>
#include "cpp.h"
#include <string.h>

using namespace std;


facility *barber;

void Generate(); // Another process
void Customer(); // Another process

extern "C" void sim() // Alice is the main process
{
	barber = new facility("Joe the Barber");
	
	create("sim"); 
	Generate(); 
	
	hold(480);
}
void Generate()
{
	create("Generate");
	
	while(1){
		Customer();
		hold(uniform(12, 24));
	}
}

void Customer()
{
	create("Customer");
	barber->use(uniform(12, 20));
	
}
