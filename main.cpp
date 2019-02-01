#include <iostream>
#include "cpp.h"
#include <string.h>
using namespace std;
extern "C" void sim() // main process
{
int i;
create("sim"); // separate from background system
for(i=1;i<10;i++)
{
cout << "Alice says hello " << i << " times." << endl;
}
}
