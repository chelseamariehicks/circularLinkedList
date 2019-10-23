/***********************************************************
* Author: Chelsea Hicks
* Email: hicksche@oregonstate.edu
* Date Created: 7/13/2019
* Filename: circularList.c
*
* Overview:
*   This program is a circular doubly linked list implementation
*	of a deque with a front sentinel.
*	It allows for the following behavior:
*		- adding a new link to the front/back
*		- getting the value of the front/back links
*		- removing the front/back link
*		- checking if the deque is empty
*		- printing the values of all the links
*		- reversing the order of the links
*
*	Note that this implementation uses double links (links with
*	next and prev pointers) and that given that it is a circular
*	linked deque the last link points to the sentinel and the first
*	link points to the Sentinel -- instead of null.
************************************************************/
#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include "circularList.h"

#ifndef FORMAT_SPECIFIER
#define FORMAT_SPECIFIER "%g"
#endif

// Double link
struct Link
{
	TYPE value;
	struct Link * next;
	struct Link * prev;
};

struct CircularList
{
	int size;
	struct Link * sentinel;
};

/**
  	Allocates the deque's sentinel and sets the size to 0.
  	The sentinel's next and prev should point to the sentinel itself.
 	param: 	deque 	struct CircularList ptr
	pre: 	deque is not null
	post: 	deque sentinel not null
			sentinel next points to sentinel
			sentinel prev points to sentinel
			deque size is 0
 */
static void init(struct CircularList * deque)
{
	assert(deque != NULL);
	struct Link * newLink = malloc(sizeof(struct Link));
	assert(newLink != NULL);

	newLink->next = newLink;
	newLink->prev = newLink;
	deque->size = 0;

	deque->sentinel = newLink;
	deque->sentinel->value = 0;
}

/**
	Creates a link with the given value and NULL next and prev pointers.
	param: 	value 	TYPE
	pre: 	none
	post: 	newLink is not null
			newLink value init to value
			newLink next and prev init to NULL
 */
static struct Link * createLink(TYPE value)
{
	struct Link * newLink = malloc(sizeof(struct Link));
	assert(newLink != NULL);

	newLink->next = NULL;
	newLink->prev = NULL;
	newLink->value = value;

	return newLink;
}

/**
	Adds a new link with the given value after the given link and
	increments the deque's size.
	param: 	deque 	struct CircularList ptr
 	param:	link 	struct Link ptr
 	param: 	TYPE
	pre: 	deque and link are not null
	post: 	newLink is not null
			newLink w/ given value is added after param link
			deque size is incremented by 1
 */
static void addLinkAfter(struct CircularList * deque, struct Link * link, TYPE value)
{
	assert(deque != NULL);
	struct Link * newLink = createLink(value);
	assert(newLink != NULL);

	newLink->next = link->next;
	newLink->prev = link;

	link->next->prev = newLink;
	link->next = newLink;

	deque->size++;
}

/**
	Removes the given link from the deque and decrements the deque's size.
	param: 	deque 	struct CircularList ptr
 	param:	link 	struct Link ptr
	pre: 	deque and link are not null
	post: 	param link is removed from param deque
			memory allocated to link is freed
			deque size is decremented by 1
 */
static void removeLink(struct CircularList * deque, struct Link * link)
{
	assert(deque != NULL);
	assert(link != NULL);

	struct Link * current = deque->sentinel;

	while(current->next != link)
	{
		current = current->next;	
	}

	current->next = link->next;

	free(link);

	deque->size--;
}

/**
	Allocates and initializes a deque.
	pre: 	none
	post: 	memory allocated for new struct CircularList ptr
			deque init (call to init func)
	return: deque
 */
struct CircularList * circularListCreate()
{
	struct CircularList * deque = malloc(sizeof(struct CircularList));
	init(deque);
	return deque;
}

/**
	Deallocates every link in the deque and frees the deque pointer.
	pre: 	deque is not null
	post: 	memory allocated to each link is freed
			" " sentinel " "
			" " deque " "
 */
void circularListDestroy(struct CircularList * deque)
{
	assert(deque != NULL);

	struct Link * current = deque->sentinel;
	struct Link * next;

	while (current != NULL)
	{
		next = current->next;
		free(current);
		current = NULL;
		current = next;
	}

	free(deque->sentinel);
	deque->sentinel = NULL;

	free(deque);
	deque = NULL;
}

/**
	Adds a new link with the given value to the front of the deque.
	param:	deque 	struct CircularList ptr
	param: 	value 	TYPE
	pre: 	deque is not null
	post: 	link is created w/ given value before current first link
			(call to addLinkAfter)
 */
void circularListAddFront(struct CircularList * deque, TYPE value)
{
	assert(deque != NULL);

	addLinkAfter(deque, deque->sentinel, value);
}

/**
	Adds a new link with the given value to the back of the deque.
	param: 	deque 	struct CircularList ptr
	param: 	value 	TYPE
	pre: 	deque is not null
	post: 	link is created w/ given value after the current last link
			(call to addLinkAfter)
 */
void circularListAddBack(struct CircularList * deque, TYPE value)
{
	assert(deque != 0);

	addLinkAfter(deque, deque->sentinel->prev, value);
}

/**
	Returns the value of the link at the front of the deque.
	param: 	deque 	struct CircularList ptr
	pre:	deque is not null
	pre:	deque is not empty
	post:	none
	ret:	first link's value
 */
TYPE circularListFront(struct CircularList * deque)
{
	assert(deque != NULL);
	assert(!circularListIsEmpty(deque));

	return deque->sentinel->next->value;
}

/**
  	Returns the value of the link at the back of the deque.
	param: 	deque 	struct CircularList ptr
	pre:	deque is not null
	pre:	deque is not empty
	post:	none
	ret:	last link's value
 */
TYPE circularListBack(struct CircularList * deque)
{
	assert(deque != NULL);
	assert(!circularListIsEmpty(deque));

	return deque->sentinel->prev->value;
}

/**
	Removes the link at the front of the deque.
	param: 	deque 	struct CircularList ptr
	pre:	deque is not null
	pre:	deque is not empty
	post:	first link is removed and freed (call to removeLink)
 */
void circularListRemoveFront(struct CircularList * deque)
{
	assert(deque != NULL);
	assert(!circularListIsEmpty(deque));

	removeLink(deque, deque->sentinel->next);
}

/**
	Removes the link at the back of the deque.
	param: 	deque 	struct CircularList ptr
	pre:	deque is not null
	pre:	deque is not empty
	post:	last link is removed and freed (call to removeLink)
 */
void circularListRemoveBack(struct CircularList * deque)
{
	assert(deque != NULL);
	assert(!circularListIsEmpty(deque));

	removeLink(deque, deque->sentinel->prev);
}

/**
	Returns 1 if the deque is empty and 0 otherwise.
	param:	deque	struct CircularList ptr
	pre:	deque is not null
	post:	none
	ret:	1 if its size is 0 (empty), otherwise 0 (not empty)
 */
int circularListIsEmpty(struct CircularList * deque)
{
	assert(deque != NULL);

	return deque->size == 0;
}

/**
	Prints the values of the links in the deque from front to back.
	param:	deque	struct CircularList ptr
	pre:	deque is not null
	post:	none
	ret:	outputs to the console the values of the links from front
			to back; if empty, prints msg that is empty
 */
void circularListPrint(struct CircularList * deque)
{
	assert(deque != NULL);

	struct Link * current = deque->sentinel->next;
	int i = 0;
	assert(current != NULL);

	if (circularListIsEmpty(deque))
	{
		printf("The linked list is empty.");
	}
	else
	{
		while (i < deque->size)
		{
			printf("%g \n", current->value);

			current = current->next;
			i++;
		}	
	}
}

/**
	Reverses the deque in place without allocating any new memory.
	The process works as follows: current starts pointing to sentinel;
	tmp points to current's next, current's next points to current's prev,
	current's prev is assigned to tmp and current points to current's next
	(which points to current's prev), so you proceed stepping back through
	the deque, assigning current's next to current's prev, until current
	points to the sentinel then you know the each link has been looked at
	and the link order reversed.
	param: 	deque 	struct CircularList ptr
	pre:	deque is not null
	pre:	deque is not empty
	post:	order of deque links is reversed
 */
void circularListReverse(struct CircularList * deque)
{
	assert(deque != NULL);
	assert(!circularListIsEmpty(deque));

	struct Link * prev = NULL;
	struct Link * current = deque->sentinel->next;
	struct Link * next;

	int i = 0;

	while (i < deque->size)
	{
		next = current->next;
		current->next = prev;
		prev = current;
		current = next;
		i++;
	}
	deque->sentinel->next = prev;

}
