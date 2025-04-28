#include<stdio.h>
#include<stdlib.h>

int main(int argc, char** argv) {
    int age;
    char* word1;
    char* word2;
  //  char* buffer;
    word1 = malloc(20 * sizeof(char));
    word2 = malloc(20 * sizeof(char));

    sscanf("19 cool  kid", "%d %s  %s", &age, word1,word2);

    printf("%s is %d years old%s\n", word1, age ,word2);
    
    return 0;
}
