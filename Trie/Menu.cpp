#include "Header.h"
#include <conio.h>
#include <iostream>
#include <cstdlib>
#include <iomanip>

void displayMenu() {
    CompressedTrie trie;
    trie.createTrie("words.txt");

    int option = 1;
    bool quit = false;

    while (!quit) {
        system("cls");
        std::cout << "\n\n";

        // Center the logo
        int width = 80; // Adjust this value to center the logo as needed
        std::cout << "\t\t" << "\033[31m               ,,                  ,,                                                  \033[0m\n";
        std::cout << "\t\t" << "\033[32m`7MM\"\"\"Yb.     db           mm     db                                                  \033[0m\n";
        std::cout << "\t\t" << "\033[33m  MM    `Yb.                MM                                                         \033[0m\n";
        std::cout << "\t\t" << "\033[34m  MM     `Mb `7MM  ,p6\"bo mmMMmm `7MM  ,pW\"Wq.`7MMpMMMb.   ,6\"Yb.  `7Mb,od8 `7M'   `MF'\033[0m\n";
        std::cout << "\t\t" << "\033[35m  MM      MM   MM 6M'  OO   MM     MM 6W'   `Wb MM    MM  8)   MM    MM' \"'   VA   ,V  \033[0m\n";
        std::cout << "\t\t" << "\033[36m  MM     ,MP   MM 8M        MM     MM 8M     M8 MM    MM   ,pm9MM    MM        VA ,V   \033[0m\n";
        std::cout << "\t\t" << "\033[31m  MM    ,dP'   MM YM.    ,  MM     MM YA.   ,A9 MM    MM  8M   MM    MM         VVV    \033[0m\n";
        std::cout << "\t\t" << "\033[32m.JMMmmmdP'   .JMML.YMbmd'   `Mbmo.JMML.`Ybmd9'.JMML  JMML.`Moo9^Yo..JMML.       ,V     \033[0m\n";
        std::cout << "\t\t" << "\033[33m                                                                               ,V      \033[0m\n";
        std::cout << "\t\t" << "\033[34m                                                                            OOb\"       \033[0m\n";

        std::cout << "\n\n\n";

        std::cout << "\t\t\t\t\t ----------====****====----------\n\n";
        if (option == 1)
            std::cout << "\t\t\t\t\t\t \x1B[33m -*  SEARCH  *-\33[0m\n\n";
        else
            std::cout << "\t\t\t\t\t\t      SEARCH\n\n";
        if (option == 2)
            std::cout << "\t\t\t\t\t\t \x1B[33m -*  INSERT  *-\33[0m\n\n";
        else
            std::cout << "\t\t\t\t\t\t      INSERT\n\n";

        if (option == 3)
            std::cout << "\t\t\t\t\t\t \x1B[33m -*  REMOVE  *-\33[0m\n\n";
        else
            std::cout << "\t\t\t\t\t\t      REMOVE\n\n";
        std::cout << "\t\t\t\t\t ----------====****====----------\n";

        // Get option
        char key = _getch();

		if (key == 'w' && option > 1) {
			option--;
		}
		else if (key == 's' && option < 3) {
			option++;
		}
		else if (key == ' ') {
			switch (option) {
			case 1:
				search(trie);
				break;
			case 2:
				insert(trie);
				break;
			case 3:
				remove(trie);
				break;
			}
		}
	}

	system("cls");
}

void search(CompressedTrie trie) {
    system("cls");

    std::cout << "\n\n\n\n\n";
    std::cout << "\t\t\t\t\t   Choose algorithm\n";
    std::cout << "\t\t\t\t\t   [1] Compressed Trie\n";
    std::cout << "\t\t\t\t\t   [2] Brute Force\n";
    int algorithm = 0;
    while (true) {
        char c = _getch();
        if (c == '1' || c == '2') {
            algorithm = c - '0';
            break;
        }
    }

    system("cls");
    
    std::cout << "\n\n\n\n\n";
    std::cout << "\t\t\t\t   How many words do you want suggested? : ";
    int num = 0;
    std::cin >> num;

    system("cls");

    std::cout << "\n\n\n\n\n";
    std::cout << "\t\t\t\tSEARCH: ";

    std::string input = "";
    std::vector<std::string> suggestList;

    while (true) {
        char c = _getch();
        
        if (c == 27) {
            break;
        }
        else if (c == 8 && !input.empty()) {
            input.pop_back();
        }
        else if (c != 8 && c != 27)
            input += c;

        system("cls");

        std::cout << "\n\n\n\n\n";
        std::cout << "\t\t\t\tSEARCH: " << input << '\n';

        if (input.empty()) continue;

        if (algorithm == 1)
            suggestList = autocomplete(trie, input, num);
        else if (algorithm == 2)
            suggestList = autocomplete("words.txt", input, num);

        std::cout << "\n\t\t\t\tRESULT:\n";

        if (suggestList.empty()) {
            std::cout << "\t\t\t\tNOT FOUND!!\n";
        }
        else {
            for (auto i : suggestList) {
                std::cout << "\t\t\t\t" << i << '\n';
            }
        }

        suggestList.clear();
    }
}

void insert(CompressedTrie& trie) {
    system("cls");
    
    std::string word;
    std::cout << "\n\n\n\n\n";
    std::cout << "\t\t\t\tEnter the word you want to remove from the dictionary: ";
    getline(std::cin, word);
    trie.insert(word);
    std::cout << "\t\t\t\tAdded successfully!!. Press any key to continue.";
    char c = _getch();
}

void remove(CompressedTrie& trie) {
    system("cls");

    std::string word;
    std::cout << "\n\n\n\n\n";
    std::cout << "\t\t\t\tEnter the word you want to add to the dictionary: ";
    getline(std::cin, word);
    if (trie.remove(word))
        std::cout << "\t\t\t\tRemoved successfully!!. Press any key to continue.";
    else 
        std::cout << "\t\t\t\tWord not found!!. Press any key to continue.";
    char c = _getch();
}