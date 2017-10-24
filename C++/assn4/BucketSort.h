#ifndef BUCKET_SORT_H
#define BUCKET_SORT_H

#include <vector>

struct BucketSort {
	
	// vector of numbers
	std::vector<unsigned int> numbersToSort;
	// total cores allowed to be used.
	int totCores;

	void sort(unsigned int numCores);
	void sortNormal();

};


#endif /* BUCKET_SORT_H */