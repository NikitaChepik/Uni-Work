/*
 * File: WordLadder.cpp
 * --------------------
 * Name: Nikita Chepik
 * This file is the starter project for the word ladder problem on Assignment #1.
 */

#include "genlib.h"
#include "lexicon.h"
#include <fstream>
#include <queue>
#include <vector>
#include <list>
#include <iostream>
#include <algorithm>

struct Info {
  std::string currName;
  std::vector<std::string> namesUsed;
}info;

void remainingLadders(std::queue<Info> wordQ ,const int &endLength,const int &startWordLength,
	const std::string &endWord,std::vector<std::vector<std::string>> &ladderToPrint);
void printLadders(std::vector<std::vector<std::string>> &ladderToPrint);

int main() {

	Lexicon english("EnglishWords.dat");
	// Empty lexicon to hold words visited.
	Lexicon checkVisited;
	std::string startWord,endWord;
	std::cout <<"Enter start word (RETURN to quit): ";
	std::cin >> startWord;

	if(startWord == "RETURN"){
		return 0;
	}
	
	std::cout <<"Enter destination word: ";
	std::cin >> endWord;
	
	if(startWord == endWord){
		std::cout <<"Found ladder: " << startWord << " " << endWord << "\n";
		return 0;
	}
	// Holds the ladders that are correct.
	std::vector<std::vector<std::string>> ladderToPrint;
	// Queue of word ladders.
	std::queue<Info> wordQ;
	
	Info firstPush;
	firstPush.currName = startWord;
	firstPush.namesUsed.push_back(startWord);
	wordQ.push(firstPush);
	
	int depth = 0;
	std::vector<std::string> unusableWords;
	
	// If wordQ is = 0 means no words remaining so loop will break.
	while(wordQ.size() != 0){
		Info tempInfo;
		tempInfo = wordQ.front();
		wordQ.pop();
		std::string tempWord = tempInfo.currName;
		//Loops through each letter of the currentWord.
		for(unsigned int i = 0; i < startWord.size() ; ++i){
			tempWord = tempInfo.currName;
			//Loops through each possible letter for the current poistion 
			for(int j = 'a' ; j < 'z' ; ++j){
				tempWord[i] = j;
				if(tempWord == endWord){
					Info goodWord;
					goodWord.namesUsed = tempInfo.namesUsed;
					goodWord.namesUsed.push_back(tempWord);
					ladderToPrint.push_back(goodWord.namesUsed);
					wordQ.push(goodWord);
					remainingLadders(wordQ,goodWord.namesUsed.size(),depth++,
						endWord,ladderToPrint);
					printLadders(ladderToPrint);
					return 0;
				} else if(!checkVisited.containsWord(tempWord)&& english.containsWord(tempWord)){
					Info goodWord;
					goodWord.namesUsed = tempInfo.namesUsed;
					goodWord.namesUsed.push_back(tempWord);
					goodWord.currName = tempWord;
					wordQ.push(goodWord);
					unusableWords.push_back(tempWord);
				}
			}
		}	
		// Once the depth changes add all the used words for the previous depth
		// to the checkVisited lexicon
		if(tempInfo.namesUsed.size() != depth){
			for(auto &i:unusableWords){
				checkVisited.add(i);
			}
			unusableWords.clear();
			depth = tempInfo.namesUsed.size();
		}
	}
	std::cout <<"No ladder found.\n";
	return 0;
}

/* 
 *Checks all the remaining nodes at current depth and checks if the endWord is possible to 
 * reach from the current node.
 */
void remainingLadders(std::queue<Info> wordQ ,const int &endLength,const int &startWordLength,
	const std::string &endWord,std::vector<std::vector<std::string>> &ladderToPrint){
	while(wordQ.size() != 0){
		Info tempInfo;
		tempInfo = wordQ.front();
		wordQ.pop();
		std::string tempWord = tempInfo.currName;
		for(int i = 0; i < startWordLength ; ++i){
			tempWord = tempInfo.currName;
			for(int j = 'a' ; j < 'z' ; ++j){
				tempWord[i] = j;
				if(tempWord == endWord){
					tempInfo.namesUsed.push_back(tempWord);
					ladderToPrint.push_back(tempInfo.namesUsed);
				} else {
					wordQ.push(tempInfo);
				}
			}
		}	
		if(wordQ.size() > 0 && wordQ.front().namesUsed.size() >= endLength){
			break;
		}
		
	}
}

// Sorts ladders into lexigraphic order and prints them out.
void printLadders(std::vector<std::vector<std::string>> &ladderToPrint){
	std::sort(ladderToPrint.begin(), ladderToPrint.end());
	std::cout <<"Found ladder: ";
	for(const auto &i : ladderToPrint){
		for(const auto &j:i){
			std::cout << j << " ";
		}
		std::cout<< "\n";
	}
}