#ifndef LINKEDLIST_H
#define LINKEDLIST_H
#include "Node.h"


class LinkedList
{
public:
    LinkedList();
    ~LinkedList();

    int size(); //calls the list size
    void clear();
    FoodItem* get(int index); //find an index based on serarch parameters
    FoodItem* findItemById(std::string id);
    void addBack(Node* node);
    void addFront(Node* node);

    void addFoodItem(std::string id, std::string name, std::string description, unsigned dollar, unsigned cents); //adds item to list
    bool removeFoodItem(std::string itemId); //removes item from list
    void sortByName();//Sorts food list from A->Z (not lowercase)

    // more functions to be added perhaps...

private:
    // the beginning of the list
    Node* head;
  
    // how many nodes are there in the list?
    //unsigned count;
};

#endif  // LINKEDLIST_H


