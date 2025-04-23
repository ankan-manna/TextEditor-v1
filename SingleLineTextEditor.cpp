#include <iostream>
#include <stack>
#include <string>

using namespace std;

struct Node {
    char data;
    Node* prev;
    Node* next;

    Node(char data) : data(data), prev(nullptr), next(nullptr) {}
};

class SingleLineTextEditor {
private:
    Node* head;
    Node* cursor;
    stack<string> undoStack;
    stack<string> redoStack;

public:
    SingleLineTextEditor() {
        cursor = new Node('|');
        head = cursor;
    }

    string text2String() {
        string result;
        Node* curr = head;
        while (curr != nullptr) {
            result += curr->data;
            curr = curr->next;
        }
        return result;
    }

    void string2Text(const string& str) {
        head = nullptr;
        cursor = nullptr;
        Node* prevNode = nullptr;
        bool cursorPresent = false;

        for (char c : str) {
            Node* newNode = new Node(c);
            if (!head) {
                head = newNode;
            } else {
                prevNode->next = newNode;
                newNode->prev = prevNode;
            }
            prevNode = newNode;
            if (c == '|') {
                cursor = newNode;
                cursorPresent = true;
            }
        }

        if (!cursorPresent) {
            Node* cursorNode = new Node('|');
            if (prevNode) {
                prevNode->next = cursorNode;
                cursorNode->prev = prevNode;
            } else {
                head = cursorNode;
            }
            cursor = cursorNode;
        }
    }

    void saveState() {
        undoStack.push(text2String());
        while (!redoStack.empty()) redoStack.pop();
    }

    void undo() {
        if (!undoStack.empty()) {
            redoStack.push(text2String());
            string2Text(undoStack.top());
            undoStack.pop();
        }
        displayText();
    }

    void redo() {
        if (!redoStack.empty()) {
            undoStack.push(text2String());
            string2Text(redoStack.top());
            redoStack.pop();
        }
        displayText();
    }

    void insert(const string& text) {
        saveState();
        for (char c : text) {
            Node* newNode = new Node(c);
            newNode->prev = cursor->prev;
            newNode->next = cursor;
            if (newNode->prev) {
                newNode->prev->next = newNode;
            } else {
                head = newNode;
            }
            cursor->prev = newNode;
        }
        displayText();
    }

    void deleteChar() {
        saveState();
        if (cursor->prev) {
            Node* toDelete = cursor->prev;
            cursor->prev = toDelete->prev;
            if (toDelete->prev) {
                toDelete->prev->next = cursor;
            } else {
                head = cursor;
            }
            delete toDelete;
        }
        displayText();
    }

    void moveLeft() {
        if (cursor->prev) {
            swap(cursor->data, cursor->prev->data);
            cursor = cursor->prev;
        }
        displayText();
    }

    void moveRight() {
        if (cursor->next) {
            swap(cursor->data, cursor->next->data);
            cursor = cursor->next;
        }
        displayText();
    }

    void displayText() {
        cout << "------------------------ Text Editor Start ------------------------" << endl;
        Node* curr = head;
        while (curr != nullptr) {
            cout << curr->data;
            curr = curr->next;
        }
        cout << endl;
        cout << "------------------------- Text Editor End -------------------------" << endl;
    }
};

int main() {
    SingleLineTextEditor editor;
    char command;

    while (true) {
        cout << "Enter command (I: Insert, D: Delete, L: Left, R: Right, U: Undo, Y: Redo): ";
        cin >> command;
        cin.ignore();

        switch (command) {
            case 'I': {
                cout << "Enter text to insert: ";
                string text;
                getline(cin, text);
                editor.insert(text);
                break;
            }
            case 'D':
                editor.deleteChar();
                break;
            case 'L':
                editor.moveLeft();
                break;
            case 'R':
                editor.moveRight();
                break;
            case 'U':
                editor.undo();
                break;
            case 'Y':
                editor.redo();
                break;
            default:
                cout << "Invalid command!" << endl;
        }
    }
    return 0;
}
