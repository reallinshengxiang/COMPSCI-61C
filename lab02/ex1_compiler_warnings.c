#include <stdlib.h>
#include <stdio.h>
#include <string.h>

struct Course {
    int id;
    char *name;
};

struct Course *make_course(int id, char *name) {
    struct Course *new_course = (struct Course*)malloc(sizeof(struct Course));
    if(new_course==NULL) return NULL;
    new_course->id = id;
    new_course->name = (char*)malloc(strlen(name)+1);
    if(new_course->name==NULL)
    {
        free(new_course);
        return NULL;
    }
    strcpy(new_course->name,name);
    return new_course;
}
int main() {
    struct Course *cs161 = make_course(161, "Computer Security");
    if(cs161==NULL) return 1;
    printf("Welcome to CS%d: %s!\n", cs161->id, cs161->name);
    free(cs161->name);
    free(cs161);
    return 0;
}
