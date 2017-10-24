#include <fstream>
#include <iostream>
#include <algorithm>
#include <iterator> 
#include <vector>
#include <list>
#include <cmath>
#include "EuclideanVector.h"

evec::EuclideanVector::EuclideanVector() : EuclideanVector{0.0} {}

// Constructor with just the dimension given.
evec::EuclideanVector::EuclideanVector(unsigned int dimension) :
    dimension_{dimension} {
    v_ = new double [dimension];
    for(unsigned int i = 0;i < dimension;++i){
        v_[i] = 0.0;
    }
}
// Dimension and magnitude input constructor
evec::EuclideanVector::EuclideanVector(unsigned int dimension,double magnitude) :
    dimension_{dimension} {
    v_ = new double [dimension];
    for(unsigned int i = 0;i <dimension;++i){
        v_[i] = magnitude;
    }
}
// List input constructor
evec::EuclideanVector::EuclideanVector(std::initializer_list<double> input) {
    dimension_ = input.size();
    v_ = new double [dimension_];
    int pos = 0;
    for(auto i : input){
        v_[pos] = i;
        pos++;
    }
}
// Copy consturctor
evec::EuclideanVector::EuclideanVector(const EuclideanVector& evec) :
    dimension_{evec.dimension_}
{
    v_ = new double[dimension_];
    for(unsigned int i = 0; i < evec.dimension_;++i){
        v_[i] = evec.v_[i];
    }
}
// Move constructor
evec::EuclideanVector::EuclideanVector(EuclideanVector&& evec) :
    dimension_{evec.dimension_}
{
    v_ = evec.v_;
    dimension_ = evec.dimension_;
    evec.v_ = nullptr;
    evec.dimension_ = 0;
}
// Destructor 
evec::EuclideanVector::~EuclideanVector()
{
    delete[] v_;
}

// Move operator
evec::EuclideanVector& evec::EuclideanVector::operator=(EuclideanVector&& evec){
    delete v_;
    dimension_ = evec.dimension_;
    for(unsigned int i = 0; i < dimension_ ;++i){
        v_[i] = evec.v_[i];
    }
    evec.v_ = nullptr;
    return *this;
}

// Copy operator 
evec::EuclideanVector& evec::EuclideanVector::operator=(EuclideanVector& evec){
    dimension_ = evec.dimension_;
    for(unsigned int i = 0; i < dimension_ ;++i){
        v_[i] = evec.v_[i];
    }
    return *this;
}

const unsigned int evec::EuclideanVector::getNumDimensions() const { return dimension_; }
const double evec::EuclideanVector::get(unsigned int dimension) const { return v_[dimension]; }

 // Returns the norm of the current vector
 double evec::EuclideanVector::getEuclideanNorm() {
    if(norm_ == -1){
        norm_ = 0;
        for(unsigned int i = 0; i < dimension_  ; ++i){
            norm_ += v_[i]*v_[i];
        }
        norm_ = sqrt(norm_);
        return norm_;
    }
    return norm_;
}
// Creates a a unit vector which is the current vector divided by the norm.
 evec::EuclideanVector evec::EuclideanVector::createUnitVector() {
    // If norm has never been initialised calculate.
    if(norm_ == -1){
        getEuclideanNorm();
    }
    EuclideanVector newVec = EuclideanVector(dimension_);
    for(unsigned int i = 0; i < dimension_ ; ++i){
       newVec.v_[i] = v_[i]/norm_;
    }
    norm_ = -1;
    return newVec;
}


double& evec::EuclideanVector::operator[] (int index) {
    return v_[index];
}

// Addition Operator
evec::EuclideanVector& evec::EuclideanVector::operator+= (const EuclideanVector rhs){
    for(unsigned int i = 0; i < dimension_;++i){
        v_[i] += rhs.v_[i];
    }
    return *this;
}

// Subtraction operator
evec::EuclideanVector& evec::EuclideanVector::operator-= (const EuclideanVector rhs){
    for(unsigned int i = 0; i < dimension_;++i){
        v_[i] -= rhs.v_[i];
    }
    return *this;
}

// Multiplication Operator
evec::EuclideanVector& evec::EuclideanVector::operator*= (const double rhs){
    for(unsigned int i = 0; i < dimension_;++i){
        v_[i] *= rhs;
    }
    return *this;
}
// Division Operator
evec::EuclideanVector& evec::EuclideanVector::operator/= (const double rhs){
    for(unsigned int i = 0; i < dimension_;++i){
        v_[i] /= rhs;
    }
    return *this;
}

// Check equals operator 
bool evec::operator==(const EuclideanVector lhs,const EuclideanVector rhs){
    for(unsigned int i = 0; i < lhs.dimension_;++i){
        if(lhs.v_[i] == rhs.v_[i]){
            return true;
        }
    }
    return false;
}

// Check not equals operator
bool evec::operator!=(const EuclideanVector lhs,const EuclideanVector rhs){
    for(unsigned int i = 0; i < lhs.dimension_;++i){
        if(lhs.v_[i] == rhs.v_[i]){
            return false;
        }
    }
    return true;
}

// Addition of two vectors
evec::EuclideanVector evec::operator+(const EuclideanVector a,const EuclideanVector b){
    EuclideanVector temp;
    temp.dimension_ = a.dimension_;
    temp.v_ = new double[a.dimension_];
    for(unsigned int i = 0; i < a.dimension_;++i){
        temp.v_[i] = a.v_[i] + b.v_[i];
    }
    return temp;
}

// Subtraction of two vectors
evec::EuclideanVector evec::operator-(const EuclideanVector a,const EuclideanVector b){
    EuclideanVector temp;
    temp.dimension_ = a.dimension_;
    temp.v_ = new double[a.dimension_];
    for(unsigned int i = 0; i < a.dimension_;++i){
        temp.v_[i] = a.v_[i] - b.v_[i];
    }
    return temp;
}

// Dot product 
double evec::operator*(const EuclideanVector a,const EuclideanVector b){
    double total = 0;
    for(unsigned int i = 0; i < a.dimension_;++i){
        total += a.v_[i]*b.v_[i];
    }
    return total;
}

// Multiplying vector by a double 
evec::EuclideanVector evec::operator*(const EuclideanVector a,const double b){
    EuclideanVector temp;
    temp.dimension_ = a.dimension_;
    temp.v_ = new double[a.dimension_];
    for(unsigned int i = 0; i < a.dimension_;++i){
        temp.v_[i] = a.v_[i]*b;
    }
    return temp;
}

// Multiplying double by a vector
evec::EuclideanVector evec::operator*(double a,const EuclideanVector b){
    EuclideanVector temp;
    temp.dimension_ = b.dimension_;
    temp.v_ = new double[b.dimension_];
    for(unsigned int i = 0; i < b.dimension_;++i){
        temp.v_[i] = a*b.v_[i];
    }
    return temp;
}

// Scalar division of a vector
evec::EuclideanVector evec::operator/(const EuclideanVector a,const double b){
    EuclideanVector temp;
    temp.dimension_ = a.dimension_;
    temp.v_ = new double[a.dimension_];
    for(unsigned int i = 0; i < a.dimension_;++i){
        temp.v_[i] = a.v_[i]/b;
    }
    return temp;
}

// Print Vector
std::ostream& evec::operator<<(std::ostream& out, const EuclideanVector& a){
    out << "[";
    // Check if there is an array to print.
    if(a.v_ == nullptr){
        out <<"]";
        return out;
    }
    unsigned int temp = 0;
    for(unsigned int i = 0; i < a.dimension_ - 1;++i){
        out << a.v_[i] << " ";
        temp = i+1;
    }
    out << a.v_[temp] <<"]";
    return out;
}