/*
 * HungarianUtility.h
 *
 *  Created on: Aug 11, 2015
 *      Author: theodore
 */

#ifndef HUNGARIANUTILITY_H_
#define HUNGARIANUTILITY_H_

#include <string>
#include <vector>


using namespace std;



typedef vector<vector<double>> Matrix;
struct hung_res_t{
	vector<unsigned int> b2a_map;
	vector<unsigned int> a2b_map;
	double cost;
};

struct cover_opt_t{
	bool isRow;
	unsigned int index;
	cover_opt_t(bool row=false, unsigned int i=0):isRow(row),
			index(i){}
};
struct cover_opt_vec_t{
	vector<cover_opt_t> vec;
	unsigned int zeroCount;
};

struct dmc_bt_t{

	vector<bool> rowCovered;
	vector<bool> colCovered;
	unsigned int covCount;
};

class HungarianUtility {
private:
	HungarianUtility() {};								//Make private to be a singleton
    HungarianUtility(HungarianUtility const&) = delete;	//Ensure copy and = operator are not auto declared
    void operator=(HungarianUtility const&) = delete;
    virtual ~HungarianUtility(){};
public:


    //Left public for testing purposes with googletest. Can possibly make private/friend visible
    //but not enought time to do this.
	static HungarianUtility& Instance();	//static function
	void printMatrix(const Matrix&,const string& s ="");
	double transMax2Min(const double &,const double &max=0);
	void iterTransform_v1(Matrix&, double&,vector<bool>&, vector<bool>&);
	void iterTransform_v2(Matrix&, double&,vector<bool>&, vector<bool>&);
	void drawMinCovers(const Matrix&,vector<bool>&,vector<bool>&,unsigned int&);


	dmc_bt_t findMinCovers(const Matrix&, vector<bool>, vector<bool>,unsigned int,int);


	vector<unsigned int> findMatching(const Matrix&);
	hung_res_t runHungarian(Matrix &, const bool Max2Min=false);
	double calcCost(const Matrix&, const vector<unsigned int>&);
	vector<unsigned int> invertMatching(vector<unsigned int>, unsigned int);

	//Fun with brute force
	double bruteForce(Matrix &);
	double bruteForceRec(Matrix &,vector<bool>,vector<bool>,double,unsigned int);



	//	dmc_bt_t drawMinCoversBackTrack(bool, const Matrix&,vector<bool>,vector<bool>,unsigned int);
	//	void drawMinCoversInteriorBT(bool,const Matrix&,vector<bool>&,vector<bool>&,unsigned int&);


};

#endif /* HUNGARIANUTILITY_H_ */
