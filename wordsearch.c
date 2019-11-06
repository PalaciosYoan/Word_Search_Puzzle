#include <stdio.h>
#include <stdlib.h>
#include <string.h>

// Declarations of the two functions you will implement
// Feel free to declare any helper functions
void printPuzzle(char** arr, int n);
void searchPuzzle(char** arr, int n, char** list, int listSize);

// Main function, DO NOT MODIFY!!!
int main(int argc, char **argv) {
    int bSize = 15;
	if (argc != 2) {
		fprintf(stderr, "Usage: %s <puzzle file name>\n", argv[0]);
		return 2;
	}
    int i, j;
    FILE *fptr;
    char **block = (char**)malloc(bSize * sizeof(char*));
	char **words = (char**)malloc(50 * sizeof(char*));

    // Open file for reading puzzle
    fptr = fopen(argv[1], "r");
    if (fptr == NULL) {
        printf("Cannot Open Puzzle File!\n");
        return 0;
    }

	// Read puzzle block into 2D arrays
    for(i=0; i<bSize; i++){
        *(block+i) = (char*)malloc(bSize * sizeof(char));

        fscanf(fptr, "%c %c %c %c %c %c %c %c %c %c %c %c %c %c %c\n", *(block+i), *(block+i)+1, *(block+i)+2, *(block+i)+3, *(block+i)+4, *(block+i)+5, *(block+i)+6, *(block+i)+7, *(block+i)+8, *(block+i)+9, *(block+i)+10, *(block+i)+11, *(block+i)+12, *(block+i)+13, *(block+i)+14 );
    }
	fclose(fptr);

	// Open file for reading word list
	fptr = fopen("states.txt", "r");
	if (fptr == NULL) {
        printf("Cannot Open Words File!\n");
        return 0;
    }

	// Save words into arrays
	for(i=0; i<50; i++){
		*(words+i) = (char*)malloc(20 * sizeof(char));
		fgets(*(words+i), 20, fptr);
	}

	// Remove newline characters from each word (except for the last word)
	for(i=0; i<49; i++){
		*(*(words+i) + strlen(*(words+i))-2) = '\0';
	}

	// Print out word list
	printf("Printing list of words:\n");
	for(i=0; i<50; i++){
		printf("%s\n",*(words + i));
	}
	printf("\n");

	// Print out original puzzle grid
    printf("Printing puzzle before search:\n");
    printPuzzle(block, bSize);
	printf("\n");

	// Call searchPuzzle to find all words in the puzzle
	searchPuzzle(block, bSize, words, 50);
	printf("\n");

	// Print out final puzzle grid with found words in lower case
    printf("Printing puzzle after search:\n");
    printPuzzle(block, bSize);
	printf("\n");

    return 0;
}

void printPuzzle(char** arr, int n){
	// This function will print out the complete puzzle grid (arr). It must produce the output in the SAME format as the samples in the instructions.
	// Your implementation here
  for (int i =0; i < n; i++){
    for(int j = 0; j < n; j++){
      printf("%c ",*(*(arr+i)+j));
    }
    printf("\n");
  }


}
//coverts a lower case letter from the 2d array into upper case
void upper_case_letter(char** arr, int row, int column){
  //makes suer that the word we are trying to convert is lower
  if(*(*(arr+row)+column) >= 'a' && *(*(arr+row)+column) <= 'z' ){
    *(*(arr+row)+column) = *(*(arr+row)+column) - 32;
  }
}


//converts the letter that was converted to upper case back to lower case
void lower_case_letter(char** arr, int row, int column){
  //makes sure that the word we are trying to cover is capital to lower
  if(*(*(arr+row)+column) >= 'A' && *(*(arr+row)+column) <= 'Z' ){
    *(*(arr+row)+column) = *(*(arr+row)+column) + 32;
  }
}


int direction_search(char** arr,int row, int column,char** list, int row_l, int column_l, int word_length,int direction, int* row_dir, int* column_dir, int count,int n){
  int x = column_l;
  int Change_letter = 0;
  int rd;
  int cd;
  //searches the word in the direction that the second letter of the word was found
  while(x < word_length){
    rd = row + *(row_dir+direction);
    cd = column + *(column_dir + direction);

    if(rd >= n || rd < 0 || cd >= n || cd < 0){
      break;
    }

    if(*(*(arr+rd)+cd) >= 'a' && *(*(arr+rd)+cd) <= 'z'){
      Change_letter = 1;
      upper_case_letter(arr, rd, cd);
    }

    if(*(*(arr+rd)+cd)==*(*(list+row_l)+column_l)){
      if(Change_letter == 1){
        lower_case_letter(arr, rd, cd);
        Change_letter = 0;
      }

      row = rd;
      column = cd;
      column_l = column_l + 1;
      count = count + 1;
    }

    else {
      break;
    }
    //if word found then we are converting the letters of the word to lowercase starting from the second letter
    if(count == word_length){
      for(int h = 1; h < word_length; h++){
        if(*(*(list+row_l)+h) >= 'A' && *(*(list+row_l)+h) <= 'Z'){
          *(*(list+row_l)+h) = *(*(list+row_l)+h) + 32;
        }
      }
      //prints out the word found and returns 1(found word)
      printf("found word: %s\n",*(list+row_l));
      return 1;
    }

    x = x+ 1;
  }

  if(Change_letter == 1){
    Change_letter = 0;
    lower_case_letter(arr, rd, cd);
  }

  return 0;
}



//this fucntion will turn the letters in the 2d array where the word was found to lower case
void lower_case(char** arr, int row, int column, int* row_dir, int* column_dir, int direction, int word_length){
  int x = 0;
  while(x<word_length){
    int rd;
    int cd;

    //This if and else statement is to take into acount the first letter of the word
    if(x == 0){
      rd = row;
      cd = column;
    }

    else{
      rd = row + *(row_dir+direction);
      cd = column + *(column_dir + direction);
    }

    if(rd >= 15 || rd < 0 || cd >= 15 || cd < 0){ //checking out of bound
      break;
    }

    if(*(*(arr+rd)+cd) >= 'A' && *(*(arr+rd)+cd) <= 'Z'){
      *(*(arr + rd) + cd) = *(*(arr+rd)+cd) + 32;

      row = rd;
      column = cd;
    }

    x = x + 1;
  }
}


int wordsearch(char** arr,int row, int column,int n, char** list,int row_l,int column_l, int word_length, int* row_dir, int* column_dir, int count){
  int direction = 0;
  int rd = 0;
  int cd = 0;
  int i = 0;
  int found = 0;
  int outofboun = 0;
  int temp_rd = 0;
  int temp_cd = 0;
  int temp_row =  row;
  int temp_column = column;
  int Change_letter = 0 ;

  while(direction < 8){ //This makes sure we go in the 5 directions that we are meant to go
    //We are just adding 1 or zero depending in the direction we want to go and check that spot
    rd = row + *(row_dir+direction);
    cd = column + *(column_dir + direction);

    if(rd >= n || rd < 0 || cd >= n || cd < 0){ //checks out of bound

      outofboun = 1;
      temp_rd = rd;
      temp_cd = cd;
      rd = row;
      cd = column;
    }

    if(*(*(arr+rd)+cd) >= 'a' && *(*(arr+rd)+cd) <= 'z'){ //checks if letter in 2d array is lower case to turn it to upper
      Change_letter = 1;
      upper_case_letter(arr, rd, cd);
    }

    if(*(*(arr + rd)+cd) ==*(*(list + row_l)+column_l)){ //compares letters
      //we increament count to signified that we found another letter and that we are close on finding the whole word.
      //we call function direction_search to see if we find the word in the direction that we found the second letter
      count = count + 1;
      column_l = column_l + 1;

      found = direction_search(arr, rd, cd, list,row_l, column_l, word_length,direction, row_dir, column_dir, count, n);

      //if found = to True(1) then we want to exit this function because the word was found and calls lower_case function to lower case all the letters
      if(found == 1){
        Change_letter = 0;
        lower_case(arr, temp_row, temp_column, row_dir, column_dir, direction, word_length);
        return 1;
      }
    }

    if(Change_letter == 1){
      Change_letter = 0;
      lower_case_letter(arr, rd, cd);
    }

    if(found != 1){
      count = 1;
      column_l = 1;
    }

    if(outofboun == 1){
      rd = temp_rd;
      cd = temp_cd;
      outofboun = 0;
    }

    direction = direction + 1;
  }
  return 0;
}


void searchPuzzle(char** arr, int n, char** list, int listSize){
	// This function checks if arr contains words from list. If a word appears in arr, it will print out that word and then convert that word entry in arr into lower case.
	// Your implementation here
  int column_list = 0; //constant we are just looking at the 1st letter
  int row_list = 0;
  int length_of_word = 0; //We will store the number of letter for the current word we are searching
  int count = 0; //This count will keep count how many letters are found from the word we are looking for
  int found_word = 0;
  int arr_index = 0;
  int Change_letter = 0;
  //The following two arrays are meant to store every single possible direction
  int* row_direction = (int*)malloc(8 * sizeof(int));
  int* column_direction = (int*)malloc(8 * sizeof(int));

  //Storing the direction I want to go
  //Directions in the row
  *(row_direction+0) = 0; //right
  *(row_direction+1) = 1; //down
  *(row_direction+2) = -1; //up
  *(row_direction+3) = 1; //bottom right
  *(row_direction+4) = -1; //top right
  *(row_direction+5) = 1; //bottom left
  *(row_direction+6) = 0;//left
  *(row_direction+7) = -1; //top left

  //Directions in the column
  *(column_direction+0) = 1;
  *(column_direction+1) = 0;
  *(column_direction+2) = 0;
  *(column_direction+3) = 1;
  *(column_direction+4) = 1;
  *(column_direction+5) = -1;
  *(column_direction+6) = -1;
  *(column_direction+7) = -1;

//Bottome left
  // *(*(list+0)+0) = 'I';
  // *(*(list+0)+1) = 'T';
  // *(*(list+0)+2) = 'H';
  // *(*(list+0)+3) = 'Y';
  // *(*(list+0)+4) = 'I';
  // *(*(list+0)+5) = 'J';
  // *(*(list+0)+6) = 'R';

  //Left
  // *(*(list+0)+0) = 'Z';
  // *(*(list+0)+1) = 'L';
  // *(*(list+0)+2) = 'A';
  // *(*(list+0)+3) = 'I';
  // *(*(list+0)+4) = 'A';
  // *(*(list+0)+5) = 'N';
  // *(*(list+0)+6) = 'I';
  //
  // //Top left
  // *(*(list+0)+0) = 'N';
  // *(*(list+0)+1) = 'D';
  // *(*(list+0)+2) = 'A';
  // *(*(list+0)+3) = 'X';
  // *(*(list+0)+4) = 'G';
  // *(*(list+0)+5) = 'E';
  // *(*(list+0)+6) = 'I';

  for(int i=0; i < listSize; i++){//Turning every single character into captital in the List
    length_of_word = strlen(*(list+i));
    for(int j = 0; j < length_of_word; j++){
      if(*(*(list+i)+j) >= 'a' && *(*(list+i)+j) <= 'z'){
        *(*(list+i)+j) = *(*(list+i)+j) - 32;
      }
    }
  }

  //The while loop is to go thru every word in the list
  while (row_list < listSize) {
    column_list = 0;
    length_of_word = strlen(*(list+row_list)); //Getting the length of the word to make sure we do not go out of bound
    found_word = 0;

    //These two for loop will go thru the 2d array and testing every possible position for the word
    for(int row_arr = 0; row_arr < n; row_arr++){ //georgia is located at row 13
      for(int column_arr = 0; column_arr < n; column_arr++){
        count = 0;

        if(*(*(arr+row_arr)+column_arr) >= 'a' && *(*(arr+row_arr)+column_arr) <= 'z'){ //if the letter in the 2d array is lower case then we are turning it back to upper case
          Change_letter = 1;
          upper_case_letter(arr, row_arr, column_arr);
        }

        if(*(*(arr+row_arr)+column_arr) == *(*(list+row_list)+column_list)){ //comparing the letter from 2d array with the first letter of the list word
          column_list = column_list + 1; //moves on to the second letter of the word
          count = count + 1;//we are adding one because we found the first letters
          found_word = wordsearch(arr, row_arr, column_arr, n, list, row_list, column_list, length_of_word, row_direction, column_direction, count); //checks if word is found

          if(found_word == 1){ //if word found we do not need worry about turning the letter that was turned from lower to upper because it should be lower
            Change_letter =0;
          }
        }

        if(Change_letter == 1){ //if word not found this will turn the letter that was upper to lower to make sure we don't lose track on the ones we find
          Change_letter = 0;
          lower_case_letter(arr, row_arr, column_arr);
        }

        column_list = 0;

        if(found_word == 1){ //if the word is found then we want to break out of the 2d array and move on to the next word
          break;
        }

      }

      if(found_word == 1){
        break;
      }
    }

    row_list = row_list + 1; //increament to the next word of the list
  }

}
