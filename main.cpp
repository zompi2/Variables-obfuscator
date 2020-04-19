/**
 * Variable memory obfuscator
 *
 * This is an example application that shows the memory obfuscator in action.
 *
 * (c) 2015 Damian Nowakowski
 */

#include <iostream>
#include "safe_type.h"

int main()
{
	int Scores 					= 10;	///< The normal value
	safe_type<int> SafeScores 	= 10;	///< The obfuscated value

	while (true)
	{
		// Print both values
		std::cout << Scores <<" "<< SafeScores << std::endl;
		
		// Increment both values
		Scores 		+= 10;
		SafeScores 	+= 10;
		
		// Sleep for a while so we can see what's going on
		Sleep(1000);
	}

	return 0;
}