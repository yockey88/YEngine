#include "machy.hpp"

/* [GOALS]
	-> Command Line Arguments / Engine Flags
	-> save state load / serialization of program
	-> Main Menu in main
		- MachY [Core Machine | Admin??]
		- GameY [Game Engine]
		- ???? 
*/

int main(int argc , char* argv[]) {
	machy::App* tests = new machy::Dev;
	tests->InitArgs(argc , argv);
	
	auto& machine = machy::MachY::Instance();
	machine.run(tests);
	
	return 0;
}