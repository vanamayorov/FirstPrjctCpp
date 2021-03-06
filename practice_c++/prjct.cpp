#include <iostream>
#include <ctime>
#include <vector>
#include <fstream>
#include <string>
#include <cstdlib>
using namespace std;

#define ORDINARY_SIZE 33
//#define DEBUG
bool guessANumberGame(int* deposit);
void printMessage(string message, bool printTop, bool printBottom, int numbers);
void drawHangMan(int hangCounts);
void printLetters(string input, char from, char to);
void printAvailableLetters(string initial);
bool printWordAndCheckWin(string word, string guessed);
string loadRandomWord(string path);
void chooseGame(int number, int* deposit, bool* exitCondition);
int triesLeft(string word, string guessed);
void alterDeposit(int* deposit, bool win);
void depositCheck(int* deposit);
bool validateString(string& name);
void drawArray(char array[][3]);
void inputPosition(char array[][3]);
void togglePlayer(char array[][3]);
char checkWin(char array[][3]);
int discoverWinner(char validation);
int main() {
	system("color F2");
	setlocale(LC_ALL, "Rus");
	srand(time(NULL));
	const int MAX_SIZE_OF_NAME = 50;
	string name;
	int deposit,
		game,
		tries;
	char response;
	bool exitCondition = false,
		stringValidated = false;

	printMessage("WELCOME TO OUR CASINO!", true, true, ORDINARY_SIZE);
	cout << "Please enter your name:" << endl << "> ";
	getline(cin, name);
	stringValidated = validateString(name);
	while (name.length() > MAX_SIZE_OF_NAME || name.empty() || !stringValidated) {
		printMessage("Please enter your name", true, false, ORDINARY_SIZE + 5);
		printMessage("name cannot be larger than 50 symbols", false, true, ORDINARY_SIZE + 5);
		cout << "> ";
		getline(cin, name);
		stringValidated = validateString(name);
	}
	cout << "Welcome, " << name << endl;
	cout << "Deposit your amount: $ > ";
	cin >> deposit;
	do {
		depositCheck(&deposit);
		cout << name << ", your balance is " << deposit << '$' << endl;
		cout << "List of games: \n 1 - Guess a number\n 2 - Hangman game\n 3 - TicTacToe\n 4 - Exit" << endl << "> ";
		while (!(cin >> game) || (game != 1 && game != 2 && game != 3 && game != 4)) {
			cin.clear();
			cin.ignore(INT_MAX, '\n');
			cout << "Invalid type of number!" << endl;
			cout << "List of games: \n 1 - Guess a number\n 2 - Hangman game\n 3 - TicTacToe\n 4 - Exit" << endl << "> ";
		}
		chooseGame(game, &deposit, &exitCondition);
		if (exitCondition) {
			break;
		}
		if (deposit > 0) {
			response = 'y';
			continue;
		}
		else {
			cout << "> ";
			cin >> deposit;
			depositCheck(&deposit);
			response = 'y';
			continue;
		}
	} while (response == 'y' || response == 'Y');
	return 0;
}
void chooseGame(int number, int* deposit, bool* exitCondition) {
	char response;
	bool win;
	switch (number) {
	case 1:
		printMessage("You chose a guess number game!", true, true, ORDINARY_SIZE);
		do {
			win = guessANumberGame(deposit);
			if (win) {
				cout << "Would you like to continue playing guess a number game?(y/Y - yes)" << endl << "> ";
				cin >> response;
				system("cls");
			}
			else {
				cout << "> ";
				cin >> *deposit;
				depositCheck(deposit);
				response = 'y';
				continue;
			}
		} while (response == 'y' || response == 'Y');
		break;
	case 2:
		do {
			string guesses;
			string wordToGuess = loadRandomWord("words.txt");
			int tries = 0;
			bool win = false;
			do {
				system("cls");
				printMessage("You chose a Hangman game", true, true, ORDINARY_SIZE);
				drawHangMan(tries);
				printAvailableLetters(guesses);
				printMessage("Guess the word", true, true, ORDINARY_SIZE);
				win = printWordAndCheckWin(wordToGuess, guesses);
				if (win) {
					break;
				}
				char x;
				cout << "> ";
				cin >> x;
				if (guesses.find(x) == string::npos) {
					guesses += x;
				}
				tries = triesLeft(wordToGuess, guesses);
			} while (tries < 7 && *deposit > 10);
			if (win) {
				printMessage("YOU WON!", true, true, ORDINARY_SIZE);
				alterDeposit(deposit, true);
				cout << "Your balance is " << *deposit << '$' << endl;
			}
			else {
				printMessage("GAME LOST", true, true, ORDINARY_SIZE);
				alterDeposit(deposit, false);
				if (*deposit <= 0) {
					printMessage("You don`t have enough money ", false, false, ORDINARY_SIZE + 5);
					printMessage("to continue playing!", false, false, ORDINARY_SIZE + 5);
					printMessage("Please, deposit money to play a game", false, true, ORDINARY_SIZE + 5);
					cout << "> ";
					cin >> *deposit;
					depositCheck(deposit);
					response = 'y';
					continue;
				}
				else {
					cout << "Your balance is " << *deposit << '$' << endl;
				}
			}
			cout << "Would you like to continue playing a hangman game?(y/Y - yes)" << endl << "> ";
			cin >> response;
			system("cls");
		} while (response == 'y' || response == 'Y');
		system("cls");
		break;
	case 3:
		do {
			char array[3][3] = { {'1', '2', '3' }, {'4', '5', '6'}, {'7', '8', '9'} };
			int n = 0;
			char validation = '/';
			system("cls");
			printMessage("YOU CHOOSE TICTACTOE", true, true, ORDINARY_SIZE);
			while (n < 9 && validation == '/') {
				inputPosition(array);
				validation = checkWin(array);
				if (discoverWinner(validation) == 1) {
					printMessage("PLAYER 1 IS A WINNER", true, false, ORDINARY_SIZE);
					alterDeposit(deposit, true);
					printMessage("YOUR AWARD IS +100$", false, true, ORDINARY_SIZE);
					break;
				}
				else if (discoverWinner(validation) == 2) {
					printMessage("PLAYER 2 IS A WINNER", true, false, ORDINARY_SIZE);
					alterDeposit(deposit, true);
					printMessage("YOUR AWARD IS +100$", false, true, ORDINARY_SIZE);
					break;
				}
				else {
					n++;
#ifdef DEBUG
					cout << "Score" << n << endl;
#endif
				}
				if (n == 9) {
					printMessage("IT`S A DRAW!", true, false, ORDINARY_SIZE);
					alterDeposit(deposit, false);
					printMessage("YOU ARE NOT LUCKY TODAY -100$", false, true, ORDINARY_SIZE);
					break;
				}
				togglePlayer(array);
				validation = checkWin(array);
				if (discoverWinner(validation) == 1) {
					printMessage("PLAYER 1 IS A WINNER", true, false, ORDINARY_SIZE);
					alterDeposit(deposit, true);
					printMessage("YOUR AWARD IS +100$", false, true, ORDINARY_SIZE);
					break;
				}
				else if (discoverWinner(validation) == 2) {
					printMessage("PLAYER 2 IS A WINNER", true, false, ORDINARY_SIZE);
					alterDeposit(deposit, true);
					printMessage("YOUR AWARD IS +100$", false, true, ORDINARY_SIZE);
					break;
				}
				else {
					n++;
#ifdef DEBUG
					cout << "Score" << n << endl;
#endif
				}

			if (n == 9) {
				printMessage("IT`S A DRAW!", true, false, ORDINARY_SIZE);
				alterDeposit(deposit, false);
				printMessage("YOU ARE NOT LUCKY TODAY -100$", false, true, ORDINARY_SIZE);
				break;
			}
			}
			cout << "Would you like to continue playing a tictactoe game?(y/Y - yes)" << endl << "> ";
			cin >> response;
			system("cls");
		} while (response == 'y' || response == 'Y');
		break;
	case 4:
		*exitCondition = true;
		break;
	default:
		cout << "Invalid type of number!" << endl;
		break;
	}
}

int discoverWinner(char validation) {
	if (validation == 'X') {
		return 1;
	}
	else if (validation == 'O') {
		return 2;
	}
	return 0;
}
char checkWin(char array[][3]) {

	if (array[0][0] == 'X' && array[0][1] == 'X' && array[0][2] == 'X') {
		return 'X';
	}
	else if (array[1][0] == 'X' && array[1][1] == 'X' && array[1][2] == 'X') {
		return 'X';
	}
	else if (array[2][0] == 'X' && array[2][1] == 'X' && array[2][2] == 'X') {
		return 'X';
	}
	else if (array[0][0] == 'X' && array[1][0] == 'X' && array[2][0] == 'X') {
		return 'X';
	}
	else if (array[0][1] == 'X' && array[1][1] == 'X' && array[2][1] == 'X') {
		return 'X';
	}
	else if (array[0][2] == 'X' && array[1][2] == 'X' && array[2][2] == 'X') {
		return 'X';
	}
	else if (array[0][0] == 'X' && array[1][1] == 'X' && array[2][2] == 'X') {
		return 'X';
	}
	else if (array[2][0] == 'X' && array[1][1] == 'X' && array[0][2] == 'X') {
		return 'X';
	}
	if (array[0][0] == 'O' && array[0][1] == 'O' && array[0][2] == 'O') {
		return 'O';
	}
	else if (array[1][0] == 'O' && array[1][1] == 'O' && array[1][2] == 'O') {
		return 'O';
	}
	else if (array[2][0] == 'O' && array[2][1] == 'O' && array[2][2] == 'O') {
		return 'O';
	}
	else if (array[0][0] == 'O' && array[1][0] == 'O' && array[2][0] == 'O') {
		return 'O';
	}
	else if (array[0][1] == 'O' && array[1][1] == 'O' && array[2][1] == 'O') {
		return 'O';
	}
	else if (array[0][2] == 'O' && array[1][2] == 'O' && array[2][2] == 'O') {
		return 'O';
	}
	else if (array[0][0] == 'O' && array[1][1] == 'O' && array[2][2] == 'O') {
		return 'O';
	}
	else if (array[2][0] == 'O' && array[1][1] == 'O' && array[0][2] == 'O') {
		return 'O';
	}
	return '/';
}
void togglePlayer(char array[][3]) {
	int position;
	cout << "Player 2" << endl;
	do {
		cin.clear();
		cin.ignore(INT_MAX, '\n');
		drawArray(array);
		cout << "Choose Position:" << endl << "> ";
		cin >> position;

	} while (!cin.good() || position > 9 || position < 1);
	switch (position) {
	case 1:
		if (array[0][0] == 'X' || array[0][0] == 'O') {
			cout << "This position is occupied, try again." << endl;
			togglePlayer(array);
		}
		else {
			array[0][0] = 'O';
		}
		break;
	case 2:
		if (array[0][1] == 'X' || array[0][1] == 'O') {
			cout << "This position is occupied, try again." << endl;
			togglePlayer(array);
		}
		else {
			array[0][1] = 'O';
		}

		break;
	case 3:
		if (array[0][2] == 'X' || array[0][2] == 'O') {
			cout << "This position is occupied, try again." << endl;
			togglePlayer(array);
		}
		else {
			array[0][2] = 'O';
		}
		break;
	case 4:
		if (array[1][0] == 'X' || array[1][0] == 'O') {
			cout << "This position is occupied, try again." << endl;
			togglePlayer(array);
		}
		else {
			array[1][0] = 'O';
		}
		break;
	case 5:
		if (array[1][1] == 'X' || array[1][1] == 'O') {
			cout << "This position is occupied, try again." << endl;
			togglePlayer(array);
		}
		else {
			array[1][1] = 'O';
		}
		break;
	case 6:
		if (array[1][2] == 'X' || array[1][2] == 'O') {
			cout << "This position is occupied, try again." << endl;
			togglePlayer(array);
		}
		else {
			array[1][2] = 'O';
		}
		break;
	case 7:
		if (array[2][0] == 'X' || array[2][0] == 'O') {
			cout << "This position is occupied, try again." << endl;
			togglePlayer(array);
		}
		else {
			array[2][0] = 'O';
		}

		break;
	case 8:
		if (array[2][1] == 'X' || array[2][1] == 'O') {
			cout << "This position is occupied, try again." << endl;
			togglePlayer(array);
		}
		else {
			array[2][1] = 'O';
		}

		break;
	case 9:
		if (array[2][2] == 'X' || array[2][2] == 'O') {
			cout << "This position is occupied, try again." << endl;
			togglePlayer(array);
		}
		else {
			array[2][2] = 'O';
		}

		break;
	}

}
void inputPosition(char array[][3]) {
	int position;
	cout << "Player 1" << endl;
	do {
		cin.clear();
		cin.ignore(INT_MAX, '\n');
		drawArray(array);
		cout << "Choose Position:" << endl << "> ";
		cin >> position;

	} while (!cin.good() || position > 9 || position < 1);
	switch (position) {
	case 1:
		if (array[0][0] == 'O' || array[0][0] == 'X') {
			cout << "This position is occupied, try again." << endl;
			inputPosition(array);
		}
		else {
			array[0][0] = 'X';
		}
		break;
	case 2:
		if (array[0][1] == 'O' || array[0][1] == 'X') {
			cout << "This position is occupied, try again." << endl;
			inputPosition(array);
		}
		else {
			array[0][1] = 'X';
		}

		break;
	case 3:
		if (array[0][2] == 'O' || array[0][2] == 'X') {
			cout << "This position is occupied, try again." << endl;
			inputPosition(array);
		}
		else {
			array[0][2] = 'X';
		}		
		
		break;
	case 4:
		if (array[1][0] == 'O' || array[1][0] == 'X') {
			cout << "This position is occupied, try again." << endl;
			inputPosition(array);
		}
		else {
			array[1][0] = 'X';
		}
		break;
	case 5:
		if (array[1][1] == 'O' || array[1][1] == 'X') {
			cout << "This position is occupied, try again." << endl;
			inputPosition(array);
		}
		else {
			array[1][1] = 'X';
		}
		break;
	case 6:
		if (array[1][2] == 'O' || array[1][2] == 'X') {
			cout << "This position is occupied, try again." << endl;
			inputPosition(array);
		}
		else {
			array[1][2] = 'X';
		}
		break;
	case 7:
		if (array[2][0] == 'O' || array[2][0] == 'X') {
			cout << "This position is occupied, try again." << endl;
			inputPosition(array);
		}
		else {
			array[2][0] = 'X';
		}

		break;
	case 8:
		if (array[2][1] == 'O' || array[2][1] == 'X') {
			cout << "This position is occupied, try again." << endl;
			inputPosition(array);
		}
		else {
			array[2][1] = 'X';
		}

		break;
	case 9:
		if (array[2][2] == 'O' || array[2][2] == 'X') {
			cout << "This position is occupied, try again." << endl;
			inputPosition(array);
		}
		else {
			array[2][2] = 'X';
		}
		break;
	}

}
void alterDeposit(int* deposit, bool win) {
	if (win) {
		*deposit += 100;
	}
	else {
		if (*deposit - 100 < 0) {
			*deposit = 0;
		}
		else {
			*deposit -= 100;
		}
	}
}
bool guessANumberGame(int* deposit) {
	int number, range, guess;
	bool win = true;
	cout << "Choose a range you want to guess a number from!" << endl << "> ";
	cin >> range;
	while (!cin.good() || range <= 0) {
		cin.clear();
		cin.ignore(INT_MAX, '\n');
		printMessage("Invalid type of number!", true, false, ORDINARY_SIZE + 5);
		printMessage("Range must be positive and integer!", false, true, ORDINARY_SIZE + 5);
		cout << "> ";
		cin >> range;
	}
	number = rand() % range;
	printMessage("A number has been generated!", true, false, ORDINARY_SIZE);
	printMessage("Try to guess it now!", false, true, ORDINARY_SIZE);
	cout << "> ";
	cin >> guess;
	while (guess != number) {
		*deposit -= 10;
		if (*deposit <= 0) {
			win = false;
			break;
		}
		cout << "Wrong answer! Try again :)" << endl;
		cout << "Your balance is " << *deposit << '$' << endl << "> ";
		cin >> guess;
	}
	if (win) {
		printMessage("Congrats!", true, false, ORDINARY_SIZE);
		printMessage("You have guessed a number!", false, false, ORDINARY_SIZE);
		printMessage("Here is your reward: +100$", false, true, ORDINARY_SIZE);
		*deposit += 100;
		cout << "Your balance is " << *deposit << '$' << endl;
	}
	else {
		printMessage("Wrong answer! ", true, false, ORDINARY_SIZE + 5);
		printMessage("You don`t have enough money ", false, false, ORDINARY_SIZE + 5);
		printMessage("to continue playing!", false, false, ORDINARY_SIZE + 5);
		printMessage("Please, deposit money to play a game", false, true, ORDINARY_SIZE + 5);
	}
	return win;
}

void printMessage(string message, bool printTop, bool printBottom, int numbers) {
	bool front = true;
	if (printTop) {
		if (numbers > 33) {
			cout << "****************************************" << endl;
			cout << "|";
		}
		else if (numbers == 33) {
			cout << "***********************************" << endl;
			cout << "|";
		}
		else {
			cout << "*************************" << endl;
			cout << "|";
		}


	}
	else {
		cout << "|";
	}

	for (int i = message.length(); i < numbers; i++) {
		if (front) {
			message = " " + message;
		}
		else {
			message = message + " ";
		}
		front = !front;
	}
	cout << message.c_str();
	if (printBottom) {
		if (numbers > 33) {
			cout << "|" << endl;
			cout << "****************************************" << endl;
		}
		else if (numbers == 33) {
			cout << "|" << endl;
			cout << "***********************************" << endl;
		}
		else {
			cout << "|" << endl;
			cout << "*************************" << endl;
		}

	}
	else {
		cout << "|" << endl;
	}
}

void drawHangMan(int hangCounts = 0) {
	if (hangCounts >= 1) {
		printMessage("|", false, false, ORDINARY_SIZE);
	}
	else {
		printMessage("", false, false, ORDINARY_SIZE);
	}
	if (hangCounts >= 2) {
		printMessage("|", false, false, ORDINARY_SIZE);
	}
	else {
		printMessage("", false, false, ORDINARY_SIZE);
	}

	if (hangCounts >= 3) {
		printMessage("O", false, false, ORDINARY_SIZE);
	}
	else {
		printMessage("", false, false, ORDINARY_SIZE);
	}
	if (hangCounts >= 4) {
		printMessage("/|\\", false, false, ORDINARY_SIZE);
	}
	else {
		printMessage("", false, false, ORDINARY_SIZE);
	}

	if (hangCounts >= 5) {
		printMessage("|", false, false, ORDINARY_SIZE);
	}
	else {
		printMessage("", false, false, ORDINARY_SIZE);
	}
	if (hangCounts >= 6) {
		printMessage("/ \\", false, false, ORDINARY_SIZE);
	}
	else {
		printMessage("", false, false, ORDINARY_SIZE);
	}

}
void printLetters(string input, char from, char to) {

	string newString;

	for (int i = from; i <= to; i++) {
		if (input.find(i) == string::npos) {
			newString += i;
			newString += " ";
		}
		else {
			newString += " ";
		}
	}
	printMessage(newString, false, false, ORDINARY_SIZE);
}

void drawArray(char array[][3]) {
	for (int i = 0; i < 3; i++) {
		for (int j = 0; j < 3; j++) {
			cout << array[i][j] << " ";
		}
		cout << endl;
	}
	//cout << "Choose Position:" << endl << "> ";
}
void printAvailableLetters(string initial) {
	printMessage("Available letters:", true, true, ORDINARY_SIZE);
	printLetters(initial, 'A', 'M');
	printLetters(initial, 'N', 'Z');
}

bool printWordAndCheckWin(string word, string guessed) {
	bool won = true;
	string s;
	for (int i = 0; i < word.length(); i++) {
		if (guessed.find(word[i]) == string::npos) {
			won = false;
			s += "_ ";
		}
		else {
			s += word[i];
			s += " ";
		}
	}
	printMessage(s, false, true, ORDINARY_SIZE);
	return won;
}

string loadRandomWord(string path) {
	int lineCount = 0;
	string word;
	vector<string> v; // ������ �����

	ifstream reader(path);
	if (reader.is_open()) {

		while (std::getline(reader, word)) {
			v.push_back(word);
		}
		int randomLine = rand() % v.size();
		word = v.at(randomLine);
		reader.close();
	}

	return word;

}
int triesLeft(string word, string guessed) {
	int errors = 0;
	for (int i = 0; i < guessed.length(); i++) {
		if (word.find(guessed[i]) == string::npos) {
			errors++;
		}
	}
	return errors;
}
void depositCheck(int* deposit) {
	while (!cin.good() || *deposit <= 0) {
		cin.clear();
		cin.ignore(INT_MAX, '\n');
		printMessage("***Error***", true, false, ORDINARY_SIZE);
		printMessage("Invalid type of input", false, false, ORDINARY_SIZE);
		printMessage("Enter again:", false, true, ORDINARY_SIZE);
		cout << "> ";
		cin >> *deposit;
	}
}

bool validateString(string& name) {

	bool cond = false;

	for (int i = 0; i < name.length(); i++) {
		cond = false;
		if (!isalpha(name[i]) && !isspace(name[i])) {
			cond = false;
		}
		if (isspace(name[i])) {
			cond = true;
		}
	}
	if (cond) {
		return false;
	}
	else {
		return true;
	}
}