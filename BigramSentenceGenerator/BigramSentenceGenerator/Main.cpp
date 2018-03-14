#include "stdafx.h"
#include <string>
#include <fstream>
#include <sstream>
#include <ctime>
#include <iostream>
#include <vector>
#include <map>

using namespace std;

// Mad Lib Code ( Is Mad Lib copywritted? Should I change this? )

int sizeFileByLines(ifstream &FILE)
{
	int z = 0;

	if (FILE.fail())
		cout << "\nERROR: File could not open.\n";

	string s;
	while (getline(FILE, s)) {
		z++;
	}

	FILE.clear();
	FILE.seekg(0, ios::beg);

	return z;
}

string inputToOutput(int cnt, string in, ifstream &FILE)
{
	srand(time(NULL));
	int r = rand() % cnt;

	int ctr = 0;
	string line;
	while (getline(FILE, line))
	{
		if (ctr == r)
		{
			return line;
			break;
		}

		ctr++;
	}

	FILE.clear();
	FILE.seekg(0, ios::beg);
}

void madLib()
{
	char cont;
	do
	{
		vector<string> input;
		vector<string> output;

		int x = 0;

		cout << "[-------------Input-------------]\n\n\tadj - adjectives\n\tadv - adverbs\n\tar - articles\n\tn - nouns\n\tv - verbs\n\tp - pronouns\n\tx - end sentence\n\n       Ex: p n v adv x\n[--------------------------------]\n";


		// Begin input step
		cout << "\nEnter Sentence: ";
		string inp;
		cin >> inp;
		do
		{
			input.push_back(inp);
			x++;

			cin >> inp;
		} while (inp.compare("x") != 0);

		// Open all the files
		ifstream adjFile("adjectives.txt");
		ifstream advFile("adverbs.txt");
		ifstream artFile("articles.txt");
		ifstream nounsFile("nouns.txt");
		ifstream verbsFile("verbs.txt");
		ifstream pronounsFile("pronouns.txt");

		// Initialize sizes of files
		int adjSize = 0, advSize = 0, artSize = 0, nounSize = 0, verbSize = 0, proSize = 0, c = 0;

		// Check if files openned and size files by number of lines
		adjSize = sizeFileByLines(adjFile);
		advSize = sizeFileByLines(advFile);
		artSize = sizeFileByLines(artFile);
		nounSize = sizeFileByLines(nounsFile);
		verbSize = sizeFileByLines(verbsFile);
		proSize = sizeFileByLines(pronounsFile);

		for (c = 0; c < x; c++)
		{
			string temp = input.at(c);
			if (temp.compare("adj") == 0)
			{
				output.push_back(inputToOutput(adjSize, temp, adjFile));
			}
			else if (temp.compare("adv") == 0)
			{
				output.push_back(inputToOutput(advSize, temp, advFile));
			}
			else if (temp.compare("ar") == 0)
			{
				output.push_back(inputToOutput(artSize, temp, artFile));
			}
			else if (temp.compare("n") == 0)
			{
				output.push_back(inputToOutput(nounSize, temp, nounsFile));
			}
			else if (temp.compare("v") == 0)
			{
				output.push_back(inputToOutput(verbSize, temp, verbsFile));
			}
			else if (temp.compare("p") == 0)
			{
				output.push_back(inputToOutput(proSize, temp, pronounsFile));
			}
			else
			{
				output.push_back(temp);
			}
		}

		adjFile.close();
		advFile.close();
		artFile.close();
		nounsFile.close();
		verbsFile.close();

		cout << "\n\n";
		for (c = 0; c < x; c++)
		{

			if (c + 1 < x && (output.at(c + 1).at(0) == '-'))
			{
				cout << output.at(c);
				output.at(c + 1).erase(0, 1);
			}
			else
				cout << output.at(c) << " ";


		}

		cout << "\n\n\nDo you want to continue(y/n)? \n";
		cin >> cont;

		cout << "\n~Flushing Stream~\n" << flush;
		system("CLS");
	} while (cont == 'y');
}

// Bigram Code

std::map<std::pair<std::string, std::string>, int> bigrams;
int avgSentenceLength = 0;
void bigramSentenceCreator()
{
	char cont;
	do
	{
		// Bigram Creation Code 

		int sentenceCount = 0;

		char fileScanCont;

		do
		{
			int cnt;
			cout << "\nEnter the name of the text file: ";
			string fileName;
			cin >> fileName;
			ifstream FILE(fileName);
			string line;
			
			while (getline(FILE, line))
			{
				vector<pair<string, string>> bigramKey;
				istringstream iss(line);
				string tok;
				vector<string> sentenceToks;

				while (iss >> tok)
					sentenceToks.push_back(tok);


				for (auto it = cbegin(sentenceToks); it != prev(cend(sentenceToks)); ++it) {
					bigramKey.push_back(std::make_pair(*it, *next(it)));
				}

				for (const auto& e : bigramKey) {
					bigrams[e] = bigrams[e]++;
				}
				avgSentenceLength += sentenceToks.size();
				sentenceCount++;
			}

			FILE.clear();
			FILE.seekg(0, ios::beg);

			cout << "\nDo you have more files you would like to read?(y/n) ";
			cin >> fileScanCont;
		} while (fileScanCont == 'y');

		cout << "\n~Flushing Stream~\n" << flush;
		system("CLS");

		avgSentenceLength = avgSentenceLength / sentenceCount;
		cout << "The avg length of sentences is: " << avgSentenceLength;
		
		// Sentence Generation Code

		string startingWord;
		int sentenceLen;
		int numSentences;

		cout << "\nGive a starting word to generate a sentence(Type 'x' to stop): ";

		cin >> startingWord;
		if (startingWord != "x")
		{
			cout << "How long should the line be: ";
			cin >> sentenceLen;
		}

		srand(time(NULL));

		while(startingWord != "x")
		{
			// Generate bigram sentence
			string curWord = startingWord;
			cout << "\n\nLine: ";
			for (int x = 0; x < sentenceLen; x++)
			{
				vector<pair<string, int>> nextOptions;
				for (std::map<std::pair<std::string, std::string>, int>::iterator iter = bigrams.begin(); iter != bigrams.end(); ++iter)
				{
					pair<string, string> k = iter->first;
					if (k.first == curWord)
						nextOptions.push_back(make_pair(k.second, bigrams[k]));
				}
				cout << curWord << " ";

				if (nextOptions.size() > 0)
				{
					curWord = nextOptions[rand() % nextOptions.size()].first;
				}
				else
				{
					// There were no connected words, so what ever, let's just guess
					auto it = bigrams.begin();
					int randWord = rand() % bigrams.size();
					advance(it, randWord);
					curWord = it->first.first;
				}
				
			}
			cout << "\n";
			cout << "\nGive a starting word to generate a sentence(Type 'x' to stop): ";
			cin >> startingWord;
			if (startingWord != "x")
			{
				cout << "How long should the line be: ";
				cin >> sentenceLen;
			}
		} 

		cout << "\n\n\nDo you want to reset and run the bigram code again?(y/n)? ";
		cin >> cont;

		cout << "\n~Flushing Stream~\n" << flush;
		system("CLS");
	} while (cont == 'y');
}

int main()
{
	char inp;
	while (1 == 1)
	{
		cout << "Type 'b' to use bigram code or type 'm' to load madLib code: ";
		
		cin >> inp;

		if (inp == 'b' || inp == 'm')
			break;
		
	}

	if (inp == 'b')
		bigramSentenceCreator();
	else
		madLib();

	system("PAUSE");

    return 0;
}

