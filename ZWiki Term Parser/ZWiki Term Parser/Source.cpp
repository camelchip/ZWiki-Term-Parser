// Source.cpp : This file contains the 'main' function. Program execution begins and ends there.
//

#include <iostream>
#include <string>
#include <fstream>
#include <sstream>
//#include <locale>
#include <Windows.h>
//#include <locale.h>
#include <vector>

using namespace std;

void dataToCsv() {
	ifstream infile;
	ofstream outfile;

	string inputFileName;
	string lineStr;

	int delIndex;

	cout << "Please enter name of input file: ";
	cin >> inputFileName;

	infile.open(inputFileName);
	outfile.open(inputFileName.erase(inputFileName.size() - 4, inputFileName.size()) + "_output" + ".txt");

	//Check for failure to open input
	if (infile.fail()) {
		cout << "ERROR. OPEN FAILED." << endl << endl;
		outfile << "ERROR. OPEN FAILED." << endl << endl;
	}

	//Check for blank file
	else if (infile.peek() == ifstream::traits_type::eof()) {
		cout << "ERROR. BLANK FILE." << endl << endl;
		outfile << "ERROR. BLANK FILE." << endl << endl;
	}

	int counterIndex = 0;
	int counterRemains = 0;

	outfile << "Name_Internal,Name_Displayed" << endl;

	while (getline(infile, lineStr)) {
		stringstream ss(lineStr);

		//Static/fixed string cleaning
		switch (counterIndex) {
		case 0:
			lineStr.erase(0, 2);
			lineStr.erase(lineStr.size() - 1, lineStr.size());
			cout << lineStr << ",";
			outfile << lineStr << ",";

			if (lineStr.find("Remains") != -1) { //Contains "Remains" (i.e. is Divine Beast)
				counterRemains = 11;
			}

			counterIndex++;
			break;
		case 1:
			if (counterRemains == 0) {
				counterIndex++;
			}
			else {
				counterRemains--;
			}
			break;
		case 2:
			lineStr.erase(0, 14);
			cout << lineStr << endl;
			outfile << lineStr << endl;

			counterIndex = 0;
			break;
		}

	}

	infile.close();
	outfile.close();
}


void csvToWikiText() {
	ifstream infile;
	ofstream outfile;

	string inputFileName;
	string gameAbbr;
	string lineStr;

	cout << "Please enter name of input file: ";
	cin >> inputFileName;

	cout << endl << "Please enter relevant game abbreviation: ";
	cin >> gameAbbr;

	infile.open(inputFileName);
	outfile.open(inputFileName.erase(inputFileName.size() - 4, inputFileName.size()) + "_output" + ".txt");

	string esName = "";
	bool sameAsENG = false;

	//For now, just want base ID & actual name (if applicable)
	while (getline(infile, lineStr)) {
		stringstream ss(lineStr);

		esName = "";
		sameAsENG = false;

		//English name
		getline(ss, lineStr, ',');
		outfile << lineStr << ": " << endl;

		//Spanish name
		getline(ss, lineStr, ',');
		esName = lineStr;
		outfile << "|esL= " << lineStr;

		//Same as ENG?
		getline(ss, lineStr, ',');
		if (lineStr == "Y") {
			sameAsENG = true;
		}

		//Translation
		getline(ss, lineStr, ',');
		if (sameAsENG) {
			outfile << "|esLM= Same as English.";
		}
		else {
			outfile << "|esLM= " << lineStr;
		}

		//Citation (via Map)
		////outfile << "|esLR= {{Cite|" << esName << "|Map|BotW}}" << endl << endl;
		outfile << "|esLR= {{Cite|" << esName << "|Map|" << gameAbbr << "}}" << endl << endl;

	}

	infile.close();
	outfile.close();
}


void dataListToText() {
	ifstream infile;
	ofstream outfile;
	ofstream errLog;

	string inputFileName;
	string lineStr;

	vector<string> langCodeList;
	//vector<string> langCodeList = { "USen", "EUen", "JPja", "CNzh", "TWzh", "", "", "EUnl", "",
	//	"USfr", "EUfr", "EUde", "", "", "EUit", "KRko", "", "", "", "", "EUru", "EUes", "USes", ""};
	vector<string> langWikiAliasList;


	string currID;
	string currText;
	bool nameFound = false;
	int currTermNum = 0;

	vector<vector<string>> dataVect;
	int langVectOffset = 1;
	errLog.open("errLog.txt");

	char langChoice;

	cout << "Use all languages, including non-Romanized?" << endl;
	cout << "(y/n): ";
	cin >> langChoice;

	if (langChoice == 'y') { //Use all languages
		langCodeList = { "USen", "EUen", "JPja", "CNzh", "TWzh", "EUnl",
			"USfr", "EUfr", "EUde", "EUit", "KRko", "EUru", "EUes", "USes" };
		langWikiAliasList = { "BASE", "enBr", "ja", "zhS", "zhT", "nl",
		"frC", "frF", "de", "it", "ko", "ru", "esS", "esL" };
	}
	else if (langChoice == 'n') {
		langCodeList = { "USen", "EUen", "EUnl",
			"USfr", "EUfr", "EUde", "EUit", "EUes", "USes" };
		langWikiAliasList = { "BASE", "enBr", "nl",
		"frC", "frF", "de", "it", "esS", "esL" };
	}

	//Error/Discrepancy formatting info
	cout << endl << "Errors/discrepancies, with format:" << endl;
	cout << "-<Term> (<lang>): '<Current Term>' vs. '<Expected Term>'" << endl;
	errLog << "Errors/discrepancies, with format:" << endl;
	errLog << "-<Term> (<lang>): '<Current Term>' vs. '<Expected Term>'" << endl;


	for (int index = 0; index < langCodeList.size(); index++) {
		inputFileName = langCodeList.at(index) + ".txt";

		infile.open(inputFileName);
		////outfile.open(inputFileName.erase(inputFileName.size() - 4, inputFileName.size()) + "_output" + ".txt");

		nameFound = false;
		currTermNum = 0;

		//Get all file lines
		while (getline(infile, lineStr)) {
			stringstream ss(lineStr);

			getline(ss, lineStr);
			//cout << lineStr << endl; //output full line

			//Get ID
			if (lineStr.find_first_not_of(' ') == 2) { //Assumption: only ID's start w/ 2 spaces & then the ID
				lineStr.pop_back(); //Remove ending colon
				currID = lineStr.substr(2);

				if (index == 0) { //Only do for first run
					//Push into new vector row
					dataVect.push_back(vector<string>());
					dataVect.at(currTermNum).push_back(currID);
				}

				//cout << currID << endl;
				nameFound = false;
				currTermNum++;
			}

			else if (!nameFound && lineStr.size() > 14) { //Only want one name per object; e.g. only Kanji, no Katakana
				if (lineStr.substr(6, 7) == "- text:") {
					currText = lineStr.substr(14);

					//Check for weird surrounding quotation marks
					if (langCodeList.at(index) == "USfr" || langCodeList.at(index) == "EUfr") {
						if (currText.at(0) == '"' && currText.at(currText.size() - 1) == '"') {
							//currText.find("\n") == string::npos
							if (currText.size() < 40) {
								cout << "Note: removing quotation marks of " << currText;

								currText.erase(0, 1);
								currText.pop_back();

								cout << " (" << currText << ")" << endl;
							}
						}
					}


					//Push into vector column
					dataVect.at(currTermNum - 1).push_back(currText);

					////cout << "Name: " << currText << endl;
					////outfile << "Name: " << currText << endl;
					nameFound = true;
				}
			}
		}
		infile.close();
		////outfile.close();
	}


	//Import data to translation page
	inputFileName = "translationPage.txt";

	infile.open(inputFileName);
	outfile.open(inputFileName.erase(inputFileName.size() - 4, inputFileName.size()) + "_output" + ".txt");

	string currTerm;
	int currTermRow = 0;
	bool validTerm = false;

	//Get all file lines
	while (getline(infile, lineStr)) {
		stringstream ss(lineStr);
		getline(ss, lineStr);

		if (lineStr.size() > 20 && (lineStr.substr(0, 20) == "{{Translation/Store|" || lineStr.substr(3, 20) == "{{Translation/Store|")) { //Check for template header (given line is long enough)
			validTerm = false;
			outfile << lineStr << endl;

			//Isolate name in string
			for (int index = lineStr.size() - 1; index > 0; index--) {
				if (lineStr.at(index) == '|') {
					lineStr.erase(0, index + 1);
					currTerm = lineStr;
					break;
				}
			}
			////cout << endl << "Term name: " << currTerm << endl;
			////errLog << endl << "Term name: " << currTerm << endl;

			//Find Term in data vector
			for (int index = 0; index < dataVect.size(); index++) {
				if (dataVect.at(index).at(1) == currTerm) { //check for valid English name
					currTermRow = index;
					validTerm = true;
				}
			}
		}

		else {
			for (int index = 0; index < langWikiAliasList.size(); index++) {
				if (validTerm && lineStr.substr(0, 2 + langWikiAliasList.at(index).size()) == "|" + langWikiAliasList.at(index) + "=") {
					if (lineStr.substr(2 + langWikiAliasList.at(index).size(), 2) == " |") { //blank (no current translation)
						//Output modified line to wiki file
						outfile << "|" + langWikiAliasList.at(index) + "= " + dataVect.at(currTermRow).at(index + 1) + lineStr.substr(3 + langWikiAliasList.at(index).size()) << endl;
					}
					else if (lineStr.substr(3 + langWikiAliasList.at(index).size(), (lineStr.substr(1).find_first_of('|') - (2 + langWikiAliasList.at(index).size()))) == dataVect.at(currTermRow).at(index + 1)) { // ja= |jaM= |jaR= 
						////cout << "-(" << langWikiAliasList.at(index) << ") NOTE: exists, but verified: " << endl;
						////cout << "       '" << lineStr.substr(3 + langWikiAliasList.at(index).size(), (lineStr.substr(1).find_first_of('|') - (2 + langWikiAliasList.at(index).size()))) << "' vs. '" << dataVect.at(currTermRow).at(index + 1) << "'" << endl;

						//Output existing line to wiki file as normal
						outfile << lineStr << endl;
					}
					else { //check validity/notify user
						//Long version
						////cout << endl << "Term name: " << currTerm << endl;
						////errLog << endl << "Term name: " << currTerm << endl;
						////cout << "-(" << langWikiAliasList.at(index) << ") WARNING: exists, but no match: " << endl;
						////cout << "       '" << lineStr.substr(3 + langWikiAliasList.at(index).size(), (lineStr.substr(1).find_first_of('|') - (2 + langWikiAliasList.at(index).size()))) << "' vs. '" << dataVect.at(currTermRow).at(index + 1) << "'" << endl;
						////errLog << "-(" << langWikiAliasList.at(index) << ") WARNING: exists, but no match: " << endl;
						////errLog << "       '" << lineStr.substr(3 + langWikiAliasList.at(index).size(), (lineStr.substr(1).find_first_of('|') - (2 + langWikiAliasList.at(index).size()))) << "' vs. '" << dataVect.at(currTermRow).at(index + 1) << "'" << endl;

						//Shorter version
						cout << endl << "-" << currTerm << " (" << langWikiAliasList.at(index) << "): ";
						cout << "'" << lineStr.substr(3 + langWikiAliasList.at(index).size(), (lineStr.substr(1).find_first_of('|') - (2 + langWikiAliasList.at(index).size()))) << "' vs. '" << dataVect.at(currTermRow).at(index + 1) << "'";

						errLog << endl << "-" << currTerm << " (" << langWikiAliasList.at(index) << "): ";
						errLog << "'" << lineStr.substr(3 + langWikiAliasList.at(index).size(), (lineStr.substr(1).find_first_of('|') - (2 + langWikiAliasList.at(index).size()))) << "' vs. '" << dataVect.at(currTermRow).at(index + 1) << "'";

						//Output existing line to wiki file as normal
						outfile << lineStr << endl;
					}
					goto end;
				}
			}
			outfile << lineStr << endl;
		end:
			cout << "";
		}

		////else if (validTerm && lineStr.substr(0, 4) == "|ja=") {
		////	if (lineStr.substr(4, 2) == " |") { //blank (no current translation)
		////		outfile << "|ja= " + dataVect.at(currTermRow).at(3) + lineStr.substr(5) << endl;
		////	}
		////	else if (lineStr.substr(5, (lineStr.substr(1).find_first_of('|') - 4)) == dataVect.at(currTermRow).at(3)) { // ja= |jaM= |jaR= 
		////		cout << "Exists, but verified" << endl;
		////	}
		////	else { //check validity/notify user
		////		cout << "WARNING: exists, but no match. " << endl;
		////		cout << "    '" << lineStr.substr(5, (lineStr.substr(1).find_first_of('|') - 4)) << "' vs. '" << dataVect.at(currTermRow).at(3) << "'" << endl;
		////	}
		////}


	}



	cout << endl << endl;
	infile.close();
	outfile.close();
	errLog.close();


	ofstream outcsv;
	outcsv.open("output.csv");
	const char *bom = "\xef\xbb\xbf";
	outcsv << bom;

	//Output Headers
	outcsv << "Internal ID" << ",";
	for (int index = 0; index < langCodeList.size(); index++) {
		outcsv << langCodeList.at(index) << ",";
	}
	outcsv << endl;

	//Output Data
	for (int row = 0; row < dataVect.size(); row++) {
		for (int column = 0; column < dataVect.at(row).size(); column++) {
			outcsv << dataVect.at(row).at(column) << ",";
		}
		outcsv << endl;
	}
	outcsv.close();
}


void csvLangToWikiText() {
	ifstream infile;
	ofstream outfile;
	ofstream errLog;
	ofstream errLogRomanization;

	string inputFileName;
	string lineStr;

	vector<string> langCodeList;
	vector<string> langWikiAliasList;


	string currID;
	string currText;
	bool nameFound = false;
	int currTermNum = 0;

	vector<vector<string>> dataVect;
	int langVectOffset = 1;
	errLog.open("errLog.txt");
	errLogRomanization.open("errLogRomanization.txt");

	char langChoice;

	//cout << "Use all languages, including non-Romanized?" << endl;
	//cout << "(y/n): ";
	//cin >> langChoice;
	langChoice = 'y';

	if (langChoice == 'y') { //Use all languages
		langCodeList = { "USen", "EUfr", "USfr", "EUde", "USes", "EUes",
			"EUit", "EUnl", "EUru", "EUruLATIN", "JPja", "JPjaLATIN",
			"TWzh", "TWzhLATIN", "CNzh", "CNzhLATIN", "KRko", "KRkoLATIN" };
		langWikiAliasList = { "BASE", "frF", "frC", "de", "esL", "esS",
			"it", "nl", "ru", "ru_LATIN", "ja", "ja_LATIN",
			"zhT", "zhT_LATIN", "zhS", "zhS_LATIN", "ko", "ko_LATIN" };

	}

	//CSV input stuff
	////inputFileName = "testSheet.csv";
	inputFileName = "Term Translations Input.csv";
	infile.open(inputFileName);

	//Allocate vector space (col's for each lang)
	for (int index = 0; index < langCodeList.size(); index++) {
		dataVect.push_back(vector<string>());
	}

	//Loop through all lines of CSV file
	while (getline(infile, lineStr)) {
		stringstream ss(lineStr);

		//For each language (i.e. column), store name in data vector
		for (int index = 0; index < langCodeList.size(); index++) {
			getline(ss, lineStr, ',');
			dataVect.at(index).push_back(lineStr);
		}
	}
	infile.close();



	//Import data to translation page
	inputFileName = "translationPage.txt";

	infile.open(inputFileName);
	outfile.open(inputFileName.erase(inputFileName.size() - 4, inputFileName.size()) + "_output" + ".txt");

	string currTerm;
	int currTermRow = 0;
	bool validTerm = false;

	string langAlias;
	int langAliasLength = 0;

	string langTerm;
	string langMeaning;

	///User inputs
	string termRef = "";
	cout << "Please enter the Reference name for the given CSV (e.g. Map, Hyrule Compendium): ";
	getline(cin, termRef);

	string gameAbbr = "";
	cout << "Please enter the relevant game abbreviation (e.g. BotW): ";
	getline(cin, gameAbbr);


	//Error/Discrepancy formatting info
	cout << endl << "Errors/discrepancies, with format:" << endl;
	cout << "-<Term> (<lang>): '<Current Translation>' vs. '<New Translation>'" << endl;
	errLog << "Errors/discrepancies, with format:" << endl;
	errLog << "-<Term> (<lang>): '<Current Translation>' vs. '<New Translation>'" << endl;
	errLogRomanization << "List of skipped Terms, due to missing Romanization/Latinization:" << endl;


	//Get all file lines
	while (getline(infile, lineStr)) {
		stringstream ss(lineStr);
		getline(ss, lineStr);

		//TODO: -Add "Reference" support <--(e.g. "|nl= Orange|nlM= |nlR= {{Cite|BotW|Orange|Map}}")
		//		-Also add support for pre-existing Terms? (provided they match)
		//		-Could just be user input via console (i.e. ASSUME all from sheet have same source)
		//			-Alternatively, could add an extra col. to csv for each Term's lang's (but would add a LOT of empty space to csv...)

		//TODO: -Add "Meaning" support? <--(e.g. "|nl= Orange|nlM= Orange|nlR= ")
		//		-Also add support for pre-existing Terms? (provided they match)
		//		-Would have to have an extra col. in csv for each lang.
		//			-Expect lots to be empty; *but*, if have support for pre-exising, could always add to CSV & run script again later
		//		-Since adding new col. for each, can just modify "for" loop
		//			-E.g. Multiply x2, then skip every-other loop/col.?
		//			-Alternatively: Read in headers properly, then loop thru normally (& skip col.'s marked w/ e.g. "_"0
		//			-e.g. en | de | ja --> _en | _de | _ja
		//				-(Technically, don't need Meaning col. for English, though)
		//				-That way, could also verify lang.'s present & have custom orders

		//////////////////////////////////////////////////////////////////////////////////////
		// Template Header (e.g. "{{Translation/Store|BotW|Acorn")							//
		//////////////////////////////////////////////////////////////////////////////////////
		if (lineStr.size() > 20 && (lineStr.substr(0, 20) == "{{Translation/Store|" || lineStr.substr(3, 20) == "{{Translation/Store|")) {
			validTerm = false;
			outfile << lineStr << endl;

			//Isolate Term name in string/line (e.g. "Acorn")
			for (int index = lineStr.size() - 1; index > 0; index--) {
				if (lineStr.at(index) == '|') {
					lineStr.erase(0, index + 1);	//Erase everything preceding Term name (i.e. "{{Translation/Store|BotW|")
					currTerm = lineStr;
					break;
				}
			}
			////cout << endl << "Term name: " << currTerm << endl;
			////errLog << endl << "Term name: " << currTerm << endl;

			//Find Term in data vector
			for (int index = 0; index < dataVect.at(0).size(); index++) {
				if (dataVect.at(0).at(index) == currTerm) { //check for valid English name
					currTermRow = index;
					validTerm = true;
					break;
				}
			}
		}

		//////////////////////////////////////////////////////////////////////////////////////
		// Normal Data Line																	//
		//////////////////////////////////////////////////////////////////////////////////////
		else {

			//Loop through all languages
			for (int index = 0; index < langWikiAliasList.size(); index++) {
				langAlias = langWikiAliasList.at(index);
				langAliasLength = langWikiAliasList.at(index).size();

				langTerm = dataVect.at(index).at(currTermRow);
				langMeaning = "";

				//If a header has been found & line matches current language
				if (validTerm && lineStr.substr(0, 2 + langAliasLength) == ("|" + langAlias + "=")) {


					//////////////////////////////////////////////////////////////////////////////////////
					// Translation is blank (no current translation)   <--(e.g. "|esS= |esSM= |esSR= ") //
					//////////////////////////////////////////////////////////////////////////////////////
					if (lineStr.substr(2 + langAliasLength, 2) == " |") {
						if (langTerm != "") {
							////////////////////////
							// Non-Latin Language //
							////////////////////////
							if (langAlias == "ru" || langAlias == "ja" || langAlias == "zhT" || langAlias == "zhS" || langAlias == "ko") {
								string romanizedTerm = dataVect.at(index + 1).at(currTermRow);	//ASSUMES: All non-Latin lang's will have next col. be Romanization!

								//If CSV doesn't have a Romanization/Latin-equivalent yet, then skip
								if (romanizedTerm == "" || romanizedTerm == "\"") { //empty or <">
									errLogRomanization << "-(" << langAlias << ") " << currTerm << ": ";
									errLogRomanization << "no Romanization available (" << langTerm << " == '" << dataVect.at(index + 1).at(currTermRow) << "')" << endl;

									//Output existing line (no changes!)
									outfile << lineStr << endl;
								}
								else {
									//Output modified line (w/ Term & Romanization)	<--(e.g. "|ja= {{Romanize|***|Rinku}}|jaM= |jaR= ")
									////outfile << "|" + langAlias + "= {{Romanize|" + langTerm + "|" + romanizedTerm + "}}" + lineStr.substr(3 + langAliasLength) << endl;	// Rest of OG line after "|ja= " (e.g. "|jaM= |jaR= ")
									outfile << "|" + langAlias + "= {{Romanize|" + langTerm + "|" + romanizedTerm + "}}";
									outfile << "|" + langAlias + "M= " + langMeaning;

									if (termRef != "") {
										////outfile << "|" + langAlias + "R= " + "{{Cite|" + langTerm + "|" + termRef + "|BotW}}" << endl;
										outfile << "|" + langAlias + "R= " + "{{Cite|" + langTerm + "|" + termRef + "|" << gameAbbr << "}}" << endl;
									}
									else {
										outfile << "|" + langAlias + "R= " << endl;
									}
								}
							}

							////////////////////////
							//   Latin Language	  //
							////////////////////////
							else {
								//Output modified line (w/ Term) <--(e.g. "|nl= Orange|nlM= |nlR= ")
								////outfile << "|" + langAlias + "= " + langTerm + lineStr.substr(3 + langAliasLength) << endl;
								outfile << "|" + langAlias + "= " + langTerm;
								outfile << "|" + langAlias + "M= " + langMeaning;

								if (termRef != "") {
									outfile << "|" + langAlias + "R= " + "{{Cite|" + langTerm + "|" + termRef + "|" << gameAbbr << "}}" << endl;
								}
								else {
									outfile << "|" + langAlias + "R= " << endl;
								}
							
							}
						}

						//Missing Term from CSV, too
						else {
							//Output existing line (no changes!)
							outfile << lineStr << endl;
						}
					}


					//////////////////////////////////////////////////////////////////////////////////////
					// Translation exists & matches CSV (Latin language)   <--(e.g. "|nl= Appel|nlM= |nlR= ")
					//////////////////////////////////////////////////////////////////////////////////////
					else if (lineStr.substr(3 + langAliasLength, (lineStr.substr(1).find_first_of('|') - (2 + langAliasLength))) == langTerm) {
						//Add Term Reference if missing
						if (termRef != "" && lineStr.substr(lineStr.size() - (4 + langAliasLength)) == ("|" + langAlias + "R= ")) {	//<--(e.g. "|nlR= ")
							outfile << lineStr.substr(0, lineStr.size() - (4 + langAliasLength));
							outfile << "|" + langAlias + "R= {{Cite|" + langTerm + "|" + termRef + "|" << gameAbbr << "}}" << endl;
						}
						else {
							//Output existing line (no changes!)
							outfile << lineStr << endl;
						}
					}


					//////////////////////////////////////////////////////////////////////////////////////
					// Translation exists & matches CSV (non-Latin language)   <--(e.g. "|ja= {{Romanize|xyz|Ringo}}|jaM= Apple|jaR= ")  (isolates OG lang. term to just "xyz")
					//////////////////////////////////////////////////////////////////////////////////////
					else if (lineStr.substr(14 + langAliasLength, (lineStr.substr(1).find_first_of('|', 14 + langAliasLength)) - (13 + langAliasLength)) == langTerm) { // ja= |jaM= |jaR= 
						//Add Term Reference if missing
						if (termRef != "" && lineStr.substr(lineStr.size() - (4 + langAliasLength)) == ("|" + langAlias + "R= ")) {	//<--(e.g. "|jaR= ")
							outfile << lineStr.substr(0, lineStr.size() - (4 + langAliasLength));
							outfile << "|" + langAlias + "R= {{Cite|" + langTerm + "|" + termRef + "|" << gameAbbr << "}}" << endl;
						}
						else {
							//Output existing line (no changes!)
							outfile << lineStr << endl;
						}

					}


					//////////////////////////////////////////////////////////////////////////////////////
					// Translation exists, but DOESN'T match CSV! (check validity & notify user!)		//
					//////////////////////////////////////////////////////////////////////////////////////
					else {

						//Print error msg to console & log (if CSV isn't an empty cell)
						if (langTerm != "") {
							//Non-Latin vs. Latin language
							if (langAlias == "ru" || langAlias == "ja" || langAlias == "zhT" || langAlias == "zhS" || langAlias == "ko") {
								cout << endl << "-" << currTerm << " (" << langAlias << "): ";
								cout << "'" << lineStr.substr(14 + langAliasLength, (lineStr.substr(1).find_first_of('|', 14 + langAliasLength)) - (13 + langAliasLength)) << "' vs. '" << langTerm << "'";

								errLog << endl << "-" << currTerm << " (" << langAlias << "): ";
								errLog << "'" << lineStr.substr(14 + langAliasLength, (lineStr.substr(1).find_first_of('|', 14 + langAliasLength)) - (13 + langAliasLength)) << "' vs. '" << langTerm << "'";
							}
							else {
								cout << endl << "-" << currTerm << " (" << langAlias << "): ";
								cout << "'" << lineStr.substr(3 + langAliasLength, (lineStr.substr(1).find_first_of('|') - (2 + langAliasLength))) << "' vs. '" << langTerm << "'";

								errLog << endl << "-" << currTerm << " (" << langAlias << "): ";
								errLog << "'" << lineStr.substr(3 + langAliasLength, (lineStr.substr(1).find_first_of('|') - (2 + langAliasLength))) << "' vs. '" << langTerm << "'";
							}
						}

						//Output existing line (no changes!)
						outfile << lineStr << endl;
					}
					goto end;
				}
			}
			outfile << lineStr << endl;
		end:
			cout << "";
		}
	}



	cout << endl << endl;
	infile.close();
	outfile.close();
	errLog.close();
	errLogRomanization.close();


	/*
	ofstream outcsv;
	outcsv.open("output.csv");
	const char *bom = "\xef\xbb\xbf";
	outcsv << bom;

	//Output Headers
	outcsv << "Internal ID" << ",";
	for (int index = 0; index < langCodeList.size(); index++) {
		outcsv << langCodeList.at(index) << ",";
	}
	outcsv << endl;

	//Output Data
	for (int row = 0; row < dataVect.size(); row++) {
		for (int column = 0; column < dataVect.at(row).size(); column++) {
			outcsv << dataVect.at(row).at(column) << ",";
		}
		outcsv << endl;
	}
	outcsv.close();
	*/
}


int main()
{
	SetConsoleOutputCP(1252);
	SetConsoleCP(1252);
	SetConsoleOutputCP(65001);

	//dataToCsv();
	//csvToWikiText();
	//dataListToText();
	csvLangToWikiText();





	//End Program
	system("pause");
	return 0;
}

