#include <iostream>
#include "tinyGit.cpp"
#include <fstream>

using namespace std;

// #include <experimental/filesystem>
// namespace fs = experimental::filesystem;

#include <filesystem>
namespace fs = filesystem;

void printRepository(doublyNode * start){
    doublyNode * cur = start;
    while(cur!=NULL){
        cout << cur->commitNumber << "-->";
        singlyNode* node = cur->head;
        while(node!=NULL){
            cout << node->fileName << "-->";
            node = node->next;
        }
        cur = cur->next;
        cout << endl;
    }
}

void displayMenu(){
    cout << ">>>>Menu<<<<" << endl;
    cout << "1. Add a file" << endl;
    cout << "2. Remove file" << endl;
    cout << "3. Commit" << endl;
    cout << "4. Checkout" << endl;
    cout << "5. Quit" << endl << endl;
    cout<< "Enter your choice: ";
}          

int main(){
    cout<<"Would you like to create a new repository? (y/n)"<<endl;
    char choice;
    cin>>choice;

    if(choice == 'n' || choice == 'N'){
        return 0;
    }else if(choice != 'Y' && choice != 'y'){
        cout<< "Invalid choice"<<endl;
        return 0;
    }
    fs::remove_all(".tinygit");
    fs::create_directory(".tinygit"); 
    
    // initializing the class and creating the first node of doubly linked list
    tinyGit repo;
    doublyNode * start = new doublyNode;
    start->commitNumber = 0;
    start->previous = NULL;
    start->next = NULL;
    start->head = NULL;
    repo.head = start;
    displayMenu();

   int num;
   cin >> num;
   while (num != 5){
        switch(num){
            case 1:{
                string filename;
                cout << "Enter a file name:" << endl;
                cin >> filename;
                ifstream infile;
                infile.open(filename);
                while(infile.fail()){
                    cout << "File does not exist. Please enter a valid file name."<<endl;
                    cin >> filename;
                    infile.open(filename);
                }
                doublyNode * lastNode = repo.traverseDoublyLL();
                if(repo.traverseSinglyLLFindFile(filename,lastNode) != NULL){
                    cout << "The file already exists!" << endl;
                    displayMenu();
                    cin >> num;
                    break;
                } else {
                    cout << "Adding file..." << endl;
                }

                singlyNode * newfile = new singlyNode;
                newfile->fileName = filename;
                newfile->fileVersion = filename + "_" + to_string(newfile->versionNumber);
                newfile->next = NULL;
                doublyNode * result = repo.traverseDoublyLL(); // find the last node in the Doubly Linked List
                repo.insertSinglyLL(result, newfile);

                cout << "File added successfully!" << endl;
                displayMenu();
                cin>>num;
                break;
            }
            break;
            case 2:{
                string fname;
                cout<< "Enter the file name you want to remove: "<<endl;
                cin >> fname;
                doublyNode * result = repo.traverseDoublyLL();
                while (repo.traverseSinglyLLFindFile(fname, result) == NULL) {
                    cout << "File does not exist"<<endl;
                    cout<< "Enter another filename";
                    cin >> fname;
                }
                repo.deleteSinglyLL(result, fname);
                displayMenu();
                cin >> num;
                break;
            } 
            break;
            case 3:{
                // finding the last doubly linked list node
                doublyNode * lastDouble = repo.traverseDoublyLL();
                // check if it exists in tinygit or has been modified 
                repo.traverseSinglyLLCheckTinyGit(lastDouble);
                // new Doubly Linked List node to insert
                doublyNode * newDouble = new doublyNode; 
                repo.insertDoublyLL(newDouble);
                repo.copySinglyLL(newDouble);
                cout << "Commit successful" << endl;
                displayMenu();
                cin >> num;
                break;

            }
            break;
            case 4:{
                int commit;
                cout << "Enter version number you would like to checkout:" << endl;
                cin >> commit;
                doublyNode * commitNode = repo.traverseDoublyLLFindCommit(commit);
                if(commitNode==NULL){
                    cout << "This commit number does not exist" << endl;
                    break;
                }
                cout << "The files for commit number " << commitNode->commitNumber << " are" << endl;
                repo.printSinglyLL(commitNode);
                cout << endl;
                cout<<"Any uncommitted changes will be lost. Would you like to continue? (y/n)"<<endl;
                char choice;
                cin>>choice;
                if (choice == 'n' || choice =='N') {
                    displayMenu();
                    cin >> num;
                    break;
                } else {
                    repo.traverseSinglyLLForCheckout(commitNode);
                    cout << "Checkout successful !" << endl;
                    displayMenu();
                    cin >> num;
                    break;
                }
            }
            default:{
                cout << "Invalid choice" << endl;
                cout<< "Enter your choice: ";
                displayMenu();
                cin >> num;
                break;            
            }
            break;
            
        } 
    }
}
