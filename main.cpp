/*this CSIM program simulates an M/M/1 service center*/



#include <cpp.h> /*include the CSIM C++ header file*/
facility *f; /*the service center*/






extern “C” void sim() /*sim process*/
{
 create("sim"); /*make this a process*/
 f = new facility("f"); /*create the service center - f*/
 while(simtime() < 5000.0) { /*loop until end of simulation*/
 hold(exponential(1.0)); /*delay between customer arrivals*/
 customer(); /*generate new customer*/
 }
 report(); /*produce statistics report*/
}




void customer()
{
 create("customer"); /*make this a process*/
 f->use(exponential(0.5)); /*obtain needed amount of service*/
}
