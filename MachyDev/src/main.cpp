#include "machy.hpp"
#include "dev.hpp"

/* [GOALS]
	-> save state load / serialization of program
	-> Main Menu in main
		- MachY [Core Machine | Admin??]
		- GameY [Game Engine]
		- ???? 
	[FINISHED]
	Command Line Arguments / Engine Flags $$$
*/

int main() {
	machy::App* MachyDevApp = new machy::Dev;
	MachyDevApp->InitNoArgs();
	
	auto& machine = machy::MachY::Instance();
	machine.run(MachyDevApp);
	
	return 0;
}