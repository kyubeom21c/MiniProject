#include "SimulateSet.h"

int main()
{
	srand((unsigned int)time(NULL));
	try
	{
		WSASession Session;
		SimulateSet simulate;
		simulate.runScenario();
	}
	catch (system_error& e) { cout << e.what(); }
}