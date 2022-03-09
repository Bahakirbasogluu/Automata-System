#include <iostream>
#include <vector>
#include <string>
#include <fstream>
using namespace std;

int counter=0;
char temp2 = '9';
bool deleteCheck;
bool valCheck ;
bool correctCheck;
bool enoughCheeck;
string dothrakiKey;
string dothrakiVal;
const int ALPHABET_SIZE = 26;

// This is my trie node
struct TrieNode {
    struct TrieNode* children[ALPHABET_SIZE];

    // isEndOfWord is true if the node has val;
    bool isEndOfWord;
    string dothVal;
};

// Returns a new trie node (initialized to NULL's)
struct TrieNode* getNode()
{
    auto* pNode = new TrieNode;

    pNode->isEndOfWord = false;

    for (auto & i : pNode->children)
        i = nullptr;

    return pNode;
}

// Returns true if key has a meaning in trie, else returns false
bool search(struct TrieNode* root, const string& key, const string& type, const string& dothValue,  ofstream& out)
{
    struct TrieNode* pCrawl = root;

    for (char i : key) {
        int index = i - 'a';
        if (!pCrawl->children[index] && pCrawl == root){
            return false;
        }
        if(!pCrawl->children[index] && pCrawl != root){
            correctCheck = true;
            return false;
        }

        pCrawl = pCrawl->children[index];
    }

    dothrakiVal = pCrawl->dothVal;
    if(type == "insert" && pCrawl->dothVal != dothValue ){
        pCrawl->dothVal = dothValue;
        valCheck = true;
    }
    if(!pCrawl->isEndOfWord && type != "insert"){
        enoughCheeck = true;
        out<<'"'<<"not enough Dothraki word"<<'"'<<endl;
    }
    return (pCrawl->isEndOfWord);
}

// If do not have anny meaning, inserts key into trie
// If the key is prefix of trie node, just marks leaf node
void insert(struct TrieNode* root, const string& key, const string& dothValue, ofstream& out)
{
    bool searchCheck = search(root,key,"insert", dothValue, out);
    if(searchCheck && valCheck){
        out<<'"'<<key<<'"'<<" was updated\n";
        return;
    }
    else if (searchCheck && !valCheck){
        out<<'"'<<key<<'"'<<" already exist\n";
        return;
    }
    struct TrieNode* pCrawl = root;

    for (char i : key) {
        int index = i - 'a';
        if (!pCrawl->children[index])
            pCrawl->children[index] = getNode();

        pCrawl = pCrawl->children[index];
    }

    // mark last node as leaf
    pCrawl->isEndOfWord = true;
    pCrawl->dothVal = dothValue;
    out<<'"'<<key<<'"'<<" was added\n";
}

// Returns true if root has no children, else false
bool isEmpty(TrieNode* root)
{
    for (auto & i : root->children)
        if (i)
            return false;
    return true;
}

// Recursive function to delete a key from given Trie
TrieNode* remove(TrieNode* root, string key, int depth = 0)
{
    // If tree is empty
    if (!root)
        return nullptr;

    // If last character of key is being processed
    if (depth == key.size()) {

        // This node is no more end of word after removal of given key
        if (root->isEndOfWord){
            deleteCheck = true;
            root->isEndOfWord = false;
        }
        // If given is not prefix of any other word
        if (isEmpty(root)) {
            delete (root);
            root = nullptr;
        }
        return root;
    }

    // If not last character, recur for the child obtained using ASCII value
    int index = key[depth] - 'a';
    root->children[index] = remove(root->children[index], key, depth + 1);

    // If root does not have any child "its only child got deleted", and it is not end of another word.
    if (isEmpty(root) && !root->isEndOfWord) {
        delete (root);
        root = nullptr;
    }

    return root;
}

bool isLeafNode(struct TrieNode* root){
    return root->isEndOfWord;
}

void display(struct TrieNode* root, char str[], int level, ofstream& out)
{
    // If node is leaf node, it indicates end of string, so a null character is added and string is displayed

    char temp = str[0];

    counter +=1;
    if (isLeafNode(root))
    {
        if(temp != temp2){
            out<<"-"<<temp<<endl;
        }
        temp2 = temp;
        str[level] = '\0';
        out <<"    -"<< str << "("<<root->dothVal<<")"<<endl;
        counter = 0 ;
    }

    int i;
    for (i = 0; i < ALPHABET_SIZE; i++){
        // if non NULL child is found
        // add parent key to str and
        // call the display function recursively
        // for child node
        if (root->children[i])
        {
            str[level] = i + 'a';
            display(root->children[i], str, level + 1, out);
        }
    }
}

// This is my main function.  Just has 1 for makes all operations
int main(__attribute__((unused)) int argc, char *argv[]){
    char str[26];
    struct TrieNode* root = getNode();

    ifstream input1(argv[1]);
    ofstream out(argv[2]);
    for (string line; getline(input1, line); ){
        // If list then lists the all words
        if(line == "list"){
            display(root, str, 0, out);
        }
        else{
            // If insert then inserts the word to trie
            if(line.substr(0,6) == "insert"){
                int a = line.find(',');
                dothrakiKey = line.substr(7,a-7);
                dothrakiVal = line.substr(a+1,line.size()-(a+2));
                insert(root, dothrakiKey, dothrakiVal, out);
            }
            else if (line.substr(0,6) == "search"){
                // if search then finds the word that we try to find.
                dothrakiKey = line.substr(7,line.size()-8);
                bool searchCheck = search(root,dothrakiKey,"", dothrakiVal, out);
                if(searchCheck){
                    out<<'"'<<"The english equivalent is "<<dothrakiVal<<'"'<<endl;
                }
                else if (!enoughCheeck && !correctCheck){
                    out<<'"'<<"no record"<<'"'<<endl;
                }
                else if (!enoughCheeck && correctCheck){
                    out<<'"'<<"incorrect Dothraki word"<<'"'<<endl;
                }
            }
            // if delete then deletes the word that if exits
            else if (line.substr(0,6) == "delete"){
                dothrakiKey = line.substr(7,line.size()-8);
                bool searchCheck = search(root,dothrakiKey,"", dothrakiVal, out);
                if(searchCheck){
                    remove(root, dothrakiKey);
                }
                else if (!enoughCheeck && !correctCheck){
                    out<<'"'<<"no record"<<'"'<<endl;
                }
                else if (!enoughCheeck && correctCheck){
                    out<<'"'<<"incorrect Dothraki word"<<'"'<<endl;
                }
                if(deleteCheck) out<<'"'<<dothrakiKey<<'"'<<"deletion is successful"<<endl;
            }
        }
        deleteCheck = false;
        valCheck = false;
        correctCheck = false;
        enoughCheeck = false;
    }
}
