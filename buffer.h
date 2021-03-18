#ifndef BIT_BUFFER
#define BIT_BUFFER

#include <stdio.h>
#include <stdlib.h>

struct node{
	struct node *prev;
	unsigned int data;
	unsigned int Y;
	unsigned int X;
	struct node *next;
};

unsigned int nodes = 0;

struct node *head = NULL;
struct node *tail = NULL;

void newNodeP(unsigned int data, unsigned long int pos, unsigned int Y, unsigned int X);
void newNode(unsigned int data, unsigned int Y, unsigned int X)
void delNode(unsigned int pos);
void lrefresh();

void newNode(unsigned int data, unsigned int Y, unsigned int X){
	struct node *new = malloc(20);
	if(new == NULL){
		endwin();
	}
		new->data = data;
		new->Y = Y;
		new->X = X;
		new->next = NULL;

		if(head == NULL){
			new->prev = NULL;
			head = new;
			tail = new;
		}
			new->prev = tail;
			tail->next = new;
			tail = new;

			nodes += 1;
};

void delNode(unsigned int pos){

	struct node *temp = head;
	unsigned int stat = 1;
	while(stat < pos-1){
		temp = temp->next;
		stat += 1;
	};

	struct node *temp1 = temp->next;
	struct node *temp2 = temp1->next;

	temp->next = temp2;
	temp2->prev = temp;

	free(temp1);
};

void lrefresh(){
	struct node *temp = head;
	while(temp->next != NULL){
		printw("%c", temp->data);

		temp = temp->next;
	};
		printw("%c", temp->data);
};


void newNodeP(unsigned int data, unsigned long int pos){
	struct node *new = malloc(20);
	if(new == NULL){
		endwin();
	}
		new->data = data;
		new->Y = Y;
		new->X = X;

		if(head == NULL){
			new->prev = NULL;
			head = new;
			tail = new;
		}
		 	if(pos == 0){
				new->prev = NULL;
			 	new->next = head;
			 	head->prev = new;
			 	head = new;
				nodes += 1;
		  } else if(pos > nodes){
				new->next = NULL;
				new->prev = tail;
			 	tail->next = new;
			 	tail = new;
				nodes += 1;
		  } else{
		 	struct node *temp = head;
		 	unsigned long int stat = 1;

		 	while(stat < pos-1){
				 temp = temp->next;
			 	stat += 1;
			 };

		 	new->next = temp->next;
			 new->prev = temp;
		 	temp->next = new;

			 struct node *ptr = new->next;
			 ptr->prev = new;
			 nodes += 1;
		 };
};

#endif
