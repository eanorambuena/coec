# Coec

## Usage (with gcc)
    
``` r
python3 -m coec <input file> <output file> [executable file] [options]
```

### Options

- `-h` or `--help`: Show help message **coming soon**
- `-v` or `--version`: Show version **coming soon**
- `-e` or `--execute`: Execute the output file after compiling

## Example

``` c
ptr sayHi (List* args) {
    printf("Hi!\n");
    yield;
}

ptr add (List* args) {
    yield *(int*) args->head->data + *(int*) args->head->next->data;
}

int main () {
    class Person:
        property age;
        method sayHi;
        method add;
    
    int Person.age = 52;
    Person.CALL(sayHi);

    int a = 1;
    int b = 6;
    ARG(a);
    ARG(b);
    printf("Person.add(1, 6): %d\n", Person.CALL(add));
    printf("Person.age: %d\n", Person.GET(age));

    class Student (Person):
        property grade;

    int Student.grade = 12;
    int Student.age = 17;
    !int Student.age = 18;

    printf("Student.grade: %d\n", Student.GET(grade));
    printf("Student.age: %d\n", Student.GET(age));

    freeClass(Person);
    freeClass(Student);

    return 0;
}
```

## You can also use it on vanilla C

``` c
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
        SET(SELF, age, &c);

        CALL(SELF, sayHi);

        int a = 1;
        int b = 6;
        ARG(a);
        ARG(b);
        printf("%s.add(1, 6): %d\n", SELF->name, CALL(SELF, add));
        printf("%s.age: %d\n", SELF->name, GET(SELF, age));
        #undef SELF

    EXTEND(Person, Student);
        #define SELF Student
        PROPERTY(grade);
        int d = 12;
        SET(SELF, grade, &d);
        int j = 17;
        SET(SELF, age, &j);
    
        printf("%s.grade: %d\n", SELF->name, GET(SELF, grade));
        printf("%s.age: %d\n", SELF->name, GET(SELF, age));
        #undef SELF

    freeClass(Person);
    freeClass(Student);

    END;
    return 0;
}
```