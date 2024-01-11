#include <stdio.h>
#include <stdlib.h>
#include <string.h>

struct Person {
    char *name;
    int age;
};

int main() {
    struct Person *person = malloc(sizeof(struct Person));
    person->name = strdup("John");
    person->age = 25;

    // Example 1
    char *json = malloc(sizeof(char *));
    asprintf(&json, "{\n  \"name\": \"%s\",\n  \"age\": %d\n}",
             person->name, person->age);
    printf("%s", json); // or send(clientSocket, json, strlen(json), 0);
    free(json);

    // Example 2 - Build incrementally
    char *json2 = NULL;
    size_t len = 0;

    len += asprintf(&json2, "{\n"); // Append incrementally
    len += asprintf(&json2, "%s  \"name\": \"%s\",\n", json2, person->name);
    len += asprintf(&json2, "%s  \"age\": %d\n", json2, person->age);
    len += asprintf(&json2, "%s}", json2);
    printf("%zu", len);
    printf("%s", json2); // or send(clientSocket, json2, len, 0);
    free(json2);

    // Example 3 - Print to screen
    printf("{\n");
    printf("  \"name\": \"%s\",\n", person->name);
    printf("  \"age\": %d\n", person->age);
    printf("}");

    free(person->name);
    free(person);
}