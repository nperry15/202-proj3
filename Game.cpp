#include "Game.h"
#include "Pokemon.h"
#include <fstream>

// Name: Game Constructor
// Desc - Creates a new game and sets m_filename based on string passed
// Additionally, makes m_list and userPoken = new PokemonLists
// Preconditions - Input file passed and populated with Pokemon
// Postconditions - m_filename, m_list, and userPocket created
Game::Game(string fileName) {
	m_filename = fileName;
	m_list = new PokemonList;
	m_userPocket = new PokemonList;
	m_enemyPocket = new PokemonList;
}

// Name: Game Destructor
// Desc - Calls destructor for all linked lists in game
// Preconditions - Linked lists populated
// Postconditions - m_list, userPocket, and enemyPocket are all destructed
Game::~Game() {
	m_list->~PokemonList();
	delete m_list;
	m_userPocket->~PokemonList();
	delete m_userPocket;
	m_enemyPocket->~PokemonList();
	delete m_enemyPocket;
}

// Name: LoadFile
// Desc - Opens file and reads in each Pokemon. Each Pokemon dynamically allocated
// and put into m_list
// Preconditions - Input file passed and populated with Pokemon
// Postconditions - m_list populated with Pokemon
void Game::LoadFile() {
	cout << "loading file..." << endl;
	ifstream file;
	file.open(m_filename);

	//create temp values
	int index;
	string name;
	string type;
	string strength;

	//loops through the whole list
	for (int i = 0; i < NUM_POKEMON; i++) {
		file >> index;
		file.ignore(256, ',');

		getline(file, name, ',');
		getline(file, type, ',');
		getline(file, strength);
		//makes new Pokemon pointer to an object and appends to the list
		Pokemon *pokemonPtr = new Pokemon(index, name, type, strength);
		m_list->InsertEnd(pokemonPtr);
	}

	//closes up the file :)
	file.close();
}

// Name: ChooseTeam
// Desc - Allows user to choose team and populates m_userPocket
// Preconditions - m_list populated with Pokemon
// Postconditions - m_userPocket populated with Pokemon to battle
void Game::ChooseTeam() {
	cout << "------------------------------" << endl;
	int numLeft = NUM_TEAM;
	int choice;
	//runs til all 5 objects have been choosen
	while (numLeft > 0) {
		cout << "Here is a list of Pokemon you can choose from :" << endl;
		cout << "------------------------------" << endl;
		m_list->Display();

		cout << "Pick a Pokemon by enter the index(" << numLeft << " left) :" << endl;
		cin >> choice;
		//if the user picks and object that does not exist
		while (!m_list->Exist(choice)) {
			cout << "Pick a Pokemon by enter the index(" << numLeft << " left) :" << endl;
			cin >> choice;
		}
		//movees choice to a pocket
		m_list->Transfer(choice, m_userPocket);

		//updates number of objects needed to be selected
		numLeft = NUM_TEAM - m_userPocket->GetSize();
	}

	//repeates the selected objects
	cout << "------------------------------" << endl;
	cout << "Print player pocket" << endl;
	m_userPocket->Display();
}

// Name: Menu
// Desc - Displays menu and returns choice
// Preconditions - m_list, m_userPocket, and m_enemyPocket all populated
// Postconditions - Returns choice
int Game::Menu() {
	int choice;
	cout << "Menu:\n1. Attack\n2. Swap\n3. Forfeit" << endl;
	cin >> choice;
	//runs if a 1 ,2, or 3 was not choosen til one is choosen
	while (choice > 3 || choice < 1) {
		cout << "Please enter a valid choice" << endl;
		cin.ignore();
		cin.clear();
		cin >> choice;
	}
	return choice;
	cout << "------------------------------" << endl;
}

// Name: Battle
// Desc - Manages the battle between m_userPocket and m_enemyPocket.
// Displays both the m_userPocket and m_enemyPocket
// For each round, makes sure each team has Pokemon. Makes Pokemon in head fight.
// Indicates if the round yields user win, enemy win, and if a pokemon was switched
// Fights continue until there are no remaining Pokemon in one of the lists
// Preconditions - m_userPocket and m_enemyPocket must all be > 0 and populated
// Postconditions - Returns result of battle back to Start (1=user win, 2=cpu win)
int Game::Battle() {
	int winner = 0;
	int roundCounter = 0;

	//runs until there is a winner
	while (winner == 0) {
		cout << "------------------------------" << endl;
		
		roundCounter++;
		cout << "Round " << roundCounter << ":" << endl;
		//displayes user and cpu selected Pokemon to batt;es
		cout << "CPU's Pokemon: " << m_enemyPocket->GetHead()->GetName() << " (";
		cout << m_enemyPocket->GetHead()->GetType() << " " << m_enemyPocket->GetHead()->GetHealth() << " Health)" << endl;
		cout << "Your Pokemon: " << m_userPocket->GetHead()->GetName() << " (";
		cout << m_userPocket->GetHead()->GetType() << " " << m_userPocket->GetHead()->GetHealth() << " Health)" << endl;

		cout << "------------------------------" << endl;
		
		//calls menu and retreives values; executes the choosen values commands
		switch (Menu()) {
		case ATTACK_VALUE:
			cout << "------------------------------" << endl;
			winner = m_userPocket->Attack(m_enemyPocket);
			break;
		case SWAP_VALUE:
			m_userPocket->SwapPokemon();
			break;
		case FORFEIT_VALUE:
			return 2;
			break;
		}
	}
	//returns once a winner has been choosen
	return winner;
}

// Name: Start
// Desc - Loads input file, allows user to choose their team, randomly populates the
// m_enemyPocket with remaining Pokemon and returns the result of the battle
// Preconditions - m_list, m_userPocket, and m_enemyPocket must all be PokemonLists
// Postconditions - Returns result of battle back to main
int Game::Start(){
	int numLeft = NUM_TEAM;
	int randChoice = -1;

	//prompts the user
	cout << "Welcome to the game" << endl; 

	//loads the main list and displays possible objects to be choosen from
	LoadFile();
	m_list->Display();

	//makes the user select their lineup
	ChooseTeam();

	//Load CPU Pokemon randomly
	while (numLeft > 0) {
		while (!m_list->Exist(randChoice)) {
			randChoice = (rand() % NUM_POKEMON) + 1;
		}
		m_list->Transfer(randChoice, m_enemyPocket);
		numLeft = NUM_TEAM - m_enemyPocket->GetSize();
	}
	//displays the cpu randonly selected team
	cout << "------------------------------" << endl;
	cout << "Print cpu pocket" << endl;
	m_enemyPocket->Display();

	//returns the winner decided from calling the battle function
	return Battle();
}