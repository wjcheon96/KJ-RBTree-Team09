#include "rbtree.c"

int main(void){
    rbtree *t = new_rbtree();
    rbtree_insert(t, 26);
    rbtree_insert(t, 17);
    rbtree_insert(t, 42);
    rbtree_insert(t, 47);
    rbtree_insert(t, 21);
    rbtree_find(t,21);

    int arr[100] = {0,};
    rbtree_to_array(t,arr,5);
    
    

}
