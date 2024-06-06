#include <iostream>

#include "LinkedList.h"

LinkedList::LinkedList() {
   head = nullptr;


}

LinkedList::~LinkedList(){
    clear();

}

int LinkedList::size() { //working, not yet complete
   int count = 0;
    Node* current = head;
    if(head == nullptr){
        std::cout << "Menu is empty" << std::endl;
        return count;
    }else{
        while(current != nullptr){
            count++;
            current = current->next; //steps to next node
        }
        return count;
    }
}

void LinkedList::clear() {
    Node* current = head;

    if(head == nullptr){
        std::cout << "Linked List is empty" << std::endl;
    }else{
        while(current != nullptr){
            Node* next = current->next;
            delete current;
            current = next;
        }
        head = nullptr;
    }
}

FoodItem* LinkedList::get(int index){ // working, not complete
    int count = 0;
    Node* current = head;
    if(index < size()){
        while(count < index){
            count++;
            current = current->next;
        }
        return current->data;
    }
    return nullptr;
}

FoodItem* LinkedList::findItemById(std::string id){
    int count = 0;
    Node* current = head;
    while( count < size()){
        if (current->data->id == id){
            return current->data;
        } else {
            current = current->next;
            count ++;
        }
    }
    return 0;
}

void LinkedList::addBack(Node* node){
    if (head == nullptr) {
        head = node;
    } else {
        Node* current = head;
        while (current->next != nullptr) {
            current = current->next;
        }
        current->next = node;
    }
    node->next = nullptr;
}

void LinkedList::addFront(Node* node){
    node->next = head;
    head = node;
}

void LinkedList::addFoodItem(std::string id, std::string name, std::string description, unsigned dollar, unsigned cents, std::string parentList){
    //for adding a single item *note search for duplicate names.
    Node* newNode = new Node();
    newNode->data  = new FoodItem(id, name, description, dollar, cents, parentList);
    addBack(newNode);
}

bool LinkedList::removeFoodItem(std::string itemId){

    if(head == nullptr) return false; // Case: Empty list

    // Case: removing the head node
    if(head->data->id == itemId){
        Node* toDelete = head;
        head = head->next;
        delete toDelete;
        return true;
    }

    Node* current = head;
    while(current->next != nullptr){
        if(current->next->data->id == itemId){
            Node* toDelete = current->next;
            current->next = current->next->next; // Re-link before deleting
            delete toDelete;
            return true;
        }
        current = current->next;
     }
    return false; // Item not found
}


void LinkedList::sortByName() {
    if (head == nullptr || head->next == nullptr) {
        return;
    }

    bool swapped;
    Node* ptr1;
    Node* lptr = nullptr;

    do {
        swapped = false;
        ptr1 = head;

        while (ptr1->next != lptr) {
            if (ptr1->data->name > ptr1->next->data->name) {
                FoodItem* temp = ptr1->data;
                ptr1->data = ptr1->next->data;
                ptr1->next->data = temp;
                swapped = true;
            }
            ptr1 = ptr1->next;
        }
        lptr = ptr1;
    } while (swapped);
}
