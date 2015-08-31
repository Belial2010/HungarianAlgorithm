/*
 * HungarianUtility.cpp
 *
 *  Created on: Aug 11, 2015
 *      Author: theodore
 */

#include "HungarianUtility.h"
#include <algorithm>
#include <iostream>



//0 is drawMinCovers
//1 i drawMinCoversBackTrack which isn't working
//2 is drawMinCoversInteriorBT
#define COVERMODE 0


HungarianUtility& HungarianUtility::Instance()
{
	static HungarianUtility R;
	return R;
}

/**
 * printMatrix prints out the current state of matrix to cout. If a string s is passed
 * in, it will be printed first with a newline at the end.
 * @param matrix
 * @param s
 */
void HungarianUtility::printMatrix(const Matrix& matrix, const string& s)
{
	//print the transformed matrix
	if(s.length()>0)
		cout << s<<endl;

	for(const vector<double> &row:matrix)
	{
		for(const double &val:row)
		{
			cout.width(5);
			cout << val<< " ";
		}
		cout<<endl;
	}
}

/*
 * Multiple methods to transform a maximization problem to a minimization
 * A)val = MAX_ENTRY-val
 * B)val = 1/val
 * C)val = -val
 */

/**
 * transMax2Min converts a requested maximization problem, to that of a minimization problem
 * which is what the Hungarian algorithm is designed to solve. Performing the transformation(regardless of
 * the method) maintains the mapping relationships with the final solution, so a solution for the Hungarian
 * algorithm's minimzation, is a solution to the requested maximization.
 *
 * Three methods of performing the transformation were tested,
 * MAXSUB = max-val
 * INVERSION = 1.0/val
 * NEGATION = -val
 *
 * MAXSUM and NEGATION seem to work fine. INVERSION does not. Leaving MAXSUB as the default
 *
 * @param val: Value to be transformed
 * @param max: Optional argument (defaulted to 0 in header) which represents the cost matrix's max value
 * @return double containing the transformed value
 */
double HungarianUtility::transMax2Min(const double &val, const double &max)
{
#define MAXSUB
	//	#define INVERSION
	//	#define NEGATION
#if defined(MAXSUB)
	if(max == 0)
		cout<<"MAX VALUE IS 0 in transMax2Min!!!!!"<< endl;
	return max - val ;
#elif defined(INVERSION)
	return 1.0/val;
#elif defined(NEGATION)
	return -val;
#endif
}


/**
 * Perform the iterative matrix manipulation step, referred to as Step 5 at
 * http://www.math.harvard.edu/archive/20_spring_05/handouts/assignment_overheads.pdf
 *
 * -Subtract minimum uncovered value from each uncovered row
 * -Add minimum uncovered value to each covered column
 * @param matrix
 * @param minVal
 * @param rowCovered
 * @param colCovered
 */
void HungarianUtility::iterTransform_v1(Matrix& matrix, double& minVal,vector<bool>& rowCovered, vector<bool>& colCovered)
{
	unsigned int dim = matrix.size();
	//Alter the matrix
	for(unsigned int i=0;i<dim;++i)//loop over the rows
	{
		if(!rowCovered[i])//SUBTRACT from each UNCOVERED row
		{
			for(unsigned int j=0;j<dim;++j)//loop over the columns
			{
				matrix[i][j]-=minVal;
			}
		}
	}
	for(unsigned int j=0;j<dim;++j)//loop over the columns
	{
		if(colCovered[j])//ADD to each COVERED column
		{
			for(unsigned int i=0;i<dim;++i)//loop over the rows
			{
				matrix[i][j]+=minVal;
			}
		}
	}
}

/**
 * Perform the iterative matrix manipulation step, referred to as Step 4 at
 * http://www.hungarianalgorithm.com/hungarianalgorithm.php
 *
 * -Subtract minimum uncovered value from each uncovered element
 * -Add minimum uncovered value to each doubly covered element
 *
 * @param matrix
 * @param minVal
 * @param rowCovered
 * @param colCovered
 */
void HungarianUtility::iterTransform_v2(Matrix& matrix, double& minVal,vector<bool>& rowCovered, vector<bool>& colCovered)
{
	unsigned int dim = matrix.size();
	//Alter the matrix
	for(unsigned int i=0;i<dim;++i)//loop over the rows
	{
		for(unsigned int j=0;j<dim;++j)//loop over the columns
		{
			if(!rowCovered[i] && !colCovered[j])
			{
				matrix[i][j] -= minVal;
			}
			else if(rowCovered[i] && colCovered[j])
			{
				matrix[i][j] += minVal;
			}
		}

	}
}

/**
 * Loop over the matrix to find the locations of the minimum number of lines required to cover all
 * the 0's in the matrix. An important step to note is that the rowCovered and colCovered vectors,
 * as well as the covCount counter get reset to false/0 at the beginning in case a new iterative
 * manipulation of the matrix has caused a new optimal mappin of covering lines to be viable.
 *
 * @param matrix
 * @param rowCovered
 * @param colCovered
 * @param covCount
 */
void HungarianUtility::drawMinCovers(const Matrix& matrix,vector<bool>& rowCovered,vector<bool>& colCovered,unsigned int& covCount)
{
	unsigned int dim = matrix.size();
	bool targIsRow = true;
	unsigned int targInd = 0;
	int zeroCount = -1;

	//Reset the rowCovered and colCovered elements
	fill(rowCovered.begin(), rowCovered.end(), false);
	fill(colCovered.begin(), colCovered.end(), false);
	covCount = 0;

	while(zeroCount!=0)
	{
#ifdef DEBUG
		cout << "covCount = " << covCount << endl;
		cout << "rowCovered=";
		for(const bool& b : rowCovered)
		{
			cout << b << " ";
		}
		cout << endl;
		cout<<"colCovered=";
		for(const bool& b : colCovered)
		{
			cout << b << " ";
		}
		cout << endl;

#endif
		zeroCount = 0;

		//Iterate over each row to find the one containing the most zeros
		for(unsigned int i=0;i<dim;++i)//loop over the rows
		{
			if(rowCovered[i])
				continue;
			int temp=0;
			for(unsigned int j=0;j<dim;++j)//loop over the columns
			{
				if(!colCovered[j] && matrix[i][j] == 0)
				{
					temp++;
				}
			}
			if(temp>zeroCount)
			{
				targInd = i;
				targIsRow = true;
				zeroCount=temp;
			}
		}
		//Iterate over each column to find the one containing the most zeros
		for(unsigned int j=0;j<dim;++j)//loop over the columns
		{
			if(colCovered[j])
				continue;
			int temp=0;
			for(unsigned int i=0;i<dim;++i)//loop over the rows
			{
				if(!rowCovered[i] && matrix[i][j] == 0)
				{
					temp++;
				}
			}
			if(temp>zeroCount)
			{
				targInd = j;
				targIsRow = false;
				zeroCount=temp;
			}
		}

		if(zeroCount>0)
		{
			if(targIsRow)
			{
				rowCovered[targInd]=true;
			}
			else
			{
				colCovered[targInd]=true;
			}
			covCount++;
		}
	}
}

dmc_bt_t HungarianUtility::findMinCovers(const Matrix& matrix, vector<bool>rowCovered, vector<bool> colCovered,unsigned int covCount,int level)
{
	dmc_bt_t ans;
	unsigned int dim = matrix.size();
	cover_opt_vec_t covStruct;
	covStruct.zeroCount = 0;
	covStruct.vec.clear();
	covStruct.vec.push_back(cover_opt_t());//just so the while loop begins
	while(covStruct.vec.size()>0)
	{

		if(covCount==dim)
		{
			ans.covCount = covCount;
			ans.rowCovered = rowCovered;
			ans.colCovered = colCovered;
			return ans;
		}

#ifdef DEBUG
		cout << "LEVEL "<<level<<": covCount = " << covCount << endl;
		cout << "LEVEL "<<level<<": rowCovered=";
		for(const bool& b : rowCovered)
		{
			cout << b << " ";
		}
		cout << endl;
		cout<<"LEVEL "<<level<<": colCovered=";
		for(const bool& b : colCovered)
		{
			cout << b << " ";
		}
		cout << endl;

#endif




		covStruct.zeroCount = 0;
		covStruct.vec.clear();				//clear out the option vector

		cout << "covStruct should be clear with size=" << covStruct.vec.size()<<endl;

		//Iterate over each row to find the one containing the most zeros
		for(unsigned int i=0;i<dim;++i)//loop over the rows
		{
			if(rowCovered[i])
				continue;
			unsigned int temp=0;
			for(unsigned int j=0;j<dim;++j)//loop over the columns
			{
				if(!colCovered[j] && matrix[i][j] == 0)
				{
					temp++;
				}
			}

			if(temp >0)//Only consider if we found a row with at least 1 zero
			{
				if(temp>covStruct.zeroCount)//Reset the struct for this new max
				{
					cover_opt_t temp_cover_opt(true,i);
					covStruct.vec.clear();
					//todo combine these two lines
					covStruct.vec.push_back(temp_cover_opt);
					covStruct.zeroCount=temp;
				}
				else if(temp == covStruct.zeroCount)//Add the option
				{
					cover_opt_t temp_cover_opt(true,i);
					//todo combine these two lines
					covStruct.vec.push_back(temp_cover_opt);
				}
			}
		}

		//Iterate over each column to find the one containing the most zeros
		for(unsigned int j=0;j<dim;++j)//loop over the columns
		{
			if(colCovered[j])
				continue;
			unsigned int temp=0;
			for(unsigned int i=0;i<dim;++i)//loop over the rows
			{
				if(!rowCovered[i] && matrix[i][j] == 0)
				{
					temp++;
				}
			}
			if(temp>0)//Only consider if we found a column with at least 1 zero
			{
				if(temp>covStruct.zeroCount)//Reset the struct for this new max
				{
					cover_opt_t temp_cover_opt(false,j);
					covStruct.vec.clear();
					//todo combine these two lines
					covStruct.vec.push_back(temp_cover_opt);
					covStruct.zeroCount=temp;
				}
				else if(temp == covStruct.zeroCount)//Add the option
				{
					cover_opt_t temp_cover_opt(false,j);
					//todo combine these two lines
					covStruct.vec.push_back(temp_cover_opt);
				}
			}
		}

		//If there is only one greedy choice, then take it.
		if(covStruct.vec.size() == 1)
		{
			if(covStruct.vec[0].isRow)
			{
#ifdef DEBUG
				cout << "Picking default choice at row " << covStruct.vec[0].index<<endl;
#endif
				rowCovered[covStruct.vec[0].index]=true;
			}
			else
			{
#ifdef DEBUG
				cout << "Picking default choice at column " << covStruct.vec[0].index<<endl;
#endif
				colCovered[covStruct.vec[0].index]=true;
			}
			covCount++;
		}
		else if(covStruct.vec.size()>1)
		{
			dmc_bt_t dmcMin;
			dmcMin.covCount = dim*100;//impossible situation
			dmcMin.rowCovered=rowCovered;
			dmcMin.colCovered=colCovered;

			for(unsigned int t = 0;t<covStruct.vec.size();++t)
			{

				//Cover the row or column
				if(covStruct.vec[t].isRow)
				{
				#ifdef DEBUG
					cout << "LEVEL "<< level<<": Picking one of "<<covStruct.vec.size()<<" choices of "<<covStruct.zeroCount << " covercount at row " << covStruct.vec[t].index<<endl;
				#endif
					rowCovered[covStruct.vec[t].index] = true;
				}
				else
				{
				#ifdef DEBUG
					cout << "LEVEL "<< level<<": Picking one of "<<covStruct.vec.size()<<" choices of "<<covStruct.zeroCount << " covercount at col " << covStruct.vec[t].index<<endl;
				#endif
					colCovered[covStruct.vec[t].index] = true;
				}
				covCount++;
				//Determine the number of covers, should we choose this path.
				dmc_bt_t recAns = findMinCovers(matrix,rowCovered,colCovered,covCount,level+1);
				if(covStruct.vec[t].isRow)
				{
					rowCovered[covStruct.vec[t].index] = false;
				}
				else
				{
					colCovered[covStruct.vec[t].index] = false;
				}
				covCount--;
				if(recAns.covCount<dmcMin.covCount)
				{
					dmcMin = recAns;
				}
			}
			return dmcMin;
		}
	}
	ans.covCount = covCount;
	ans.rowCovered = rowCovered;
	ans.colCovered = colCovered;
	return ans;
}




/**
 * Find the viable matching of the matrix. First check for rows or columns with one 0, which is a fully
 * constrained solution, aka that row must be matched with that column. Secondly loop over the remaining
 * uncovered matrix and assign matching according to the order in which you encounter them.
 *
 * @param matrix
 * @return vector<unsigned int> which represents a mapping of the columns(buyers) to rows(advertisers).
 * return[0] is the index into the advertiser vector which is mapped to buyer 0.
 */
vector<unsigned int> HungarianUtility::findMatching(const Matrix& matrix)
{
	unsigned int dim=matrix.size();
	//Covered now means that this row/column is already matched
	vector<bool> rowMatched(dim,false);
	vector<bool> colMatched(dim,false);
	unsigned int matchCount=0;

	vector<unsigned int> buyer2AdvMap(dim,dim+1000);	//initialize to a value that cant be possible


#ifdef DEBUG
	cout << "matchCount before single 0 check = " << matchCount << endl;
#endif
	/*
	 * Check for columns or rows with a single 0, which is a required match
	 *
	 */
	bool singleCheck = true;
	//keep looping while there is a single element constraint
	while(singleCheck)
	{
		singleCheck = false;
		for(unsigned int i=0;i<dim;++i)//loop over the rows
		{
			if(rowMatched[i])
				continue;
			unsigned int temp=0;
			unsigned int x=0,y=0;
			for(unsigned int j=0;j<dim;++j)//loop over the columns
			{
				if(!colMatched[j] && matrix[i][j] == 0)
				{
					x=i;
					y=j;
					temp++;
				}
			}
			if(temp == 1)
			{
				//buyer is y, adv is x
				buyer2AdvMap[y] = x;
				matchCount++;
				rowMatched[x] = true;
				colMatched[y] = true;
				singleCheck= true;
			}
		}
		for(unsigned int j=0;j<dim;++j)//loop over the columns
		{
			if(colMatched[j])
				continue;
			unsigned int temp=0;
			unsigned int x=0,y=0;
			for(unsigned int i=0;i<dim;++i)//loop over the rows
			{
				if(!rowMatched[i] && matrix[i][j] == 0)
				{
					x=i;
					y=j;
					temp++;
				}
			}
			if(temp == 1)
			{
				//buyer is y, adv is x
				buyer2AdvMap[y] = x;
				matchCount++;
				rowMatched[x] = true;
				colMatched[y] = true;
				singleCheck= true;
			}
		}
	}
#ifdef DEBUG

	for(unsigned int i=0;i<buyer2AdvMap.size();++i)//loop over the matches
	{
		cout << "Buyer " << i << " is matched to adv " << buyer2AdvMap[i] << endl;
	}
	cout << "matchCount after single 0 check = " << matchCount << endl;
#endif
	while(matchCount!=dim)
	{
		for(unsigned int i=0;i<dim;++i)//loop over the rows
		{
			if(rowMatched[i])
				continue;
			for(unsigned int j=0;j<dim;++j)//loop over the columns
			{
				if(!colMatched[j] && matrix[i][j] == 0)
				{
					//buyer is y, adv is x
					buyer2AdvMap[j] = i;
					matchCount++;
					rowMatched[i] = true;
					colMatched[j] = true;
					break;
				}
			}
		}
#ifdef DEBUG
		cout << "matchCount in underconstrained while loop = " << matchCount << endl;
#endif
	}
	return buyer2AdvMap;

}


/**
 * Run the hungarian algorithm on the given input matrix, transforming the matrix from a maximization
 * to a minimization if necessary.
 *
 * @param matrix: Assumes that the Matrix is rectangular, but not necessarily square
 * @param Max2Min
 * @return hung_res_t which contains the final cost of the matching, and a vector of indeces which maps buyers
 * advertisers
 */
hung_res_t HungarianUtility::runHungarian(Matrix & matrix, const bool Max2Min)
{
	hung_res_t ans;
	unsigned int dim=0;
	Matrix matrix_original = matrix;
	/*
	 * Arg Check
	 */
	if(matrix.size()<=0)
	{
		cout<<"EMPTY MATRIX ENTERED!"<<endl;
		return ans;
	}

	/*
	 * Transform the problem from a maximization to a minimization if needed
	 */
	if(Max2Min==true)
	{
#ifdef MAXSUB
		//Find the max value for the max2min conversion, should it be needed
		double maxValue = 0;
		for(vector<double> &row:matrix)//Iterate through to find the max value
		{
			for(double &val:row)
			{
				if(val>maxValue)
					maxValue = val;
			}
		}
#ifdef DEBUG
		cout << "maxValue = " <<maxValue << endl;
#endif
		for(vector<double> &row:matrix)//Transform each element from a max to a min problem
		{
			for(double &val:row)
			{
				val = transMax2Min(val,maxValue);
			}
		}
#else
		for(vector<double> &row:matrix)//Transform each element from a max to a min problem
		{
			for(double &val:row)
			{
				val = transMax2Min(val);
			}
		}
#endif

#ifdef DEBUG
		//print the transformed matrix
		printMatrix(matrix,"Matrix Post Max-Min Transformation=");
#endif
	}


	/*
	 * Add dummy rows or columns if need be. Matrix is assumed to be rectangular.
	 */
	int dimDiff = matrix[0].size() - matrix.size();
	if(dimDiff!=0)
	{

#ifdef DEBUG
		//print the square matrix
		printMatrix(matrix,"Matrix Pre Dummy Insertions=");
#endif


		cout<<"Non Square Matrix Input, converting to a square matrix."<<endl;
		if(dimDiff<0)
		{
			dimDiff = abs(dimDiff);
			cout << "Adding " << dimDiff << " columns of 0s"<<endl;
			//More advertisers than buyers. Need to add columns
			for(unsigned int i=0;i<matrix.size();++i)
			{
				for(unsigned int j=0; j< (unsigned int)dimDiff;++j)
				{
					matrix[i].push_back(0.0);
				}
			}
		}
		else
		{
			cout << "Adding " << dimDiff << " rows of 0s"<<endl;
			vector<double> temp (matrix[0].size(),0);
			//More buyers than advertisers. Need to add rows.
			for(unsigned int i=0; i<(unsigned int)dimDiff;++i)
			{
				matrix.push_back(temp);
			}
		}

#ifdef DEBUG
		//print the square matrix
		printMatrix(matrix,"Matrix Post Dummy Insertions=");
#endif
	}


	dim = matrix.size();
	cout << "Dim of square input = " << dim << endl;


	/*
	 * Subtract smallest element from each row
	 */
	for(unsigned int i=0;i<dim;++i)//loop over the rows
	{
		//todo min heaps?
		double minVal = matrix[i][0];
		for(unsigned int j=0;j<dim;++j)//loop over the columns
		{
			if(matrix[i][j]<minVal)
				minVal = matrix[i][j];
		}
		//Subtract the min element from that row
		for(unsigned int j=0;j<dim;++j)//loop over the cols
		{
			matrix[i][j]-=minVal;
		}
	}
#ifdef DEBUG
	HungarianUtility::Instance().printMatrix(matrix,"Matrix Post Row Min Subtractions=");
#endif
	/*
	 * Subtract smallest element from each column
	 */
	for(unsigned int j=0;j<dim;++j)//loop over the columns
	{
		double minVal = matrix[0][j];
		//Find the minimum element in the jth column, across each row
		for(unsigned int i=0;i<dim;++i)//loop over the rows
		{
			if(matrix[i][j]<minVal)
				minVal = matrix[i][j];
		}
		//Subtract the min element from that column
		for(unsigned int i=0;i<dim;++i)//loop over the rows
		{
			matrix[i][j]-=minVal;
		}
	}
#ifdef DEBUG
	HungarianUtility::Instance().printMatrix(matrix,"Matrix Post Column Min Subtractions=");
#endif
	vector<bool> rowCovered(dim,false);
	vector<bool> colCovered(dim,false);
	unsigned int covCount = 0;


	/*Run the iterative step until we have the needed number of covering lines, which is dim
	 *
	 */



#if COVERMODE==0
	drawMinCovers(matrix,rowCovered,colCovered,covCount);
#elif COVERMODE==1
	dmc_bt_t dmc = drawMinCoversBackTrack(true,matrix,rowCovered,colCovered,covCount);
	rowCovered = dmc.rowCovered;
	colCovered = dmc.colCovered;
	covCount = dmc.covCount;
#elif COVERMODE == 2
	drawMinCoversInteriorBT(true,matrix,rowCovered,colCovered,covCount);
#elif COVERMODE==3
	fill(rowCovered.begin(), rowCovered.end(), false);
	fill(colCovered.begin(), colCovered.end(), false);
	covCount = 0;
	dmc_bt_t dmc = findMinCovers(matrix,rowCovered,colCovered,covCount,0);
	rowCovered = dmc.rowCovered;
	colCovered = dmc.colCovered;
	covCount = dmc.covCount;
#endif

	while(covCount<dim)
	{
		double minVal = 0;
		bool started = false;
		//find the smallest element in the remainin uncovered matrix
		for(unsigned int i=0;i<dim;++i)//loop over the rows
		{
			if(!rowCovered[i])
			{
				for(unsigned int j=0;j<dim;++j)//loop over the columns
				{
					if(!colCovered[j])
					{
						if(!started)
						{
							minVal = matrix[i][j];
							started=true;
						}
						else
						{
							if(matrix[i][j]<minVal)
							{
								minVal = matrix[i][j];
							}
						}
					}
				}
			}
		}
#ifdef DEBUG
		cout << "Smallest number in matrix is " << minVal << endl;
#endif

		//		either will work
		//		iterTransform_v1(matrix,minVal, rowCovered, colCovered);
		iterTransform_v2(matrix,minVal, rowCovered, colCovered);


#ifdef DEBUG
		HungarianUtility::Instance().printMatrix(matrix,"Matrix Post iterTransform_v[X] step=");
#endif

		//Redo the drawing
#if COVERMODE==0
		drawMinCovers(matrix,rowCovered,colCovered,covCount);
#elif COVERMODE==1
		dmc_bt_t dmc = drawMinCoversBackTrack(true,matrix,rowCovered,colCovered,covCount);
		rowCovered = dmc.rowCovered;
		colCovered = dmc.colCovered;
		covCount = dmc.covCount;
#elif COVERMODE==2
		drawMinCoversInteriorBT(true,matrix,rowCovered,colCovered,covCount);
#elif COVERMODE==3
		fill(rowCovered.begin(), rowCovered.end(), false);
		fill(colCovered.begin(), colCovered.end(), false);
		covCount = 0;
		dmc_bt_t dmc = findMinCovers(matrix,rowCovered,colCovered,covCount,0);
		rowCovered = dmc.rowCovered;
		colCovered = dmc.colCovered;
		covCount = dmc.covCount;
#endif
	}

#ifdef DEBUG
	cout << endl<<endl;
	cout << "covCount = " << covCount << endl;
	cout << "rowCovered=";
	for(const bool& b : rowCovered)
	{
		cout << b << " ";
	}
	cout << endl;
	cout<<"colCovered=";
	for(const bool& b : colCovered)
	{
		cout << b << " ";
	}
	cout << endl;

#endif

	ans.b2a_map=findMatching(matrix);
	ans.cost=calcCost(matrix_original,ans.b2a_map);
	ans.a2b_map=invertMatching(ans.b2a_map,dim);

	return ans;
}


/**
 * Calculate the cost of a given matching with a given cost matrix.
 *
 * @param matrix
 * @param b2a
 * @return
 */
double HungarianUtility::calcCost(const Matrix& matrix, const vector<unsigned int>& b2a)
{
	double sum = 0;
	for(unsigned int j= 0;j<b2a.size();++j)
	{
		if(b2a[j]>=matrix.size() || j>=matrix[0].size())//Skip matchings which incorporate dummy matches
			continue;
		sum += matrix[b2a[j]][j];
	}

	return sum;
}

vector<unsigned int> HungarianUtility::invertMatching(vector<unsigned int> inp,unsigned int maxDim)
{
	auto inpSize = inp.size();
	vector<unsigned int> ans(maxDim,-1);
	for(unsigned int i=0;i<inpSize;++i)
	{
		auto ind = inp[i];
		ans[ind] = i;
	}
	return ans;
}


/*
 * Fun with brute force calculations.
 */
double HungarianUtility::bruteForce(Matrix & matrix)
{

	unsigned dim = matrix.size();
	vector<bool> rowCovered(dim,false);
	vector<bool> colCovered(dim,false);


	return bruteForceRec(matrix, rowCovered,colCovered,0,0);
}

double HungarianUtility::bruteForceRec(Matrix & matrix,vector<bool> rowCovered,vector<bool> colCovered, double runCost,unsigned int selected)
{
	double costMin=runCost;
	unsigned dim = matrix.size();

	if(selected == dim)
		return runCost;

	for(unsigned int i=0;i<dim;++i)
	{
		if(rowCovered[i])
			continue;
		for(unsigned int j=0;j<dim;++j)
		{
			if(rowCovered[j])
				continue;
			rowCovered[i] = true;
			colCovered[j] = true;

			double temp =bruteForceRec(matrix,rowCovered,colCovered,runCost+matrix[i][j],selected+1);
			rowCovered[i] = false;
			colCovered[j] = false;

			if(selected==0)
			{
				costMin = 9999;
			}

			if(temp<costMin)
			{
				costMin = temp;
			}

		}
	}
	return costMin;
}


