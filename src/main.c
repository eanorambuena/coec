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

void copyList (List *list, List *copy);
Class* new (Class* class, char *name);
void freeClass (Class *class);

#define BEGIN                   List* ARGS = newList()
#define END                     free(ARGS)
#define CLASS(name)             Class*name = newClass(#name)
#define CALL(class, name)       *call(class, #name, ARGS)
#define PROPERTY(name)          property(SELF, #name)
#define METHOD(name)            method(SELF, #name, name)
#define SET(class, name, value) setProperty(class, #name, (ptr) value)
#define GET(class, name)        *getProperty(class, #name)
#define EXTEND(class, obj)      Class* obj = new(class, #obj)

List* newList () {
    List *list = calloc(1, sizeof(List));
    list->head = NULL;
    list->tail = NULL;
    return list;
}

Node* newNode (ptr data) {
    Node *node = calloc(1, sizeof(Node));
    node->data = data;
    node->next = NULL;
    return node;
}

void append2List (List *list, ptr data) {
    Node *node = newNode(data);
    if (list->head == NULL) {
        list->head = node;
        list->tail = node;
    } else {
        list->tail->next = node;
        list->tail = node;
    }
}

void freeList (List *list) {
    Node *node = list->head;
    while (node != NULL) {
        Node *next = node->next;
        free(node);
        node = next;
    }
}

Class* newClass (char *name) {
    Class *class = calloc(1, sizeof(Class));
    class->name = name;
    class->properties = newList();
    class->methods = newList();
    return class;
}

void property (Class *class, char *name) {
    KeyValuePair *pair = calloc(1, sizeof(KeyValuePair));
    pair->key = name;
    pair->value = NULL;
    append2List(class->properties, (ptr) pair);
}

void method (Class *class, char *name, ptr (*func) (List*)) {
    KeyValuePair *pair = calloc(1, sizeof(KeyValuePair));
    pair->key = name;
    pair->value = (ptr) func;
    append2List(class->methods, (ptr) pair);
}

ptr call (Class *class, char *name, List* args) {
    Node *node = class->methods->head;
    while (node != NULL) {
        KeyValuePair *pair = (KeyValuePair*) node->data;
        if (strcmp(pair->key, name) == 0) {
            ptr result = (ptr) ((function) pair->value)(args);
            freeList(args);
            return result;
        }
        node = node->next;
    }
    freeList(args);
    return NULL;
}

void setProperty (Class *class, char *name, ptr value) {
    Node *node = class->properties->head;
    while (node != NULL) {
        KeyValuePair *pair = (KeyValuePair*) node->data;
        if (strcmp(pair->key, name) == 0) {
            pair->value = value;
            return;
        }
        node = node->next;
    }
}

ptr getProperty (Class *class, char *name) {
    Node *node = class->properties->head;
    while (node != NULL) {
        KeyValuePair *pair = (KeyValuePair*) node->data;
        if (strcmp(pair->key, name) == 0) {
            return pair->value;
        }
        node = node->next;
    }
    return NULL;
}

void copyList (List *list, List *copy) {
    Node *node = list->head;
    while (node != NULL) {
        append2List(copy, node->data);
        node = node->next;
    }
}

Class* new (Class* class, char *name) {
    Class *object = calloc(1, sizeof(Class));
    object->name = name;
    object->properties = newList();
    object->methods = newList();
    copyList(class->properties, object->properties);
    copyList(class->methods, object->methods);
    return object;
}

void freeClass (Class *class) {
    freeList(class->properties);
    freeList(class->methods);
    free(class);
}
ptr sayHi (List* args){BEGIN;
printf("Hi!\n");
int* RETURN = calloc(1, sizeof(int));END;YIELD;
}

ptr add (List* args){BEGIN;
int* RETURN = calloc(1, sizeof(int));*RETURN=*(int*) args->head->data + *(int*) args->head->next->data;END;YIELD;
}

int main (){BEGIN;
CLASS(Person);
#undef SELF
#define SELF Person
PROPERTY(age);
METHOD(sayHi);
METHOD(add);

int int_Person_age =  17 + 40;SET(Person,age,&int_Person_age);
CALL(Person,sayHi);

int a = 1;
int b = 6;
ARG(a);
ARG(b);
printf("Person.add(1, 6): %d\n", CALL(Person,add));
printf("Person.age: %d\n", GET(Person,age));

EXTEND(Person,Student);
#undef SELF
#define SELF Student
PROPERTY(grade);

int int_Student_grade =  12;SET(Student,grade,&int_Student_grade);
int int_Student_age =  17;SET(Student,age,&int_Student_age);
int_Student_age =  18;SET(Student,age,&int_Student_age);

printf("Student.grade: %d\n", GET(Student,grade));
printf("Student.age: %d\n", GET(Student,age));

freeClass(Person);
freeClass(Student);

END;return 0;
}