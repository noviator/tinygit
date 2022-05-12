#include<iostream>
using namespace std;

struct singlyNode{
    int versionNumber;
    string fileName;
    string fileVersion;
    singlyNode * next;
};

struct doublyNode{
    int commitNumber;
    singlyNode * head;
    doublyNode * previous;
    doublyNode * next;
};

class tinyGit{
    public:    
        doublyNode * head;
        singlyNode* traverseSinglyLLFindFile(string fname, doublyNode* node);
        doublyNode * traverseDoublyLLFindCommit(int commitNum);
        doublyNode * traverseDoublyLL();
        void insertSinglyLL(doublyNode * node, singlyNode * insert);
        void deleteSinglyLL(doublyNode * node, string fname);
        void printSinglyLL(doublyNode * node);
        void traverseSinglyLLCheckTinyGit(doublyNode * node);
        void copySinglyLL(doublyNode * insert);
        void traverseSinglyLLForCheckout(doublyNode * node);
        void insertDoublyLL(doublyNode * insert);
        void copyFileToTinyGit(string file1, string file2);
        bool fileChanged(string f1, string f2);
};
