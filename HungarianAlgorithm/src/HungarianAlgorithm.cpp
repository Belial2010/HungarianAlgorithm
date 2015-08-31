//============================================================================
// Name        : HungarianAlgorithm.cpp
// Author      : Theodore Tzanetps
// Version     :
// Copyright   : MIT License
// Description : Hello World in C++, Ansi-style
//============================================================================

#include <iostream>
#include <fstream>
#include <vector>
#include <string>
#include "HungarianUtility.h"

/*
 *
 * Notes:
 * -Compiling with -std=c++1y
 * -Using googletest C++ testing framework
 * http://pezad-games.googlecode.com/svn/trunk/GTestTest/HOWTO for a good coverage of googletest setup
 * https://www.ics.uci.edu/~pattis/common/modules46/googletest.html
 * This eclipse project also has the googlecode test runner integration into eclipse
 */


using namespace std;

int main() {


	cout << "Hello World! Running the Hungarian Algorithm on the test cost matrix."<< endl;
	cout << "In this implementation Advertisers are matched to Buyers to either minimize/maximize /"
			"the cost associated with each match." <<endl;

	/*
	 * Populate the cost matrix
	 */
	Matrix matrix = {{58,27,76,49,23,81,75,73,60,14},
			{38,14,22,67,22,65,36,69,75,71},
			{38,86,52,59,83,77,30,84,7,62},
			{5,28,15,25,65,44,49,3,35,54},
			{81,70,27,88,54,35,28,91,93,1},
			{41,36,85,45,45,22,39,44,35,55},
			{32,63,96,75,94,4,43,68,46,63},
			{54,94,25,47,81,94,57,63,37,15},
			{40,93,79,80,95,28,37,69,34,19},
			{25,47,12,83,53,16,61,37,45,48}};


	HungarianUtility::Instance().printMatrix(matrix,"Input Matrix=");


	/*
	 * Run the MAXIMIZATION hungarian algorithm on the input matrix, regardless of square-ness
	 */
	hung_res_t ans = HungarianUtility::Instance().runHungarian(matrix, true);

#ifdef DEBUG
	HungarianUtility::Instance().printMatrix(matrix,"Final Matrix State=");
#endif
	for(unsigned int j= 0;j<ans.b2a_map.size();++j)
	{
		cout << "Buyer "<< j<< " matched to Advertiser " << ans.b2a_map[j]<<endl;
	}

	for(unsigned int j= 0;j<ans.a2b_map.size();++j)
	{
		cout << "Advertiser "<< j<< " matched to Buyer " << ans.a2b_map[j]<<endl;
	}

	cout << "Total Maximized Cost = " << ans.cost << endl;







	/*
	 * Run the MINIMIZATION hungarian algorithm on the input matrix, regardless of square-ness
	 */
	hung_res_t ans = HungarianUtility::Instance().runHungarian(matrix, false);

#ifdef DEBUG
	HungarianUtility::Instance().printMatrix(matrix,"Final Matrix State=");
#endif
	for(unsigned int j= 0;j<ans.b2a_map.size();++j)
	{
		cout << "Buyer "<< j<< " matched to Advertiser " << ans.b2a_map[j]<<endl;
	}

	for(unsigned int j= 0;j<ans.a2b_map.size();++j)
	{
		cout << "Advertiser "<< j<< " matched to Buyer " << ans.a2b_map[j]<<endl;
	}

	cout << "Total Minimized Cost = " << ans.cost << endl;


	return 0;
}
