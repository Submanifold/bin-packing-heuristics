/*!
	@file 	best-fit.cpp
	@brief	Implementations of the "Best-Fit" heuristic

	@author Bastian Rieck
*/

#include <iostream>
#include <queue>

#include <cstring>
#include <ctime>
#include <cstdlib>
#include <cmath>

#include "bin-packing.h"
#include "simple-heap.h"

/*!
	Performs the "Best-Fit" heuristic for the current problem. Worst-case
	running time is O(n^2).
*/

unsigned int best_fit(const unsigned* objects, unsigned int* positions, double& time)
{
	unsigned int num_open_bins = 1;
	unsigned int num_full_bins = 0;
	unsigned int* bins = new unsigned int[n];
	
	memset(bins, 0, n*sizeof(unsigned int));
	memset(positions, 0, n*sizeof(unsigned int));

	unsigned int limit_capacity = K-min_size;
	
	clock_t start = clock();
	for(unsigned int i = 0; i < n; i++)
	{
		unsigned int best_bin = n; // best bin that has been determined so far
		unsigned int best_cap = 0; // capacity for said bin if the object has been added

		for(unsigned int j = 0; j < num_open_bins; j++)
		{
			unsigned int temp_cap = bins[j]+objects[i];
			if(temp_cap <= K && temp_cap > best_cap)
			{
				best_bin = j;
				best_cap = temp_cap;
			}
		}

		// Best bin has been found...
		if(best_bin < n)
		{
			bins[best_bin] += objects[i];
			positions[i] = best_bin;

			// Remove (almost) full bins
			if(bins[best_bin] > limit_capacity)
			{
				num_open_bins--;
				num_full_bins++;
				bins[best_bin] = bins[num_open_bins];
			}
		}

		// ...else create a new one
		else
		{
			bins[num_open_bins] = objects[i];
			positions[i] = num_open_bins++;
		}
	}

	clock_t end = clock();
	time = (end-start)/static_cast<double>(CLOCKS_PER_SEC);

	delete[] bins;
	return(num_open_bins+num_full_bins);

}

/*!
	An implementation of the "Best-Fit" heuristic that uses a heap in order
	to determine the best bin more rapidly.
*/

unsigned int best_fit_heap(const unsigned* objects, double& time)
{
	unsigned int num_bins = 0;

	simple_heap bins(n);
	std::queue<unsigned int> heap_queue;
	
	clock_t start = clock();
	for(unsigned int i = 0; i < n; i++)
	{
		unsigned int best_bin = n; // best bin that has been determined so far
		unsigned int best_cap = 0; // capacity for said bin if the object has been added

    		if(num_bins != 0 && (bins.elements[1] + objects[i]) <= K)
                {
			// Perform a breadth-first-search through the bin
                        heap_queue.push(1);
                        while(!heap_queue.empty())
                        {
                                unsigned int j = heap_queue.front();
                                unsigned int temp_cap = bins.elements[j]+objects[i];
                                if(temp_cap <= K)
                                {
                                        if(temp_cap > best_cap)
                                        {
                                                best_bin = j;
                                                best_cap = temp_cap;
                                        }

                                        heap_queue.pop();

					// Check whether the indices are still in range
                                        if(2*j <= num_bins)
                                                heap_queue.push(2*j);
                                        if((2*j+1) <= num_bins)
                                                heap_queue.push(2*j+1);
                                }
                                else
                                        heap_queue.pop();
                        }
                }

		// Best bin has been found...
		if(best_bin < n)
		{
			bins.elements[best_bin] += objects[i];
			bins.reheap_down(best_bin);
		}

		// ...else create a new one
		else
		{
			bins.push(objects[i]);
			num_bins++;
		}
	}

	clock_t end = clock();
	time = (end-start)/static_cast<double>(CLOCKS_PER_SEC);

	return(num_bins);
}

/*!
	An implementation of the "Best-Fit" heuristic that uses a lookup table
	to determine the proper bin more rapidly. The running time thus
	decreases to O(n*K).
*/

unsigned int best_fit_lookup(const unsigned* objects, double& time)
{
        unsigned int num_bins = 0;
        unsigned int* bin_count = new unsigned int[K+1];
        memset(bin_count, 0, (K+1)*sizeof(unsigned int));

	// At the beginning of the algorithm, there are n bins with a remaining
	// capacity of K.
        bin_count[K] = n;

        unsigned int req_size = 0; 	// Minimum required remaining capacity; finding a bin
					// with this capacity would be optimal.
        unsigned int cur_size = 0; 	// Stores current capacity while searching for a 
					// suitable bin. 

        clock_t start = clock();
        for(unsigned int i = 0; i < n; i++)
        {
                req_size  = objects[i];
                cur_size  = objects[i];

                while(bin_count[cur_size] == 0)
			cur_size++;

                bin_count[cur_size]--;
                bin_count[cur_size-req_size]++;
        }

        clock_t end = clock();
        time = (end-start)/static_cast<double>(CLOCKS_PER_SEC);
        
	for(unsigned int i = 0; i < K; i++)
                num_bins += bin_count[i];

        delete[] bin_count;
        return(num_bins);
}
