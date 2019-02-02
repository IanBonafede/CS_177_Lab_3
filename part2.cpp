#include <iostream>
#include "cpp.h"
#include <string.h>
#include <math.h>       /* log */

using namespace std;

const double profit = .025; // dollars per litre
const double cost = 20;
const int MEAN = 50;

facility_ms *pumps;

void Generate(); // Another process
void Car(); // Another process




//---------------------------------------------------------------------------------
// inter arrival time
//---------------------------------------------------------------------------------


double interarrivalTime ()
// The interarrival time of the next car is exponentially distributed.
{
	
	// seconds, because r is a rate in 1/sec, and this is 1/r
	return - MEAN * log (uniform(0, 1));
}


//---------------------------------------------------------------------------------





//---------------------------------------------------------------------------------
// balking
//---------------------------------------------------------------------------------



bool DoesCarBalk (double litres, int queueLength)
// Deciding whether to balk is an activity that forms part of the
// arrival event, so this procedure belongs among the event routines.
//
// The probability that a car leaves without buying gas (i.e., balks)
// grows larger as the queue length gets larger, and grows smaller
// when the car requries a greater number of litres of gas, such that:
// (1) there is no balking if the queueLength is zero, and
// (2) otherwise, the probability of *not* balking is
//       (40 + litres)/(25 * (3 + queueLength))
{
	return queueLength > 0 && uniform(0, 1)
		> (40 + litres) / (25 * (3 + queueLength));
}
//---------------------------------------------------------------------------------










//---------------------------------------------------------------------------------
// service time
//---------------------------------------------------------------------------------

double serviceTime(double litresNeeded)
// Determine how long the service will take. This is a property of the
// pump-car combination.
//
// Service times will have a near Normal distribution, where the mean is 150
// seconds plus 1/2 second per litre, and the standard deviation is 30 seconds.
// (It can be shown that the sum of 12 random numbers from the
// uniform distribution on (0,1) has a near Normal distribution
// with mean 6 and standard deviation 1.)
{

	double howLong = -6;

	for (int i = 1; i <= 12; i++)
		howLong += uniform(0, 1);

	return 150 + 0.5 * litresNeeded + 30 * howLong;
}

//---------------------------------------------------------------------------------







//---------------------------------------------------------------------------------
// Collecting and displaying statistics
//---------------------------------------------------------------------------------


class statsClass {
private:
	int TotalArrivals, customersServed, balkingCustomers;
	double TotalLitresSold, TotalLitresMissed, TotalWaitingTime,
		TotalServiceTime;

public:
	statsClass ();
	void countArrival () {TotalArrivals += 1;};
	void accumSale (double litres);
	void accumBalk (double litres);
	void accumWaitingTime (double interval) {TotalWaitingTime += interval;};
	void accumServiceTime (double interval) {TotalServiceTime += interval;};
	void snapshot ();
};

statsClass * stats;

statsClass::statsClass ()
{
	TotalArrivals = 0;
	customersServed = 0;
	balkingCustomers = 0;
	TotalLitresSold = 0.0;
	TotalLitresMissed = 0.0;
	TotalWaitingTime = 0.0;
	TotalServiceTime = 0.0;
}

void statsClass::accumSale (double litres)
{
	customersServed += 1;
	TotalLitresSold += litres;
}

void statsClass::accumBalk (double litres)
{
	balkingCustomers += 1;
	TotalLitresMissed += litres;
}

void statsClass::snapshot ()
{
	/*printf("%8.0f%7i", simulationTime, TotalArrivals);
	printf("%8.3f", carQueue -> emptyTime()/simulationTime);
	if (TotalArrivals > 0) {
		printf("%9.3f%8.3f", simulationTime/TotalArrivals,
			(TotalLitresSold + TotalLitresMissed) / TotalArrivals);
	}
	else
		printf ("%9s%8s", "Unknown", "Unknown");
	*/
	printf ("%7i", balkingCustomers);
	/*if (customersServed > 0)
		printf ("%9.3f", TotalWaitingTime / customersServed);
	else
		printf ("%9s", "Unknown");
	printf ("%7.3f", TotalServiceTime
		/ (pumpStand -> howManyPumps() * simulationTime));
	printf ("%9.2f", TotalLitresSold * profit
		- cost * pumpStand -> howManyPumps());*/
	printf ("%7.2f\n", TotalLitresMissed * profit);
}


//---------------------------------------------------------------------------------













extern "C" void sim() // Alice is the main process
{	
	int numP, numT;
	cout << "Enter Pumps: ";
	cin << numP;
	cout << endl << "Enter Time: ";
	cin >> numT;
	cout << endl;
	
	pumps = new facility_set("pumps", numP);
	stats = new statsClass;
	
	create("sim"); 
	Generate(); 
	
	hold(numT);
	report();
}






void Generate()
{
	create("Generate");
	
	while(1){
		Car();
		hold(interarrivalTime());
	}
}








void Car()
{
	create("Car");
	double litresNeeded = uniform(10, 60);
	int len = pumps->qlegnth();
	stats->countArrival();
	if(DoesCarBalk(litresNeeded, len)) {
		stats->accumBalk(litresNeeded);
	}
	else {
		pumps->reserve();
		
		stats->accumSale(litresNeeded);
		stats->accumServiceTime(serviceTime(litresNeeded));
		
		pumps->release();
	}
	
}









