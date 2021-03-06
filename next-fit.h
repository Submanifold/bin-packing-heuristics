/*!
	@file	next-fit.h
	@brief	Prototypes for "Next-Fit" and "Next-Fit-Decreasing" heuristics

	@author Bastian Rieck
*/

#ifndef NEXT_FIT_H
#define NEXT_FIT_H

unsigned int next_fit(const unsigned int*, unsigned int*, double&);
unsigned int next_fit_decreasing(	const unsigned*,
					double&,
					void (*)(void*, size_t, size_t, int (*)(const void*, const void*)));

#endif
