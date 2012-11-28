#ifndef KEITHLEY_FUNCTIONS
#define KEITHLEY_FUNCTIONS

using namespace std;

class KeithleyDevice {
public:
	void pulsesweepvoltage(double,double,int);
	void rampvoltagedown(int,int);
	void rampvoltageup(int,int);
	int write(const char *);
	int read(void *,int);
	KeithleyDevice();
private:
 
	int Device;                   /* Device unit descriptor                  */
	int BoardIndex;               /* Interface Index (GPIB0=0,GPIB1=1,etc.)  */
	char stringinput[512]; 
};
#endif
