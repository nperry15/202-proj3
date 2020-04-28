#include "PokemonList.h"
#include "Pokemon.h"

// Name: PokemonList
// Desc - Default constructor for the PokemonList (linked list)
// Preconditions - None
// Postconditions - Creates an empty PokemonList (linked list)
PokemonList::PokemonList() {
	m_head = nullptr;
	m_size = 0;
}

// Name: ~PokemonList
// Desc - Destructor for the PokemonList (linked list)
// Preconditions - None
// Postconditions - Clears out a PokemonList (linked list)
PokemonList::~PokemonList() {
    Pokemon *current = m_head;
    while (current != nullptr){
        //saves next list object, deletes current and recycles current to next object
        Pokemon *next = current->GetNext();
        delete current;
        current = next;
        m_size--;
    }
    //last object in the list
    m_head = NULL;
}

// Name: InsertEnd
// Desc - Inserts an already dynamically allocated Pokemon to end of linked list
// Preconditions - PokemonList already created
// Postconditions - The passed Pokemon appears at the end of the list
void PokemonList::InsertEnd(Pokemon* pokemonPtr) {
    //no objects in the list
    if (m_head == NULL) {
        m_head = pokemonPtr;
        m_size = 1;

        m_head->SetNext(pokemonPtr);
        pokemonPtr->SetNext(NULL);
    }
    else {
        m_size++;
        Pokemon *temp = m_head;
        //finds the end
        while (temp->GetNext() != NULL) {
            temp = temp->GetNext();
        }

        //sets the object and the end of the list values
        temp->SetNext(pokemonPtr);
        pokemonPtr->SetNext(NULL);
    }
}

// Name: Display
// Desc - Iterates over PokemonList and displays each Pokemon formatted as in output
// Preconditions - PokemonList already created and has Pokemon
// Postconditions - Displayed Pokemon information
void PokemonList::Display() {
    int indexCount;
    int nameCount;
    int typeCount;

    if (m_size == 0) { //Checks to see if the linked list is empty
        cout << "The linked list is empty" << endl; //Output if it is empty
    }
    else {
        Pokemon *temp = m_head; //Creates a new pointer to iterate over list
        while(temp != nullptr) {
            indexCount = temp->GetIndex();
            nameCount = temp->GetName().length();
            typeCount = temp->GetType().length();

            //displaying the outputs
            cout << "Index: ";
            if (indexCount > 99)
                cout << indexCount;
            else if (indexCount > 9)
                cout << " " << indexCount;
            else
                cout << "  " << indexCount;
            
            cout << "  Name: ";
            cout << temp->GetName();
            for (int i = 0; i < 12 - nameCount; i++) {
                cout << " ";
            }

            cout << "Type: ";
            cout << temp->GetType();
            for (int i = 0; i < 10 - typeCount; i++) {
                cout << " ";
            }

            cout << "Health: ";
            cout << temp->GetHealth();

            cout << endl;
            temp = temp->GetNext(); //Moves to m_next node
        }
    }
}

// Name: Transfer
// Desc - Searches for a Pokemon with a specific index. If found,
// creates a new Pokemon and copies it into the PokemonList passed in
// Preconditions - int is the index of the desired Pokemon and
// PokemonList is the destination
// Postconditions - Adds new Pokemon to the destination list and removes it from
// the source
void PokemonList::Transfer(int index, PokemonList* pokeList) {
    if (!Exist(index)) {
        cout << "Pokemon does not exist" << endl;
    }
    else {
        Pokemon* curr = m_head;
        Pokemon* prev = m_head;
        Pokemon* copy = m_head;
        //if index is the first value
        if (m_head->GetIndex() == index) {
            Remove(m_head->GetIndex());
            pokeList->InsertEnd(curr);
        }
        else {
            while (curr != nullptr) {
                //when the index is found
                if (curr->GetIndex() == index) {
                    copy = curr;
                    curr = curr->GetNext();
                    prev->SetNext(curr);
                }
                else {
                    prev = curr;
                    curr = curr->GetNext();
                }
            }
            //swaps the order of the found object
            pokeList->InsertEnd(copy);
            Remove(copy->GetIndex());
        }
    }
}

// Name: Remove (int)
// Desc - Searches for a Pokemon with a specific index. If found,
// removes it from the linked list
// Preconditions - int is the index of the desired Pokemon
// Postconditions - Removes Pokemon from linked list
void PokemonList::Remove(int index) {
    Pokemon* prev = m_head;
    Pokemon* curr = m_head;

    //no list objects
    if (m_size == 0) {
        cout << "Size is zero" << endl;
    }
    //if only one object in the list
    else if (m_size == 1) {
        //and object is the index needed
        if (m_head->GetIndex() == index) {
            m_head = nullptr;
            m_size--;
        }
        else {
            cout << "Value does not exist" << endl;
        }
    }
    else {
        //for any object on the second - end values
        Pokemon* temp = m_head;
        if (m_head->GetIndex() == index) {
            m_head = temp->GetNext();
        }
        else{
            //loops through the next two slots available
            while (curr != nullptr && curr->GetNext() != nullptr) {
                //goes to next value
                if (curr->GetIndex() != index) {
                    prev = curr;
                    curr = curr->GetNext();
                }
                //removes values from the list
                else {
                    temp = curr;
                    curr = curr->GetNext();
                    prev->SetNext(curr);
                    temp->SetNext(nullptr);
                }
            }
            //accounts for last value
            if (curr->GetNext() == nullptr && curr->GetIndex() == index) {
                prev->SetNext(nullptr);
            }
        }
        m_size--;
    }
    
}

// Name: GetSize
// Desc - Returns the size of the linked list
// Preconditions - PokemonList is populated with Pokemon
// Postconditions - Returns the size of the linked list
int PokemonList::GetSize() {
	return m_size;
}

// Name: GetHead
// Desc - Returns the Pokemon at the head of the linked list
// Preconditions - PokemonList is populated with Pokemon
// Postconditions - Returns the Pokemon at the head of the linked list
Pokemon* PokemonList::GetHead() {
    return m_head;
}

// Name: Attack(PokemonList)
// Desc - Passes an enemy pocket to the user's pocket to attack
// User always attacks the enemy Pokemon in this game
// Preconditions - This list is populated and enemy list is populated
// Postconditions - Does damage to the user Pokemon and enemy Pokemon
// and updates their health. Returns 2 if enemy Pokemon is less than 0
// Returns 2 if user's health is less than 0 else returns 0.
int PokemonList::Attack(PokemonList* enemyList) {
    //enemy and user starting points in the list
    Pokemon* enemyObj = enemyList->GetHead();
    Pokemon* userObj = m_head;

    //true if user and enemt are strong against enemy Pokemon
    bool userStrong = m_head->GetStrong() == enemyList->GetHead()->GetType();
    bool enemyStrong = enemyList->GetHead()->GetStrong() == m_head->GetType();

    //enemy and user health values
    int enemyHealth = enemyList->GetHead()->GetHealth();
    int userHealth = m_head->GetHealth();
    
    //enemy and user index values
    int enemyIndex = enemyList->GetHead()->GetIndex();
    int userIndex = m_head->GetIndex();

    //USER GOES FIRST
    //if user is strong take 5 damage or if not take only 2 damage
    if (userStrong) {
        cout << "CPU's pokemon took 5 damage" << endl;
        enemyList->GetHead()->SetHealth(enemyHealth - STRONG_DAMAGE);
    }
    else {
        cout << "CPU's pokemon took 2 damage" << endl;
        enemyList->GetHead()->SetHealth(enemyHealth - DAMAGE2ENEMY);
    }

    //update enemy health
    enemyHealth = enemyList->GetHead()->GetHealth();
    //if the enemy is dead then this runs
    if (enemyHealth < 1) {
        //if not tell the console, update the list and delete the object
        cout << "CPU's pokemon has been defeated" << endl;
        enemyList->Remove(enemyIndex);
        delete enemyObj;

        //if no enemy objects are left the user wins
        if (enemyList->GetSize() == 0) {
            return USER_WIN;
        }
        //else the values of health and strength are updated
        enemyStrong = enemyList->GetHead()->GetStrong() == m_head->GetType();
        enemyHealth = enemyList->GetHead()->GetHealth();
        //prints out new Pokemon
        cout << "CPU changed its pokemon to : ";
        cout << enemyList->GetHead()->GetName() << " (";
        cout << enemyList->GetHead()->GetType() << " " << enemyList->GetHead()->GetHealth() << " Health)" << endl;
        return 0;
    }
    //if the enemy is still alive it attacks
    else {
        //if enemy is strong against user deal 5 damage or else deal 3
        if (enemyStrong) {
            cout << "Your pokemon took 5 damage" << endl;
            m_head->SetHealth(userHealth - STRONG_DAMAGE);
        }
        else {
            cout << "Your pokemon took 3 damage" << endl;
            m_head->SetHealth(userHealth - DAMAGE2USER);
        }

        //update health and check to see if the user Pokemon is still alive
        userHealth = m_head->GetHealth();
        if (userHealth < 1) {
            //if not tell the console, update the list and delete the object
            cout << "Your pokemon has been defeated" << endl;
            Remove(userIndex);
            delete userObj;

            //if no objects are left then the cpu won
            if (m_size == 0) {
                return CPU_WIN;
            }
            //if the user has a choice to swap then do so
            if (m_size > 1) {
                SwapPokemon();
            }
            else {
                //tell the user they are about to loose
                cout << "THIS IS YOUR LAST POKEMON!!!" << endl;
            }
            //update values
            userStrong = m_head->GetStrong() == enemyList->GetHead()->GetType();
            userHealth = m_head->GetHealth();

            //console output the new Pokemon
            cout << "You changed your pokemon to : ";
            cout << m_head->GetName() << " (";
            cout << m_head->GetType() << " " << m_head->GetHealth() << " Health)" << endl;
            return 0;
        }
    }
    
    return 0;
}

// Name: SwapPokemon
// Desc - Allows user to move a Pokemon from any position into the first position
// Special cases: Empty list, Pokemon chosen at m_head or choice doesn't exist
// Preconditions - Linked list is populated
// Postconditions - User selects a Pokemon and it is moved into the first position
void PokemonList::SwapPokemon() {
    int index = 0;
    //makes sure a swap will be possible
    if (m_size == 0) {
        cout << "Empty List" << endl;
    }
    else {
        //if the index doesnt exist, repeates til possible index is given
        while (!Exist(index)) {
            cout << "------------------------------" << endl;
            cout << "Which Pokemon would you like to choose? (Enter the index#)" << endl;
            Display();
            cin >> index;
        }

        //loops tjrough til the index is found, index possibility already checked
        Pokemon* current = m_head;
        while (current->GetIndex() != index) {
            current = current->GetNext();
        }
        //if alreadt in the first slot nothing happens
        if (current == m_head) {
            cout << "Pokemon is in the first slot" << endl;
        }
        else {
            //swaps to firs slot and appended to the back
            Remove(index);
            current->SetNext(m_head);
            m_head = current;
            m_size++;
        }

    }
}

// Name: Exist(int)
// Desc - Iterates through linked list looking for a specific index of a Pokemon
// Preconditions - PokemonList is populated with Pokemon
// Postconditions - Returns true if found else false
bool PokemonList::Exist(int index) {
    //loops through the list 
    Pokemon* temp = m_head;
    while (temp != nullptr) {
        //if a specific index is found return it
        if (temp->GetIndex() == index) {
            return true;
        }
        temp = temp->GetNext();
    }
    //if the whole list was looped and the index wasnt found return false
	return false;
}