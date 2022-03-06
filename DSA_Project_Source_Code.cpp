#include <bits/stdc++.h>
using namespace std;
class TrieNode
{
public:
    char data;
    TrieNode **children;
    bool isTerminal;

    TrieNode(char data)
    {
        this->data = data;
        children = new TrieNode *[26];
        for (int i = 0; i < 26; i++)
        {
            children[i] = NULL;
        }
        isTerminal = false;

    }
};
class Trie
{
    TrieNode *root;
    void insertWordhelper(TrieNode *root, string word)
    {
        // Base case
        if (word.size() == 0)
        {
            root->isTerminal = true;
            return;
        }

        // Small Calculation
        int index = word[0] - 'a';
        TrieNode *child;
        if (root->children[index] != NULL)
        {
            child = root->children[index];
        }
        else
        {
            child = new TrieNode(word[0]);
            root->children[index] = child;
        }

        // Recursive call
        insertWordhelper(child, word.substr(1));
    }
    bool searchhelper(string word, TrieNode *root)
    {
        if (word.empty() && root->isTerminal == false)
            return false;
        else if (word.empty() && root->isTerminal == true)
            return true;
        int index = word[0] - 'a';
        if (root->children[index] == NULL)
            return false;
        else
        {
            return searchhelper(word.substr(1), root->children[index]);
        }
    }
    void removeWordhelper(TrieNode *root, string word)
    {
        // Base case
        if (word.size() == 0)
        {
            root->isTerminal = false;
            return;
        }

        // Small calculation
        TrieNode *child;
        int index = word[0] - 'a';
        if (root->children[index] != NULL)
        {
            child = root->children[index];
        }
        else
        {
            // Word not found
            return;
        }

        removeWordhelper(child, word.substr(1));

        // Remove child Node if it is useless
        if (child->isTerminal == false)
        {
            for (int i = 0; i < 26; i++)
            {
                if (child->children[i] != NULL)
                {
                    return;
                }
            }
            delete child;
            root->children[index] = NULL;
        }
    }
    bool searchhelperprefix(string word, TrieNode *root)
    {
        if (word.empty())
            return true;
        int index = word[0] - 'a';
        if (root->children[index] == NULL)
            return false;
        else
        {
            return searchhelperprefix(word.substr(1), root->children[index]);
        }
    }
    // AUTO COMPLETE
    bool isLeafNode(TrieNode *root)
    {
        for (int i = 0; i < 26; i++)
            if (root->children[i])
                return false;
        return true;
    }
    TrieNode *findWord(TrieNode *root, string word)
    {
        if (root == NULL)
            return NULL;
        else if (word.empty())
            return root;
        int index = word[0] - 'a';
        if (root->children[index] == NULL)
            return NULL;
        else
        {
            return findWord(root->children[index], word.substr(1));
        }
    }

    void printWords(TrieNode *root, string word1, vector<string> &suggestedWords)
    {
        if (root == NULL)
        {
            return;
        }

        if (root->isTerminal)
            suggestedWords.push_back(word1);

        for (int i = 0; i < 26; i++)
        {
            if (root->children[i] != NULL)
                printWords(root->children[i], word1 + root->children[i]->data, suggestedWords);
        }
    }

public:
    Trie()
    {
        root = new TrieNode('\0');
    }

    // For user
    void insertWord(string word)
    {
        insertWordhelper(root, word);
    }

    bool search(string word)
    {
        return searchhelper(word, root);
    }

    void removeWord(string word)
    {
        removeWordhelper(root, word);
    }

    bool startsWith(string prefix)
    {
        return searchhelperprefix(prefix, root);
    }
    void autoComplete(string pattern, vector<string> &suggestedWords)
    {
        string output = pattern;
        TrieNode *rootnew = findWord(root, pattern);
        printWords(rootnew, pattern, suggestedWords);
    }
} t;

void insertWord(unordered_map<string, string> &myDictionary, string word, string meaning)
{
    transform(word.begin(), word.end(), word.begin(), ::tolower);
    t.insertWord(word);
    myDictionary[word] = meaning;
    // Adding to CSV file
    ofstream f;
    f.open("dict.csv", ios::app);
    string newline;
    newline = word + "," + meaning;
    f << newline << endl;
    f.close();
}
void insertFromCSV(unordered_map<string, string> &myDictionary)
{
    ifstream data("dict.csv");
    string line, newline;
    vector<string> row;
    while (getline(data, line))
    {
        stringstream lineStream(line);
        string cell;
        string newline;
        row.clear();
        while (getline(lineStream, cell, ','))
        {
            row.push_back(cell);
        }
        string word = row[0];
        string meaning = row[1];

        // cout << word;
        // cout << meaning;
        // insertWord(myDictionary, word, meaning);
        transform(word.begin(), word.end(), word.begin(), ::tolower);
        t.insertWord(word);
        myDictionary[word] = meaning;
    }
    data.close();
}
void AutoComplete(unordered_map<string, string> &myDictionary, string pattern)
{
    transform(pattern.begin(), pattern.end(), pattern.begin(), ::tolower);
    vector<string> listWords;
    t.autoComplete(pattern, listWords);
    if (!listWords.empty())
    {
        cout << "Suggested Words are : " << endl;
        for (int i = 0; i < listWords.size(); i++)
        {
            cout << i + 1 << ". " << listWords[i] << endl;
        }
    }
    else
    {
        cout << "No Word Found related to the entered string" << endl;
        return;
    }
    char opt = 'n';
    cout << "Do you want to search meaning for any of these words? (Enter 'y' for yes and 'n' for no) " << endl;
    cin >> opt;
    if (opt == 'y')
    {
        int index;
        cout << "Enter index of word from above list" << endl;
        cin >> index;
        index--; // we started list from 1 but indexing is from 0
        cout << listWords[index] << " : " << myDictionary[listWords[index]] << endl;
    }
}
void searchDictionary(unordered_map<string, string> &myDictionary, string word)
{
    transform(word.begin(), word.end(), word.begin(), ::tolower);
    vector<string> listWords;
    if (t.search(word))
    {
        cout << "Meaning : " << myDictionary[word] << endl;
    }
    else
    {
        cout << "Word not found in dictionary" << endl;
        AutoComplete(myDictionary, word);
    }
}

void removeWord(unordered_map<string, string> &myDictionary, string word)
{
    transform(word.begin(), word.end(), word.begin(), ::tolower);
    if (t.search(word))
    {
        t.removeWord(word);
        myDictionary.erase(word);
        cout << word << " removed from dictionary" << endl;

        ifstream data("dict.csv");
        string line, newline;
        ofstream f;
        vector<string> row;
        f.open("temp.csv");
        while (getline(data, line))
        {
            stringstream lineStream(line);
            string cell;
            row.clear();
            while (getline(lineStream, cell, ','))
            {
                row.push_back(cell);
            }
            if (row[0] == word)
            {
                continue;
            }
            f << line << endl;
        }
        f.close();
        data.close();
        remove("dict.csv");
        rename("temp.csv", "dict.csv");
    }
    else
    {
        cout << word << " not found in dictionary" << endl;
    }
}
int main()
{
    unordered_map<string, string> myDictionary;
    insertFromCSV(myDictionary);
    char opt;
    do
    {
        int choice;
        cout << "1. Insert Word in dictionary" << endl;
        cout << "2. Search Word in Dictionary" << endl;
        cout << "3. Search pattern for autoComplete" << endl;
        cout << "4. Remove Word from dictioanry" << endl;
        cout << "Enter Choice (S.No) " << endl;
        cin >> choice;
        string word;
        string meaning;
        string pattern;
        switch (choice)
        {
        case 1:
            cout << "Enter Word and Meaning respectively" << endl;
            cin >> word;
            getline(cin,meaning);
            insertWord(myDictionary, word, meaning);
            break;
        case 2:
            cout << "Enter Word to search" << endl;
            cin >> word;
            searchDictionary(myDictionary, word);
            break;
        case 3:
            cout << "Enter pattern for autocomplete" << endl;
            cin >> pattern;
            AutoComplete(myDictionary, pattern);
            break;
        case 4:
            cout << "Enter Word you want to remove" << endl;
            cin >> word;
            removeWord(myDictionary, word);
            break;
        default:
            cout << "No operation corresponding to the entered choice !" << endl;
        }

        cout << "Do you want to continue (Enter 'y' for yes and 'n' for no) " << endl;
        cin >> opt;
    } while (opt == 'y');
}
