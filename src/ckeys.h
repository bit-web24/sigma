#ifndef C_KEYS
#define C_KEYS

#ifndef ENTER
#define ENTER 10
#endif

#ifndef BACKSPACE
#define BACKSPACE 127
#endif

#ifndef ZOOM_IO
#define ZOOM_IO 410
#endif

struct node {
	struct node *prev;
	unsigned int data;
	int x;
	int y;
	struct node *next;
};

#endif
