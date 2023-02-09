#include "Vector.h"
#include <iostream>
#include <string>
#include <vector>
#include <unordered_map>

typedef void (*handler) ();

void printMenu();

void createVector();
void handleAdd();
void handleRemove();
void copyVector();
void clearVector();
void printVector();
void handleQuit();


// You can add more choices here
std::vector<std::string> choices = {
    "Create a vector",
    "Add to a vector",
    "Remove from a vector",
    "Create a copy of a vector",
    "Clear a vector",
    "Print a vector",
    "Quit"
};

// Add choice handlers here
std::vector<handler> handlers= {
    createVector,
    handleAdd,
    handleRemove,
    copyVector,
    clearVector,
    printVector,
    handleQuit
};

std::unordered_map<std::string, Vector<int>> vectors;

int main() {
    while (true) {
        printMenu();
        size_t choice = -1;

        std::cin >> choice;

        if (std::cin.fail() || choice - 1 >= choices.size()) {
            std::cout << "That is not a valid choice" << std::endl << std::endl;
            std::cin.clear();
            std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
            continue;
        }

        std::cout << std::endl << std::endl;
        handlers[choice - 1]();
        std::cout << std::endl;
    }
}

void printMenu() {
    std::cout << "What would you like to do?" << std::endl;
    std::cout << "----------------------------------" << std::endl;
    
    for (size_t i = 0; i < choices.size(); ++i) {
        std::cout << (i+1) << ". " << choices[i] << std::endl;
    }

    std::cout << std::endl << "Please enter your numbered choice: ";
}

void createVector() {
    while (true) {
        std::cout << "Please enter a unique name for your vector (no spaces)" << std::endl;
        std::cout << "If you would like to return to the menu, enter \"back\": ";
        std::string name;
        std::cin >> name;
        std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');

        if (name == "back") {
            return;
        }

        if (vectors.count(name)) {
            std::cout << "That vector name already exists!" << std::endl << std::endl;
            continue;
        }

        vectors[name];
        std::cout << "The vector \"" << name << "\" has been created" << std::endl << std::endl;
        return;
    }
}

void handleAdd() {
    bool quit = false;

    while (!quit) {
        std::cout << "Please enter the name of the vector to add to" << std::endl;
        std::cout << "If you would like to return to the menu, enter \"back\": ";
        std::string name;
        std::cin >> name;
        std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');

        if (name == "back") {
            return;
        }

        if (!vectors.count(name)) {
            std::cout << "That vector does not exist" << std::endl << std::endl;
            continue;
        }

        std::cout << std::endl;
        std::cout << "How would you like to add to a vector" << std::endl;
        std::cout << "1. An element to the end" << std::endl;
        std::cout << "2. An element at an index" << std::endl;
        std::cout << "3. Multiple elements at an index" << std::endl;
        std::cout << "4. Return to menu" << std::endl;
        std::cout << std::endl << "Please enter your numbered choice: ";

        int choice = -1;
        std::cin >> choice;

        if (std::cin.fail() || choice < 1 || choice > 4) {
            std::cout << "That is not a valid choice" << std::endl << std::endl;
            std::cin.clear();
            std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
            continue;
        }

        int num;
        size_t idx;

        switch (choice) {
            case 1:
                std::cout << "What int would you like to insert? ";
                std::cin >> num;
                std::cout << std::endl;

                if (std::cin.fail()) {
                    std::cout << "That is not an int" << std::endl << std::endl;
                    std::cin.clear();
                    std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
                    break;
                }

                vectors[name].push_back(num);
                quit = true;
                std::cout << "Added " << num << " to the end of vector \"" << name << "\"" << std::endl << std::endl;
                break;

            case 2:
                std::cout << "What index would you like to insert at? ";
                std::cin >> idx;
                std::cout << std::endl;

                if (std::cin.fail() || idx > vectors[name].size()) {
                    std::cout << "That is not a valid index" << std::endl << std::endl;
                    std::cin.clear();
                    std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
                    break;
                }

                std::cout << "What int would you like to insert? ";
                std::cin >> num;
                std::cout << std::endl;

                if (std::cin.fail()) {
                    std::cout << "That is not an int" << std::endl << std::endl;
                    std::cin.clear();
                    std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
                    break;
                }

                {
                    Vector<int>& v = vectors[name];
                    v.insert(v.begin() + idx, num);
                }

                quit = true;
                std::cout << "Added " << num << " to the vector \"" << name << "\" at index " << idx << std::endl << std::endl;
                break;

            case 3:
                std::cout << "What index would you like to insert at? ";
                std::cin >> idx;
                std::cout << std::endl;

                if (std::cin.fail() || idx > vectors[name].size()) {
                    std::cout << "That is not a valid index" << std::endl << std::endl;
                    std::cin.clear();
                    std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
                    break;
                }

                std::cout << "What int would you like to insert? ";
                std::cin >> num;
                std::cout << std::endl;

                if (std::cin.fail()) {
                    std::cout << "That is not an int" << std::endl << std::endl;
                    std::cin.clear();
                    std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
                    break;
                }

                std::cout << "How many times do you want to insert it? ";
                
                size_t count;
                std::cin >> count;
                std::cout << std::endl;

                if (std::cin.fail()) {
                    std::cout << "That is not a valid count" << std::endl << std::endl;
                    std::cin.clear();
                    std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
                    break;
                }

                {
                    Vector<int>& v = vectors[name];
                    v.insert(v.begin() + idx, count, num);
                }

                quit = true;
                std::cout << "Added " << count << " " << num << "'s to the vector \"" << name << "\" at index " << idx << std::endl << std::endl;
                break;

            case 4:
                std::cout << std::endl;
                return;
        }
    }
}

void handleRemove() {
    bool quit = false;

    while (!quit) {
        std::cout << "Please enter the name of the vector to remove from" << std::endl;
        std::cout << "If you would like to return to the menu, enter \"back\": ";
        std::string name;
        std::cin >> name;
        std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');

        if (name == "back") {
            return;
        }

        if (!vectors.count(name)) {
            std::cout << "That vector does not exist" << std::endl << std::endl;
            continue;
        }

        std::cout << std::endl;
        std::cout << "How would you like to remove from the vector" << std::endl;
        std::cout << "1. An element from the end" << std::endl;
        std::cout << "2. An element from an index" << std::endl;
        std::cout << "3. Multiple elements between 2 indices" << std::endl;
        std::cout << "4. Return to menu" << std::endl;
        std::cout << std::endl << "Please enter your numbered choice: ";

        int choice = -1;
        std::cin >> choice;

        if (std::cin.fail() || choice < 1 || choice > 4) {
            std::cout << "That is not a valid choice" << std::endl << std::endl;
            std::cin.clear();
            std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
            continue;
        }

        size_t idx, idx_end;
        int temp;

        switch (choice) {
            case 1:
                temp = vectors[name].back();
                vectors[name].pop_back();
                quit = true;
                std::cout << "Removed " << temp << " from the end of vector \"" << name << "\"" << std::endl << std::endl;
                break;

            case 2:
                std::cout << "What index would you like to remove? ";
                std::cin >> idx;
                std::cout << std::endl;

                if (std::cin.fail() || idx >= vectors[name].size()) {
                    std::cout << "That is not a valid index" << std::endl << std::endl;
                    std::cin.clear();
                    std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
                    break;
                }

                temp = vectors[name][idx];

                {
                    Vector<int>& v = vectors[name];
                    v.erase(v.begin() + idx);
                }

                quit = true;
                std::cout << "Removed " << temp << " from the vector \"" << name << "\" at index " << idx << std::endl << std::endl;
                break;

            case 3:
                std::cout << "What is the starting index (first to be erased)? ";
                std::cin >> idx;
                std::cout << std::endl;

                if (std::cin.fail() || idx >= vectors[name].size()) {
                    std::cout << "That is not a valid index" << std::endl << std::endl;
                    std::cin.clear();
                    std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
                    break;
                }

                std::cout << "What is the last index (erase up to, but not including)? ";
                std::cin >> idx_end;
                std::cout << std::endl;

                if (std::cin.fail() || idx_end > vectors[name].size() || idx_end <= idx) {
                    std::cout << "That is not a valid ending index" << std::endl << std::endl;
                    std::cin.clear();
                    std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
                    break;
                }

                {
                    Vector<int>& v = vectors[name];
                    v.erase(v.begin() + idx, v.begin() + idx_end);
                }

                quit = true;
                std::cout << "Removed elements from index " << idx << " to index " << idx_end << " from the vector \"" << name << "\"" << std::endl << std::endl;
                break;

            case 4:
                std::cout << std::endl;
                return;
        }
    }
}

void copyVector() {
    bool quit = false;

    while (!quit) {
        std::cout << "Please enter the name of the vector you would like to make a copy of" << std::endl;
        std::cout << "If you would like to return to the menu, enter \"back\": ";
        std::string name;
        std::cin >> name;
        std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');

        if (name == "back") {
            return;
        }

        if (!vectors.count(name)) {
            std::cout << "That vector does not exist" << std::endl << std::endl;
            continue;
        }

        std::cout << std::endl;
        std::cout << "How would you like to copy the vector" << std::endl;
        std::cout << "1. Copy contents to another vector" << std::endl;
        std::cout << "2. Move contents to another vector" << std::endl;
        std::cout << "3. Return to menu" << std::endl;
        std::cout << std::endl << "Please enter your numbered choice: ";

        int choice = -1;
        std::cin >> choice;

        if (std::cin.fail() || choice < 1 || choice > 3) {
            std::cout << "That is not a valid choice" << std::endl << std::endl;
            std::cin.clear();
            std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
            continue;
        }

        std::string name2;

        switch (choice) {
            case 1:
                std::cout << "Please enter the name of the vector you would like to copy contents into" << std::endl;
                std::cout << "If the vector doesn't already exist, a new one with the name will be made: ";
                std::cin >> name2;
                std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');

                if (!vectors.count(name2)) {
                    vectors.insert({name2, Vector<int>(vectors[name])});
                    std::cout << "Created vector \"" << name2 << "\" as a copy of vector \"" << name << "\"" << std::endl << std::endl;
                    break;
                }

                vectors[name2] = vectors[name];
                std::cout << "Made vector \"" << name2 << "\" into a copy of vector \"" << name << "\"" << std::endl << std::endl;
                quit = true;
                break;

            case 2:
                std::cout << "Please enter the name of the vector you would like to move contents into" << std::endl;
                std::cout << "If the vector doesn't already exist, a new one with the name will be made: ";
                std::cin >> name2;
                std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');

                if (!vectors.count(name2)) {
                    vectors.insert({name2, Vector<int>(std::move(vectors[name]))});
                    std::cout << "Moved the contents of vector \"" << name << "\" into the new vector \"" << name2 << "\"" << std::endl << std::endl;
                    break;
                }

                vectors[name2] = std::move(vectors[name]);
                std::cout << "Moved the contents of vector \"" << name << "\" into vector \"" << name2 << "\"" << std::endl << std::endl;
                quit = true;
                break;

            case 3:
                std::cout << std::endl;
                return;
        }
    }
}

void clearVector() {
    while (true) {
        std::cout << "Please enter the name of the vector you would like to clear" << std::endl;
        std::cout << "If you would like to return to the menu, enter \"back\": ";
        std::string name;
        std::cin >> name;
        std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');

        if (name == "back") {
            return;
        }

        if (!vectors.count(name)) {
            std::cout << "That vector does not exist" << std::endl << std::endl;
            continue;
        }

        vectors[name].clear();
        std::cout << "Cleared vector \"" << name << "\"" << std::endl << std::endl;
        return;
    }
}

void printVector() {
    while (true) {
        std::cout << "Please enter the name of the vector you would like to print the contents of" << std::endl;
        std::cout << "If you would like to return to the menu, enter \"back\": ";
        std::string name;
        std::cin >> name;
        std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');

        if (name == "back") {
            return;
        }

        if (!vectors.count(name)) {
            std::cout << "That vector does not exist" << std::endl << std::endl;
            continue;
        }

        Vector<int>& v = vectors[name];

        std::cout << "The contents of vector \"" << name << "\":" << std::endl;
        std::cout << "{";

        for (Vector<int>::iterator itr = v.begin(); itr != v.end(); ++itr) {
            std::cout << *itr;

            if (itr != --v.end()) {
                std::cout << ", ";
            }
        }

        std::cout << "}" << std::endl << std::endl;
        return;
    }
}

void handleQuit() {
    std::cout << "Goodbye!" << std::endl;
    exit(0);
}