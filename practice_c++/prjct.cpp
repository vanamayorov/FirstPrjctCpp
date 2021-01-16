#include <iostream> // подключаем библиотеку iostream(ввод и вывод информации)
#include <ctime>
#include <vector>
#include <fstream>
#include <string>
#include <cstdlib>
using namespace std; // подключаем обьекты из пространства имен std
/*
	Хранилищем данных в программе выступает оперативная память.
	Для определенного типа данных нужно выбрать подходящий тип данных.
	Тип данных - понятие, определяющее максимальный размер в байтах и тип информации, которая
	будет использоваться программой.
	Разделяют 3 основных типа данных:
	1) Числовые
		1. Целые(int - 4 bytes, short - 2 bytes, long - 4 bytes)
		2. Вещественные(float - 4 bytes, double - 8 bytes)
	2) Символьные
		1. char - 1 byte
	3) Логические
		1. bool - 1 byte

	Const - слово, которая предназначена для хранения постоянных данных.
	Именуем большими буквами.

	cin - для ввода данных

	break - слово выхода из цикла for
	continue - слово, которое начинает новую итерацию цикла

	sizeof возвращает размер сколько занимает тот или иной обьект в байтах
	sizeof(arr) / sizeof(int) - узнаем сколько элементов в массиве

	ctime библиотека для подключения функции time

	srand(time(NULL)) - создает точку входа для последующей функции rand
	rand() - generates random numbers and add % to have numbers in a particular range

	в двухмерном массиве колонки обязательно записываем при инициализации

	массивы в функцию всегда передаются по указателю

	прототипы функций сообщают компилятору о существовании функции

	область видимости(scope)
	фигурные скобки образуют некий блок(область видимости) и переменные, внутри него,
	не могут быть видны за ним.
	глобальные переменные создаются вне всякого scopa.
	локальные создаются внутри scopa.

	Параметры по умолчанию записываются в конце аргументов

	inline - ключ. слово для встраивания функции. inline используют для маленьких функций

	Перегрузка функций = реализация функции

	для того, чтобы не писать разные перегрузки функций, можно использовать шаблонные функции
	template <typename/class T> обьявление шаблона
	T/void/int etc (тип возращаемого значения) Foo(T variable, T var...){
	func realisation;
	}
	нужно помнить, что в разных случаях не всегда результат будет прогнозируемым,
	если мы используем разные типы переменных

	стек - область памяти, где выполняется программа. в стек помещают задачи, откуда наша программа их достает.
	реализован по принципу: первый зашел, последний вышел и наоборот.

	для статических переменных программа заранее знает сколько они будут занимать памяти.
	на этапе компиляции компилятор заранее выделяет статическую память для выполнения программы.
	динамическую память(heap) должен предусмотреть программист при разработке программы, когда не знает,
	сколько памяти потребуется.
	для того, чтобы использовать динамическую память нужны указатели.
	динамическая память используется тогда, когда разработчик не знает, сколько будет весить переменные.

	при передаче переменных по ссылке в функцию можно получить несколько значений

	int &name = a - обьявление ссылки на переменную а
	!У ссылок нету оператора разыменования!
	!ссылочный тип не нуждается в операторе разыменования!

	cin.good() возвращает true or false, если корректно ввели/ввели данные
	string::npos - несуществующая позиция

	vector - динамический массив, который самостоятельно определяет свой размер

*/
#define ORDINARY_SIZE 33
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
int main() {
	system("color F2");
	setlocale(LC_ALL, "Rus"); // подкл функцию для распознавания русских символов
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
	while (name.length() > MAX_SIZE_OF_NAME || name.empty() || !stringValidated) { // 
		printMessage("Please enter your name", true, false, ORDINARY_SIZE+5);
		printMessage("name cannot be larger than 50 symbols", false, true, ORDINARY_SIZE+5);
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
		cout << "List of games: \n 1 - Guess a number\n 2 - Hangman game\n 3 - Exit" << endl << "> ";
		while (!(cin >> game) ||  (game != 1 && game != 2 && game != 3)) {
			cin.clear();
			cin.ignore(INT_MAX, '\n');
			cout << "Invalid type of number!" << endl;
			cout << "List of games: \n 1 - Guess a number\n 2 - Hangman game\n 3 - Exit" << endl << "> ";
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
		*exitCondition = true;
		break;
	default:
		cout << "Invalid type of number!" << endl;
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
	vector<string> v; // массив строк

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