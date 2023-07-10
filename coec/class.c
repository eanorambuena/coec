#include "class.h"

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