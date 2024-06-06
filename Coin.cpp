#include <iostream>
#include <string>

#include "Coin.h"


 // implement functions for managing coins; this may depend on your design.
Coin::Coin(){
}

Coin::Coin(Denomination denom, unsigned count){
    this->denom = denom;
    this->count = count;
}