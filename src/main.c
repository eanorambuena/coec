#include "class.h"

ptr sayHi (List* args) {
    int* RETURN = calloc(1, sizeof(int));
    *RETURN = 0;
    printf("Hi!\n");
    YIELD;
}

ptr add (List* args) {
    int* RETURN = calloc(1, sizeof(int));
    *RETURN = *(int*) args->head->data + *(int*) args->head->next->data;
    YIELD;
}

int main () {
    BEGIN;

    CLASS(Person);
        #define SELF Person
        PROPERTY(age);
        METHOD(sayHi);
        METHOD(add);
        int c = 52;
        SET(age, &c);

        CALL(sayHi);

        int a = 1;
        int b = 6;
        ARG(a);
        ARG(b);
        printf("%s.add(1, 6): %d\n", SELF->name, CALL(add));
        printf("%s.age: %d\n", SELF->name, GET(age));
        #undef SELF

    EXTEND(Person, Student);
        #define SELF Student
        PROPERTY(grade);
        int d = 12;
        SET(grade, &d);
        int j = 17;
        SET(age, &j);
    
        printf("%s.grade: %d\n", SELF->name, GET(grade));
        printf("%s.age: %d\n", SELF->name, GET(age));
        #undef SELF

    END;
    return 0;
}