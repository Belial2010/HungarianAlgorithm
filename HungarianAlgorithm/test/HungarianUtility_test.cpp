/*
 * HungarianUtility_test.cpp
 *
 *  Created on: Aug 11, 2015
 *      Author: theodore
 */

#include "HungarianUtility.h"
#include "gtest/gtest.h"

/*
 * runHungarian Tests
 *
 * http://www.hungarianalgorithm.com/solve.php used for generating test cases
 *
 */

TEST(runHungarian_Max, r2x2_1)
{
	Matrix inp = {{14,	15},
				{80,36}};
	vector<unsigned int> matching = {1,0};
	double cost = 95;
	hung_res_t ans = HungarianUtility::Instance().runHungarian(inp, true);
	ASSERT_EQ(matching,ans.b2a_map) << "Matching from b2a should be {1,0}";
	ASSERT_EQ(cost,ans.cost) << "Cost should be "<< cost;
}

TEST(runHungarian_Max, r2x2_2)
{
	Matrix inp = {{78,	80},
				{42,73}};
	vector<unsigned int> matching = {0,1};
	double cost = 151;
	hung_res_t ans = HungarianUtility::Instance().runHungarian(inp, true);
	ASSERT_EQ(matching,ans.b2a_map) << "Matching from b2a should be {0,1}";
	ASSERT_EQ(cost,ans.cost) << "Cost should be "<< cost;
}


TEST(runHungarian_Max, r3x3_1)
{
	Matrix inp = {{80,	7,	93},
			{23,	38,	56},
			{79,	62,	89}};
	vector<unsigned int> matching = {2,1,0};
	double cost = 210;
	hung_res_t ans = HungarianUtility::Instance().runHungarian(inp, true);
	ASSERT_EQ(matching,ans.b2a_map) << "Matching from b2a should be {2,1,0}";
	ASSERT_EQ(cost,ans.cost) << "Cost should be "<< cost;
}

TEST(runHungarian_Max, r5X5_1)
{
	Matrix inp = {{80,73,59,16,76},
			{81,83,85,81,89},
			{85,30,57,10,41},
			{82,43,58,33,77},
			{45,83,78,80,15}};
	vector<unsigned int> matching = {2,0,1,4,3};
	double cost = 400;
	hung_res_t ans = HungarianUtility::Instance().runHungarian(inp, true);
	ASSERT_EQ(matching,ans.b2a_map) << "Matching from b2a should be {2,0,1,4,3}";
	ASSERT_EQ(cost,ans.cost) << "Cost should be "<< cost;
}

TEST(runHungarian_Max, r5X5_2)
{
	Matrix inp = {{22,81,56,61,74},
			{77,62,61,39,42},
			{90,21,28,11,30},
			{21,35,16,78,60},
			{48,8,2,65,47}};
	vector<unsigned int> matching = {2,0,1,3,4};
	double cost = 357;
	hung_res_t ans = HungarianUtility::Instance().runHungarian(inp, true);
	ASSERT_EQ(matching,ans.b2a_map) << "Matching from b2a should be {2,0,1,3,4}";
	ASSERT_EQ(cost,ans.cost) << "Cost should be "<< cost;
}

TEST(runHungarian_Max, r10x10_1)
{
	Matrix inp = {{36,81,9,14,23,96,99,99,6,12},
			{29,42,81,84,42,22,94,33,4,70},
			{30,15,50,83,26,20,79,67,7,20},
			{44,69,66,65,24,34,66,86,83,63},
			{22,20,73,95,36,1,21,92,66,28},
			{56,60,98,45,33,3,83,51,52,38},
			{95,70,24,25,73,25,49,92,64,46},
			{67,23,24,30,54,78,41,53,9,94},
			{79,16,2,79,94,43,28,65,50,45},
			{22,75,26,90,71,48,79,61,19,39}};

	vector<unsigned int> matching = {6,9,5,2,8,0,1,4,3,7};
	double cost = 904;
	hung_res_t ans = HungarianUtility::Instance().runHungarian(inp, true);
	ASSERT_EQ(matching,ans.b2a_map) << "Matching from b2a should be {6,9,5,2,8,0,1,4,3,7}";
	ASSERT_EQ(cost,ans.cost) << "Cost should be "<< cost;
}

TEST(runHungarian_Max, r10x10_2)
{
	Matrix inp = {{58,27,76,49,23,81,75,73,60,14},
			{38,14,22,67,22,65,36,69,75,71},
			{38,86,52,59,83,77,30,84,7,62},
			{5,28,15,25,65,44,49,3,35,54},
			{81,70,27,88,54,35,28,91,93,1},
			{41,36,85,45,45,22,39,44,35,55},
			{32,63,96,75,94,4,43,68,46,63},
			{54,94,25,47,81,94,57,63,37,15},
			{40,93,79,80,95,28,37,69,34,19},
			{25,47,12,83,53,16,61,37,45,48}};

	vector<unsigned int> matching = {4,8,5,9,6,7,0,2,1,3};
	double cost = 818;
	hung_res_t ans = HungarianUtility::Instance().runHungarian(inp, true);
	ASSERT_EQ(matching,ans.b2a_map) << "Matching from b2a should be {4,8,5,9,6,7,0,2,1,3}";
	ASSERT_EQ(cost,ans.cost) << "Cost should be "<< cost;
}



/*
 * Unbalanced Tests
 */
TEST(runHungarian_Min_Unbal, r3x4_1)
{
	Matrix inp = {{71,78,13,45},
			{37,7,7,3},
			{83,97,72,28}};
	vector<unsigned int> matching = {3,1,0,2};
	double cost = 48;
	hung_res_t ans = HungarianUtility::Instance().runHungarian(inp, false);
	ASSERT_EQ(matching,ans.b2a_map) << "Matching from b2a should be {3,1,0,2}";
	ASSERT_EQ(cost,ans.cost) << "Cost should be "<< cost;
}

TEST(runHungarian_Min_Unbal, r4x3_1)
{
	Matrix inp = {{11,19,17},
			{21,15,13},
			{15,18,21},
			{18,15,17}};
	vector<unsigned int> matching = {0,3,1,2};
	double cost = 39;
	hung_res_t ans = HungarianUtility::Instance().runHungarian(inp, false);
	ASSERT_EQ(matching,ans.b2a_map) << "Matching from b2a should be {0,3,1,2}";
	ASSERT_EQ(cost,ans.cost) << "Cost should be "<< cost;
}


TEST(runHungarian_Min_Unbal, r8x6_1)
{
	Matrix inp = {{85,65,77,49,98,25},
			{60,74,70,70,2,54},
			{62,41,40,96,4,91},
			{45,97,67,63,51,50},
			{4,68,86,79,45,47},
			{42,30,82,78,53,45},
			{20,46,88,85,72,1},
			{54,42,71,66,24,81}};
	vector<unsigned int> matching = {4,5,2,0,1,6,3,7};
	double cost = 126;
	hung_res_t ans = HungarianUtility::Instance().runHungarian(inp, false);
	ASSERT_EQ(matching,ans.b2a_map) << "Matching from b2a should be {4,5,2,0,1,6,3,7}";
	ASSERT_EQ(cost,ans.cost) << "Cost should be "<< cost;
}


/*
 * Failing test case. Perhaps this undercontrained case requires backtracking to solve??
 */

/*
 *
 *
 *
 *
 *
 *
 *
 *
 *
 *
http://www.hungarianalgorithm.com/hungarianalgorithm.php
http://csclab.murraystate.edu/bob.pilgrim/445/munkres.html
http://www.hungarianalgorithm.com/solve.php?c=85-65-77-49-98-25--60-74-70-70-2-54--62-41-40-96-4-91--45-97-67-63-51-50--4-68-86-79-45-47--42-30-82-78-53-45--20-46-88-85-72-1--54-42-71-66-24-81&obj=max&random=1
http://www.hungarianalgorithm.com/solve.php?c=63-18-90-85-76-3-0-0-93-87--70-57-18-15-57-77-5-66-95-29--69-84-49-16-73-79-20-32-92-79--55-30-33-34-75-65-33-13-16-36--77-79-26-4-63-98-78-7-33-71--43-39-1-54-66-96-16-48-47-61--4-29-75-74-26-74-50-7-35-53--32-76-75-69-45-21-58-46-90-5--20-83-97-20-5-56-71-34-49-54--77-24-73-9-28-51-20-38-80-60
http://www.hungarianalgorithm.com/solve.php?c=13-33-21-49-0-73-0-0--38-24-28-28-96-44-0-0--36-57-58-2-94-7-0-0--53-1-31-35-47-48-0-0--94-30-12-19-53-51-0-0--56-68-16-20-45-53-0-0--78-52-10-13-26-97-0-0--44-56-27-32-74-17-0-0
http://www.hungarianalgorithm.com/solve.php?c=7-9-87-57--28-98-13-54--31-85-84-98--51-56-40-1&random=1

http://www.frc.ri.cmu.edu/~lantao/code.html

TEST(runHungarian_Min_PostUnbal, r8x8_0)
{
	Matrix inp = {{13,33,21,49,0,73,0,0},
			{38,24,28,28,96,44,0,0},
			{36,57,58,2,94,7,0,0},
			{53,1,31,35,47,48,0,0},
			{94,30,12,19,53,51,0,0},
			{56,68,16,20,45,53,0,0},
			{78,52,10,13,26,97,0,0},
			{44,56,27,32,74,17,0,0}};
	vector<unsigned int> matching = {1,3,6,2,0,7,5,4};
	double cost = 68;
	hung_res_t ans = HungarianUtility::Instance().runHungarian(inp, false);
	ASSERT_EQ(matching,ans.b2a_map) << "Matching from b2a should be {1,3,6,2,0,7,5,4}";
	ASSERT_EQ(cost,ans.cost) << "Cost should be "<< cost;
}

TEST(runHungarian_Max_Unbal, r8x6_1)
{
	Matrix inp = {{85,65,77,49,98,25},
			{60,74,70,70,2,54},
			{62,41,40,96,4,91},
			{45,97,67,63,51,50},
			{4,68,86,79,45,47},
			{42,30,82,78,53,45},
			{20,46,88,85,72,1},
			{54,42,71,66,24,81}};
	vector<unsigned int> matching = {1,3,6,2,0,7,5,4};
	double cost = 520;
	hung_res_t ans = HungarianUtility::Instance().runHungarian(inp, true);
	ASSERT_EQ(matching,ans.b2a_map) << "Matching from b2a should be {1,3,6,2,0,7,5,4}";
	ASSERT_EQ(cost,ans.cost) << "Cost should be "<< cost;
}

*/






/*
TEST(bruteForce, r3x3_1)
{
	Matrix inp = {{7,9,87,57},
			{28,98,13,54},
			{31,85,84,98},
			{51,56,40,1}};
	vector<unsigned int> matching = {2,0,1,3};
	double cost = 54;
	hung_res_t ans = HungarianUtility::Instance().runHungarian(inp, false);
	ASSERT_EQ(matching,ans.b2a_map) << "Matching from b2a should be {2,0,1,3}";
	ASSERT_EQ(cost,ans.cost) << "Cost should be "<< cost;
	double bf = HungarianUtility::Instance().bruteForce(inp);
	ASSERT_EQ(bf,cost)<< "BF Cost should be "<< cost;
}
*/
