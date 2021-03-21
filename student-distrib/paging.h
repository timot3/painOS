#ifndef _PAGING_H
#define _PAGING_H

// constants

#define ONE_KB 1024

void * get_physaddr(void * virtualaddr);

void map_page(void * physaddr, void * virtualaddr, unsigned int flags);

#endif // _PAGING_H
