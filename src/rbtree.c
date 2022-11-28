#include "rbtree.h"
#include <stdlib.h>
#include <stdio.h>

rbtree *new_rbtree(void) {
    // * tree = new_tree(): RB tree 구조체 생성
    // * 여러 개의 tree를 생성할 수 있어야 하며 각각 다른 내용들을 저장할 수 있어야 합니다.
  rbtree *p = (rbtree *)calloc(1, sizeof(rbtree));

  p->nil = (node_t *)calloc(1, sizeof(node_t));
  p->nil->color = RBTREE_BLACK;
  p->root = p->nil;
  return p;
}

void right_rotate(rbtree *t, node_t *y){
    node_t *x = y->left;
    y->left = x->right;
    
    if (x->right != t->nil)
      x->right->parent = y;
    
    x->parent = y->parent;
    if (y->parent == t->nil)
      t->root = x;
    
    else if(y == y->parent->left)
      y->parent->left = x;
    
    else
      y->parent->right = x;
    
    x->right = y;
    y->parent = x;
}

void left_rotate(rbtree *t, node_t *x){
    node_t *y = x->right;
    x->right = y->left;
    
    if (y->left != t->nil)
      y->left->parent = x;
    
    y->parent = x->parent;
    if (x->parent == t->nil)
      t->root = y;
    
    else if(x == x->parent->left)
      x->parent->left = y;
    
    else
      x->parent->right = y;
    
    //connect y to x 
    y->left = x;
    x->parent = y;
}


node_t *rbtree_insert_fixup(rbtree *t, node_t *z){
  while (z->parent->color == RBTREE_RED){

    if( z->parent == z->parent->parent->left){
      //y is uncle
      node_t *y = z->parent->parent->right;
      if (y->color == RBTREE_RED){
        //case1 parent's and uncle's colors are red
        z->parent->color = RBTREE_BLACK;
        y->color = RBTREE_BLACK;
        z->parent->parent->color = RBTREE_RED;
        z = z->parent->parent;
      }
      //uncle's color is black
      else{
        if( z == z->parent->right ){
        //case2, parent is red, uncle is black and
        //the order of grandfather,parents and children is bent
          z = z->parent;
          left_rotate(t, z);
       }
       //case3, The order of grandfater, parents and children is in line.
        z->parent->color = RBTREE_BLACK;
        z->parent->parent->color = RBTREE_RED;
        right_rotate(t, z->parent->parent);
      } 
    }
    else{
      //"right" <-> "left"
      node_t *y = z->parent->parent->left;
      if (y->color == RBTREE_RED){
        //case1 parent's and uncle's colors are red
        z->parent->color = RBTREE_BLACK;
        y->color = RBTREE_BLACK;
        z->parent->parent->color = RBTREE_RED;
        z = z->parent->parent;
      }
      //uncle's color is black
      else{
        if( z == z->parent->left ){
        //case2, parent is red, uncle is black and
        //the order of grandfather,parents and children is bent
          z = z->parent;
          right_rotate(t, z);
       }
       //case3, The order of grandfater, parents and children is in line.
        z->parent->color = RBTREE_BLACK;
        z->parent->parent->color = RBTREE_RED;
        left_rotate(t, z->parent->parent);
      } 
    }
  }
  t->root->color = RBTREE_BLACK;
  return t->root;
}
node_t *rbtree_insert(rbtree *t, const key_t key) {

  // TODO: implement insert
  //   * 구현하는 ADT가 multiset이므로 이미 같은 key의 값이 존재해도 하나 더 추가 합니다.
  node_t *z = (node_t*)calloc(1,sizeof(node_t));
  z->key = key;

  node_t *y = t->nil;
  node_t *x = t->root;

  //find insert z's position
  while (x != t->nil){
      y = x;
      if (z->key < x->key)
        x = x->left;
      else
        x = x->right;
  }
  z->parent = y;
  if (y == t->nil)
    t->root = z;
  else if (z->key < y->key)
    y->left = z;
  else
    y->right = z;

  z->left = t->nil;
  z->right = t->nil;
  z->color = RBTREE_RED;
  rbtree_insert_fixup(t,z);
  return t -> root;
}

node_t *rbtree_find(const rbtree *t, const key_t key) {
    // TODO: implement find
    // * RB tree내에 해당 key가 있는지 탐색하여 있으면 해당 node pointer 반환
    // * 해당하는 node가 없으면 NULL 반환
  node_t *cur_node = t->root;
  //find insert z's position
  while (cur_node != t->nil){
      if (cur_node->key ==key){
        return cur_node;
      }
      else if (cur_node->key < key)
        cur_node = cur_node->left;
      else
        cur_node = cur_node->right;
  }

  return NULL;
}

node_t *tree_min(rbtree *t, node_t *n)
{
	node_t *new_node = t -> root;
	while (new_node -> left != t -> nil)
		new_node = new_node -> left;
	return new_node;
}	

node_t *rbtree_min(const rbtree *t) {
  // TODO: implement find
  // RB tree 중 최소 값을 가진 node pointer 반환
  return t->root;
}


node_t *rbtree_max(const rbtree *t) {
    // TODO: implement find
    // 최대값을 가진 node pointer 반환

  return t->root;
}

void del_one(rbtree *t, node_t *n)
{
	if (n != t -> nil)
	{
		del_one(t, n -> left);
		del_one(t, n -> right);
		free(n);
	}
}

void delete_rbtree(rbtree *t) {
  // TODO: reclaim the tree nodes's memory
//   * delete_tree(tree): RB tree 구조체가 차지했던 메모리 반환
//     * 해당 tree가 사용했던 메모리를 전부 반환해야 합니다. (valgrind로 나타나지 않아야 함)
	del_one(t, t -> root);
	free(t -> nil);
	free(t);
}

void rbtree_transplant(rbtree *t, node_t *u, node_t *v)
{
	if (u -> parent == t -> nil)
		t -> root = v;
	else if (u == u -> parent -> left)
		u -> parent -> left = v;
	else
		u -> parent -> right = v;
	v -> parent = u -> parent;
}

int rbtree_erase_fixup(rbtree *t, node_t *x)
{

}

int rbtree_erase(rbtree *t, node_t *p) {
    //   TODO: implement erase
    // RB tree 내부의 ptr로 지정된 node를 삭제하고 메모리 반환
	node_t	*y;
	node_t	*x;
	color_t	y_original_color;

	if (p -> left == t -> nil)
	{
		x = p -> right;
		rbtree_transplant(t, p, p -> right);
	}
	else if (p -> right == t -> nil)
	{
		x = p -> left;
		rbtree_transplant(t, p, p -> left);
	}
	else
	{
		y = tree_min(t, p -> right);
		y_original_color = y -> color;
		x = y -> right;
		if (y -> parent == p)
			x -> parent = y;
		else
		{
			rbtree_transplant(t, y, y-> right);
			y -> right = p -> right;
			y -> right -> parent = y;
		}
		rbtree_transplant(t, p, y);
		y -> left = p -> left;
		y -> left -> parent = y;
		y -> color = p -> color;
	}
	if (y_original_color == RBTREE_BLACK)
		rbtree_erase_fixup(t, x);
	return 0;
}

int in_order(node_t *cur, key_t *arr, size_t n, int index,const rbtree *t){
  if (cur == t->nil)
    return index;

  if (index < n){
    if (cur->left != t->nil){
       index = in_order(cur->left, arr, n, index,t);
    }
    printf("%d\n", cur->key);
    // printf("index : %d arr[index] : %d \n", index, arr[index]);
    arr[index++] = cur->key;
    if (cur->right != t->nil){
      index = in_order(cur->right, arr, n, index,t);
    }
  }
  return index;
}


int rbtree_to_array(const rbtree *t, key_t *arr, const size_t n) {
  // TODO: implement to_array
  // * RB tree의 내용을 key 순서대로 주어진 array로 변환
  // * array의 크기는 n으로 주어지며 tree의 크기가 n 보다 큰 경우에는 순서대로 n개 까지만 변환
  // * array의 메모리 공간은 이 함수를 부르는 쪽에서 준비하고 그 크기를 n으로 알려줍니다.
  //inorder traversing
  node_t * cur = calloc(1,sizeof(node_t));
  cur = t->root;
  
  in_order(cur, arr, n, 0, t);
  
  for (int i =0; i<5; i++){
      printf("%d\n", arr[i]);
  }
    //rbtree_to_array
    return 0;
}



