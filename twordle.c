//Project 10: Twordle
//Author: Anthony Lozano

#include <stdio.h>
#include <stdbool.h>

#define STR_CAP 100
#define WORDFILE "word.txt"

bool getAnswer(char answer[]);
void getGuess(char guesses[][STR_CAP], int guessNum);
bool compareGuess(char guesses[][STR_CAP], char answer[], char correctLetters[][6], int guessNum);
bool checkGuess(char guesses[][STR_CAP], int guessNum);
void showGuesses(char guesses[][STR_CAP], char correctLetters[][6], int guessNum);
void gameWon(char guesses[][STR_CAP], int guessNum);
void gameLost(char guesses[][STR_CAP], char correctLetters[][6], int guessNum);

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
		
		i = 0;
		do{
			if (guesses[guessNum - 1][i] >= 65 && guesses[guessNum - 1][i] <= 90) {
				guesses[guessNum - 1][i] += 32;
				lettersGood = true;
				
			} else if (guesses[guessNum - 1][i] >= 97 && guesses[guessNum - 1][i] <= 122) {
				lettersGood = true;
				i++;
			} else {
				lettersGood = false;
				for(i ; guesses[guessNum - 1][i] != '\0' ; i++) {
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
	char doubleLetters[5] = {"     "};
	int guessInd, ansInd, tempInd, doubleLetInd = 0;
	bool tempMatch;
	
	for(guessInd = 0; guessInd < 5; guessInd++) {
		for(ansInd = 0; ansInd < 5; ansInd++) {
			if(guesses[guessNum - 1][guessInd] == answer[ansInd]) {
				for (tempInd = 0; tempInd < 5; tempInd++){
					if(guesses[guessNum - 1][guessInd] == doubleLetters[tempInd]) {
						tempMatch = true;
					}
				}
				
				if(!tempMatch) {
					if(guessInd == ansInd){
						guesses[guessNum - 1][guessInd] -= 32;
						
					} else {
						correctLetters[guessNum - 1][guessInd] = '^';
					}
					
					doubleLetters[doubleLetInd] = guesses[guessNum - 1][guessInd];
					doubleLetInd++;
				}
				tempMatch = false;
			}
		}
	}
	
	if (checkGuess(guesses, guessNum)) {
		return true;
	} else {
		return false;
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
