#ifndef EUCLIDEANVECTOR_H
#define EUCLIDEANVECTOR_H

#include <fstream>
#include <iostream>
#include <ostream>
#include <algorithm>
#include <list>
#include <iterator> 
#include <vector>

namespace evec{
	class EuclideanVector {
	friend bool operator==(const EuclideanVector lhs, const EuclideanVector rhs);
	friend bool operator!=(const EuclideanVector lhs,const EuclideanVector rhs);
	friend EuclideanVector operator+(const EuclideanVector a,const EuclideanVector b);
	friend EuclideanVector operator-(const EuclideanVector a,const EuclideanVector b);
	friend double operator*(const EuclideanVector a,const EuclideanVector b);
	friend EuclideanVector operator*(const EuclideanVector a,const double b);
	friend EuclideanVector operator*(const double a,const EuclideanVector b);
	friend EuclideanVector operator/(const EuclideanVector a,const double b);
	friend std::ostream& operator<<(std::ostream& out, const EuclideanVector& a);
	public:
		EuclideanVector();
		EuclideanVector(unsigned int dimension);
		EuclideanVector(unsigned int dimension,double magnitude);
		// Iterator Constructor 
		template <class Iter>
		EuclideanVector(Iter begin,Iter end){
				dimension_ = 0;
				for (Iter it = begin; it != end; ++it){
    				dimension_++;
    			}	
				v_ = new double[dimension_];
				int i = 0;
    			for(Iter it = begin; it != end; ++it){
    				v_[i] = *it;
    				++i;
    			}
		}
		EuclideanVector(std::initializer_list<double> input);
		EuclideanVector(const EuclideanVector& evec);
		EuclideanVector(EuclideanVector&& evec);
		~EuclideanVector();

		EuclideanVector& operator=(EuclideanVector&& evec);
		EuclideanVector& operator=(EuclideanVector& evec);
		const unsigned int getNumDimensions() const;
		const double get(unsigned int dimension) const;
	    double getEuclideanNorm() ;
		EuclideanVector createUnitVector();
		double& operator[] (const int index) ;
		EuclideanVector& operator+= (const EuclideanVector rhs);
		EuclideanVector& operator-= (const EuclideanVector rhs);
		EuclideanVector& operator*= (const double rhs);
		EuclideanVector& operator/= (const double rhs);
		// Convert EuclideanVector to a vector of doubles.
		operator std::vector<double>() {
			std::vector<double> temp;
			for(unsigned int i = 0; i < dimension_ ; ++i){
				temp.push_back(v_[i]);
			}
			return temp;
		}
		
		// Convert a EuclideanVector to a list of doubles.
		operator std::list<double> () {
		    std::list<double> temp;
    		for(unsigned int i = 0; i < dimension_ ; ++i){
        		temp.push_back(v_[i]);
    		}
    		return temp;
		}

	private:	
		unsigned int dimension_;
		double * v_;
		double norm_ = -1;
	};
	bool operator==(const EuclideanVector lhs, const EuclideanVector rhs);
	bool operator!=(const EuclideanVector lhs,const EuclideanVector rhs);
	EuclideanVector operator+(const EuclideanVector a,const EuclideanVector b);
	EuclideanVector operator-(const EuclideanVector a,const EuclideanVector b);
	double operator*(const EuclideanVector a,const EuclideanVector b);
	EuclideanVector operator*(const EuclideanVector a,const double b);
	EuclideanVector operator*(const double a,const EuclideanVector b);
	EuclideanVector operator/(const EuclideanVector a,const double b);
	std::ostream& operator<<(std::ostream& out, const EuclideanVector& a);
}

#endif 

