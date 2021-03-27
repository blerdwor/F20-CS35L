#include <stdio.h>
#include <stdlib.h>

/* returns an int result that is negative, zero, or positive depending 
 * on whether a is less than, equal to, or greater than b.*/
int frobcmp(char const *a, char const *b) {
  while (*a!=' ' && *b!=' ')
  {
    char deobfus_a = *a ^ 42, deobfus_b = *b ^ 42;
    if (deobfus_a != deobfus_b)
      return deobfus_a < deobfus_b ? -1: 1;
    a++; b++;
  }
  
  if (*a==' ' && *b==' ')
    return 0;
  else if (*a==' ')
    return -1;
  return 1;
}

/*compare function */
int cmp(const void * a, const void * b) {
   return frobcmp(*(char const**) a,*(char const**) b);
}

int main() { 
	 
  int **word_arr;
  int read = 0, word_len = 0, word_count = 0;
   
  // cast to double pointer, total size of 1 pointer (8 bytes)
  word_arr = (int **)malloc(sizeof(int *));
  
  // check if memory was not allocated successfully
  if (word_arr == NULL) {
    fprintf(stderr, "Memory not allocated.\n");
    exit(1);
  }
  
  // initialize first pointer to NULL
  word_arr[0] = NULL;

  // read in words
  read = getchar();
  while (read != EOF) {

    // space char = end of word, allocate memory for new row
    if (read == ' ') {
      word_arr[word_count] = (int *)realloc(word_arr[word_count], sizeof(int)*(word_len+1));
      word_arr[word_count][word_len] = ' ';
  
      // if the input had a space at the end
      if ((read = getchar()) == EOF)
        break;
      else {
        word_len = 0;
        word_count += 1;
        int **tmp = (int **)realloc(word_arr, sizeof(int *)*(word_count+1));
        if (tmp) {
          word_arr = tmp;
          word_arr[word_count] = (int *)malloc(sizeof(int *));
        }
        else {
          fprintf(stderr, "Memory not allocated. \n");
          exit(1);
        }
      }
    }
    else { // add char to current word, allocates memory for new col
      word_len += 1;
      word_arr[word_count] = (int *)realloc(word_arr[word_count], sizeof(int)*word_len);
      word_arr[word_count][word_len-1] = read;
    
      // if the input doesn't have a space at the end
      if ((read = getchar()) == EOF) {
        word_arr[word_count] = (int *)realloc(word_arr[word_count], sizeof(int)*(word_len+1));
        word_arr[word_count][word_len] = ' ';
        break;
      }
    }
  }
  
  // sort the words
  qsort(word_arr, word_count+1, sizeof(int *), cmp); 

  // print out array of words
  for (int i = 0; i <= word_count; i++) {
    for (int j = 0; word_arr[i][j] != ' '; j++)
       putchar(word_arr[i][j]);
    putchar(' ');
  }

  // free memory
  for (int i = 0; i <= word_count; i++)
    free(word_arr[i]);
  free(word_arr);

  return 0;
}
