#include <iostream>
#include <fstream>
#include "tinyGit.hpp"
using namespace std;

// Traverse the Singly Linked List to find the file with the given name
singlyNode * tinyGit::traverseSinglyLLFindFile(string fname, doublyNode * node){
    singlyNode * headNode = node->head;
    while(headNode!=NULL){
        if(headNode->fileName == fname){
            return headNode;
        }
        headNode = headNode->next;
    }
    return NULL;
}

// Function to find the commit with the given commit number from the doubly linked list
doublyNode * tinyGit::traverseDoublyLLFindCommit(int commitNum){
    doublyNode * cur = head;
    while(cur!=NULL){
        if(cur->commitNumber == commitNum){
            return cur;
        }
        cur = cur->next;
    }
    return NULL;
}

// Function to find the last node in Doubly Linked List
doublyNode * tinyGit::traverseDoublyLL(){
    doublyNode * cur = head;
    while(cur->next != NULL){
        cur = cur->next;
    }
    return cur;
}

// Function to insert a Singly Lined List node at end of Singly Linked List of the most recent Doubly Linked List
void tinyGit::insertSinglyLL(doublyNode * node, singlyNode * newSingle){
    if(node->head == NULL){
        node->head = newSingle;
        newSingle->next = NULL;
        return;
    }
    singlyNode * cur = node->head;
    while(cur->next != NULL)
    {
        cur = cur->next;
    }
    cur->next = newSingle;
    newSingle->next = NULL;
}

// Function to insert a Doubly Linked List node at the end of the Doubly Linked List
void tinyGit::insertDoublyLL(doublyNode * insert){
    doublyNode * cur = head;
    while(cur->next != NULL){
        cur = cur->next;
    }
    cur->next = insert;
    insert->previous = cur;
    insert->next = NULL;
    insert->head = NULL;
    int newCommitNum = cur->commitNumber + 1;
    insert->commitNumber = newCommitNum; 
}

// deleting a Singly Linked list node
void tinyGit::deleteSinglyLL(doublyNode * node, string fname)
{
    singlyNode * cur = node->head;
    singlyNode * prev = NULL;

    while(cur->next!=NULL && cur->fileName != fname){
        prev = cur;
        cur = cur->next;  
    }
    if(cur->next == NULL && cur->fileName != fname){
        cout<<"File not found"<<endl;
        return;
    }else if(cur == node->head && cur->fileName == fname){ // deleting the head node
        node->head = node->head->next;
        delete cur;
    } else if(cur!= node->head && cur->fileName == fname){ // deleting a node (not head)
        prev->next = cur->next;
        delete cur;
    }
    cout<<"File deleted"<<endl;
}

// Function to print the file names in Singly Linked List
void tinyGit::printSinglyLL(doublyNode * node){
    singlyNode * cur = node->head;
    int count = 1;
    while(cur!= NULL){
        cout << count << ". " << cur->fileName << endl;
        cur = cur->next;
        count++;
    }
}

// Function to copy file to tinygit
void tinyGit::copyFileToTinyGit(string file1, string file2){
    ifstream src;
    src.open(file1); // original file from main directory
    ofstream dest;
    dest.open(file2); // file we are writing to in tinygit
    string line;
    while(getline(src, line )){
        dest << line; // copies file1 line by line to file2(in tinygit)
    }
    src.close();
    dest.close();
}

// Function which returns true if there are any changes between the two files(f1, f2)
bool tinyGit::fileChanged(string f1, string f2){
    ifstream file1;
    file1.open(f1);
    string object1;
    string line = "";

    while(getline(file1, line)){
        object1 = object1 + line;
    }
    file1.close();

    ifstream file2;
    file2.open(f2);
    string line2 = "";
    string object2;

    while(getline(file2, line2)){
        object2 = object2 + line2;
    }
    file2.close();
    if(object1 == object2){
        return false;
    }
    return true;
}

// check if each file in Singly Linked list exists in .tinygit , has changed
void tinyGit::traverseSinglyLLCheckTinyGit(doublyNode * node){

    singlyNode * cur = node->head;
    while(cur!=NULL){
        string f1 = cur->fileName;
        ifstream fileIn;
        string f2 = cur->fileName + "_" + to_string(cur->versionNumber);
        string fileName = ".tinygit/" + f2;
        fileIn.open(fileName);
        if(fileIn.fail()){
            copyFileToTinyGit(f1, fileName);
        } 
        else{
            bool changed = fileChanged(f1, fileName);
            if(changed){
                cur->versionNumber++;
                string updatedFile = cur->fileName + "_" + to_string(cur->versionNumber);
                string newtinyGitfile= ".tinyGit/" + updatedFile;
                copyFileToTinyGit(f1, newtinyGitfile);
                cur->fileVersion = updatedFile;
            }
        }
        cur = cur->next;
    }
}

// copy the previous Doubly Linked List's singly linked list to the current Doubly Linked List
void tinyGit::copySinglyLL(doublyNode * newDoubly){
    if(newDoubly->previous == NULL || newDoubly->previous->head == NULL){
        return;
    }
    singlyNode * cur = newDoubly->previous->head;
    while(cur != NULL){
        singlyNode * copiedSingly = new singlyNode;
        copiedSingly->versionNumber = cur->versionNumber;
        copiedSingly->fileName = cur->fileName;
        copiedSingly->fileVersion = cur->fileVersion;
        insertSinglyLL(newDoubly, copiedSingly);             
        cur = cur->next;
        copiedSingly = NULL;
    }
}

// traverses through Singly Linked List of the Doubly Linked List node
// overwrites the current local file with the version from that commit if the file versions are different
void tinyGit::traverseSinglyLLForCheckout(doublyNode * node){
    singlyNode * cur = node->head;
    while(cur != NULL){
        string file = cur->fileVersion;
        string openfile = ".tinygit/" + file;
        ifstream infile;
        ofstream outfile;
        bool changed = fileChanged(cur->fileName, openfile);
        if(changed){
            outfile.open(cur->fileName);
            infile.open(openfile);
            string line;
            while(getline(infile, line)){
                outfile << line << endl;
            }
        }
        
        cur = cur->next;
    }
}