#include <iostream>
#include <fstream>
#include <sstream>
#include <string>

#include "Node.h"
Price::Price(){
    //Default Constructor
}

Price::Price(unsigned dollars, unsigned cents){
    this->dollars = dollars;
    this->cents = cents;
}

FoodItem::FoodItem(){
    //Default Constructor
}

FoodItem::FoodItem(const std::string& id, const std::string& name, const std::string& description, unsigned dollars, unsigned cents, const std::string& parentList){
    this->id = id;
    this->name = name;
    this->description = description;
    this->price.dollars = dollars;
    this->price.cents = cents;
    this->parentList = parentList;
}

Node::Node(){
    //Default Constructor
}

Node::Node(FoodItem* data, Node* next = nullptr){
    this->data = data;
    this->next = next;
}

Node::~Node(){
    delete data;
}
