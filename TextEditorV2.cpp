#include <iostream>
#include <vector>
#include <string>
#include <stack>
#include <list>

using namespace std;

class MultiLineTextEditor {
private:
    list<string> lines;
    int cursorRow;
    int cursorCol;
    stack<list<string>> undoStack;
    stack<list<string>> redoStack;

    void saveState() {
        undoStack.push(lines);
        while (!redoStack.empty()) redoStack.pop();
    }

    list<string>::iterator getIterator(int row) {
        auto it = lines.begin();
        advance(it, row);
        return it;
    }

public:
    MultiLineTextEditor() : cursorRow(0), cursorCol(0) {
        lines.emplace_back("");
    }

    void insert(const string& text) {
        saveState();
        auto it = getIterator(cursorRow);
        it->insert(cursorCol, text);
        cursorCol += text.length();
        displayText();
    }

    void insertMultipleLines(const vector<string>& texts) {
        saveState();
        auto it = getIterator(cursorRow);
        string remainingText = it->substr(cursorCol);
        it->erase(cursorCol);

        for (const auto& line : texts) {
            ++cursorRow;
            it = next(it);
            lines.insert(it, line);
        }

        ++cursorRow;
        it = next(it);
        lines.insert(it, remainingText);
        cursorCol = 0;
        displayText();
    }

    void deleteChar() {
        auto it = getIterator(cursorRow);
        if (cursorCol < it->length()) {
            saveState();
            it->erase(cursorCol, 1);
            displayText();
        } else {
            cout << "Nothing to delete at this position!" << endl;
        }
    }


    void moveLeft() {
        if (cursorCol > 0) {
            cursorCol--;
        } else if (cursorRow > 0) {
            cursorRow--;
            cursorCol = getIterator(cursorRow)->length();
        }
        displayText();
    }

    void moveRight() {
        if (cursorCol < getIterator(cursorRow)->length()) {
            cursorCol++;
        } else if (cursorRow < lines.size() - 1) {
            cursorRow++;
            cursorCol = 0;
        }
        displayText();
    }

    void moveUp() {
        if (cursorRow > 0) {
            cursorRow--;
            cursorCol = min(cursorCol, (int)getIterator(cursorRow)->length());
        }
        displayText();
    }

    void moveDown() {
        if (cursorRow < lines.size() - 1) {
            cursorRow++;
            cursorCol = min(cursorCol, (int)getIterator(cursorRow)->length());
        }
        displayText();
    }

    void newLine() {
        saveState();
        auto it = getIterator(cursorRow);
        string remainingText = it->substr(cursorCol);
        it->erase(cursorCol);

        ++cursorRow;
        lines.insert(next(it), remainingText);
        cursorCol = 0;
        displayText();
    }

    void undo() {
        if (!undoStack.empty()) {
            redoStack.push(lines);
            lines = undoStack.top();
            undoStack.pop();
            cursorRow = min(cursorRow, (int)lines.size() - 1);
            cursorCol = min(cursorCol, (int)getIterator(cursorRow)->length());
        }
        displayText();
    }

    void redo() {
        if (!redoStack.empty()) {
            undoStack.push(lines);
            lines = redoStack.top();
            redoStack.pop();
            cursorRow = min(cursorRow, (int)lines.size() - 1);
            cursorCol = min(cursorCol, (int)getIterator(cursorRow)->length());
        }
        displayText();
    }

    void displayText() {
        cout << "------ Text Editor ------" << endl;
        int row = 0;
        for (auto& line : lines) {
            if (row == cursorRow) {
                cout << line.substr(0, cursorCol) << "|" << line.substr(cursorCol) << endl;
            } else {
                cout << line << endl;
            }
            row++;
        }
        cout << "---------------------------" << endl;
    }
};

int main() {
    MultiLineTextEditor editor;
    string command;

    while (true) {
        cout << "Enter command (I: Insert, M: Multiline Insert, D: Delete, L: Left, R: Right, U: Undo, Y: Redo, N: New Line, W: Up, S: Down): ";
        getline(cin, command);

        if (command == "I") {
            cout << "Enter text to insert: ";
            string text;
            getline(cin, text);
            editor.insert(text);
        } else if (command == "M") {
            cout << "Enter multiple lines (Enter empty line to finish): " << endl;
            vector<string> texts;
            while (true) {
                string line;
                getline(cin, line);
                if (line.empty()) break;
                texts.push_back(line);
            }
            editor.insertMultipleLines(texts);
        } else if (command == "D") {
            cout << "Press 'D' to confirm deletion or other key to cancel: ";
            string confirm;
            getline(cin, confirm);
            if (confirm == "D") editor.deleteChar();
        } else if (command == "L") {
            editor.moveLeft();
        } else if (command == "R") {
            editor.moveRight();
        } else if (command == "W") {
            editor.moveUp();
        } else if (command == "S") {
            editor.moveDown();
        } else if (command == "N") {
            editor.newLine();
        } else if (command == "U") {
            editor.undo();
        } else if (command == "Y") {
            editor.redo();
        } else {
            cout << "Invalid command!" << endl;
        }
    }

    return 0;
}