#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <stdbool.h>
#include <time.h>

const int MIN_HIVE_SIZE = 2;
const int MAX_HIVE_SIZE = 12;
const int MIN_WORD_LENGTH = 4;

// WordList struct
// store a dynamic list of words 
typedef struct WordList_struct {
    char** words; // dynamic array of pointers to strings 
    int numWords; // number of words stored in the list 
    int capacity; // total number of word the array can hold
} WordList;

WordList* createWordList() {
    WordList* newList = malloc(sizeof(WordList));
    newList->capacity = 4;
    newList->numWords = 0;
    newList->words = malloc(newList->capacity * sizeof(char*));

    return newList;
}

void appendWord(WordList* thisWordList, char* newWord) {
    //---------------------------------------------------------------------
    /* TODO (Task 1-A): Write appendWord
    - adds newWord to the end of the thisWordList (malloc'ing memory and copying the string; do NOT just copy the input pointer)
    - increments thisWordList's numWords. If there is no space for the new word, reallocate with double the capacity
    - by malloc'ing new memory for the words array and freeing the old memory
    */
    if(thisWordList->numWords == thisWordList->capacity) {
        int newCapacity = thisWordList->capacity * 2;
        char** newArray = (char**)malloc(sizeof(char*) * newCapacity);

        for (int i = 0; i < thisWordList->numWords; i++) {
            newArray[i] = thisWordList->words[i];
        }
        free(thisWordList->words);
        thisWordList->words = newArray;
        thisWordList->capacity = newCapacity;
    }
    
    char* copyWords = (char*)malloc(sizeof(char)* (strlen(newWord) + 1));
        
    strcpy(copyWords, newWord);
        
    thisWordList->words[thisWordList->numWords] = copyWords;

    thisWordList->numWords++;
}

int buildDictionary(char* filename, WordList* dictionaryList, int minLength) {
    //---------------------------------------------------------------------
    /* TODO (Task 1-B): Write buildDictionary
    - Opens a valid file and adds all words that are at least minLength long to the dictionaryList
    - Returns the length of the longest valid word if all goes well, -1 otherwise
    */
    FILE* file = fopen(filename, "r");
    if (file == NULL) {
        return -1;
    }
    
    char word[1000];
    int longestWord = 0;

    while(fscanf(file, "%s", word) != EOF) {
        int length = strlen(word);

        if(length >= minLength) {
            appendWord(dictionaryList, word);

            if(length > longestWord) {
                longestWord = length;
            }
        }
    }
    fclose(file);

    if(dictionaryList->numWords == 0) {
        return -1;
    }
    return longestWord;
}

void freeWordList(WordList* list) {
    //---------------------------------------------------------------------
    /* TODO (Task 1-C): Write freeWordList
    - Frees the memory used by the WordList, both the words' char* arrays and the char** words array itself
    */
    for(int i = 0; i < list->numWords; i++) {
        free(list->words[i]);
    }
    free(list->words);
    free(list);
}

int findLetter(char* str, char aLet) {
    //---------------------------------------------------------------------
    /* TODO (Task 2-A): Write findLetter
    - Returns the index of aLet in str, if it exists
    - Otherwise returns -1
    */
    for(int i = 0; str[i] != '\0'; i++) {
        if(str[i] == aLet) {
            return i;
        }
    }
    return -1;
}

void buildHive(char* str, char* hive) {
    //---------------------------------------------------------------------
    /* TODO (Task 2-B): Write buildHive
    - take each character of the string and add it to the hive in alphabetical order
    - no letter should be added twice
    - ensure that hive is null-terminated at the end
      (findLetter is helpful here)
    */
    hive[0] = '\0';
    int h = 0;
    for(int i = 0; str[i] != '\0'; i++) {
        if(findLetter(hive, str[i]) == -1) {
            hive[h] = str[i];
            h++;
            hive[h] = '\0';
        }
    }

    for(int i = 0; hive[i] != '\0'; i++) {
        for(int j = i + 1; hive[j] != '\0'; j++) {
            if(hive[i] > hive[j]) {
                char temp = hive[i];
                hive[i] = hive[j];
                hive[j] = temp;
            }
        }
    }
}

int countUniqueLetters(char* str) {
    //---------------------------------------------------------------------
    /* TODO (Task 3-A): Write countUniqueLetters
    - Counts and returns the number of unique letters in the input string 
      (note: this only needs to work for lower-case alpha characters, i.e. letters, for this project, 
            but it should work generally for all characters)
    */
    int count = 0;

    for (int i = 0; str[i] != '\0'; i++) {
        int uniqueLetter = 1;
        for (int j = 0; j < i; j++) {
            if (str[i] == str[j]) {
                uniqueLetter = 0;
                break;
            }
        }
        if (uniqueLetter){
            count++;
        }
    }
    return count;
}

WordList* findAllFitWords(WordList* dictionaryList, int hiveSize) {
    WordList* fitWords = createWordList();
    //---------------------------------------------------------------------
    /* TODO (Task 3-B): Write findAllFitWords
    - Creates a WordList and adds all fit words from dictionaryList to the new WordList 
      (A fit word has exactly hiveSize unique letters)
    */
    for (int i = 0; i < dictionaryList->numWords; i++) {
        char* word = dictionaryList->words[i];
        int uniqueLetter = countUniqueLetters(word);
        if (uniqueLetter == hiveSize) {
            appendWord(fitWords, word);
        }
    }
    return fitWords;
}

bool isValidWord(char* word, char* hive, char reqLet) {
    //---------------------------------------------------------------------
    /* TODO (Task 4-A, part 1 of 2): Write isValidWord 
    - Returns true if word 
      (a) uses ONLY letters that are in the hive, and 
      (b) MUST use the required letter. 
    - Returns false otherwise
    */
    bool required = false;
    for (int i = 0; word[i] != '\0'; i++) {
        if(word[i] == reqLet) {
            required = true;
        }
    
        bool found = false;
        for (int j = 0; hive[j] != '\0'; j++) {
            if(word[i] == hive[j]) {
                found = true;
                break;
            }
        }  
        if(!found) {
            return false;
        }    
    }
    if(!required) {
        return false;
    }
    return true;
}

bool isPangram(char* str, char* hive) {
    //---------------------------------------------------------------------
    /* TODO (Task 4-B): Write isPangram
    - Returns true if str is a pangram (uses all the letters in hive at least once)
    - Returns false otherwise
    */
    for (int i = 0; hive[i] != '\0'; i++) {
        bool found = false;
        for (int j = 0; str[j] != '\0'; j++) {
            if(hive[i] == str[j]) {
                found = true;
                break;
            }
        }
        if(!found) {
            return false;
        }
    }
    return true;
}


void printHive(char* hive, int reqLetInd) {
    printf("  Hive: \"%s\"\n", hive);
    printf("         ");
    for (int i = 0; i < reqLetInd; i++) {
        printf(" ");
    }
    printf("^");
    int hiveSize = strlen(hive);
    for (int i = reqLetInd + 1; i < hiveSize; i++) {
        printf(" ");
    }
    printf(" (all words must include \'%c\')\n\n", hive[reqLetInd]);
}

void printList(WordList* thisWordList, char* hive) {
    //---------------------------------------------------------------------
    /* TODO (Task 4-C): Write printList
    - Outputs the words in the list 
    - Marks pangrams with asterisk and perfect pangrams with three asterisks
    - Displays score for each word as well as total score at the bottom
    See example shown below.
    Word List:
    *** (16) computer
        ( 7) compute
        ( 1) mute
     *  (17) recompute
        ( 5) comet
    Total Score: 46    
    */
    printf("  Word List:\n");
    int totScore = 0;
    int hiveSize = strlen(hive);
    
    for (int i = 0; i < thisWordList->numWords; i++) {
        char* word = thisWordList->words[i];
        int wordLength = strlen(word);
        int score;

        if(wordLength == MIN_WORD_LENGTH) {
            score = 1;
        }
        else{
            score = wordLength;
        }

        if (isPangram(word, hive)) {
            score = wordLength + hiveSize;
            if(wordLength == hiveSize) {
                printf("  ***");
            }
            else {
                printf("   * ");
            }   
        }
        else {
            printf("     ");
        }
        printf(" (%2d) %s\n",score,word);
        totScore += score;
    }
    printf("  Total Score: %d\n", totScore);
}

void bruteForceSolve(WordList* dictionaryList, WordList* solvedList, char* hive, char reqLet) {
    //---------------------------------------------------------------------
    /* TODO (Task 5-A): Write bruteForceSolve
    - Goes through each word in the dictionary and determines if that is a solution to the puzzle
    - Adds the word to solvedList if it is a solution
    */
    for (int i = 0; i < dictionaryList->numWords; i ++) {
        char* word = dictionaryList->words[i]; 
        if(isValidWord(word, hive, reqLet)) {
            appendWord(solvedList, word);
        }
    }
}

bool isPrefix(char* partWord, char* fullWord) {
    //---------------------------------------------------------------------
    /* TODO (Task 6-A): Write isPrefix
    - checks if partWord is the root/prefix of fullWord, such that 
        partWord is a perfect match to the beginning (arbitrary length) of fullWord
    - returns true is partWord is a root/prefix of fullWord; returns false otherwise 
    */
    int prefixLength = strlen(partWord);
    int wordLength = strlen(fullWord);

    if (prefixLength > wordLength) {
        return false;
    }

    for (int i = 0; i < prefixLength; i++) {
        if (partWord[i] != fullWord[i]) {
            return false;
        }
    }
    return true; //this line is here so that the starter code compiles; replace/modify this line
}

int findWord(WordList* thisWordList, char* aWord, int loInd, int hiInd) {
    /* TODO (Task 6-B): Complete findWord
    * - Fix all the if statements that say 'if(true)' with the correct binary search cases
    */
    if (hiInd < loInd) { // Base case 2: aWord not found in words[]

        if (loInd < thisWordList->numWords && isPrefix(aWord, thisWordList->words[loInd])) { //TODO: fix the second part of this if statement
            return -1; //words match this root (partial match)
        }
        else {
            return -99; //no more words matching this root (no match)
        }
    }

    int mdInd = (hiInd + loInd) / 2;

    if (strcmp(aWord, thisWordList->words[mdInd]) == 0) { // Base case 1: found tryWord at midInd
        return mdInd;
    }

    if (strcmp(aWord, thisWordList->words[mdInd]) > 0) { // Recursive case: search upper half
        return findWord(thisWordList, aWord, mdInd + 1, hiInd);
    }

    // Recursive case: search lower half
    return findWord(thisWordList, aWord, loInd, mdInd - 1);
}

void findAllMatches(WordList* dictionaryList, WordList* solvedList, char* tryWord, char* hive, char reqLet) {
    //---------------------------------------------------------------------
    /* TODO (Task 6-C): Complete findAllMatches

    */
    int curLen = strlen(tryWord);
    int index = findWord(dictionaryList, tryWord, 0, dictionaryList->numWords - 1);
    if (index >= 0) {
        if (isValidWord(tryWord, hive, reqLet)) {
            appendWord(solvedList, tryWord);
        }
        tryWord[curLen] = hive[0];
        tryWord[curLen + 1] = '\0';
        findAllMatches(dictionaryList, solvedList, tryWord, hive, reqLet);
    }

    if (index == -1) {
        tryWord[curLen] = hive[0];
        tryWord[curLen + 1] = '\0';
        findAllMatches(dictionaryList, solvedList, tryWord, hive, reqLet);
    }
    else if (index == -99) {
        char lastLetter = tryWord[curLen - 1];
        int hiveIdx = findLetter(hive, lastLetter);

        if(hive[hiveIdx +1] != '\0') {
            tryWord[curLen - 1] = hive[hiveIdx + 1];
            findAllMatches(dictionaryList, solvedList, tryWord, hive, reqLet);
        }
        else {
            tryWord[curLen - 1] = '\0';
            curLen--;
        

            while(curLen > 0) {
                lastLetter = tryWord[curLen - 1];
                hiveIdx = findLetter(hive, lastLetter);
                if (hive[hiveIdx + 1] != '\0') {
                    tryWord[curLen - 1] = hive[hiveIdx + 1];
                    findAllMatches(dictionaryList, solvedList, tryWord, hive, reqLet);
                    return;
                }
                else {
                    tryWord[curLen - 1] = '\0';
                    curLen--;
                }   
            }
        }
    }
    //call recursive function here ONLY if tryWord is not empty
    if (tryWord[0] == '\0') { return; }

}

// Provided for you, to determine the program settings based on parameters
bool setSettings(int argc, char* argv[], bool* pRandMode, int* pNumLets, char dictFile[100], bool* pPlayMode, bool* pBruteForceMode, bool* pSeedSelection) {
    *pRandMode = false;                 // random hive creater is OFF
    *pNumLets = 0;                      // number of hive letters
    strcpy(dictFile, "dictionary.txt"); // copy of dictioanry file 
    *pPlayMode = false;                 // playMode is OFF
    *pBruteForceMode = true;            // brute fore solver is ON 
    *pSeedSelection = false;            // seed selection is OFF
    srand((int)time(0));
    //--------------------------------------
    for (int i = 1; i < argc; ++i) { // start as index 1 as index 0 is the program name 
        if (strcmp(argv[i], "-r") == 0) { 
            ++i;
            if (argc == i) { // if no value is -r, the argumnt is invalid
                return false;
            }
            *pRandMode = true; // random hive creater is ON
            *pNumLets = atoi(argv[i]); // convert hive size from string to integer
            if (*pNumLets < MIN_HIVE_SIZE || *pNumLets > MAX_HIVE_SIZE) { // check if hive size is in the valid range 
                return false;
            }
        }
        else if (strcmp(argv[i], "-d") == 0) { // specify dictioanry file 
            ++i;
            if (argc == i) { // if missing file return error 
                return false;
            }
            strcpy(dictFile, argv[i]); // copy porvided dictionary file
            FILE* filePtr = fopen(dictFile, "r"); // open file and read
            if (filePtr == NULL) {
                return false;
            }
            fclose(filePtr);
        }
        else if (strcmp(argv[i], "-s") == 0) { // sed random seed
            ++i;
            if (argc == i) {
                return false;
            }
            *pSeedSelection = true; // seed selection is ON
            int seed = atoi(argv[i]); // convert seed value from string to integer
            srand(seed);
        }
        else if (strcmp(argv[i], "-p") == 0) { // enable play mode 
            *pPlayMode = true; // play mode is on
        }
        else if (strcmp(argv[i], "-o") == 0) { // use optimized solver 
            *pBruteForceMode = false; // brute force method is OFF 
        }
        else {
            return false; // return false if invalid is detected 
        }
    }
    return true; // return true if all arguments processed success
}

void printONorOFF(bool mode) {
    if (mode) {
        printf("ON\n");
    }
    else {
        printf("OFF\n");
    }
}

void printYESorNO(bool mode) {
    if (mode) {
        printf("YES\n");
    }
    else {
        printf("NO\n");
    }
}



int main(int argc, char* argv[]) {
    
    // Print program heading when running the program
    printf("\n----- Welcome to the CS 211 Spelling Bee Game & Solver! -----\n\n");
    
    // Program initialized default values
    bool randMode = false;               // whether letters are randomly created
    int hiveSize = 0;                    // number of letters in the hive 
    char dict[100] = "dictionary.txt";   // dictionary file
    bool playMode = false;               // whether use the play the game 
    bool bruteForce = true;              // whether solve using brute force method 
    bool seedSelection = false;          // whether random seed is selected 
    char hive[MAX_HIVE_SIZE + 1] = {};   // array to store hive letters 
    hive[0] = '\0';
    int reqLetInd = -1;                  // required letter index 
    char reqLet = '\0';                  // required letter 

    // Read the command line argument whether it satisfies setSettings() configures program settings
    if (!setSettings(argc, argv, &randMode, &hiveSize, dict, &playMode, &bruteForce, &seedSelection)) {
        printf("Invalid command-line argument(s).\nTerminating program...\n");
        return 1;
    }
    else { // Print the settings of the program
        printf("Program Settings:\n");
        printf("  random mode = ");
        printONorOFF(randMode);
        printf("  play mode = ");
        printONorOFF(playMode);
        printf("  brute force solution = ");
        printONorOFF(bruteForce);
        printf("  dictionary file = %s\n", dict);
        printf("  hive set = ");
        printYESorNO(randMode);
        printf("\n\n");
    }

    // build word array (only words with desired minimum length or longer) from dictionary file
    printf("Building array of words from dictionary... \n");
    WordList* dictionaryList = createWordList();                                // Creat a dynamic WordList structure to store dictionary words
    int maxWordLength = buildDictionary(dict, dictionaryList, MIN_WORD_LENGTH); // add valid words to dictionaryList 
    if (maxWordLength == -1) { // If dictioanry is failed to load print out 
        printf("  ERROR in building word array.\n");
        printf("  File not found or incorrect number of valid words.\n");
        printf("Terminating program...\n");
        return 0;
    }
    printf("   Word array built!\n\n");


    printf("Analyzing dictionary...\n");

    // end program if file words is less than zero or can not be read
    if (dictionaryList->numWords < 0) {
        printf("  Dictionary %s not found...\n", dict);
        printf("Terminating program...\n");
        return 0;
    }

    // end program if file has zero words of minimum desired length or longer
    if (dictionaryList->numWords == 0) {
        printf("  Dictionary %s contains insufficient words of length %d or more...\n", dict, MIN_WORD_LENGTH);
        printf("Terminating program...\n");
        return 0;
    }
    else { 
        printf("  Dictionary %s contains \n  %d words of length %d or more;\n", dict, dictionaryList->numWords, MIN_WORD_LENGTH);
    }




    if (randMode) {
        printf("==== SET HIVE: RANDOM MODE ====\n");
        //find number of words in words array that use hiveSize unique letters
        WordList* fitWords = findAllFitWords(dictionaryList, hiveSize);
        int numFitWords = fitWords->numWords;
        //pick one at random
        int pickOne = rand() % numFitWords;
        char* chosenFitWord = fitWords->words[pickOne];

        //and alaphabetize the unique letters to make the letter hive
        buildHive(chosenFitWord, hive);
        freeWordList(fitWords);

        // randomly choose the required letter 
        reqLetInd = rand() % hiveSize;
        reqLet = hive[reqLetInd];

    }
    else {
            printf("==== SET HIVE: USER MODE ====\n");
        while(1) {    
            printf("  Enter a single string of lower-case,\n  unique letters for the letter hive... ");

            //---------------------------------------------------------------------
            /* TODO (Task 2-C): Get hive input (part 1 of 2)
            - Write code for randMode OFF, where user enters the hive; use the print statements below in the exact order you see them
            - ask the user to enter a string, following these requirements:
                - must be lowercase letters
                - string length must not be below MIN_HIVE_SIZE or above MAX_HIVE_SIZE
                - must not have any duplicate letters (you might find a helper function useful for this part)
            - If a word is invalid, display the appropriate error below and repeat until there is valid input
            - You may use buildHive() to help with this task
            (Task 2-C is continued a few lines below, getting the required letter)
            */
            scanf("%s", hive);

            int length = strlen(hive);
            if(length < MIN_HIVE_SIZE || length > MAX_HIVE_SIZE) {
                printf("  HIVE ERROR: \"%s\" has invalid length;\n  valid hive size is between %d and %d, inclusive\n\n",hive, MIN_HIVE_SIZE, MAX_HIVE_SIZE);   
                continue;
            }
        
            int validLetter = 1;
            for(int i = 0; hive[i] != '\0'; i++) {
                if(!islower(hive[i])) {
                    validLetter = 0;
                    break;
                }
            }
        
            
            if(!validLetter) {
                printf("  HIVE ERROR: \"%s\" contains invalid letters;\n  valid characters are lower-case alpha only\n\n",hive);
                continue;
            }

            int duplicateFound = 0;
            for(int i = 0; hive[i]  != '\0'; i++) {
                for(int j = i + 1; hive[j] != '\0'; j++) {
                    if(hive[i] == hive[j]) {
                    duplicateFound = 1;
                    break;
                    }
                }
                if(duplicateFound){
                    break;
                }
            }

            if(duplicateFound) {
                printf("  HIVE ERROR: \"%s\" contains duplicate letters\n\n",hive);
                continue;
            }
            break;
        }
        char tempHive[MAX_HIVE_SIZE + 1];
        buildHive(hive, tempHive);
        strcpy(hive,tempHive);

        reqLetInd = -1;
        reqLet = '\0';

        printf("  Enter the letter from \"%s\"\n  that is required for all words: ", hive);
        //---------------------------------------------------------------------
        /* TODO (Task 2-C): Get hive input (part 2 of 2)
        - Ask the user to enter the required letter, using the print statement above
        - If the letter is not in the hive, display the error message below and repeat the prompt until input is valid
        - If the letter the user enters is within the hive, then find the index of that letter in the hive,
        set that value you found for reqLetInd, and change reqLet to the required letter
        - Note that uppercase letters ARE valid here, unlike earlier; you will find it helpful to make them lowercase in code
        */
        while(reqLetInd == -1) {
            scanf(" %c", &reqLet);

            reqLet = tolower(reqLet);
            reqLetInd = findLetter(hive, reqLet);

            if(reqLetInd == -1) {
                printf("  HIVE ERROR: \"%s\" does not contain the character \'%c\'\n\n",hive,reqLet);
                printf("  Enter the letter from \"%s\"\n that is required for all words: ", hive);
            }
        }  
    }
    printHive(hive, reqLetInd);

    if (playMode) {
        printf("==== PLAY MODE ====\n");

        char* userWord = (char*)malloc((maxWordLength + 1) * sizeof(char)); // allocate memory to store user enter words
        strcpy(userWord, "default");

        WordList* userWordList = createWordList(); // valid word list the user finds

        printf("............................................\n");
        printHive(hive, reqLetInd);


        printf("  Enter a word (enter DONE to quit): "); // ask user to enter the first word 
        scanf("%s", userWord);
        printf("\n");

        // continue until user enter "DONE"
        while (strcmp(userWord, "DONE")) {

            //---------------------------------------------------------------------
            /* TODO (Task 4-A, part 2 of 2): Validate input from the user (for play mode)
            - userWord is the input from the user (their attempted word)
            - If the word is not valid (it's too short, not in the dictionary, not valid for the hive), 
                or it has already been entered, display the appropriate error message below.
            - If the word is valid, add it to userWordList
            
            - The end of the while loop is already given to you, make sure not to remove that
            - As always, helper functions will benefit you greatly here
            */
            if (strlen(userWord) < MIN_WORD_LENGTH) {    
                printf(" !!! \"%s\" is TOO SHORT (min length = %d) !!!\n", userWord, MIN_WORD_LENGTH);
            }
            else {
                bool inDictionary = false;

                for (int i = 0; i < dictionaryList->numWords; i++) {
                    if (strcmp(dictionaryList->words[i], userWord) == 0) {
                        inDictionary = true;
                        break;
                    }
                }
                
                if (!inDictionary) {
                    printf(" !!! \"%s\" is NOT a valid word in the dictionary !!!\n", userWord);
                }
                else if (!isValidWord(userWord, hive, reqLet)) {
                    printf(" !!! \"%s\" is NOT valid for the hive !!!\n", userWord);
                }
                else{
                    bool duplicate = false;

                    for (int i = 0; i < userWordList->numWords; i++) {
                        if (strcmp(userWordList->words[i], userWord) == 0) {
                            duplicate = true;
                            break;
                        }
                    }
                    if (duplicate) {
                        printf(" !!! \"%s\" is already in your word list !!!\n", userWord);
                    }
                    else{
                        appendWord(userWordList, userWord);
                    }
                }
            }
            
            // Keep the code below this comment, which prints the list and the hive, and gets the next input
            printf("\n");
            printList(userWordList, hive);
            printf("............................................\n");
            printHive(hive, reqLetInd);

            printf("  Enter a word (enter DONE to quit): ");
            scanf("%s", userWord);
            printf("\n");

        }

        // Do until user enters quit request
        //  Display Hive & Word List & Current Score
        //  X Allow user to enter a word
        //  X Check if word is in dict
        //  X Check if word is a duplicate
        //  Check if word is valid for the hive
        //  If all valid, add it to word list, report score  

        freeWordList(userWordList);
        free(userWord);
    }

    printf("==== SPELLING BEE SOLVER ====\n");

    printf("  Valid words from hive \"%s\":\n", hive);
    printf("                         ");
    for (int i = 0; i < reqLetInd; i++) {  // print spacing before required letter 
        printf(" ");
    }
    printf("^\n");

    WordList* solvedList = createWordList(); // create a solved list to store user solved results

    if (bruteForce) { //find all words that work... (1) brute force
        bruteForceSolve(dictionaryList, solvedList, hive, reqLet);
    }
    else {
        char* tryWord = (char*)malloc(sizeof(char) * (maxWordLength + 1));

        tryWord[0] = hive[0];
        tryWord[1] = '\0';
        findAllMatches(dictionaryList, solvedList, tryWord, hive, reqLet);
        free(tryWord);

    }

    // longest word length solved 
    int longestSolvedWordLen = 0;
    for (int i = 0; i < solvedList->numWords; i++) {
        if (strlen(solvedList->words[i]) > longestSolvedWordLen) {
            longestSolvedWordLen = strlen(solvedList->words[i]);
        }
    }

    // Helpful variables
    int numValidWords = 0;
    int numPangrams = 0;
    int numPerfectPangrams = 0;
    int totScore = 0;
    bool isBingo = true;

    //---------------------------------------------------------------------
    /* TODO (Task 5-B): Display solver results
    - Display list of words solver found, noting pangrams and perfect pangrams with asterisks like printList
    - Calculate the number of valid words, number of pangrams, number of perfect pangrams, 
          total possible score, and whether there is a bingo. Save those values in the corresponding
          variables, or update the printf statements below to dispay those values
    */
    numValidWords = solvedList->numWords;
    hiveSize = strlen(hive);

    for (int i = 0; i < solvedList->numWords; i++) {
        char* word = solvedList->words[i];
        int wordLength = strlen(word);
        int score = 0;

        if (wordLength == MIN_WORD_LENGTH) {
            score = 1;
        }
        else {
            score = wordLength;
        }

        if (isPangram(word, hive)) {
            numPangrams++;
            score = wordLength + hiveSize;
            if (wordLength == hiveSize) {
                numPerfectPangrams++;
                printf("*** ");
            } 
            else{
                printf("  * ");
            }
        }
        else{
            printf("     ");
        }
        printf(" (%2d) %s\n", score, word);
        totScore += score;
    }


    for (int i = 0; i < hiveSize; i++) {
        char letter = hive[i];
        bool found = false;

        for (int j = 0; j < solvedList->numWords; j++) {
            if (solvedList->words[j][0] == letter) {
                found = true;
                break;
            }
        }
        if (!found) {
            isBingo = false;
            break;
        }
    }

    // Additional results are printed here:
    printf("\n");
    printf("  Total counts for hive \"%s\":\n", hive);
    printf("                         ");
    for (int i = 0; i < reqLetInd; i++) {
        printf(" ");
    }
    printf("^\n");
    printf("    Number of Valid Words: %d\n", numValidWords);
    printf("    Number of ( * ) Pangrams: %d\n", numPangrams);
    printf("    Number of (***) Perfect Pangrams: %d\n", numPerfectPangrams);
    printf("    Bingo: ");
    printYESorNO(isBingo);
    printf("    Total Score Possible: %d\n", totScore);

    //---------------------------------------------------------------------
    /* TODO Extra credit: Display frequency table
    - Display a table showing the number of words starting with each letter and duration in a table.
    - For example, with the hive abcde and required letter e:
             Frequency Table:
             let  4  5  6  7  8
             -------------------
              a:  3  1  1  1  0
              b:  3  1  4  0  0
              c:  3  1  0  0  1
              d:  3  0  3  0  0
              e:  0  1  0  0  0
       The table shows that there are 3 words that start with a and are 4 letters long, 
       and 1 each that are 5, 6, and 7 letters long and start with a.
       Note that the lengths 4-8 are shown here because the longest word is 8 letters long,
       but that will change depending on the hive
    */

    // free all dynamically allocated memory 
    freeWordList(dictionaryList);
    freeWordList(solvedList);
    printf("\n\n");
    return 0;
}

