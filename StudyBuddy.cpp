//Preprocessor Directives
#include <iostream>
#include <iomanip>
#include <fstream> 
#include <time.h>
#include <stdlib.h>
#include <string.h>
#include <vector>
#define WORD_INFO "1-InputWords.txt"
#define STUDY_INFO "2-StudyData.dat"
#define WORDLIST "3-OutputWordList.txt"

using namespace std;

//Name Constant Definitions
struct term {
	string name;
	string definition;
	bool isStudy = false;
	int mastery = 0;
};

struct chosenTerm{
	int unit, wordNum;
};
vector<vector<term> > wordList;
vector<vector<int> > studyData;
vector<vector<int> > masteryData;

int unitCount = 0;
vector<int> wordCount;
const string line = string(80, '_');

void Title() {
	cout << " STUDY BUDDY\t\t\t\t\t     CREATED BY: SWAGAT GHIMIRE"<< endl << line << endl;
}

void Input() {
    ifstream inWord; //Used to connect to the file
    term currentWord; //Temporarily store the current set of information before pushing to the global vector
	inWord.open(WORD_INFO, ios::in); //open the connection
 	
 	currentWord.definition = "a";
 	currentWord.name = "a";
	vector<term> newRow;
	newRow.push_back(currentWord);
	vector<int> newNum;
	//newNum.push_back(0);
	//wordList.push_back(newRow);
 	inWord.seekg (0, inWord.end);
    //int length = is.tellg();
    
    
	if(!inWord) //Ensure that the file is opened
	{
		cout << "  Can't find the input file: '" << WORD_INFO << "'.\n  There may have been an error...\n\n  "; //If not, tell them there is no info
		system("Pause");
		system("CLS");
		return;
	}
	else if(inWord.tellg() == 0){
		cout << "  The Input file (" << WORD_INFO << ") is empty.\n  There may have been an error...\n\n  "; //If not, tell them there is no info
		system("Pause");
		system("CLS");
		return;
	}
	else //If there is a file with data
	{ 
		//cout << inWord.tellg() << endl << endl;
		inWord.seekg (0, inWord.beg);
 		while (!inWord.eof()) //Until the end of file is reached
 		{
 			getline(inWord, currentWord.name);
 			if(currentWord.name == "UNIT"){
 				wordList.push_back(newRow);
 				unitCount ++;
 				wordList[unitCount-1][0].name = currentWord.name;
 				getline(inWord, wordList[unitCount-1][0].definition);
			}else if (unitCount > 0){
				getline(inWord, currentWord.definition);
 				wordList[unitCount-1].push_back(currentWord);
			}
			/*input.read((char *) &currentCar, sizeof(DealerInfo)); //Read info and store to temporary struct
			cars.push_back(currentCar); //push the info on temporary struct to the main global vector*/
		}
		//wordList.pop_back(); //I didn't recognize then end of file character as the end of file and did one more loop. Delete that false input.
		cout << "  Successfully read word list.\n\n";
		wordCount.push_back(0);
		for (int i = 0; i < wordList.size(); i++){
			wordCount.push_back(wordList[i].size() - 1);
			cout<< "      Unit " << i+1 << ":\t" << wordList[i][0].definition <<"   (" << wordCount[i+1] << " Words)\n";
			wordCount[0] += wordCount [i+1];
		}
		cout << "      TOTAL Word Count:\t" << wordCount[0] << "\n\n";
	}
	inWord.close(); //close connection
	
	ifstream inStudy; //Used to connect to the file
	inStudy.open(STUDY_INFO, ios::in|ios::binary); //open the connection
	if(!inStudy || inStudy.peek() == std::ifstream::traits_type::eof()) //Ensure that the file is opened
	{
		for (int i = 0; i < wordList.size(); i++){
			studyData.push_back(newNum);
			masteryData.push_back(newNum);
			for (int j = 1; j < wordList[i].size(); j++){
				masteryData[i].push_back(j);
			}
		}
		cout << "  Could not find previously saved words and mastery from the file:\n      '" << STUDY_INFO << "'.\n  "; //If not, tell them there is no info
	}
	else //If there is a file with data
	{
		int currU = -1, currT = 1, readMastery, readStudy;
		//getline(inStudy, readStudy);
		while (!inStudy.eof()) {//Until the end of file is reached
	 		inStudy.read((char *) &readStudy, sizeof(int));
			if (readStudy == -1){
				if (currU != -1){					
					while (currT < wordCount[currU+1]){
						masteryData[currU].push_back(currT);
						currT++;
					}
				}
				currU ++;
				studyData.push_back(newNum);
				masteryData.push_back(newNum);
				currT = 1;
			}else{
				//getline(inStudy, readMastery);
				inStudy.read((char *) &readMastery, sizeof(int));
				//cout << "STUDY: " << readStudy << "/tMASTERY: " << readMastery << endl;
				if (readStudy == 0){
					wordList[currU][currT].isStudy = false;
				} else{
					wordList[currU][currT].isStudy = true;
					studyData[currU].push_back(currT);	
				}
				wordList[currU][currT].mastery = readMastery;
				if (readMastery < 3){
					masteryData[currU].push_back(currT);
				}
				currT ++;
			}
		}
		masteryData[currU].pop_back();
		while (currT < wordCount[currU+1]){
						masteryData[currU].push_back(currT);
						currT++;
		}
		while(currU < wordCount.size() - 2){
			currU ++;
			currT = 1;
			studyData.push_back(newNum);
			masteryData.push_back(newNum);
			while (currT < wordCount[currU+1]){
						masteryData[currU].push_back(currT);
						currT++;
			}
		}
		cout << "  Successfully input saved data, and mastery info.\n\n  ";
	}
	inStudy.close(); //close connection
	
	system("PAUSE");
	system("CLS");
}	

void OutputList() {
	Title();
	cout << "  Outputting the wordlist to the file '" << WORDLIST << "'\n";
	
	char isOverwrite;
	ofstream outList;
	ifstream isExist;
	isExist.open(WORDLIST, ios::in);
	outList.open(WORDLIST, ios::out);
	
	if(isExist){
		do{
			cout << "\n  The file '" << WORDLIST << "' already exists.\n  Would you like to overwrite it?\n    [Y] Yes, Overwrite the file\n    [N] No, Do NOT Overwrite the file\n\n    Please Enter Your Selection: ";
			cin >> isOverwrite;
		} while (tolower(isOverwrite) != 'y' && tolower(isOverwrite) != 'n');
		if (tolower(isOverwrite) == 'n'){
			cout << "\n  The file was not overwritten.\n  ";
			system("PAUSE");
			isExist.close();
			outList.close();
			system("CLS");
			return;
		}
	}
	isExist.close();
	
	outList << "This list was created by Swagat Ghimire's STUDY BUDDY!\n\n";
	for (int i = 0; i < wordList.size(); i++){
		for (int j = 1; j < wordList[i].size(); j++){
			outList << wordList[i][j].name << ": " << wordList[i][j].definition << endl;
		}
	}
	outList.close();
	cout << "\n  The word list has been output.\n\n  Copy all of its contents onto a separate program (Microsoft Word, Google Docs, etc.) to alphabetize it.\n\n  ";
	system("PAUSE");
	system("CLS");	
}

void SaveData() {
	ofstream output; //define the variable through which the program will write
		int unitMarker = -1, yes = 1, no = 0;
	output.open(STUDY_INFO, ios::out|ios::binary); //set the file as "CarRecords.dat", and ensure that it is output in binary
	for (int i = 0; i < wordList.size(); i ++) //for each car in the vector
	{
		output.write((char *) &unitMarker, sizeof(int));
		for (int j = 1; j < wordList[i].size(); j++){
			if (wordList[i][j].isStudy){
				output.write((char *) &yes, sizeof(int));
			} else {
				output.write((char *) &no, sizeof(int));
			}
			output.write((char *) &wordList[i][j].mastery, sizeof(int));
		}
	}
	output.close(); //close the connection to the file
}

void SaveWords() {
	ofstream outWords;
	outWords.open(WORD_INFO, ios::out);
	for (int i = 0; i < wordList.size(); i++){
		for (int j = 0; j < wordList[i].size(); j++){
			if (i != 0 || j != 0){
				outWords << endl;
			}
			outWords << wordList[i][j].name << endl << wordList[i][j].definition;
		}
	}
}

string MainMenu() {
	Title();
	string selection = "0"; //make slection invalid until the user enters a viable value
	cout << " MAIN MENU" << endl << "\n  You Currently Have " << wordCount[0] << " Words Documented!\n\n  What would you like to do?";
	cout << "\n\t[1] Randomly Practice Words\n\t[2] Study Saved Words\n\t[3] Mastery Mode\n\t[4] See And Edit Words\n\t[5] Output Wordlist\n\t[6] Exit STUDY BUDDY\n";
	//Provide options
	do
	{
		cout << "\n  Enter Selection: "; //Prompt for selection
		cin >> selection;
		if (selection !="1" && selection !="2" && selection !="3" && selection !="4" && selection !="5" && selection !="6")//if the selection is invalid
		{
			cout << "Invalid Selection.\n"; //Inform user of invalid selection
		}
	} while (selection !="1" && selection !="2" && selection !="3" && selection !="4" && selection !="5" && selection !="6");//if the selection is valid, continue
	return selection;//return the selection
}

int ChooseUnit(string playType) {
	system("CLS");
	int studyCount=0, masteryCount=0;
	for (int i = 0; i < wordList.size(); i++){
		studyCount += studyData[i].size();
		masteryCount += masteryData[i].size();
	}
	Title();
	int selection = -1;
	cout << " UNIT SELECTION\n\n  Please select the unit that you would like to see.\n";
	if (playType == "1"){
		cout << "\t[0] All Terms (" << wordCount[0] << " word(s))\n";
	} else if (playType == "2"){
		cout << "\t[0] All Terms (" << studyCount << " word(s) saved)\n";
	} else if (playType == "3"){
		cout << "\t[0] All Terms (" << masteryCount << " word(s) not mastered)\n";
	}
	
	for (int i = 1; i < wordCount.size(); i++){
		if (playType == "1" || playType == "4"){
			cout <<"\t[" << i << "] " << wordList[i-1][0].definition << " (" << wordCount[i] << " word(s))\n";
		} else if (playType == "2"){
			cout <<"\t[" << i << "] " << wordList[i-1][0].definition << " (" << studyData[i-1].size() << " word(s) saved)\n";
		} else{
			cout <<"\t[" << i << "] " << wordList[i-1][0].definition << " (" << masteryData[i-1].size() << " word(s) not mastered)\n";
		}	
	}
	do
	{
		cout << "\n  Enter Selection: "; //Prompt for selection
		cin >> selection;
		if (playType == "4" && selection == 0){
			selection = -1;
		}
		if (selection < 0 || selection > wordCount.size())//if the selection is invalid
		{
			cout << "  Invalid Selection.\n"; //Inform user of invalid selection
		}
	} while (selection < 0 || selection > wordCount.size());//if the selection is valid, continue
	return selection;
}

chosenTerm GenerateTerm(int unit){
	int currWord;
	chosenTerm generatedWord;
	if (unit != 0){
		currWord = rand() % (wordCount[unit]) + 1;
		generatedWord.unit = unit - 1;
		generatedWord.wordNum = currWord;
	} else{
		currWord = rand() % (wordCount[unit]) + 1;
		int uCounter =  1;
		while (currWord > wordCount[uCounter]){
			currWord -= wordCount[uCounter];
			uCounter ++;
		}
		generatedWord.unit = uCounter - 1;
		generatedWord.wordNum = currWord;
	}
	return generatedWord;
}

char GetExitStatus(chosenTerm currTest, bool isBrowse){
	int temp;
	bool isGood;
	char exitStatus, editType;
	do{
		cout << "\n  OPTIONS:\n    [S] To ";
		if (wordList[currTest.unit][currTest.wordNum].isStudy == 0){
			cout << "ADD term to study list";
		} else{
			cout << "REMOVE term from study list";
		}
		if (isBrowse){
			if (currTest.wordNum < wordCount[currTest.unit+1]){
				//cout << "\n\tNUM: " << currTest.wordNum << "\tCOUNT: " << wordCount[currTest.unit+1];
				cout << "\n    [N] To see NEXT word (" << wordList[currTest.unit][currTest.wordNum+1].name << ")";
			}
			if (currTest.wordNum > 1){
				cout << "\n    [P] To see PREVIOUS word (" << wordList[currTest.unit][currTest.wordNum-1].name << ")";
			}
			cout << "\n    [E] To EDIT this term\n    [D] To DELETE this term\n    [U] To exit to UNIT menu";
		} else{
			cout << "\n    [N] To continue to next word";
		}
		cout <<  "\n    [M] To exit to MAIN menu";
		
		do{
			cout<< "\n\n    Enter Selection: ";
			cin >> exitStatus;
			isGood = false;
			if (tolower(exitStatus) == 's' || tolower(exitStatus) == 'm'){
				isGood = true;
			}
			if (isBrowse == true){
				if (tolower(exitStatus) == 'e' || tolower(exitStatus) == 'd' || tolower(exitStatus) == 'u'){
					isGood = true;
				}
				if (currTest.wordNum > 1 && tolower(exitStatus) == 'p'){
					isGood = true;
				}
				if (currTest.wordNum < wordCount[currTest.unit+1] && tolower(exitStatus) == 'n'){
					isGood = true;
				}
			} else {
				if (tolower(exitStatus) == 'n'){
					isGood = true;
				}
			}
		} while(isGood == false);
		
		if (tolower(exitStatus) == 's'){
			if (wordList[currTest.unit][currTest.wordNum].isStudy == 1){
				for (int i = 0; i < studyData[currTest.unit].size(); i++){
					if (studyData[currTest.unit][i] == currTest.wordNum){
						temp = studyData[currTest.unit][i];
						studyData[currTest.unit][i] = studyData[currTest.unit][studyData[currTest.unit].size() - 1];
						studyData[currTest.unit][studyData[currTest.unit].size() - 1] = temp;
						studyData[currTest.unit].pop_back();
						wordList[currTest.unit][currTest.wordNum].isStudy = 0;
						i = studyData[currTest.unit].size();
						cout << "\n  The term has been removed from the study list.\n";
					}
				}
			}
			else{
				wordList[currTest.unit][currTest.wordNum].isStudy = 1;
				studyData[currTest.unit].push_back(currTest.wordNum);
				cout << "\n  The term has been added to the study list.\n";
			}
		}
		if (tolower(exitStatus) == 'e'){
			cout << "\n\n  Would you like to...";
			do{
				cout << "\n    [T] Edit the TERM\n    [D] Edit the DEFINITION\n    [C] To CANCEL\n\n    Enter Your Selection: ";
				cin >> editType;
			} while (tolower(editType) != 't' && tolower(editType) != 'd' && tolower(editType) != 'c');
			
			if (tolower(editType) == 't'){
				cout << "\n  Please enter the editted TERM: ";
				cin >> wordList[currTest.unit][currTest.wordNum].name;
				cout << "\n  The Term has been editted.\n\n";
			} else if (tolower(editType) == 'd'){
				cout << "\n  Please enter the editted Definition: ";
				cin >> wordList[currTest.unit][currTest.wordNum].definition;
				cout << "\n  The Definition has been editted.\n\n";
			}
			system("Pause");
			SaveWords();
		}
		if (tolower(exitStatus) == 'd'){
			do{
				cout << "\n  Are you sure you want to DELETE the term and all related data?\n\t[Y] YES, DELETE the term\n\t[N] NO, DO NOT DELETE the term\n\tEnter Your Selection: ";
				cin >> editType;
			} while (tolower(editType) != 'y' && tolower(editType) != 'n');
			if (tolower(editType) == 'y'){
				term delTerm, finTerm;
				delTerm = wordList[currTest.unit][currTest.wordNum];
				finTerm = wordList[currTest.unit][wordCount[currTest.unit+1]];
				wordList[currTest.unit][currTest.wordNum] = finTerm;
				wordList[currTest.unit].pop_back();
				
				int delNum = currTest.wordNum, finNum = wordCount[currTest.unit+1], tempNum;
				
				if (wordCount[currTest.unit+1] == 1){
					if (studyData[currTest.unit].size() == 1){
						studyData[currTest.unit].pop_back();
					}
					if (masteryData[currTest.unit].size() == 1){
						masteryData[currTest.unit].pop_back();
					}
				} else{
					for (int i = 0; i < studyData[currTest.unit].size(); i++){
						if (studyData[currTest.unit][i] == delNum){
							studyData[currTest.unit][i] = studyData[currTest.unit][studyData[currTest.unit].size()-1];
							studyData[currTest.unit].pop_back();
						}
						if (studyData[currTest.unit][i] == finNum){
							studyData[currTest.unit][i] = delNum;
						}
					}
					for (int i = 0; i < masteryData[currTest.unit].size(); i++){
						if (masteryData[currTest.unit][i] == delNum){
							masteryData[currTest.unit][i] = masteryData[currTest.unit][masteryData[currTest.unit].size()-1];
							masteryData[currTest.unit].pop_back();
						}
						if (masteryData[currTest.unit][i] == finNum){
							masteryData[currTest.unit][i] = delNum;
						}
					}
				}
				exitStatus = 'u';
				wordCount[currTest.unit+1] = wordList[currTest.unit].size()-1;
				cout << "\n  You have deleted the term: " << delTerm.name << endl;
				system("PAUSE");
				SaveWords();
				SaveData();
			} else{
				exitStatus = 's';
			}
		}
	}while(tolower(exitStatus) == 's');// || tolower(exitStatus) == 'e');
	return exitStatus;
}

void DefinitionMode(int unit) {
	chosenTerm testWord;
	char exitStatus;
	
	do{
		Title();
		cout << " DEFINITION FIRST MODE:\n\n";
		testWord = GenerateTerm(unit);
		cout << "  Definition: " << wordList[testWord.unit][testWord.wordNum].definition << "\n\n  ";
		system("PAUSE");
		cout << "\n  Term: " <<wordList[testWord.unit][testWord.wordNum].name << endl << line;
		
		exitStatus = GetExitStatus(testWord, false);
		system("CLS");	
	} while (tolower(exitStatus) != 'm');
	
}

void StudyMode (int unit) {
	chosenTerm testWord;
	int totalStudy, uCounter, currCounter;
	char exitStatus;
	bool isWord = false;
	do{
		totalStudy = 0;
		for (int i = 0; i < studyData.size(); i++){
			totalStudy += studyData[i].size();
		}
		
		if (unit!= 0){
			if (studyData[unit-1].size() != 0){
				testWord.unit = unit-1;
				testWord.wordNum = studyData[unit-1][rand() % studyData[unit-1].size()];
				isWord = true;
			}
		} else{
			if (totalStudy != 0){
				currCounter = rand() % (totalStudy) + 1;
				uCounter =  0;
				while (currCounter > studyData[uCounter].size()){
					currCounter -= studyData[uCounter].size();
					uCounter ++;
				}
				testWord.unit = uCounter;
				testWord.wordNum = studyData[uCounter][currCounter-1];
				isWord = true;	
			}
		}
		
		Title();
		cout << " STUDY MODE:\n\n";
		if (isWord == true){
			cout << "  Definition: " << wordList[testWord.unit][testWord.wordNum].definition << "\n\n  ";
			system("PAUSE");
			cout << "\n  Term: " <<wordList[testWord.unit][testWord.wordNum].name << endl << line;
			
			exitStatus = GetExitStatus(testWord, false);	
		} else{
			cout << "  There are no saved words in this unit.\n  ";
			system("PAUSE");
			exitStatus = 'm';
		}
		system("CLS");	
	} while (tolower(exitStatus) != 'm');
}

bool IsMatch (string user, string word){
	string alterWord = "", alterUser = "";
	for (int i = 0; i < user.length(); i++){
		if (tolower(user[i]) >= 'a' && tolower(user[i]) <= 'z'){
			alterUser += tolower(user[i]);
		}
	}
	for (int i = 0; i < word.length(); i++){
		if (tolower(word[i]) >= 'a' && tolower(word[i]) <= 'z'){
			alterWord += tolower(word[i]);
		}
	}
	if (alterWord == alterUser){
		return true;
	} else{
		return false;
	}
}

void MasteryMode (int unit) {
	chosenTerm testWord;
	int totalMastery, uCounter, currCounter, testType, tempNum;
	char exitStatus;
	string userWord;
	bool isCorrect;
	do{
		Title();
		totalMastery = 0;
		for (int i = 0; i < masteryData.size(); i++){
			totalMastery += masteryData[i].size();
		}
		
		testType = rand() % 10;
		
		if (unit!= 0){
			if (masteryData[unit-1].size() < 3){
				if (testType > 1){
					testType = 9;
				}
			} else if (masteryData[unit-1].size() < 6){
				if (testType > 4){
					testType = 9;
				}
			} else if (masteryData[unit-1].size() < 10){
				if (testType > 6){
					testType = 9;
				}
			} else if (masteryData[unit-1].size() < 20){
				if (testType > 7){
					testType = 9;
				}
			}
			cout << " MASTERY MODE:\n  Number of Mastered Word: " << wordCount[unit] - masteryData[unit-1].size() <<"\n  Number of Words not Mastered: " << masteryData[unit-1].size() << endl << endl;
			if (testType == 9 || masteryData[unit-1].size() == 0){
				testWord = GenerateTerm(unit);
				currCounter = testWord.wordNum + 1;
			}else{
				currCounter = rand() % masteryData[unit-1].size() +1;
				uCounter = unit-1;
				testWord.unit = uCounter;
				testWord.wordNum = masteryData[uCounter][currCounter-1];	
			}
		} else{
			if (totalMastery < 3){
				if (testType > 1){
					testType = 9;
				}
			} else if (totalMastery < 6){
				if (testType > 4){
					testType = 9;
				}
			} else if (totalMastery < 10){
				if (testType > 6){
					testType = 9;
				}
			} else if (totalMastery < 20){
				if (testType > 7){
					testType = 9;
				}
			}
			cout << " MASTERY MODE:\n  Number of Mastered Word: " << wordCount[0] - totalMastery <<"\n  Number of Words not Mastered: " << totalMastery << endl << endl;
			if (testType == 9 || totalMastery == 0){
				testWord = GenerateTerm(0);
				currCounter = testWord.wordNum + 1;
			} else{
				currCounter = rand() % (totalMastery) + 1;
				//cout << "\nGOT CURRCOUNT OF: " << currCounter << endl;
				uCounter =  0;
				while (currCounter > masteryData[uCounter].size()){
					currCounter -= masteryData[uCounter].size();
					uCounter ++;
				//	cout << "NEW CURRCOUNT OF: " << currCounter << endl;
				}
				testWord.unit = uCounter;
				testWord.wordNum = masteryData[uCounter][currCounter-1];	
			}
		}
		
		cout << "  Definition: " << wordList[testWord.unit][testWord.wordNum].definition << endl << endl;
		cout << "  Enter Word: ";
		cin >> userWord;
		
		if (IsMatch(userWord, wordList[testWord.unit][testWord.wordNum].name)){
			wordList[testWord.unit][testWord.wordNum].mastery ++;
			cout << "\n  CORRECT!\n  Your entry matched the term: " << wordList[testWord.unit][testWord.wordNum].name <<"\n  Your streak on this word is: " << wordList[testWord.unit][testWord.wordNum].mastery;
			if (wordList[testWord.unit][testWord.wordNum].mastery == 3){
				//cout << "\nCC: " << currCounter << "\tUnit: " << testWord.unit << "\tLast Spot: " << masteryData[testWord.unit].size()-1 << endl;
				tempNum = masteryData[testWord.unit][currCounter-1];
				masteryData[testWord.unit][currCounter-1] = masteryData[testWord.unit][masteryData[testWord.unit].size()-1];
				masteryData[testWord.unit][masteryData[testWord.unit].size()-1] = tempNum;
				masteryData[testWord.unit].pop_back();
			}
		} else{
			if (wordList[testWord.unit][testWord.wordNum].mastery > 2){
				masteryData[testWord.unit].push_back(testWord.wordNum);
			}
			cout << "\n  INCORRECT...\n  The Answer Is: " << wordList[testWord.unit][testWord.wordNum].name << "\n  Your streak on this word is: 0";
			wordList[testWord.unit][testWord.wordNum].mastery = 0;
		}
		cout << endl;
		//cout << "\nTerm: " <<wordList[testWord.unit][testWord.wordNum].name << endl << line;
		
		exitStatus = GetExitStatus(testWord, false);
		
		system("CLS");	
	} while (tolower(exitStatus) != 'm');
}

char ShowWord (int unit, int selection){
	char exitStatus;
	chosenTerm currTerm;
	currTerm.unit = unit;
	currTerm.wordNum = selection;
	
	Title();
	cout << " BROWSE MODE\n\n  Unit:\t\t" << wordList[unit][0].definition << "\n  Word:\t\t" << wordList[unit][selection].name << endl;
	cout << "  Definition:\t" << wordList[unit][selection].definition << endl << "  Saved:\t";
	if (wordList[unit][selection].isStudy){
		cout << "YES\n";
	} else{
		cout << "NO\n";
	}
	cout << "  Mastery:\t" << wordList[unit][selection].mastery;
	if (wordList[unit][selection].mastery > 2){
		cout << " (Mastered)";
	}
	cout << endl;
	exitStatus = GetExitStatus(currTerm, true);
	
	return exitStatus;
}

void BrowseMode(int unit){
	int selection;
	char exitStatus = 'u';
	//Title();
	unit --;
	do{
		system("CLS");
		Title();
		if (tolower(exitStatus) == 'u'){
			cout << " BROWSE MODE\n\n  Unit " << unit +1 << ": " << wordList[unit][0].definition << " ("<< wordCount[unit+1] << " words)\n";
			for (int i = 1; i <= wordCount[unit+1]; i++){
				cout << "\t[" << i << "] " << wordList[unit][i].name << endl;
			}
			cout << "\t[0] EXIT TO MAIN MENU\n";
			do {		
				cout << "\n  Please enter your selection: ";
				cin >> selection;
			} while (selection < 0 || selection > wordCount[unit+1]);
			system("CLS");
			if (selection != 0){
				exitStatus = ShowWord(unit, selection);
			} else{
				exitStatus = 'm';
			}
		} else if (tolower(exitStatus) == 'p'){
			system("CLS");
			selection --;
			exitStatus = ShowWord(unit, selection);
		} else if (tolower(exitStatus) == 'n'){
			selection++;
			system("CLS");
			exitStatus = ShowWord(unit, selection);
		} else if (tolower(exitStatus) == 'e'){
			system("CLS");
			exitStatus = ShowWord(unit, selection);
		}
	} while (tolower(exitStatus) != 'm');
	system("CLS");
}

void ExitApp(){
	system("CLS");
	Title();
	cout << "  Thank you for using STUDY BUDDY!\n\n  This application was created by Swagat Ghimire in June 2016.\n  ";
	system("PAUSE");
}

int main() {
    //Variable Declarations:
	string mmChoice;
	int uChoice;
	srand(time(NULL));
	//Function Body
	Title();
	Input();
	if (unitCount > 0){
		do {
			SaveData();
			mmChoice = MainMenu();
			if (mmChoice != "5" && mmChoice != "6"){
				uChoice = ChooseUnit(mmChoice);
			}
			if (mmChoice == "1"){ //Definition first
				system("CLS");
				DefinitionMode(uChoice);
			} else if (mmChoice == "2"){ // Study Mode
				system("CLS");
				StudyMode(uChoice);
			} else if (mmChoice == "3"){ // Mastery Mode
				system("CLS");
				MasteryMode(uChoice);
			} else if (mmChoice == "4"){ // Browse and Edit
				system("CLS");
				BrowseMode(uChoice);
			} else if (mmChoice == "5"){
				system("CLS");
				OutputList();
			}
		} while (mmChoice != "6");
	}
	
	ExitApp();
	
    return 0;
}
//end function main


