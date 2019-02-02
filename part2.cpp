#include <iostream>
#include <stdio.h>
#include "cpp.h"
#include <string.h>
#include <math.h>       /* log */
#include <stdlib.h>

using namespace std;

const double profit = .025; // dollars per litre
const double cost = 20;
const int MEAN = 50;
int snapshotInterval, numPumps, totalTime, simulationTime = 0;

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
	return - MEAN * log (((double) rand())/((double) RAND_MAX));
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
	return queueLength > 0 && ((double) rand())/((double) RAND_MAX)
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
		howLong += ((double) rand())/((double) RAND_MAX);

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

statsClass *stats;

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
	printf("%8d%7d", simulationTime, TotalArrivals);
	//printf("%8.3f", carQueue -> emptyTime()/simulationTime);
	if (TotalArrivals > 0 && simulationTime > 0) {
		printf("%9.3f%8.3f", ((double)simulationTime)/TotalArrivals,
			(TotalLitresSold + TotalLitresMissed) / TotalArrivals);
	}
	else
		printf ("%9s%8s", "Unknown", "Unknown");
	
	printf ("%7i", balkingCustomers);
	
	/*if (customersServed > 0)
		printf ("%9.3f", TotalWaitingTime / customersServed);
	else
		printf ("%9s", "Unknown");*/
	if(simulationTime > 0)
		printf ("%7.3f", TotalServiceTime / (numPumps * simulationTime));
	else
		printf ("%7.3s", "Unknown");
	
	printf ("%9.2f", TotalLitresSold * profit - cost * numPumps);
	
	printf ("%7.2f\n", TotalLitresMissed * profit);
}


//---------------------------------------------------------------------------------













extern "C" void sim() // Alice is the main process
{	
	
	cout << "Enter Number of Pumps: ";
	cin >> numPumps;
	cout << "Enter Total Run Time: ";
	cin >> totalTime;
	cout << "Enter Snapshot Interval: ";
	cin >> snapshotInterval;
	
	pumps = new facility_ms("pumps", numPumps);
	stats = new statsClass;
	
	create("sim"); 
	
	
	
	printf ("%9s%7s%7s%8s%7s%7s%8s%7s\n", " Current", "Total ",
		"Car", "Average", "Number", "Pump ",
		"Total", " Lost ");
	printf ("%9s%7s%9s%8s%7s%7s%8s%7s\n", "   Time ", "Cars ",
		"  Time  ", " Litres ", "Balked",
		"Usage ", "Profit", "Profit");
	for (int i = 0; i < 79; i++)
		cout << "-";
	cout << "\n";
	
	
	Generate(); 
	
	hold(totalTime);
	report();
}






void Generate()
{	
	simulationTime = 0;
	int nextTime = simulationTime + interarrivalTime();
	
	create("Generate");
	
	while(1){
		
		if(simulationTime == nextTime) {
			stats->countArrival();
			Car();
			nextTime = simulationTime + interarrivalTime();
		}
		
		if(simulationTime % snapshotInterval == 0) {
			stats->snapshot();
		}
		hold(1);
		simulationTime++;
	}
}








void Car()
{
	create("Car");

	double litresNeeded = 10 + 50*((double) rand())/((double) RAND_MAX);
	int len = pumps->qlength();
	
	if(DoesCarBalk(litresNeeded, len)) {
		stats->accumBalk(litresNeeded);
	}
	else {
		pumps->reserve();
		
		stats->accumSale(litresNeeded);
		
		double t = serviceTime(litresNeeded);
		stats->accumServiceTime(t);
		hold(t);		

		pumps->release();
	}
	
}









