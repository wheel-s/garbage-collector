
#include <stdio.h>
#include <stdlib.h>

typedef struct Obj {
    int marked;
    struct Obj*next;
    struct Obj*ref;
    int value;
} Obj;

Obj *all_objects = NULL;

Obj *root = NULL;

Obj *gc_alloc(int value){
    Obj*obj = malloc(sizeof(Obj));
    obj->marked = 0;
    obj->ref = NULL;
    obj->value = value;
    obj->next = all_objects;
    all_objects = obj;
    return obj;
}


void mark(Obj* obj){
    if(!obj || obj-> marked) return;
    obj->marked = 1;
    mark(obj->ref);

}

void sweep(){
    Obj **curr = &all_objects;
    while (*curr) {
        if(!(*curr)->marked){
          Obj *unreached = *curr;
          *curr = unreached->next;
          free(unreached);
        }else{
            (*curr)->marked = 0;
            curr = &(*curr)->next;
        }
    }
}

void gc_collect(){
    mark(root);
    sweep();
}

int main(){
    root = gc_alloc(1);
    root->ref = gc_alloc(2);
    root->ref->ref = gc_alloc(3);

    printf("Allocated 3 objects\n");

    root->ref = NULL;

    gc_collect();

    printf("Garbage Collected\n");

    root = NULL;
    gc_collect();
    printf("All freed\n");


    return 0;
}