#include <stdlib.h>
#include <stdio.h>
#include <string.h>

typedef int* ptr;

typedef struct keyValuePair_t {
    char *key;
    ptr value;
} KeyValuePair;

typedef struct node_t {
    ptr data;
    struct node_t *next;
} Node;

typedef struct list_t {
    Node *head;
    Node *tail;
} List;

typedef struct class_t {
    char *name;
    List *properties;
    List *methods;
} Class;

List* newList ();
Node* newNode (ptr data);
void append2List (List *list, ptr data);
void freeList (List *list);

#define function ptr(*)(List*)
#define YIELD return(ptr)RETURN
#define ARG(a) append2List(ARGS, (ptr) &(a))

Class* newClass (char *name);
void property (Class *class, char *name);
void method (Class *class, char *name, ptr (*func) (List*));
ptr call (Class *class, char *name, List* args);
void setProperty (Class *class, char *name, ptr value);
ptr getProperty (Class *class, char *name);

char* concat (char *a, char *b);
void copyList (List *list, List *copy);
Class* new (Class* class, char *name);

#define BEGIN                   List* ARGS = newList()
#define END                     free(ARGS)
#define CLASS(name)             Class*name = newClass(#name)
#define CALL(name)              *call(SELF, #name, ARGS)
#define PROPERTY(name)          property(SELF, #name)
#define METHOD(name)            method(SELF, #name, name)
#define SET(name, value)        setProperty(SELF, #name, (ptr) value)
#define GET(name)               *getProperty(SELF, #name)
#define EXTEND(class, obj)      Class* obj = new(class, #obj)