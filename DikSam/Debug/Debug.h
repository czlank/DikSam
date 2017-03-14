#ifndef	DEBUG_H
#define	DEBUG_H

#include "MEM.h"
#include "DBG.h"

struct DBG_Controller_tag
{
	FILE	*debug_write_fp;
	int		 current_debug_level;
};

#endif	/* DEBUG_H */
