#include "mymalloc.h"
#include <stdlib.h>
#include <stdio.h>
#include <assert.h>

struct Block{
	int size;	
	int occ;
	struct Block* next;
	struct Block* prev;	
};

//Reduce Code Redundancy 
void* createBlock(struct Block* blk,int size);

//To Keep Track of the start of the linked list
static struct Block* head = NULL;

//To Keep track of the current end of the linked list, makes it easier to ref a blocks previous neighbor than doing ->next->prev
static struct Block* end = NULL;

//Memory Allocation using the best fit algorithim described on the Memory Management slide in class
void* my_malloc(int size){
	//Make sure the size to be allocated is greater than zero, if not then don't bother r	     //unning through algorithim 
	if(size <= 0){
		return NULL;
	}	
	if(head == NULL){
		head = sbrk(size+sizeof(struct Block));
		end = head;
		head->size = size+sizeof(struct Block);
		head->occ = 1;
		head->next = NULL;
		head->prev = NULL;
		return ((char*)head)+sizeof(struct Block);
	}else{
		struct Block* current = head;
		struct Block* best = NULL;
		while(current != NULL){
			//If the block is a perfect fit, then we can stop right here, slightl			     //being more efficent I suppose
			if(current->size == size+sizeof(struct Block) && current->occ == 0){
				best = current;
				best->occ = 1;
				return ((char*)best)+sizeof(struct Block);	
			}else{
				if(current->occ == 0 && current->size > size+sizeof(struct Block)){
					if(best == NULL ||  best->size > current->size){
						best = current;
					}
				}
			}
			current = current->next;
		}
	

		//When there exists no Block that cant fit the curretn data size, then we 
		//have to create a new block
		if(best == NULL){
			return createBlock(best,size);
		}
		
		//In Order to fight against fragmentation, if the block contains left over 
		//space that is the same size or bigger than the size of the allocated memory
		//we should split that into another seperate block,
		if(best->size > ((sizeof(struct Block)*2))+size){
			current = (struct Block*)(((char*)best)+(sizeof(struct Block)+size));
			current->next = best->next;
			current->prev = best;
			current->next->prev = current;
			current->occ = 0;
			current->size = (best->size-((sizeof(struct Block)+size)));
			best->occ = 1;
			best->size = size+sizeof(struct Block);
			best->prev = best->prev;
			best->next = current;
			return ((char*)best)+sizeof(struct Block);
		}else{
			best->occ = 1;
			return ((char*)best)+sizeof(struct Block);
		}	
		
	}
}


void* createBlock(struct Block* blk,int size){
	if(head == NULL){
		end = head;
	}
	
	blk = sbrk(sizeof(struct Block)+size);
	
	blk->occ = 1;
	blk->size = size+sizeof(struct Block);
	blk->prev = end;
	blk->next = NULL;
	end->next = blk;
	end = blk;	
	return ((char*)blk)+sizeof(struct Block);
}



void my_free(void* data){
	struct Block* current = data-sizeof(struct Block);
	current->occ = 0;
	if(current != end){

		if(current != head && current->prev->occ == 0){
			//Since the block is not the starting point we can coalesce with the
			//one before it
			current->prev->next = current->next;
			current->next->prev = current->prev;
			current->prev->size = current->prev->size + current->size;
			current = current->prev;
		}
		
		//Else just preform regualr coalescing	
		if(current->next->occ == 0){
			current->size = current->next->size+current->size;
			current->next = current->next->next;
			current->next->prev = current;	
		}

	
	}else{
		end = current;
		
		if(current != head){
			end = current->prev;
			end->next =  NULL;
			sbrk(-1*current->size);
			if(end->occ == 0){
				my_free(((char*)end)+sizeof(struct Block));
			}


		}else{
		//This means that the heap is now empty, as the current block is equal to
		//the start of the heap at the head, so set the brk point back to begining
		//by taking the negative of the size of the current block	
			head = NULL;
			end = NULL;
			sbrk(-1*current->size);
			return;
	
			
		}
	}
}


void dump_heap()
{
  struct Block *cur;
  printf("brk: %p\n", sbrk(0));
  printf("head->");
  for(cur = head; cur != NULL; cur = cur->next) {
    printf("[%d:%d:%d]->", cur->occ, (char*)cur - (char*)head, cur->size);
    fflush(stdout);
    assert((char*)cur >= (char*)head && (char*)cur + cur->size <= (char*)sbrk(0)); // check that block is within bounds of the heap
    if(cur->next != NULL) {
      assert(cur->next->prev == cur); // if not last block, check that forward/backward links are consistent
      assert((char*)cur + cur->size == (char*)cur->next); // check that the block size is correctly set
    }
  }
  printf("NULL\n");
}
