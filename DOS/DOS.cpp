#include <iostream>
#include <string>
#include <vector>
#include <fstream>
#include <sstream>
#include <ctime>
#include <list>
#include <deque>
#include<stack>
#include <stdio.h>
#include <conio.h>
#include <algorithm>  // for std::remove_if
#include <iterator>
#include <windows.h>
using namespace std;


string lastCommand = "";

void setColor(int color) {
	SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), color);
}
class file {
public:
	string name;
	string data;
	int charCount;
	int lineCount;
	double avgCharPerLine;
	time_t creationTime;
	string owner;
	bool readOnly;
	int priority;
	int timeToPrint;
	string fileType;
	string path;
	file(string n = "NULL", string o = "NULL", bool r = false, int p = 0, int t = 0, string fT = "NULL", string pth = "", time_t cT = time(0), string d = "") {
		name = n;
		owner = o;
		readOnly = r;
		priority = p;
		timeToPrint = t;
		fileType = fT;
		path = pth;
		creationTime = cT;
		data = d;
	}

	void print() {
		setColor(1);
		cout << "Name: " << name << endl;
		cout << "Owner: " << owner << endl;
		cout << "Read Only: " << readOnly << endl;
		cout << "Priority: " << priority << endl;
		cout << "Time to Print: " << timeToPrint << endl;
		cout << "File Type: " << fileType << endl;
		cout << "Path: " << path << endl;
	}
};

class Folder {
public:
	friend class Tree;
	string name;
	time_t creationTime;
	string owner;
	bool hidden;
	vector<Folder*> subFolders;
	vector<file*> files;
	Folder* parent;
	string path;
	Folder(string n = "NULL", string p = "", Folder* par = nullptr, vector<file*> fs = vector<file*>(), vector<Folder*> FS = vector<Folder*>()) {
		name = n;
		files = fs;
		subFolders = FS;
		parent = par;
		path = p;
		hidden = false;
	}
	bool removeFile(const string& name) {
		for (int i = 0; i < files.size(); i++) {
			if (files[i]->name == name) {
				// Swap with the last element and pop back
				swap(files[i], files[files.size() - 1]);
				files.pop_back();
				return true; // Return true for successful removal
			}
		}

		// If the loop completes without finding the file, return false
		return false;
	}
	bool removeFolder(const string& name) {
		for (int i = 0; i < subFolders.size(); i++) {
			if (subFolders[i]->name == name) {
				// Swap with the last element and pop back
				swap(subFolders[i], subFolders[subFolders.size() - 1]);
				subFolders.pop_back();
				return true; // Return true for successful removal
			}
		}

		// If the loop completes without finding the folder, return false
		return false;
	}


	void addFile(file* f) {
		files.push_back(f);
	}
	void addFolder(Folder* f) {
		subFolders.push_back(f);
	}
	void emptyDirectory() {
		files.resize(0);
		subFolders.resize(0);
	}
	file* findFile(string name) {
		for (int i = 0; i < files.size(); i++) {
			if (files[i]->name == name) {
				return files[i];
			}
		}
		return nullptr;
	}
	Folder* findFolder(string name) {
		for (int i = 0; i < subFolders.size(); i++) {
			if (subFolders[i]->name == name) {
				return subFolders[i];
			}
		}
		return nullptr;
	}
	// Modify the print function for dir
	void print(Folder* current, const string& filterExtension = "") {
		// Display the directory structure
		setColor(4);
		cout << "Directory of " << current->path << "\\" << current->name << "\\" << endl << endl;

		for (int i = 0; i < current->subFolders.size(); i++) {
			cout << "                          <DIR>      " << current->subFolders[i]->name << endl;
		}

		for (int i = 0; i < current->files.size(); i++) {
			// Extract the file extension from the file name
			size_t dotPos = current->files[i]->name.find_last_of('.');
			string fileExtension = (dotPos != string::npos) ? current->files[i]->name.substr(dotPos + 1) : "";

			// Check if the file matches the filter extension or if no filter is applied
			if (filterExtension.empty() || caseInsensitiveStringCompare(fileExtension, filterExtension)) {
				cout << "                           " << current->files[i]->name << endl;
			}
		}
	}
	// Helper function to compare strings case-insensitively
	bool caseInsensitiveStringCompare(const string& str1, const string& str2) {
		return std::equal(str1.begin(), str1.end(), str2.begin(), str2.end(), [](char ch1, char ch2) {
			return tolower(ch1) == tolower(ch2);
			});
	}
	void format(const string& formatExtension) {
		if (formatExtension == "*" || formatExtension == "*.*") {
			// If * or *.* is provided, delete all files
			emptyDirectory();
		}
		else if (!formatExtension.empty()) {
			// Delete files with the specified extension (case-sensitive)
			auto it = std::remove_if(files.begin(), files.end(),
				[&](const auto& file) {
					return file->fileType == formatExtension;
				});

			// Erase the files that match the specified extension
			files.erase(it, files.end());
		}
		// If formatExtension is empty, do nothing
	}



};

class Tree {
public:
	Folder* root;
	Folder* current;

	Tree() {
		current = root = new Folder("V");
	}

	void setPath(Folder* f) {
		string path = current->name;
		Folder* parent = new Folder();
		while (parent != nullptr) {
			path = parent->name + "/" + path;
			parent = parent->parent;
		}
		f->path = path;
	}
	void print(string prompt = "\\>") {
		setColor(14);
		// Check if the current command is "tree"
		if (lastCommand == "tree") {
			// Display the directory structure
			displayDirectoryStructure(current, "");
			lastCommand = "";
		}
		else {
			if (current->name == "V") {
				cout << "V:" << prompt;
			}
			else {
				cout << "V:" << current->path.substr(2, current->path.length()) << "\\" << current->name << prompt;
			}
		}
	}
	bool isFolder(string name) {
		for (int i = 0; i < current->subFolders.size(); i++) {
			if (current->subFolders[i]->name == name) {
				return true;
			}
		}
		return false;
	}
	void displayDirectoryStructure(Folder* folder, const string& indent) {
		cout << endl << indent << "+---" << folder->name << "\\";

		for (size_t i = 0; i < folder->subFolders.size(); ++i) {
			displayDirectoryStructure(folder->subFolders[i], indent + "|   ");
		}

		for (size_t i = 0; i < folder->files.size(); ++i) {
			cout << endl << indent + "|   " << folder->files[i]->name;
		}
		cout << endl;
	}
	bool isFile(string name) {
		for (int i = 0; i < current->files.size(); i++) {
			if (current->files[i]->name == name) {
				return true;
			}
		}
		return false;
	}
	void insert(Folder* f) {
		current->subFolders.push_back(f);
	}
	file* findFile(string name) {
		for (int i = 0; i < current->files.size(); i++) {
			if (current->files[i]->name == name) {
				return current->files[i];
			}
		}
		return nullptr;
	}
	Folder* findFolder(string name) {
		for (int i = 0; i < current->subFolders.size(); i++) {
			if (current->subFolders[i]->name == name) {
				return current->subFolders[i];
			}
		}
		return nullptr;
	}
};

class Command {
private:
	string inLowerCase(string s) {
		for (int i = 0; i < s.length(); i++) {
			s[i] = tolower(s[i]);
		}
		return s;
	}
	bool prompt = false;
public:
	Tree tree;
	string User = "Taha";
	Command() {

	}
	void print() {
		setColor(12);
		cout << "\t\t\t\t\t\t" << "Muhammad Taha : (2022-CS-139)" << endl;
		tree.print();
	}
	void run() {
		print();
		bool run = false;
		while (!run) {
			run = executeCommand();
		}
	}
	Folder* findFolder(string name) {
		for (int i = 0; i < tree.current->subFolders.size(); i++) {
			if (tree.current->subFolders[i]->name == name) {
				return tree.current->subFolders[i];
			}
		}
		return nullptr;
	}
	bool executeCommand() {
		string command;
		getline(cin, command);
		string opr = command.substr(0, command.find(' '));
		opr = inLowerCase(opr);
		if (opr == "mkdir") {
			tree.insert(new Folder(command.substr(6, command.length()), tree.current->path + "\\" + tree.current->name, tree.current));
		}
		else if (opr == "create") {
			// Extract file name and extension from the command
			string fileName = command.substr(7, command.length());
			size_t dotPos = fileName.find('.');

			// Check if an extension is provided
			if (dotPos != string::npos) {
				string fileBaseName = fileName.substr(0, dotPos);
				string fileExtension = fileName.substr(dotPos + 1);

				// Create a new file with the specified name and extension
				file* f = new file(fileBaseName, User);
				f->fileType = fileExtension;  // Set the fileType variable with the provided extension
				tree.current->files.push_back(f);

				cout << "File '" << fileName << "' created successfully." << endl;
			}
			else {
				// Create a new file without an extension
				file* f = new file(fileName, User);
				tree.current->files.push_back(f);

				cout << "File '" << fileName << "' created successfully." << endl;
			}
		}
		else if (opr == "del") {
			string fileName = command.substr(4, command.length());
			if (tree.current->removeFile(fileName)) {
				cout << "File '" << fileName << "' deleted successfully." << endl;
			}
			else {
				cout << "File '" << fileName << "' not found." << endl;
			}
		}

		else if (opr == "cd") {
			string folderPath = command.substr(3, command.length());
			if (tree.isFolder(folderPath))
				tree.current = tree.findFolder(folderPath);
			else {
				cout << "The system cannot find the path specified." << endl;
			}
		}
		else if (opr == "cd..") {
			if (tree.current->parent != nullptr)
				tree.current = tree.current->parent;
		}
		else if (opr == "cd\\") {
			if (tree.current->parent != nullptr)
				tree.current = tree.root;
		}
		/*else if (opr == "dir" || opr == "cd.") {
			tree.current->print();
		}*/
		else if (opr == "dir" || opr == "cd.") {
			size_t pos = command.find(' ');
			if (pos != string::npos) {
				string filterExtension = command.substr(pos + 1); // Extract extension after '*'
				tree.current->print(tree.current, filterExtension);
			}
			else {
				tree.current->print(tree.current);
			}
		}
		else if (opr == "tree") {
			lastCommand = "tree";
			tree.print();
		}
		else if (opr == "cls") {
			system("cls");
			print();
		}
		else if (opr == "find") {
			string name = command.substr(5, command.length());
			file* f = tree.findFile(name);

			if (f != nullptr) {
				cout << "File found: " << f->data << endl;
			}
			else {
				cout << "File not found." << endl;
			}
		}
		else if (opr == "attrib") {
			string name = command.substr(7, command.length());
			file* f = tree.findFile(name);

			if (f != nullptr) {
				cout << "File attributes for " << f->data << ":" << endl;
				f->print();
			}
			else {
				cout << "File not found." << endl;
			}
		}
		/*else if (opr == "format") {
			tree.current->emptyDirectory();
		}*/
		else if (opr == "format") {
			size_t pos = command.find(' ');
			if (pos != string::npos) {
				string formatExtension = command.substr(pos + 1); // Extract extension after '*'
				tree.current->format(formatExtension);
				cout << "Operation completed: Files with extension " << formatExtension << " deleted." << endl;
			}
			else {
				// If no extension is provided, empty the directory
				tree.current->format("");
				cout << "Operation completed: Directory formatted." << endl;
			}
		}
		else if (opr == "rmdir") {
			Folder* current = tree.current;
			tree.current = tree.current->parent;
			string folderName = current->name;

			if (tree.current->removeFolder(folderName)) {
				cout << "Folder '" << folderName << "' removed successfully." << endl;
			}
			else {
				cout << "Error: Folder '" << folderName << "' not found or not empty." << endl;
			}
		}

		else if (opr == "prompt") {
			prompt = !prompt;  // Toggle prompt value
		}
		else if (opr == "rename") {
			command = command.substr(opr.length() + 1, command.length());
			string oldName = command.substr(0, command.find(' '));
			string newName = command.substr(command.find(' ') + 1, command.length());
			file* f = tree.current->findFile(oldName);

			if (f != nullptr) {
				f->name = newName;
				cout << "File or folder renamed successfully." << endl;
			}
			else {
				cout << "Error: File or folder '" << oldName << "' not found." << endl;
			}
		}

		else if (opr == "edit") {
			string name = command.substr(5, command.length());
			file* f = tree.findFile(name);

			cout << "Enter new data for the file (press Enter when done):\n";
			string data;
			getline(cin, data);
			f->data = data;

			cout << "Operation completed: File edited." << endl;
		}
		else if (opr == "copy") {
			command = command.substr(opr.length() + 3, command.length());
			int index = command.find(' ');
			string source = command.substr(0, index);
			string destination = command.substr(index + 3, command.length());
			file* fileToCopy;
			Folder* current = tree.root;
			string f;

			// Find and navigate to the source folder
			while (source.length() > 0) {
				f = source.substr(0, source.find('\\'));
				source = source.substr(f.length() + 1, source.length());

				if (source.length() != 0)
					current = current->findFolder(f);

				if (current == nullptr) {
					cout << "The system cannot find the path specified." << endl;
					return true;
				}
			}

			// Find the file to copy
			fileToCopy = current->findFile(f);
			current = tree.root;

			// Find and navigate to the destination folder
			while (destination.length() > 0) {
				f = destination.substr(0, destination.find('\\'));
				destination = destination.substr(f.length() + 1, destination.length());

				current = current->findFolder(f);

				if (current == nullptr) {
					cout << "The system cannot find the path specified." << endl;
					return true;
				}
			}

			// Copy the file to the destination folder
			current->addFile(new file(fileToCopy->name));
		}
		else if (opr == "mov") {
			command = command.substr(opr.length() + 3, command.length());
			int index = command.find(' ');
			string source = command.substr(0, command.find(' '));
			string destination = command.substr(source.length() + 3, command.length());
			file* fileToMove;
			Folder* current = tree.root;
			string f;
			while (source.length() > 0) {
				f = source.substr(0, source.find('\\'));
				source = source.substr(f.length() + 1, source.length());
				if (source.length() != 0)
					current = current->findFolder(f);
				if (current == nullptr) {
					cout << "The system cannot find the path specified." << endl;
					return true;
				}
			}
			fileToMove = current->findFile(f);
			current->removeFile(fileToMove->name);
			current = tree.root;
			while (destination.length() > 0) {
				f = destination.substr(0, destination.find('\\'));
				destination = destination.substr(f.length() + 1, destination.length());
				current = current->findFolder(f);
				if (current == nullptr) {
					cout << "The system cannot find the path specified." << endl;
					return true;
				}
			}
			current->addFile(fileToMove);
		}
		//ver
		else if (opr == "ver") {
			cout << "Microsoft Windows [Version 10.0.19041.746]" << endl;
		}
		else if (opr == "exit") {
			setColor(10); // Set color to green (you can change this value)
			cout << "Thank you for using My DOS. Have a great day!" << endl;
			setColor(7);  // Reset color to default
			return true;
		}

		else if (opr == "convert") {
			// Extract old and new file types from the command
			size_t pos = command.find(' ');
			if (pos != string::npos) {
				string oldType = command.substr(pos + 1, command.find(' ', pos + 1) - pos - 1);
				string newType = command.substr(command.find(' ', pos + 1) + 1);

				// Convert files with the old type to the new type
				bool conversionSuccess = false;
				for (int i = 0; i < tree.current->files.size(); i++) {
					if (tree.current->files[i]->fileType == oldType) {
						tree.current->files[i]->fileType = newType;
						conversionSuccess = true;
					}
				}

				// Display acknowledgment message
				if (conversionSuccess) {
					cout << "File type converted successfully." << endl;
				}
				else {
					cout << "No files with the specified old type found." << endl;
				}
			}
		}
		else if (opr == "findf") {
			command = command.substr(opr.length() + 1, command.length());
			string name = command.substr(0, command.find(' '));
			string text = command.substr(command.find(' ') + 1, command.length());
			file* f = tree.findFile(name);
			if (f != nullptr) {
				if (f->data.find(text) != string::npos) {
					cout << "Text found in file " << name << endl;
				}
				else {
					cout << "Text not found in file " << name << endl;
				}
			}
			else {
				cout << "File not found" << endl;
			}
		}
		else if (opr == "findstr") {
			command = command.substr(opr.length() + 1, command.length());
			string text = command.substr(0, command.find(' '));
			bool found = false;  // Flag to track if any matching files are found

			for (int i = 0; i < tree.current->files.size(); i++) {
				if (tree.current->files[i]->data.find(text) != string::npos) {
					cout << tree.current->files[i]->name << endl;
					found = true;
				}
			}

			if (!found) {
				cout << "No matching files found." << endl;
			}
		}
		else if (opr == "print") {
			command = command.substr(opr.length() + 1, command.length());
			string name = command.substr(0, command.find(' '));
			file* f = tree.findFile(name);
			if (f != nullptr) {
				f->priority += 1;  // Increment priority
				cout << "File " << name << " added to print queue with priority " << f->priority << endl;
			}
			else {
				cout << "File not found" << endl;
			}
		}

		else if (opr == "pprint") {
			command = command.substr(opr.length() + 1, command.length());
			string name = command.substr(0, command.find(' '));
			file* f = tree.findFile(name);
			if (f != nullptr) {
				f->priority += 1;  // Increment priority
				cout << "File " << name << " added to priority print queue with priority " << f->priority << endl;
			}
			else {
				cout << "File not found" << endl;
			}
		}
		else if (opr == "pqueue") {
			cout << "Priority based print queue" << endl;
			for (int i = 0; i < tree.current->files.size(); i++) {
				cout << tree.current->files[i]->name << " " << tree.current->files[i]->priority << " " << tree.current->files[i]->timeToPrint << endl;
			}
		}

		else if (opr == "queue") {
			cout << "Print queue" << endl;
			for (int i = 0; i < tree.current->files.size(); i++) {
				cout << tree.current->files[i]->name << " " << tree.current->files[i]->timeToPrint << endl;
			}
		}
		else if (opr == "save") {
			command = command.substr(opr.length() + 1, command.length());
			string name = command.substr(0, command.find(' '));
			file* f = tree.findFile(name);

			if (f != nullptr) {
				// Display the current content of the file
				cout << "Current content of file " << name << ":\n";
				cout << f->data << endl;

				// Ask the user if they want to make modifications
				cout << "Do you want to make modifications? (Y/N): ";
				char choice;
				cin >> choice;

				cin.ignore(); // Clear the newline character from the input buffer

				if (toupper(choice) == 'Y') {
					// Get the new content from the user
					cout << "Enter the new content (press Ctrl+Z followed by Enter to save):" << endl;
					stringstream buffer;
					buffer << cin.rdbuf();  // Read input until Ctrl+Z is pressed

					// Update the file content
					f->data = buffer.str();
					cout << "File " << name << " saved" << endl;
				}
				else {
					cout << "File not modified" << endl;
				}
			}
			else {
				cout << "File not found" << endl;
			}
		}
		else if (opr == "pwd") {
			cout << tree.current->path << endl;
		}
		else if (opr == "loadtree") {
			command = command.substr(opr.length() + 1, command.length());
			string name = command.substr(0, command.find(' '));

			ifstream rdr(name.c_str());

			if (!rdr.is_open()) {
				cerr << "Error: Unable to open file " << name << endl;
				// Continue with the execution or handle the error as needed
			}
			else {
				loadTree(rdr);
				rdr.close();
				cout << "Success..." << endl;
			}
		}
		else if (opr == "help") {
			cout << "ATTRIB Displays file (name provided as input) attributes." << endl;
			cout << "CD Displays the name of or changes the current directory. See details below." << endl;
			cout << "CD. Prints working directory (i.e. your current directory in your tree structure)" << endl;
			cout << "CD.. Change directory to previous directory" << endl;
			cout << "CD\\ Changes directory to root directory (i.e. V:\\&gt;)" << endl;
			cout << "CONVERT Asks two types and converts extension of all files of one type to another type." << endl;
			cout << "COPY Copies one file in the current directory to another location (directory). See details below." << endl;
			cout << "CREATE Creates a file with the name provided and allows the user to enter contents of the file." << endl;
			cout << "DEL Delete a file whose name is provided in input." << endl;
			cout << "DIR Displays a list of files and subdirectories in a directory." << endl;
			cout << "EDIT Opens a file (loads into a linked list) and allows the user to edit and save the contents of" << endl;
			cout << "the file. See bullet 5 below" << endl;
			cout << "EXIT Quits the program" << endl;
			cout << "FIND Searches for a file in your current virtual directory whose name is provided as input." << endl;
			cout << "FINDF Searches for a text string in the currently open file or the file whose name is provided as" << endl;
			cout << "FINDSTR Searches for strings in all files of your current virtual directory, prints names of files with" << endl;
			cout << "the string" << endl;
			cout << "FORMAT Formats the current virtual directory i.e. empties the current directory and all" << endl;
			cout << "HELP Provides Help information for all these commands." << endl;
			cout << "LOADTREE Load a given tree in your tree structure. Given tree is in a file named tree.txt" << endl;
			cout << "MKDIR Creates a virtual directory." << endl;
			cout << "MOVE Moves one file (whose name is provided as input) from one directory to another" << endl;
			cout << "PPRINT Adds a text file to the priority based print queue, and displays the current priority queue." << endl;
			cout << "PROMPT Changes the Windows command prompt (for example from V:\\&gt; to V$)." << endl;
			cout << "PRINT Adds a text file to the print queue, and displays the current queue." << endl;
			cout << "PQUEUE Shows current state of the priority based print queue, with time left for each element" << endl;
			cout << "PWD Prints working directory" << endl;
			cout << "QUEUE Shows current state of the print queue, with time left for each element" << endl;
			cout << "RENAME Renames a file whose current and new name is provided as input." << endl;
			cout << "RMDIR Removes a directory along with its contents. Consider setting the respective sibling links" << endl;
			cout << "SAVE Saves the currently open file to disk." << endl;
			cout << "TREE Displays the complete directory structure." << endl;
		}
		if (opr != "cls") {
			if (prompt) {
				tree.print("$");
			}
			else {
				tree.print();
			}
		}

		else {
			cout << "Invalid command..." << endl;
		}
		return false;
	}

	void loadTree(ifstream& rdr) {
		if (!rdr.is_open()) {
			cerr << "Error: Unable to open file stream for loading tree." << endl;
			return;
		}

		try {
			string line;
			getline(rdr, line);

			// Assume the first line should contain the root folder information
			tree.root = new Folder(line);
			tree.current = tree.root;

			while (getline(rdr, line)) {
				if (line == "F") {
					getline(rdr, line);
					file* f = new file(line);
					tree.current->files.push_back(f);
				}
				else if (line == "D") {
					getline(rdr, line);
					Folder* f = new Folder(line, "", tree.current);
					tree.current->subFolders.push_back(f);
					tree.current = f;
				}
				else if (line == "P") {
					if (tree.current->parent) {
						tree.current = tree.current->parent;
					}
					else {
						cerr << "Error: Trying to go up, but current folder has no parent." << endl;
						// Handle the error or exit the loop
						break;
					}
				}
			}
		}
		catch (const exception& e) {
			cerr << "Error during tree loading: " << e.what() << endl;
		}
	}
};

void getRowColbyLeftClick(int& rpos, int& cpos) {
	INPUT_RECORD ir;
	DWORD count;
	HANDLE hStdin = GetStdHandle(STD_INPUT_HANDLE);
	SetConsoleMode(hStdin, ENABLE_MOUSE_INPUT);
	while (true) {
		ReadConsoleInput(hStdin, &ir, 1, &count);
		if (ir.EventType == MOUSE_EVENT) {
			if (ir.Event.MouseEvent.dwButtonState == FROM_LEFT_1ST_BUTTON_PRESSED) {
				cpos = ir.Event.MouseEvent.dwMousePosition.X;
				rpos = ir.Event.MouseEvent.dwMousePosition.Y;
				break;
			}
		}
	}
}
void gotoRowCol(int rpos, int cpos) {
	HANDLE hStdout = GetStdHandle(STD_OUTPUT_HANDLE);
	COORD position = { cpos, rpos };
	SetConsoleCursorPosition(hStdout, position);
}

list<string> files;

struct state {
	list<list<char>> text;
	list<list<char>>::iterator rowItr;
	list<char>::iterator colItr;
	int r, c;
};
class currentFile {
public:
	int cursorRow, cursorCol;
	list<list<char>> text;
	list<list<char>>::iterator rowItr;
	list<char>::iterator colItr;
	deque<state> undo;
	stack<state> redo;
	currentFile() {
		text.push_back(list<char>());
		rowItr = text.begin();
		(*rowItr).push_back(' ');
		colItr = (*rowItr).begin();
		cursorRow = 0;
		cursorCol = 0;
	}
	state saveState() {
		state* s = new state();
		for (auto row = text.begin(); row != text.end(); row++) {
			s->text.push_back(list<char>());
			for (auto col = row->begin(); col != row->end(); col++) {
				char c = *col;
				s->text.back().push_back(c);
			}
		}
		s->rowItr = s->text.begin();
		s->colItr = s->rowItr->begin();
		s->r = cursorRow;
		s->c = cursorCol;
		return *s;
	}
	void loadState(state s) {
		text = s.text;
		rowItr = text.begin();
		cursorCol = s.c;
		cursorRow = s.r;
		for (int i = 0; i < cursorRow; i++) {
			rowItr++;
		}
		colItr = (*rowItr).begin();
		for (int i = 0; i < cursorCol; i++) {
			colItr++;
		}
	}
	void option(ifstream& rdr) {
		int choice = -1;
		int row = 0, col = 0;
		gotoRowCol(6, 80);
		cout << "1. New File" << endl;
		gotoRowCol(9, 80);
		cout << "2. Open File" << endl;
		gotoRowCol(12, 80);
		cout << "3. Exit" << endl;
		cout << "Enter your choice: ";
		while (choice == -1)
		{
			cin >> choice;
		}
		switch (choice) {
		case 1:
			createFile();
			break;
		case 2:
			openFile();
			break;
		case 3:
			exit(1);
			break;
		}
	}
	void closing(ifstream& rdr) {
		rdr.close();
		ofstream wtr;
		wtr.open("SaveFile.txt");
		for (auto row = files.begin(); row != files.end(); row++) {
			wtr << (*row) << endl;
		}
		wtr.close();
	}
	void createFile() {
		system("cls");
		string fileName;
		cout << "Enter file name: ";
		cin >> fileName;
		if (find(files.begin(), files.end(), fileName) != files.end()) {
			cout << "File already exists" << endl;
			return;
		}
		files.push_back(fileName);
		ofstream wtr(fileName.c_str());
		system("cls");
		editFile(wtr);
		system("cls");
		system("color 0A");
		cout << "File created successfully" << endl;
		wtr.close();
	}
	void openFile() {
		system("cls");
		string fileName;
		cout << "Enter file name: ";
		cin >> fileName;
		if (find(files.begin(), files.end(), fileName) == files.end()) {
			cout << "File does not exist" << endl;
			return;
		}
		ifstream rdr(fileName.c_str());
		openFile(rdr);
		rdr.close();
		ofstream wtr(fileName.c_str());
		system("cls");
		print();
		gotoRowCol(cursorRow, cursorCol);
		editFile(wtr);
		system("cls");
		system("color 09");
		wtr.close();
	}
	void editFile(ofstream& wtr) {
		system("Color F0");
		char c;
		c = _getch();
		gotoRowCol(cursorRow, cursorCol);
		cout << c;
		(*colItr) = c;
		cursorCol++;
		while (true) {
			if (cursorRow == 0)
				gotoRowCol(cursorRow, cursorCol);
			else
				gotoRowCol(cursorRow, cursorCol + 1);
			c = _getch();
			if (c == -32) {
			A:
				c = _getch();
				if (c == 72) {
					if (cursorRow == 0)
						continue;
					rowItr--;
					colItr = (*rowItr).begin();
					cursorRow--;cursorCol = 0;
				}
				else if (c == 80) //down
				{
					/*if (cursorRow == text.size() - 1) {
						continue;
					}*/

					rowItr++;
					colItr = (*rowItr).begin();
					cursorRow++;cursorCol = 0;
				}
				else if (c == 75) //left
				{
					if (cursorCol - 1 > 0) {
						colItr--;
						cursorCol--;
					}
				}
				else if (c == 77) //right
				{
					if (cursorCol == 159)
						continue;
					colItr++;
					cursorCol++;
				}
				else if (c == 83) //delete
				{
					auto temp = colItr;
					colItr++;
					(*rowItr).erase(colItr);
					colItr = temp;
					system("cls");
					print();
					updateUndo();
				}
				if (cursorCol == 0)
				{
					gotoRowCol(cursorRow, 0);
					c = _getch();
					if (c == -32)
						goto A;
					(*rowItr).push_front(c);
					colItr = (*rowItr).begin();
					cursorCol = 1;
					system("cls");
					print();
					updateUndo();
				}
				continue;
			}
			else if (cursorCol == 13) {
				auto temp = rowItr;
				rowItr++;
				text.insert(rowItr, list<char>());
				rowItr = ++temp;
				cursorRow++;cursorCol = 0;
				gotoRowCol(cursorRow, cursorCol);
				c = _getch();
				if (c == -32)
					goto A;
				(*rowItr).push_back(c);
				colItr = (*rowItr).begin();
				system("cls");
				print();
				updateUndo();
				continue;
			}
			else if (c == 8) {
				if (cursorCol == 0) {
					gotoRowCol(cursorRow, 0);
					c = _getch();
					if (c == -32)
						goto A;
					if (c == 8)
						continue;
					(*colItr) = c;
					colItr = (*rowItr).begin();
					cursorCol = 1;
					continue;
				}
				auto temp = --colItr;
				colItr++;
				(*rowItr).erase(colItr);
				colItr = temp;
				cursorCol--;
				system("cls");
				print();
				updateUndo();
				continue;
			}
			else if (c == 26) {
				if (!undo.empty()) {
					state s = undo.back();
					loadState(s);
					redo.push(undo.back());
					undo.pop_back();
					system("cls");
					print();
				}
				continue;
			}
			else if (c == 25) {
				if (!redo.empty()) {
					undo.push_back(redo.top());
					state s = redo.top();
					redo.pop();
					loadState(s);
					system("cls");
					print();
				}
				continue;
			}
			else if (c == 27) {
				writeToFile(wtr);
				break;
			}
			if (cursorCol == 100) {
				text.push_back(list<char>());
				rowItr++;
				cursorCol = 0;
				cursorRow++;
				(*rowItr).push_back(c);
				colItr = (*rowItr).begin();
			}
			else {
				auto temp = colItr;
				colItr++;
				(*rowItr).insert(colItr, c);
				colItr = ++temp;
				cursorCol++;
			}
			system("cls");
			print();
			updateUndo();
		}
		system("Color 09");
	}
	void writeToFile(ofstream& wtr) {
		for (auto row = text.begin(); row != text.end(); row++) {
			for (auto col = (*row).begin(); col != (*row).end(); col++) {
				wtr << (*col);
			}
			wtr << "\n";
		}
	}
	void print() {
		for (auto row = text.begin(); row != text.end(); row++) {
			for (auto col = (*row).begin(); col != (*row).end(); col++) {
				cout << (*col);
			}
			cout << "\n";
		}
	}
	void updateUndo() {
		if (undo.size() > 5) {
			undo.erase(undo.begin());
			state s = saveState();
			undo.push_back(s);
		}
	}

	void openFile(ifstream& rdr) {
		system("cls");
		char c;
		while (!rdr.eof()) {
			rdr.get(c);
			if (c != '\n') {
				(*rowItr).push_back(c);
			}
			else {
				text.push_back(list<char>());
				rowItr++;
			}
		}
		rowItr = text.begin();
		colItr = (*rowItr).begin();
		cursorRow = 0;
		cursorCol = 0;
		gotoRowCol(cursorRow, cursorCol);
	}
};
void input(ifstream& rdr) {
	string fn;
	rdr >> fn;
	while (!rdr.eof())
	{
		files.push_back(fn);
		rdr >> fn;
	}
}
int main() {
	//write code to test dos shell
	/*Tree tree;
	Command cmd;
	cmd.run();*/
	system("color 09");
	int r = 0, c = 0;
	currentFile N;
	ifstream rdr;
	ofstream wtr;
	rdr.open("SaveFile.txt");
	wtr.open("RecentFile.txt");
	input(rdr);
	while (true)
	{
		N.option(rdr);
	}
	rdr.close();
	wtr.close();
	return 0;
}