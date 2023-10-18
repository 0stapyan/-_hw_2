#include <iostream>
#include <string>
#include <fstream>

class Text{
public:
    Text();
    void PrintText() const;
    void InsertText(int line, int symbolIndex, const std::string& text);
    void EditText(const std::string& newText);
    std::string GetText() const;

private:
    std::string text;
    std::string currentFileName;
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
    void SearchForText(const std::string& searchText);
    void ClearConsole();

private:
    Text& editor;
    FileManager& fileCommands;
};

Text::Text() {
    text = "";
    currentFileName = "";
}

void Text::PrintText() const {
    std::cout << "Current Text: " <<std::endl;
    std::cout << text << std::endl;
}

void Text::InsertText(int line, int symbolIndex, const std::string& textToInsert){
    if (line >= 0 && line <= text.length() && symbolIndex >= 0){
        text.insert(line+symbolIndex,textToInsert);
    }
    else{
        std::cout << "Invalid insertion position." << std::endl;
    }
}

void Text::EditText(const std::string& newText) {
    text = newText;
}

std::string Text::GetText() const {
    return text;
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
    size_t found = editor.GetText().find(searchText);
    if (found != std::string::npos) {
        std::cout << "Text found at position " << found << std::endl;
    } else {
        std::cout << "Text not found." << std::endl;
    }
}

void Editor::ClearConsole() {
    system("clear");
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

            editor.InsertText(line, symbolIndex, textToInsert);
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
    }
    return 0;
}

