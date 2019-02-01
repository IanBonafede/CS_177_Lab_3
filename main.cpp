#include <iostream>
#include "cpp.h"
#include <string.h>
using namespace std;
void Bob(); // Another process
extern "C" void sim() // Alice is the main process
{
int i;
create("Alice"); // Alice separates from background system
Bob(); // Alice gives Bob a chance to do the same
for(i=1;i<10;i++)
{
cout << "Alice says hello " << i << " times." << endl;
}
}
void Bob()
{
int i;
create("Bob");
for(i=1;i<10;i++)
{
cout << "Bob says hello " << i << " times." << endl;
}
}
