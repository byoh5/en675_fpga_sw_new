#ifndef __VIDENC_HEAP__H__
#define __VIDENC_HEAP__H__

typedef struct {
	char filename[24];
} encHData;

typedef struct {
	int usedsize;
	int capacity;
	encHData* node;
	encHData delHData;
} encHeap;

#define encHeap_isEmpty(h)	((h)->usedsize == 0)

extern ENX_OKFAIL encHeap_create(encHeap* h, int capacity);
extern void encHeap_destroy(encHeap* h);
extern void encHeap_resetQueue(encHeap* h);
extern void encHeap_deleteQueue(encHeap* h, char* filename);
extern int encHeap_addQueue(encHeap* h, char* filename);

#endif	// __VIDENC_HEAP__H__
