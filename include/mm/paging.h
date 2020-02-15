#ifndef __PAGING_H__
#define __PAGING_H__

#include <stdint.h>

union page {
	uint32_t present	: 1; //Is in memory
	uint32_t rw			: 1; //Is readonly?
	uint32_t user		: 1; //Supervisor only?
	uint32_t accessed	: 1; //Has page been read since last sync?
	uint32_t dirty		: 1; //Has page been written to since last sync?
	uint32_t unused		: 7; //Unused and reserved bits
	uint64_t frame		: 20;//Frame address shifted right by 12
}

typedef union page* pages;

struct page_directory {

}


#endif