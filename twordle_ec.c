//Project 10: Twordle
//Author: Anthony Lozano

#include <stdio.h>
#include <stdbool.h>

#define STR_CAP 100
#define CASE_VALUE 32
#define WORDFILE "word.txt"
#define SCOREFILE "scoreboard.txt"

bool getAnswer(char answer[]);
void getGuess(char guesses[][STR_CAP], int guessNum);
bool compareGuess(char guesses[][STR_CAP], char answer[], char correctLetters[][6], int guessNum);
void doubleLetters(char answer[], char* firstDoubleLetPtr, char* secondDoubleLetPtr, int* firstDoubleNumPtr, int* secondDoubleNumPtr);
bool checkGuess(char guesses[][STR_CAP], int guessNum);
void showGuesses(char guesses[][STR_CAP], char correctLetters[][6], int guessNum);
void gameWon(char guesses[][STR_CAP], int guessNum);
void gameLost(char guesses[][STR_CAP], char correctLetters[][6], int guessNum);
void scoreboard(int score);
void caseSwitch(char string[], int letterCase, int length);

int main() {
	char answer[6], guesses[6][STR_CAP], correctLetters[6][6] = {"     ", "     ", "     ", "     ", "     ", "     "};
	int guessNum = 0;
	bool open, correct;
	
	open = getAnswer(answer);
	if(!open) {
		return 0;
	}
	
	do{
		showGuesses(guesses, correctLetters, guessNum);
		guessNum++;
		
		if (guessNum != 6){
			printf("GUESS %d! Enter your guess: ", guessNum);	
		} else {
			printf("LAST GUESS! Enter your guess: ");
		}
		getGuess(guesses, guessNum);
		
		correct = compareGuess(guesses, answer, correctLetters, guessNum);
		printf("================================\n");
	}while(!correct && guessNum != 6);
	
	if(correct) {
		gameWon(guesses, guessNum);
		scoreboard(guessNum);
	} else {
		gameLost(guesses, correctLetters, guessNum);
	}
}

bool getAnswer(char answer[]){
	FILE *filePtr = fopen(WORDFILE, "r");
	if (filePtr == NULL) {
		printf("Couldn't open file, cannot continue.\n");
		return false;
	}
	
	fscanf(filePtr, "%s", answer);
	fclose(filePtr);
	return true;
}

void getGuess(char guesses[][STR_CAP], int guessNum){
	bool lettersGood, lengthGood;
	int i;
	
	do{
		scanf(" %s", guesses[guessNum - 1]);
		caseSwitch(guesses[guessNum - 1], 1, 5);
		
		i = 0;
		do{
			if (guesses[guessNum - 1][i] >= 97 && guesses[guessNum - 1][i] <= 122) {
				lettersGood = true;
				i++;
				
			} else {
				for(i = i; guesses[guessNum - 1][i] != '\0' ; i++) {
					lettersGood = false;
				}
			}
		}while(guesses[guessNum - 1][i] != '\0');
		
		if (i != 5) {
			lengthGood = false;
		} else {
			lengthGood = true;
		}
		
		if (!lettersGood) {
			printf("Your guess must contain only letters. ");
		}
		
		if (!lengthGood) {
			printf("Your guess must 5 letters long.");
		}
		
		if (!lettersGood || !lengthGood) {
			printf("\nPlease try again: ");
		}
	}while(!lettersGood || !lengthGood);
}

bool compareGuess(char guesses[][STR_CAP], char answer[], char correctLetters[][6], int guessNum) {
	char firstDoubleLet = '\0', secondDoubleLet = '\0';
	int temp, guessInd, ansInd, firstDoubleNums, secondDoubleNums, currentFirstDoubles = 0, currentSecondDoubles = 0;
	bool firstDoubleMatch, secondDoubleMatch, nonDoubleMatch;
	
	doubleLetters(answer, &firstDoubleLet, &secondDoubleLet, &firstDoubleNums, &secondDoubleNums);
	
	for(guessInd = 0; guessInd < 5; guessInd++) {
		for(ansInd = 0; ansInd < 5; ansInd++) {
			if(guesses[guessNum - 1][guessInd] == answer[ansInd] && guessInd == ansInd) {
				guesses[guessNum - 1][guessInd] -= CASE_VALUE;
				
				if(guesses[guessNum - 1][guessInd] == firstDoubleLet) {
					currentFirstDoubles++;
				}
				if(guesses[guessNum - 1][guessInd] == secondDoubleLet) {
					currentSecondDoubles++;
				}
			}
		}
	}
	
	for(guessInd = 0; guessInd < 5; guessInd++) {
		for(ansInd = 0; ansInd < 5; ansInd++) {
			firstDoubleMatch = false;
			secondDoubleMatch = false;
			nonDoubleMatch = false;
			if(guesses[guessNum - 1][guessInd] == answer[ansInd] && guessInd != ansInd) {
				if(guesses[guessNum - 1][guessInd] == firstDoubleLet) {
					currentFirstDoubles++;
					firstDoubleMatch = true;
				}
				if(guesses[guessNum - 1][guessInd] == secondDoubleLet) {
					currentSecondDoubles++;
					secondDoubleMatch = true;
				}
				
				if(firstDoubleMatch) {
					if(currentFirstDoubles <= firstDoubleNums) {
						correctLetters[guessNum - 1][guessInd] = '^';
					}
				} else if(secondDoubleMatch) {
					if(currentSecondDoubles <= secondDoubleNums) {
							correctLetters[guessNum - 1][guessInd] = '^';
						}
				} else {
					for(temp = guessInd - 1; temp >= 0; temp--){
						if(guesses[guessNum - 1][guessInd] == guesses[guessNum - 1][temp] || guesses[guessNum - 1][guessInd] == guesses[guessNum - 1][temp] + CASE_VALUE) {
							nonDoubleMatch = true;
						}
					}
					if(!nonDoubleMatch) {
						correctLetters[guessNum - 1][guessInd] = '^';
					}
				}
			}
		}
	}
	
	if (checkGuess(guesses, guessNum)) {
		return true;
	} else {
		return false;
	}
}

void doubleLetters(char answer[], char* firstDoubleLetPtr, char* secondDoubleLetPtr, int* firstDoubleNumPtr, int* secondDoubleNumPtr) {
	int firstIndex, secondIndex, doubleIndex, firstDoubleLocs[5], secondDoubleLocs[3];
	bool savedLoc, doubleMatch;
	
	*firstDoubleNumPtr = 0;
	*secondDoubleNumPtr = 0;
	
	for(firstIndex = 0; firstIndex < 5; firstIndex++) {
		for(secondIndex = firstIndex + 1; secondIndex < 5; secondIndex++)
			if(answer[firstIndex] == answer[secondIndex] && firstIndex != secondIndex){
				if(*firstDoubleNumPtr == 0) {
					firstDoubleLocs[0] = firstIndex;
					firstDoubleLocs[1] = secondIndex;
					*firstDoubleNumPtr += 2;
				} else {
					doubleIndex = 0;
					savedLoc = false;
					while(!savedLoc && doubleIndex < *firstDoubleNumPtr) {
						if(secondIndex == firstDoubleLocs[doubleIndex]) {
							savedLoc = true;
						}
					}
					
					if(!savedLoc) {
						doubleIndex = 0;
						doubleMatch = false;
						while(!doubleMatch && doubleIndex < *firstDoubleNumPtr) {
							if(answer[secondIndex] == answer[firstDoubleLocs[0]]) {
								doubleMatch = true;
							}
						}
						
						if(doubleMatch) {
							firstDoubleLocs[*firstDoubleNumPtr] = secondIndex;
							*firstDoubleNumPtr += 1;
						} else {
							if(*secondDoubleNumPtr == 0) {
								secondDoubleLocs[0] = firstIndex;
								secondDoubleLocs[1] = secondIndex;
								*secondDoubleNumPtr += 2;
							} else {
								secondDoubleLocs[*secondDoubleNumPtr] = secondIndex;
								*secondDoubleNumPtr += 1;
							}
						}
					}
				}
			}
	}
	
	if(*firstDoubleNumPtr != 0) {
		*firstDoubleLetPtr = answer[firstDoubleLocs[0]];
	}
	
	if(*secondDoubleNumPtr != 0) {
		*secondDoubleLetPtr = answer[secondDoubleLocs[0]];
	}
}

bool checkGuess(char guesses[][STR_CAP], int guessNum){
	int index = 0;
	bool match;
	do{
		if (guesses[guessNum - 1][index] >= 65 && guesses[guessNum - 1][index] <= 90) {
			match = true;
		} else {
			match = false;
		}
		index++;
	}while(match && index < 5);
	
	if (match) {
		return true;
	} else {
		return false;
	}
}
void showGuesses(char guesses[][STR_CAP], char correctLetters[][6], int guessNum){
	int i;
	
	for(i = 0; i < guessNum; i++){
		printf("%s\n%s\n", guesses[i], correctLetters[i]);
	}
}

void gameWon(char guesses[][STR_CAP], int guessNum){
	printf("		%s\n", guesses[guessNum - 1]);
	if (guessNum == 1) {
		printf("	You won in 1 guess!\n");
		printf("		GOATED!\n");
	} else {
		printf("	You won in %d guesses!\n", guessNum);
		printf("		Nice!\n");
	}
}

void gameLost(char guesses[][STR_CAP], char correctLetters[][6], int guessNum){
	showGuesses(guesses, correctLetters, guessNum);
	printf("You lost, better luck next time!\n");
}

void scoreboard(int score){
	char playerID[4], topID[5][4];
	int i, highScores[5], place;
	bool bad, newScore = false;
	FILE* scoreFilePtr;
	scoreFilePtr = fopen(SCOREFILE, "r");
	
	if(scoreFilePtr == NULL) {
		printf("Couldn't open scoreboard file, can't show scores :(\n");
		return;
	}
	
	fscanf(scoreFilePtr,"SCOREBOARD:\nID      Score\n%s     %d", topID[0], &highScores[0]);
	for(i = 1; i < 5; i++) {
		fscanf(scoreFilePtr," %s    %d", topID[i], &highScores[i]);
	}
	fclose(scoreFilePtr);
	
	i = 0;
	do{
		if(score < highScores[i]) {
			newScore = true;
		} else {
			i++;
		}
	}while(!newScore && i < 5);
	
	if(newScore) {
		place = i;
		scoreFilePtr = fopen(SCOREFILE, "w");
				
		if(scoreFilePtr == NULL) {
			printf("Couldn't open scoreboard file, can't show scores :(\n");
			return;
		}
		
		printf("\nEnter 3 letter ID to save your score! ");
		do{
			scanf(" %s", playerID);
			
			for(i=0; playerID[i] != '\0';i++) {
			}
			
			if(i != 3) {
				printf("Must be 3 letters, try again! ");
				bad = true;
			} else {
				bad = false;
			}
		}while(bad);
		caseSwitch(playerID, 2, 4);
			
		for(i = 4; i > place; i--) {
			topID[i][0] = topID[i - 1][0];
			topID[i][1] = topID[i - 1][1];
			topID[i][2] = topID[i - 1][2];
			highScores[i] = highScores[i - 1];
		}
		topID[place][0] = playerID[0];
		topID[place][1] = playerID[1];
		topID[place][2] = playerID[2];
		highScores[place] = score;
		
		fprintf(scoreFilePtr, "SCOREBOARD:\nID      Score\n");
		for(i = 0; i < 5; i ++) {
			fprintf(scoreFilePtr, "%s     %d\n", topID[i], highScores[i]); 
		}
		fclose(scoreFilePtr);
	} else {
		printf("Not enough to get a high score, try again next time!\n");
	}
	
	printf("SCOREBOARD:\nID      Score\n");
	for(i = 0; i < 5; i ++) {
		printf("%s     %d\n", topID[i], highScores[i]); 
	}
}

void caseSwitch(char string[], int letterCase, int length){
	int index;
	
	if(letterCase == 1){
		for(index = 0; index < length; index++){
			if (string[index] >= 65 && string[index] <= 90) {
					string[index] += CASE_VALUE;
						
			}
		}
	} else {
		for(index = 0; index < length; index++){
			if (string[index] >= 97 && string[index] <= 122) {
					string[index] -= CASE_VALUE;
						
			}
		}
	}
}
