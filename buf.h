#ifndef BUFFER_H
#define BUFFER_H

#include <stdio.h>
#include <stdlib.h>
#include <error.h>
#include <stdbool.h>

#ifndef ENTER
#define ENTER 10
#endif

extern int errno;

struct node{
	struct node *prev;
	unsigned int data;
	struct node *next;
};

int nodes = 0;

struct node *head = NULL;
struct node *tail = NULL;
/*
void newNode(unsigned int data){
	struct node *new_node = (struct node *) malloc(sizeof(struct node));
	if(new_node == NULL){
		perror("Error: ");
		exit(-1);
	}	
		new_node->data = data;
		new_node->next = NULL;

		if(head == NULL){
			new_node->prev = NULL;
			head = new_node;
			tail = new_node;
		}
			new_node->prev = tail;
			tail->next = new_node;
			tail = new_node;
			nodes += 1;
};
*/
void lrefresh(WINDOW *window){
	struct node *temp = head;
	unsigned int X = 0;
	unsigned int Y = 0;
if(temp != NULL){
	for(int i = 0; i < nodes; i++){
		if(temp->data == ENTER){
			mvwprintw(window, Y, X, "%c", (char) temp->data);
			temp = temp->next;
			X = 0;
			Y += 1;
		} else{ 
			mvwprintw(window, Y, X, "%c", (char) temp->data);
		};

		temp = temp->next;
		X += 1;
		refresh();
		wrefresh(window);
	};

	X = 0;
	Y = 0;
}
};

void delNode(unsigned int pos){
	if(nodes > 0){
		struct node *temp = head;
		unsigned int stat = 0;
		while(stat < pos){
			temp = temp->next;
			stat += 1;
		};

		struct node *temp1 = temp->next;
		struct node *temp2 = temp1->next;

		temp->next = temp2;
		temp2->prev = temp;
		nodes -= 1;
	
		free(temp1);
	}
};

void del(){
	if(nodes > 0){
	struct node *temp = tail;
	tail = tail->prev;
	tail->next = NULL;
	nodes -= 1;

	free(temp);
	} 

};

/*void newNodeP(unsigned int data, unsigned long int pos, unsigned int Y, unsigned int X){
	struct node *new_node = (struct node *) malloc(sizeof(struct node));
	if(ne == NULL){
		perror("Error: ");
		exit(-1);
	}
		new_node->data = data;
		ne->Y = Y;
		ne->X = X;

		if(head == NULL){
			ne->prev = NULL;
			head = ne;
			tail = ne;
		}
		 if(pos == 0){
				ne->prev = NULL;
			 	ne->next = head;
			 	head->prev = ne;
			 	head = ne;
				nodes += 1;
		  } else if(pos > nodes){
				ne->next = NULL;
				ne->prev = tail;
			 	tail->next = ne;
			 	tail = ne;
				nodes += 1;
		  } else{
		 	struct node *temp = head;
		 	unsigned long int stat = 1;

		 	while(stat < pos-1){
				temp = temp->next;
			 	stat += 1;
			 };

		 	ne->next = temp->next;
			ne->prev = temp;
		 	temp->next = ne;

			 struct node *ptr = ne->next;
			 ptr->prev = ne;
			 nodes += 1;
		 };
};
*/
#endif
