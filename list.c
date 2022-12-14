// list/list.c
// 
// Implementation for linked list.

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "list.h"

list_t *list_alloc() { 
  list_t* list = (list_t*)malloc(sizeof(list_t));
  list->head = NULL;
  return list; 
}

node_t *node_alloc(block_t *blk) {   
  node_t* node = malloc(sizeof(node_t));
  node->next = NULL;
  node->blk = blk;
  return node; 
}

void list_free(list_t *l){
  free(l);
  return;
}

void node_free(node_t *node){
  free(node);
  return;
}

void list_print(list_t *l) {
  node_t *current = l->head;
  block_t *b;
  
  if (current == NULL){
    printf("list is empty\n");
  }
  while (current != NULL){
    b = current->blk;
    printf("PID=%d START:%d END:%d", b->pid, b->start, b->end);
    current = current->next;
  }
}

int list_length(list_t *l) { 
  node_t *current = l->head;
  int i = 0;
  while (current != NULL){
    i++;
    current = current->next;
  }
  
  return i; 
}

// Adds a block to the end of the linked list
void list_add_to_back(list_t *l, block_t blk) {
// Create a new node containing the block
node_t newNode = node_alloc(blk);
// Set the next pointer of the new node to NULL
newNode->next = NULL;

// If the linked list is empty, set the head of the linked list
// to the new node
if(l->head == NULL) {
    l->head = newNode;
}
// If the linked list is not empty, find the last node in the linked
// list and set its next pointer to the new node
else {
    node_t *current = l->head;
    while(current->next != NULL) {
        current = current->next;
    }
    current->next = newNode;
}
}

// Adds a block to the front of the linked list
void list_add_to_front(list_t *l, block_t blk) {
// Create a new node containing the block
node_t newNode = node_alloc(blk);
// Set the next pointer of the new node to the current head of the linked list
newNode->next = l->head;
// Set the head of the linked list to the new node
l->head = newNode;
}

// Adds a block to the linked list at the specified index
void list_add_at_index(list_t *l, block_t *blk, int index) {
// Keep track of the current index and the current node in the linked list
int i = 0;
node_t *current = l->head;

  else if(index > 0){
    while(i < index && current->next != NULL){
      current = current->next;
      i++;
    }
  newNode->next = current->next;
  current->next = newNode;
  }
}

void list_add_ascending_by_address(list_t *l, block_t *newblk){
  
   /*
   * 1. Insert newblk into list l in ascending order based on the START address of the block.
   * 
   *    node_t *c = l.head;
   *    Insert newblk After Current Node if:   newblk->start > c->start
   */
  node_t *current;
  node_t *prev;
  node_t *newNode = node_alloc(newblk);  
  if(l->head == NULL){
    l->head = newNode;
    return;
  }
  if (l->head->blk->start >  newNode->blk->start){
    current = l->head;
    l->head = newNode;
    newNode->next = current;
    return;
  }
  prev = l->head;
  current = l->head->next;
  while(current != NULL){ 
    if (current->blk->start > newNode->blk->start) {
    prev->next = newNode;
    newNode->next = current;
    return;
   }
    prev = current;
    current = current->next;
  }
 prev->next = newNode;
 return;
}

void list_add_ascending_by_blocksize(list_t *l, block_t *newblk){
  node_t *current;
  node_t *prev;
  node_t *newNode = node_alloc(newblk);
  int newblk_size = newblk->end - newblk->start;
  
  if(l->head == NULL){
    l->head = newNode;
    return;
  }
  if (compareSize(newblk_size, l->head->blk)){
    current = l->head;
    l->head = newNode;
    newNode->next = current;
    return;
  }
    prev = l->head;
    current = l->head->next;
  
    

// Set the previous and current nodes to NULL
prev = NULL;
current = NULL;

// Iterate through the linked list until the end is reached
while(current != NULL){
// If the size of the new block is less than the current block, insert the new node before the current node
if (compareSize(newblk_size, current->blk)){
prev->next = newNode;
newNode->next = current;
return;
}
// Otherwise, move to the next node in the list
prev = current;
current = current->next;
}

// If we reach the end of the list, insert the new node after the last node in the list
prev->next = newNode;
return;
}

void list_add_descending_by_blocksize(list_t *l, block_t *blk){
  node_t *current;
  node_t *prev;
  node_t *newNode = node_alloc(blk);
  int newblk_size = blk->end - blk->start;
  int curblk_size;
  
  if(l->head == NULL){
    l->head = newNode;
  }
  else{
    prev = current = l->head;
    
    curblk_size = current->blk->end - current->blk->start + 1;
    
    if(current->next == NULL) {  //only one node in list
       if(newblk_size >= curblk_size) {  // place in front of current node
          newNode->next = l->head;
          l->head = newNode;
       }
       else {   // place behind current node
          current->next = newNode;
          newNode->next = NULL;
       }
    }
    else {  // two or more nodes in list
      
       if(newblk_size >= curblk_size) {  // place in front of current node
          newNode->next = l->head;
          l->head = newNode;
       }
       else {
      
          while(current != NULL && newblk_size <= curblk_size) {
               prev = current;
               current = current->next;
               
               if(current != NULL)  // the last one in the list
                     curblk_size = current->blk->end - current->blk->start;
          }
          prev->next = newNode;
          newNode->next = current;
       }
    }
  }
}

void list_coalese_nodes(list_t *l){ 
  /*
   * 1. Assuming you have passed in a sorted list of blocks based on addresses in ascending order
   * 2. While list is not empty,
   *    a. compare two nodes at a time to see if the prev.END + 1 == current.START, if so, they are physically adjacent
   *    combine them by setting the prev.END = current.END. 
   *    b. If not adjacent go to #6
   * 3. point the prev.NEXT to the current.NEXT to skip over current.
   * 4. Free current
   * 5. go back to #2
   * 6. Advance prev = current, and current = current.NEXT
   * 7. go back to #2
   * 
   * USE the compareSize()
   */
  if(l->head ==NULL || l->head->next == NULL){
    return;
  }
  

// Set the current and previous nodes to the head of the linked list
node_t *curr = l->head;
node_t *prev = l->head->next;

// Iterate through the linked list until the end is reached
while (curr!= NULL) {
// If the previous block and the current block are contiguous in memory, merge them into the previous block
if(prev->blk->end + 1 == curr->blk->start){
prev->blk->end = curr->blk->end;
prev->next = curr->next;
node_free(curr);
curr = prev->next;
}
// Otherwise, move to the next node in the list
else {
prev = curr;
curr = curr->next;
}
}
  }
}

block_t* list_remove_from_back(list_t *l){
  block_t *value = NULL;
  node_t *current = l->head;

  if(l->head != NULL){
    
    if(current->next == NULL) { // one node
         l->head->next = NULL;
         value = current->blk;
         node_free(current);
    }
    else {
         while (current->next->next != NULL){
            current = current->next;
         }
         value = current->blk;
         node_free(current->next);
         current->next = NULL;
    }
  }
  return value;
}

block_t* list_get_from_front(list_t *l) {
  block_t *value = NULL;
  if(l->head == NULL){
    return value;
  }
  else{
    node_t *current = l->head;
    value = current->blk;
  }
  return value; 
}


block_t* list_remove_from_front(list_t *l) { 
  block_t *value = NULL;
  if(l->head == NULL){
    return value;
  }
  else{
    node_t *current = l->head;
    value = current->blk;
    l->head = l->head->next;
    node_free(current);
  }
  return value; 
}

block_t* list_remove_at_index(list_t *l, int index) { 
  int i;
  block_t* value = NULL;
  
  bool found = false;
  
  if(l->head == NULL){
    return value;
  }
  else if (index == 0){
    return list_remove_from_front(l);
  }
  else if (index > 0){
    node_t *current = l->head;
    node_t *prev = current;
    
    i = 0;
    while(current != NULL && !found){
      if(i == index)
          found = true;
      else {
         prev = current;
         current = current->next;
         i++;
      }
    }
    
    if(found) {
      value = current->blk; 
      prev->next = current->next;
      node_free(current);
    }
  }
  return value; 
}

bool compareBlks(block_t* a, block_t *b) {
  
  if(a->pid == b->pid && a->start == b->start && a->end == b->end)
     return true;
  
  return false;
}

bool compareSize(int a, block_t *b) {  // greater or equal
  
  if(a <= (b->end - b->start) + 1)
     return true;
  
  return false;
}

bool comparePid(int a, block_t *b) {
  
  if(a == b->pid)
     return true;
  
  return false;
}


bool list_is_in(list_t *l, block_t* value) { 
  node_t *current = l->head;
  while(current != NULL){
    if(compareBlks(value, current->blk)){
      return true;
    }
    current = current->next;
  }
return false; 
}

block_t* list_get_elem_at(list_t *l, int index) { 
 int i;
  block_t* value = NULL;
  if(l->head == NULL){
    return value;
  }
  else if (index == 0){
    return list_get_from_front(l);
  }
  else if (index > 0){
    node_t *current = l->head;
    
    i = 0;
    while(current != NULL){
      if(i == index)
          return(current->blk);
      else {
         current = current->next;
         i++;
      }
    }
  }
  return value; 
}

int list_get_index_of(list_t *l, block_t* value){
 int i = 0;
 node_t *current = l->head;
 if(l->head == NULL){
    return -1;
  }
  
  while (current != NULL){
   if (compareBlks(value,current->blk)){
     return i;
    }
    current = current->next;
    i++;
  }
  return -1; 
}

/* Checks to see if block of Size or greater exists in the list. */
bool list_is_in_by_size(list_t *l, int Size){ 
  node_t *current = l->head;
  while(current != NULL){
    if(compareSize(Size, current->blk)){
      return true;
    }
    current = current->next;
  }
return false; 
}

/* Checks to see if pid of block exists in the list. */
bool list_is_in_by_pid(list_t *l, int pid){ 
  /* Iterate through the list to find a node with a blk that has blk->pid = pid
   * 
   * USE the comparePID()
   * 
   * Look at list_is_in_by_size()*/
   node_t* head = l->head;
   while (head != NULL) {
     bool answer = comparePid(pid, head->blk);
     if (answer == 1) {
       return 1;
     }
     head = head->next;
   }
  return 0;
}
  
/* Returns the index at which the given block of Size or greater appears. 
 * 
 */
int list_get_index_of_by_Size(list_t *l, int Size){
 int i = 0;
 node_t *current = l->head;
 if(l->head == NULL){
    return -1;
  }
  
  

// Set the current node to NULL and initialize a counter to 0
current = NULL;
i = 0;

// Iterate through the linked list until the end is reached
while (current != NULL){
// If the current block size is greater than or equal to the given size, return the current position in the list
if (compareSize(Size,current->blk)){
return i;
}
// Move to the next node in the list
current = current->next;
// Increment the counter
i++;
}

// If no node with a large enough block size is found, return -1
return -1;

                   
/* Returns the index at which the pid appears. */
int list_get_index_of_by_Pid(list_t *l, int pid){
 int i = 0;
 node_t *current = l->head;
 if(l->head == NULL){
    return -1;
  }
  
  while (current != NULL){
   if (comparePid(pid,current->blk)){
     return i;
    }
    current = current->next;
    i++;
  }

  return -1; 
}
