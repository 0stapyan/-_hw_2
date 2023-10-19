#include <iostream>
#include <string>
#include <fstream>
#include <stack>

class Text{
public:
    Text();
    void PrintText() const;
    void EditText(const std::string& newText);
    std::string GetText() const;
    void Undo();
    void Redo();
    void Cut(int line, int symbolIndex, int numSymbols);
    void Copy(int line, int symbolIndex, int numSymbols);
    void Paste(int line, int symbolIndex);


private:
    std::string text;
    std::string currentFileName;
    std::stack<std::string> history;
    std::stack<std::string> redoHistory;
    std::string clipboard;
};

class FileManager{
public:
    FileManager(Text& editor);
    void SaveTextToFile(const std::string& filename);
    void LoadTextFromFile(const std::string& filename);

private:
    Text& editor;
};

class Editor{
public:
    Editor(Text& editor, FileManager& fileCommands);
    void AddText(const std::string& textToAdd);
    void AddNewLine();
    void InsertText(int line, int symbolIndex, const std::string& text);
    void SearchForText(const std::string& searchText);
    void ClearConsole();
    void DeleteText(int line, int symbolIndex, int numSymbols);
    void ReplaceText(int line, int symbolIndex, const std::string &newText);
    void Undo();
    void Redo();
    void Cut(int line, int symbolIndex, int numSymbols);
    void Copy(int line, int symbolIndex, int numSymbols);
    void Paste(int line, int symbolIndex);

private:
    Text& editor;
    FileManager& fileCommands;
};

Text::Text() {
    text = "";
    currentFileName = "";
    history.push(text);
    clipboard = "";
}

void Text::PrintText() const {
    std::cout << "Current Text: " <<std::endl;
    std::cout << text << std::endl;
}

void Editor::InsertText(int line, int symbolIndex, const std::string& textToInsert) {
    std::string currentText = editor.GetText();

    int lineNum = 0;
    int linePos = 0;

    for (char c : currentText) {
        if (lineNum == line && linePos == symbolIndex) {
            currentText.insert(linePos + lineNum, textToInsert);
            editor.EditText(currentText);
            std::cout << "Text inserted at Line " << lineNum << ", Position " << linePos << std::endl;
            return;
        }

        if (c == '\n') {
            ++lineNum;
            linePos = 0;
        } else {
            ++linePos;
        }
    }

    std::cout << "Invalid insertion position. Text was not inserted." << std::endl;
}

void Text::EditText(const std::string& newText) {

    history.push(text);
    redoHistory = std::stack<std::string>();
    text = newText;
}

std::string Text::GetText() const {
    return text;
}

void Text::Undo() {
    if (history.size() > 1) {
        redoHistory.push(text);
        history.pop();
        text = history.top();
    }
}

void Text::Redo() {
    if (!redoHistory.empty()) {
        history.push(text);
        text = redoHistory.top();
        redoHistory.pop();
    }
}

void Text::Cut(int line, int symbolIndex, int numSymbols) {
    if (line >= 0 && line < text.length() && symbolIndex >= 0) {
        size_t position = line + symbolIndex;
        if (position + numSymbols <= text.length()) {
            clipboard = text.substr(position, numSymbols);
            text.erase(position, numSymbols);
            history.push(text);
            redoHistory = std::stack<std::string>();
        }
    }
}

void Text::Copy(int line, int symbolIndex, int numSymbols) {
    if (line >= 0 && line < text.length() && symbolIndex >= 0) {
        size_t position = line + symbolIndex;
        if (position + numSymbols <= text.length()) {
            clipboard = text.substr(position, numSymbols);
        }
    }
}

void Text::Paste(int line, int symbolIndex) {
    if (!clipboard.empty()) {
        size_t position = line + symbolIndex;
        text.insert(position, clipboard);
        history.push(text);
        redoHistory = std::stack<std::string>();
    }
}

FileManager::FileManager(Text &editor): editor(editor) {}

void FileManager::SaveTextToFile(const std::string &filename) {
    std::ofstream outputFile(filename);
    if (outputFile.is_open()){
        outputFile << editor.GetText();
        outputFile.close();
        std::cout << "Text successfully saved to " << filename << std::endl;
    }
    else{
        std::cerr << "Failed to open file for saving." <<std::endl;
    }
}

void FileManager::LoadTextFromFile(const std::string &filename) {
    std::ifstream inputFile(filename);
    if (inputFile.is_open()){
        std::string loadedText((std::istreambuf_iterator<char>(inputFile)), std::istreambuf_iterator<char>());
        editor.EditText(loadedText);
        inputFile.close();
        std::cout << "Text successfully loaded from " << filename << std::endl;
    }
    else{
        std::cerr << "Failed to open file for loading " << std::endl;
    }
}

Editor::Editor(Text& editor, FileManager& fileCommands) : editor(editor), fileCommands(fileCommands) {}

void Editor::AddText(const std::string& textToAdd) {
    editor.EditText(editor.GetText() + textToAdd);
}

void Editor::AddNewLine() {
    editor.EditText(editor.GetText() + "\n");
}

void Editor::SearchForText(const std::string& searchText) {
    std::string text = editor.GetText();

    int lineNum = 0;
    int linePos = 0;

    for (char c : text) {
        if (c == '\n') {
            ++lineNum;
            linePos = 0;
        } else {
            if (linePos == 0) {
                std::cout << "Line " << lineNum << ": ";
            }

            if (c == searchText[0]) {
                size_t foundPos = text.find(searchText, linePos);
                if (foundPos != std::string::npos) {
                    std::cout << "Found at line " << lineNum << ", position " << foundPos - linePos << std::endl;
                }
            }

            ++linePos;
        }
    }
}


void Editor::ClearConsole() {
    system("clear");
}

void Editor::DeleteText(int line, int symbolIndex, int numSymbols){
    std::string currentText = editor.GetText();

    if (line >= 0 && line < currentText.length() && symbolIndex >= 0){

        if (numSymbols > 0){
            size_t position = line + symbolIndex;

            if (position + numSymbols <= currentText.length()){
                currentText.erase(position, numSymbols);
                editor.EditText(currentText);
                std::cout << "Text deleted" << std::endl;
            }
            else{
                std::cout << "Too big deletion range" << std::endl;
            }
        }
        else{
            std::cout << "Number of symbols have to be a positive number" << std::endl;
        }
    }
    else{
        std:: cout << "Wrong deletion position" << std::endl;
    }
}

void Editor::ReplaceText(int line, int symbolIndex, const std::string &newText){
    std::string currentText = editor.GetText();

    int lineNum = 0;
    int linePos = 0;
    size_t position = 0;

    for (char c : currentText){
        if (lineNum == line && linePos == symbolIndex){
            position = linePos + lineNum;
            break;
        }

        if (c == '\n'){
            ++lineNum;
            linePos = 0;
        }
    }
    if (position < currentText.length()){
        currentText.replace(position, newText.length(), newText);
        editor.EditText(currentText);
        std::cout << "Text replaced successfully" << std::endl;
    }
    else{
        std::cout << "Wrong replacement position" << std:: endl;
    }
}

void Editor::Undo() {
    editor.Undo();
}

void Editor::Redo() {
    editor.Redo();
}

void Editor::Cut(int line, int symbolIndex, int numSymbols) {
    editor.Cut(line, symbolIndex, numSymbols);
}

void Editor::Copy(int line, int symbolIndex, int numSymbols) {
    editor.Copy(line, symbolIndex, numSymbols);
}

void Editor::Paste(int line, int symbolIndex) {
    editor.Paste(line, symbolIndex);
}

int main(){
    Text editor;
    FileManager fileManager(editor);
    Editor commandHandler(editor, fileManager);

    std::string userInput;

    while(true){
        std::cout << "Choose the command:" << std::endl;
        std::cout << "1. Enter text to append" << std::endl;
        std::cout << "2. Start a new line" << std::endl;
        std::cout << "3. Save text to a file" << std::endl;
        std::cout << "4. Load text from a file" << std::endl;
        std::cout << "5. Print current text" << std::endl;
        std::cout << "6. Insert text by line and symbol index" << std::endl;
        std::cout << "7. Search for text" << std::endl;
        std::cout << "8. Clear the text" << std::endl;
        std::cout << "9. Delete text" << std::endl;
        std::cout << "10. Undo" << std::endl;
        std::cout << "11. Redo" << std::endl;
        std::cout << "12. Cut" << std::endl;
        std::cout << "13. Copy" << std::endl;
        std::cout << "14. Paste" << std::endl;
        std::cout << "15. Replace text" << std::endl;

        std::cin >> userInput;

        if (userInput == "1"){
            std::string textToAdd;
            std::cout << "Enter text to add: ";
            std::cin.ignore(); // Clear the input buffer
            std::getline(std::cin, textToAdd);
            commandHandler.AddText(textToAdd);
        }

        else if (userInput == "2"){
            commandHandler.AddNewLine();
        }

        else if (userInput == "3"){
            std::string filename;
            std::cout << "Enter the filename to save current text: ";
            std::cin >> filename;
            fileManager.SaveTextToFile(filename);
        }

        else if (userInput == "4"){
            std::string filename;
            std::cout << "Enter the filename to load text from: ";
            std:: cin >> filename;
            fileManager.LoadTextFromFile(filename);
        }

        else if (userInput == "5"){
            editor.PrintText();
        }

        else if (userInput == "6"){
            int line, symbolIndex;
            std:: string textToInsert;

            std::cout << "Enter line index: ";
            std::cin >> line;
            std::cout << "Enter symbol index: ";
            std::cin >> symbolIndex;
            std::cout << "Enter text to insert: ";
            std::cin.ignore();
            std::getline(std::cin, textToInsert);

            commandHandler.InsertText(line, symbolIndex, textToInsert);
        }

        else if (userInput == "7"){
            std::string searchText;
            std::cout << "Enter text to search for: ";
            std:: cin.ignore(); // Clear the input buffer
            std:: getline(std::cin, searchText);
            commandHandler.SearchForText(searchText);
        }

        else if (userInput == "8"){
            commandHandler.ClearConsole();
        }

        else if (userInput == "9"){

            int line, symbolIndex, numSymbols;

            std::cout << "Please enter the line, index and number of symbols you want to delete";
            std::cin >> line >> symbolIndex >> numSymbols;

            commandHandler.DeleteText(line, symbolIndex, numSymbols);
        }

        else if (userInput == "10") {
            commandHandler.Undo();
        }

        else if (userInput == "11") {
            commandHandler.Redo();
        }

        else if (userInput == "12") {
            int line, symbolIndex, numSymbols;

            std::cout << "Please enter the line, index and number of symbols to cut: ";
            std::cin >> line >> symbolIndex >> numSymbols;
            commandHandler.Cut(line, symbolIndex, numSymbols);
        }

        else if (userInput == "13") {
            int line, symbolIndex, numSymbols;

            std::cout << "Please enter the line, index and number of symbols to copy: ";
            std::cin >> line >> symbolIndex >> numSymbols;
            commandHandler.Copy(line, symbolIndex, numSymbols);
        }

        else if (userInput == "14") {
            int line, symbolIndex;
            std::cout << "Choose line to paste: ";
            std::cin >> line;
            std::cout << "Choose index to paste: ";
            std::cin >> symbolIndex;
            commandHandler.Paste(line, symbolIndex);
        }

        else if (userInput == "15") {

            int line, symbolIndex;
            std::string newText;

            std::cout << "Choose line: ";
            std::cin >> line;

            std::cout << "Choose index: ";
            std::cin >> symbolIndex;

            std::cout << "Write text to replace: ";
            std::cin.ignore();
            std::getline(std::cin, newText);

            commandHandler.ReplaceText(line, symbolIndex, newText);
        }
    }
    return 0;
}

