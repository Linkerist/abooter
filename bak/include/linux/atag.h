
#ifndef __ATAG_H__
#define __ATAG_H__

#define ATAG_CORE	0x54410001
#define ATAG_MEM	0x54410002
#define ATAG_CMDLINE	0x54410009
#define ATAG_NONE	0x00000000

struct tag_header {
	unsigned int size;
	unsigned int tag;
};

struct tag_core {
	unsigned int flags;		
	unsigned int pagesize;
	unsigned int rootdev;
};

struct tag_mem32 {
	unsigned int	size;
	unsigned int	start;	
};

struct tag_cmdline {
	char	cmdline[1];	
};

struct tag {
	struct tag_header hdr;
	union {
		struct tag_core		core;
		struct tag_mem32	mem;
		struct tag_cmdline	cmdline;
	} u;
};

#define tag_size(type)	((sizeof(struct tag_header) + sizeof(struct type)) >> 2)
#define tag_next(t)	((struct tag *)((unsigned int *)(t) + (t)->hdr.size))

#endif

