#include "logic_src.h"

int Logic::amount_verifier()
{
	std::cout << "Amount of characters in a word (3 to 16): ";
	word_size_config = 0;
	std::cin >> word_size_config;

	// Amount that does not belong to the limits.

	if (word_size_config < 3)
	{
		std::cout << "This amount of characters is too low, please set another." << std::endl;
		system("pause");
		system("cls");
		word_size_config = 0;
		amount_verifier();
	}
	if (word_size_config > 16)
	{
		std::cout << "Maybe the words can became a bit too large, please set other amount of characters." << std::endl;
		system("pause");
		system("cls");
		word_size_config = 0;
		amount_verifier();
	}

	// Small word warning
	if (word_size_config >= 3 && word_size_config < 5)
	{
		std::cout << "Note that small words sometimes can be as hard as long words to guess, they may look random..."
			"\n(Y to proceed | N to change): ";
		char select = _getch();
		if (select == 'Y' || select == 'y')
		{
			return word_size_config;
		}
		else if (select == 'N' || select == 'n')
		{
			system("cls");
			word_size_config = 0;
			amount_verifier();
		}
		else
		{
			std::cout << "\nI will accept this as a yes..." << std::endl;
			system("pause");
			return word_size_config;
		}
	}

	// Long word warning
	if (word_size_config >= 10)
	{
		std::cout << "Note that long words will make the game way harder and longer to finish..."
			"\n(Y to proceed | N to change): ";
		char select = _getch();
		if (select == 'Y' || select == 'y')
		{
			return word_size_config;
		}
		else if (select == 'N' || select == 'n')
		{
			system("cls");
			word_size_config = 0;
			amount_verifier();
		}
	}

	return word_size_config;
}

int Logic::difficulty()
{
	system("cls");
	std::cout << "Select the game difficulty. "
		"\n(1) > Easy   [60 lifes]"
		"\n(2) > Normal [30 lifes]"
		"\n(3) > Hard   [20 lifes]"
		"\n(4) > Lucky  [10 lifes]"
		"\n-> ";
	char diff = _getch();
	if (diff < '1' || diff > '4')
	{
		std::cout << "Please select a valid difficulty. " << std::endl;
		system("pause");
		system("cls");
		difficulty();
		return 0;
	}
	else
	{
		switch (diff)
		{
		case '1':
			std::cout << "Easy";
			diff = EASY;
			break;
		case '2':
			std::cout << "Normal";
			diff = NORMAL;
			break;
		case '3':
			std::cout << "Hard";
			diff = HARD;
			break;
		case '4':
			std::cout << "Lucky";
			diff = LUCKY;
			break;
		default:
			break;
		}
		return diff;
	}
}

bool Logic::vec_contain_word(const std::vector<std::string>& vec, const std::string & word)
{
	for (const auto& w : vec)
	{
		if (w == word)
		{
			return true;
		}
	}
	return false;
}

std::vector<int> Logic::filler(const std::string & word)
{
	std::vector<int> fill(26, 0);
	for (char c : word)
	{
		/*
		Get a character between A to Z and put a index in it.

		Works like this:
		If letter A is found = c becames the inputted 'a' and get in buckets['a' - 'a'] = 0
		If letter B is found = c becames the inputted 'b' and get in buckets['b' - 'a'] = 1
		If letter C is found = c becames the inputted 'c' and get in buckets['c' - 'a'] = 2
		and so on between every value of the vector.
		*/

		fill[c - 'a']++;
	}
	return fill;
}

int Logic::score_match(const std::string & word1, const std::string & word2, int w_size)
{
	const auto buckets1 = filler(word1);
	const auto buckets2 = filler(word2);

	std::string up_guess = word1;
	std::transform(up_guess.begin(), up_guess.end(), up_guess.begin(), ::toupper);

	int score = 0;
	for (int i = 0; i < 26; i++)
	{
		score += std::min(buckets1[i], buckets2[i]);
	}

	std::cout << "-----------------------------------------------" << std::endl;
	std::cout << up_guess << std::endl;
	std::cout << "-----------------------------------------------" << std::endl;
	std::cout << "A total of " << score << " correct letter(s)       [ + " << score << " ]\n";

	for (int i = 0; i < w_size; i++)
	{
		if (word1[i] == word2[i])
		{
			std::cout << "Letter \"" << word1[i] << "\" is at the right position! [ + 1 ]" << std::endl;
			score++;
		}
	}

	std::cout << "Total score: " << score << " out of " << w_size * 2 << std::endl;
	std::cout << "-----------------------------------------------" << std::endl;

	// Push back the current word and score to a vector, and print the history everytime.
	return score;
}

void Logic::Generate(int w_size, int game_diff)
{

	// This is not working properly when the game is restarted :(
	std::mt19937 rng(std::random_device{}());
	std::uniform_int_distribution<size_t> dist(0, words.size() - 1);
	std::string target = words[dist(rng)];

	if (target == "quit")
	{
		system("cls");
		std::cout << "A error occurred loading the game, please restart the game." << std::endl;
		system("pause");
		exit(0);
	}

	int life_amount;
	switch (game_diff)
	{
	case EASY:
		life_amount = 50;
		break;
	case NORMAL:
		life_amount = 30;
		break;
	case HARD:
		life_amount = 20;
		break;
	case LUCKY:
		life_amount = 10;
		break;
	default:
		break;
	}

	int guess_total = 0;

	// Game Loop
	while (true)
	{
		int calc_w_size = w_size * 2;
		std::cout << "Guess a " << w_size << " letters word [ " << guess_total + 1 << " / " << life_amount << " ]: ";
		std::string guess;
		std::getline(std::cin, guess);
		system("cls");

		if (guess_total == life_amount - 2)
		{
			system("color c");
		}

		if (guess_total != life_amount - 1)
		{
			for (auto& c : guess)
			{
				c = std::tolower(c);
			}

			if (guess == "quit")
			{
				system("cls");
				std::string leave = "Leaving";
				std::string dots = " . . .";
				for (auto i = 0; i < leave.length(); i++)
				{
					std::cout << leave[i];
					Sleep(50);
				}

				for (auto i = 0; i < dots.length(); i++)
				{
					std::cout << dots[i];
					Sleep(150);
				}

				exit(0);
			}
			else
			{
				if (guess.size() != w_size)
				{
					if (guess == "")
					{
						// Empty guess used for cleaning the screen.
						continue;
					}
					else
					{
						std::cout << "-----------------------------------------------" << std::endl;
						std::cout << "\"" << guess << "\" does not have " << w_size << " letters.\n";
						std::cout << "ans: " << target << std::endl; // debug only 
						std::cout << "-----------------------------------------------\n" << std::endl;
						guess_total++;
						continue;
					}
				}

				if (!vec_contain_word(words, guess))
				{
					std::cout << "\n-----------------------------------------------" << std::endl;
					std::cout << "\"" << guess << "\" does not exists in the dictionary.\n";
					std::cout << "-----------------------------------------------\n" << std::endl;
					guess_total++;
					continue;
				}

				const int score = score_match(guess, target, w_size);
				if (score == calc_w_size)
				{
					system("cls");
					system("color a");
					std::cout << "You did it! \"" << guess << "\" is the correct word!\n"
						"\nPress enter to leave the game." << std::endl;
					std::cin.get();
					system("cls");
					exit(0);
				}
				else
				{
					if (score < calc_w_size && score > 1)
					{
						std::cout << "You are close! Try again.\n";
						std::cout << "-----------------------------------------------\n" << std::endl;
						guess_total++;
					}
					else if (score < 1)
					{
						std::cout << "There is no correct letters in \"" << guess << "\", try again." << std::endl;
						std::cout << "-----------------------------------------------" << std::endl;
						guess_total++;
					}
					continue;
				}
			}
		}
		else
		{
			system("cls");
			system("color f");
			std::cout << "Game Over!" << std::endl;
			std::cout << "The correct word was \"" << target << "\", good luck next time :(" << std::endl;

			std::cout << "\n\nPress enter to leave the game." << std::endl;
			std::cin.get();
			system("cls");
			exit(0);
		}

		std::cin.clear();
		std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
		std::cin.get();
	}
}

void Logic::init()
{
	system("color f");
	std::ifstream words_data("common_words_data.txt");

	int word_selected_size = amount_verifier();
	int curr_dif = difficulty();

	std::cout << "\nYou can leave the game writing \"quit\" at any moment." << std::endl;
	system("pause");

	// Verify if current line matches with word size configurations.
	for (std::string line; std::getline(words_data, line);)
	{
		// Clear the line if the current line isn't equal to the amount of characters selected.
		if (line.size() != word_selected_size)
		{
			line.clear();
		}

		// Saves the space of the cleared line above.
		if (line.empty())
		{
			continue;
		}
		words.push_back(line);
	}

	system("cls");

	Generate(word_selected_size, curr_dif);

	// Tests if the words are being loaded properly

	// for (const auto& w : words)
	// {
	// 	std::cout << w << std::endl;
	// }

	std::cout << std::endl;
}
