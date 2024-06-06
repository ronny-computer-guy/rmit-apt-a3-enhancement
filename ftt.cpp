#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include <vector>
#include <iomanip>
#include <locale>
#include <limits>


#include "LinkedList.h"
#include "Coin.h"

#define EMPTY "empty"

/* 
**Student details and Input**

Paul Dichiera s3969863 (input 33.33%)
Yao Lin                 (input 33.33%)
Adam Lee                (input 33.33%)
*/

/**
 * manages the running of the program, initialises data structures, loads
 * data, display the main menu, and handles the processing of options. 
 * Make sure free memory and close all files before exiting the program.
 **/
void displayMainMenu();
void loadMenuData(const std::string& fileName1, LinkedList* menu);
void loadCoinsData(const std::string& fileName, std::vector<Coin> &coins);
void displayMealOptions(LinkedList* list);
bool isSubmenuExisted(const std::vector<std::string>& submenu, const std::string& parentList);
void purchaseMeal(LinkedList* menu, std::vector<Coin>& coins);
void printCoinData(const std::vector<Coin>& coins);
void addFoodItem(LinkedList* list);
void removeFoodItem(LinkedList* list);
Denomination strToDenomination(const std::string& str);
std::string denominationToStr(Denomination denomination);
std::vector<Denomination> calculateChanges(unsigned int amount);
void displayChanges(std::vector<Denomination> changes);
void depositCoin(std::vector<Coin>& coins, const std::string& denomination_str);
void withdrawCoin(std::vector<Coin>& coins, std::vector<Denomination> changes);
void displayBalence(const std::vector<Coin>& coins);
void saveAndExit(std::vector<Coin>& coins, LinkedList* list);
void mainMenu(std::vector<Coin>& coins, LinkedList* list);
bool isValidDenomination(const std::string& str);
void clearInputBuffer();


// to run makefile and run with "./ftt foods.dat coins.dat"
int main(int argc, char **argv) {

    if(argc < 3){ // Checks three arguments have been given when running the program, further checks for content when loading the data
        std::cout << "Loading insufficient" << argv[0] << "<filename1> <filename2>" << std::endl;
        return 1; 
    }

    LinkedList* menu = new LinkedList();
    loadMenuData(argv[1], menu);
    std::vector<Coin> coins;
    loadCoinsData(argv[2], coins);
    // Main menu user selects possible actions from here
    bool is_running = true;
    while (is_running)
    {
        int response = 0;
        std::cout << std::endl;
        displayMainMenu();
        // Handle input failure
        if (!(std::cin >> response)) { 
            // Input validation
            if (std::cin.eof()) {
                std::cout << "Error: Invalid input." << std::endl; //gives error before exit
                is_running = false;
            } else {
                std::cin.clear();
                std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
            }
        } else {
            // Display menu options
            if (response == 1) 
            {
                displayMealOptions(menu);
            }
            // Purchase the Meal (in Progress)
            else if (response == 2) 
            {
                purchaseMeal(menu, coins);
            }
            // Save and Exit
            else if (response == 3) 
            {   
                saveAndExit(coins, menu);
                delete menu;
                exit(0);
            }
            // Add food item
            else if (response == 4) 
            {
                addFoodItem(menu);
            }
            // Remove food item
            else if (response == 5) 
            {
                removeFoodItem(menu);
            }
            // Display balence
            else if (response == 6) 
            {
                displayBalence(coins);
            }
            // Abort program
            else if (response == 7) 
            {
                is_running = false;
            }
            else{
                std::cout << "Error: Invalid Input." << std::endl;
            }
        }
    }
    delete menu;
    return EXIT_SUCCESS;
}


// The function displays all the options available in the main menu
void displayMainMenu() {
    std::cout << "Main Menu:" << std::endl;
    std::cout << "  1. Display Meal Options" << std::endl;
    std::cout << "  2. Purchase Meal" << std::endl;
    std::cout << "  3. Save and Exit" << std::endl;
    std::cout << "Administrator-Only Menu:" << std::endl;
    std::cout << "  4. Add Food" << std::endl;
    std::cout << "  5. Remove Food" << std::endl;
    std::cout << "  6. Display Balance" << std::endl;
    std::cout << "  7. Abort Program" << std::endl;
    std::cout << "Select your option (1-7): ";
}

// reads food data file and breaks up the line into variables for the addFooItem() method ** WORK IN PROGRESS**
void loadMenuData(const std::string& fileName1, LinkedList* menu){

    std::string line;
    std::ifstream food_data(fileName1);

    if(!food_data.is_open())
    {
        std::cout << "file failed to open" << std::endl;
    }
    else if(food_data.peek() == std::ifstream::traits_type::eof()) // Checks and closes program is foods.dat is empty
    {
        std::cout << "Foods file is empty" << std::endl;
    }
    else
    {
        while(getline(food_data, line)){
            bool check = true;
            std::istringstream iss(line);
            std::string id, name, description, dollarsStr, centsStr, parentList;
            std::getline(iss, id, '|');

            if(id[0] != 'F' || id.size() < 4){
                check = false;
            }
            std::getline(iss, name, '|');
            if(name.size() == 0 || name.size() > 40){
                check = false;
            }
            std::getline(iss, description, '|');
            if(description.size() == 0 || description.size() > 255){
                check = false;
            }
            std::getline(iss, dollarsStr, '.');
            for( size_t i = 0; i < dollarsStr.size(); i++){
                if(!isdigit(dollarsStr[i])){
                    i = dollarsStr.size();
                    check = false;
                }
            }
            std::getline(iss, centsStr, '|');
            for( size_t i = 0; i < centsStr.size(); i++){
                if(!isdigit(centsStr[i])){
                    i = centsStr.size();
                    check = false;
                }
            }
            std::getline(iss, parentList, '\n');
        
            if(check){
                unsigned int dollars = std::stoul(dollarsStr);
                unsigned int cents = std::stoul(centsStr);
                menu->addFoodItem(id, name, description, dollars, cents, parentList);
            }else{
                std::cout << "line skipped due to possible formatting issues please check menu data file" << std::endl;
            }
        }
    }
}

// The function reads coins data from coin.h, and store these data into "coins" vector variable
void loadCoinsData(const std::string& fileName, std::vector<Coin> &coins){
    std::string line;
    std::ifstream coins_data(fileName);
    if(!coins_data.is_open()){
        std::cout << "file failed to open" << std::endl;
    }else if(coins_data.peek() == std::ifstream::traits_type::eof()){ // Checks and closes program is coins.dat is empty
        std::cout << "Coins file is empty" << std::endl;
        coins_data.close();
        exit(0);
    }else{
        // Continue to read coin data from coins.dat and push data into "coins" vector
        while(getline(coins_data, line)){
            std::istringstream iss(line);
            std::string denomination_str, count_str;
            std::getline(iss, denomination_str, ',');
            std::getline(iss, count_str);

            if(isValidDenomination(denomination_str)){
                Denomination denomination = strToDenomination(denomination_str);
                unsigned int count = std::stoul(count_str);
                coins.push_back(Coin(denomination, count));
            }else{
                std::cout << "coin data input is invalid, check format and denominations are correctly formatted" << std::endl;
                coins_data.close();
                exit(0);
            }
        }
    }
}

// Meal purchases and financial transactions 
// Add availability checks and subtraction upon purpose
void purchaseMeal(LinkedList* menu, std::vector<Coin>& coins) {
    // Asking users to choose the food item base on the ID
    std::cout << "Please enter the ID of the food you wish to purchase: ";
    std::string selectedId = EMPTY;
    bool exit = false;
    clearInputBuffer();
    std::getline(std::cin, selectedId);
    
    while (menu->findItemById(selectedId) == nullptr && !exit) {
        if(selectedId.empty()){
        exit = true;
        std::cout << "Purchase cancelled" << std::endl;
        }else{
            std::cout << "Invalid ID Input" << std::endl;
            std::cout << "Please enter the ID of the food you wish to purchase: ";
            std::getline(std::cin, selectedId);
        }
    }

    if(!exit){
        std::cout << std::endl;
        // Search the menu linkedlist and find the fooditem match the selectedId
        FoodItem* selectedItem = menu->findItemById(selectedId);
        // Display the info of the food item and asking user for the money
        std::cout << "You have selected \"" << selectedItem->name << " - "<< selectedItem->description << "\". This will cost you $" << selectedItem->price.dollars << "." << selectedItem->price.cents << std::endl;
        std::cout << "Please hand over the money - type in the value of each note/coin in cents." << std::endl;
        std::cout << "Please enter ctrl-D or enter on a new line to cancel this purchase." << std::endl;

        unsigned int foodPrice = selectedItem->price.dollars * 100 + selectedItem->price.cents;            
        double unpaidAmount = foodPrice;
        bool is_running = true;
        // The loop will keep runing until customer paid equal or more than the price of food
        while(is_running && !exit){
            // asking more money when the customer paid less than the price of food
            if (unpaidAmount > 0) {
                std::string paidAmount_str = EMPTY;
                unsigned int paidAmount = 0;
                std::cout << std::fixed << std::setprecision(2);
                std::cout << "You still need to give us $ " << unpaidAmount / 100 << ": ";
                std::getline(std::cin, paidAmount_str);

                if(paidAmount_str.empty()){
                    exit = true;
                    std::cout << "Purchase cancelled" << std::endl;
                }else{
                    while (!isValidDenomination(paidAmount_str) && !exit) {
                        std::cout << "Input Invalid, Please enter a valid ammount: ";
                        std::getline(std::cin, paidAmount_str);
                        if(paidAmount_str.empty()){
                            exit = true;
                            std::cout << "Purchase cancelled" << std::endl;
                        }
                    }
                }
                if(!exit){
                    depositCoin(coins, paidAmount_str);
                    paidAmount = std::stoi(paidAmount_str);
                    unpaidAmount = unpaidAmount - paidAmount;
                }
            // display chnage when customer paid more than the price of food
            }else if (unpaidAmount <= 0 ){
                unpaidAmount = std::abs(unpaidAmount);
                std::vector<Denomination> changes;
                // convert the total change into various denomication changes
                changes = calculateChanges(unpaidAmount);
                withdrawCoin(coins,changes);
                displayChanges(changes);
                //printCoinData(coins);
                is_running = false;
            }
        }
    }
}

void depositCoin(std::vector<Coin>& coins, const std::string& denomination_str){
    Denomination denomination = strToDenomination(denomination_str);
    
    // Iterate through the coins vector to find the coin with the specified denomination
    for (Coin& coin : coins) {
        if (coin.denom == denomination) {
            coin.count = coin.count + 1;
        }
    }
}


void withdrawCoin(std::vector<Coin>& coins, std::vector<Denomination> changes) {
    for (Denomination denom : changes) {
        // Iterate through the coins vector to find the coin with the specified denomination
        for (Coin& coin : coins) {
            if (coin.denom == denom) {
                coin.count = coin.count - 1;
            }
        }
    } 
}

// Debug print for coin reading - Paul
void printCoinData(const std::vector<Coin>& coins){

    for(const Coin& coin : coins){
        std::cout << denominationToStr(coin.denom) << ": " << coin.count << std::endl;
    }
}

// Displays menu items
void displayMealOptions(LinkedList* list){ 
    
    std::vector<std::string> submenu;

    std::cout << std::endl;
    std::cout << "Food Menu" << std::endl;
    std::cout << "------------------" << std::endl;
    std::cout << std::left << std::setw(6) << "ID" << "|" << std::setw(24) << "Name" << "|" << std::setw(7) << "Price" << std::endl;
    std::cout << "-------------------------------------------" << std::endl;

    // Print Food Item without Sublist
    for(int i = 0; i < list->size(); i++){
        if (list->get(i)->parentList == "No"){
            std::cout << std::left << std::setw(6) << list->get(i)->id << "|"
            << std::setw(24) << list->get(i)->name << "|"
            << std::setw(0) << list->get(i)->price.dollars << ".";

            if (list->get(i)->price.cents < 10) {
                std::cout << "0" << list->get(i)->price.cents;
            } else {
                std::cout << list->get(i)->price.cents;
            }
            std::cout << std::endl;
        } else {
            std::string parentList = list->get(i)->parentList;
            if(!isSubmenuExisted(submenu, parentList)){
                submenu.push_back(parentList);
            }
        }
    }
    // Print Sublist
    for (const auto& item : submenu) {
        std::cout << item << std::endl;
        for(int i = 0; i < list->size(); i++){
            if (list->get(i)->parentList == item){
                std::cout << std::left << std::setw(6) << list->get(i)->id << "|"
                << std::setw(24) << list->get(i)->name << "|"
                << std::setw(0) << list->get(i)->price.dollars << ".";
                if (list->get(i)->price.cents < 10) {
                    std::cout << "0" << list->get(i)->price.cents;
                } else {
                    std::cout << list->get(i)->price.cents;
                }
                std::cout << std::endl;
            }
        }
    }
    
    std::cout << std::endl;
}

bool isSubmenuExisted(const std::vector<std::string>& submenu, const std::string& parentList) {
    bool isFound = false;
    for (const auto& item : submenu) {
        if (item == parentList) {
            isFound = true;
        }
    }
    return isFound;
}

// The function converts string into enum denomination
Denomination strToDenomination(const std::string& str) {
    if (str == "5") return FIVE_CENTS;
    else if (str == "10") return TEN_CENTS;
    else if (str == "20") return TWENTY_CENTS;
    else if (str == "50") return FIFTY_CENTS;
    else if (str == "100") return ONE_DOLLAR;
    else if (str == "200") return TWO_DOLLARS;
    else if (str == "500") return FIVE_DOLLARS;
    else if (str == "1000") return TEN_DOLLARS;
    else if (str == "2000") return TWENTY_DOLLARS;
    else if (str == "5000") return FIFTY_DOLLARS;
    else return FIVE_CENTS;
}

// The function converts denomination into string
std::string denominationToStr(Denomination denomination) {
    if (denomination == FIVE_CENTS) return "5";
    else if (denomination == TEN_CENTS)  return "10";
    else if (denomination == TWENTY_CENTS) return "20";
    else if (denomination == FIFTY_CENTS) return "50";
    else if (denomination == ONE_DOLLAR) return "100";
    else if (denomination == TWO_DOLLARS) return "200";
    else if (denomination == FIVE_DOLLARS) return "500";
    else if (denomination == TEN_DOLLARS) return "1000";
    else if (denomination == TWENTY_DOLLARS) return "2000";
    else if (denomination == FIFTY_DOLLARS) return "5000";
    else return "Unknown";
}

//gets the first character and makes it uppercase so it can sort A->Z in food list
void firstLetterUppercase(std::string& str) {
    if (!str.empty()) {
        str[0] = std::toupper(str[0]);
    }
}

// add food item, currently broken
void addFoodItem(LinkedList* menu){
    unsigned dollars = 0;
    unsigned cents = 0;
    bool is_running = false;
    
    // Get last id number and increment 
    //finding last item on menu to create now id
    unsigned lastItem = menu->size() -1;
    std::string idNumStr = menu->get(lastItem)->id.substr(1);
    unsigned newId = std::stoi(idNumStr) +1;
    std::ostringstream oss;
    oss << 'F' << std::setw(4) << std::setfill('0') << newId;
    std::string newIdStr = oss.str();

    std::cout << "This new meal item will have the Item id of " << newIdStr << "." << std::endl;
    std::string name = EMPTY, description = EMPTY, dollarsStr = EMPTY, centsStr = EMPTY, parentList = EMPTY;
    bool exit = false;
    std::string input;

    // Loop is maintained until all fields are completed or the exit escape hatch is triggered by hitting enter on an empty input
    while((name == EMPTY || description == EMPTY || dollarsStr == EMPTY || centsStr == EMPTY || parentList == EMPTY) && !exit){
        is_running = true;
        while(is_running){
            if(name == EMPTY){
                std::cout << "Enter the item name: "; 
                clearInputBuffer();
                std::getline(std::cin, input);
                firstLetterUppercase(input);
                
                if(input.empty()){
                    exit = true;
                    is_running = false;
                }
                for(size_t i = 0; i < input.size(); i++ ){
                    if(!isalpha(input[i]) && input[i] != ' '){
                        std::cout << "Please input alpha numeric characters only for name" << std::endl;
                        i = input.size();
                        input.clear();
                        is_running = false;
                    }
                }
                if(is_running){
                    if(input.size() > NAMELEN || input.size() == 0){
                        std::cout << "Please input a name under 40 characters" << std::endl;
                        is_running = false;
                    }else{
                        name = input;
                    }
                }
            }else if(description == EMPTY){
                std::cout << "Enter the item description: ";
                std::getline(std::cin, input);
                if(input.empty()){
                    exit = true;
                    is_running = false;
                }
                for(size_t i = 0; i < input.size(); i++ ){
                    if((!isalpha(input[i]) && input[i] != ' ')){
                        std::cout << "Please input alpha numeric characters only for name" << std::endl;
                        i = input.size();
                        input.clear();
                        is_running = false;
                    }
                }
                if(is_running){
                    if(input.size() > DESCLEN || input.size() == 0){
                        std::cout << "please input a description under 255 characters" << std::endl;
                        is_running = false;
                    }else{
                        description = input;
                    }
                }
            }else if(dollarsStr == EMPTY || centsStr == EMPTY || dollarsStr.size() == 0 || centsStr.size() == 0){
                std::cout << "Enter the price for this item: ";
                std::getline(std::cin, input);
                std::istringstream iss(input);
                if(input.empty()){
                    exit = true;
                    is_running = false;
                }
                for(size_t i = 0; i < input.size(); i++ ){
                    if(!isdigit(input[i]) && input[i] != '.'){
                        std::cout << "Please input numeric characters only for price" << std::endl;
                        i = input.size();
                        input.clear();
                        is_running = false;
                    }
                }
                if(is_running){
                    if(!std::getline(iss, dollarsStr, '.') || !std::getline(iss, centsStr)){
                        dollarsStr = EMPTY;
                        centsStr = EMPTY;
                    }
                    try{
                        dollars = std::stoi(dollarsStr);
                        cents = std::stoi(centsStr);

                        if(!(cents % 5 == 0)){
                            dollarsStr = EMPTY;
                            centsStr = EMPTY;
                            is_running = false;
                            std::cout << "Input cents must be multipes of 5" << std::endl;
                        }else if (cents < 10) {
                            std::cout << "Price entered: $" << dollars << "." << "0" << cents << std::endl;
                        } else {
                            std::cout << "Price entered: $" << dollars << "." << std::setw(2) << cents << std::endl;
                        }

                    }catch(const std::invalid_argument& ia){
                        std::cout << "Invalid input, numeric format only please input in <dollars>.<cents> format" << std::endl;
                        dollarsStr = EMPTY;
                        centsStr = EMPTY;
                        is_running = false;
                    }catch(const std::out_of_range& e){
                        std::cout << "Input out of range, please input a smaller number" << std::endl;
                        dollarsStr = EMPTY;
                        centsStr = EMPTY;
                        is_running = false;
                    }
                    is_running = false;
                }
            } else if(parentList == EMPTY){
                std::cout << "Enter the name of the parent list(if not, input: no): "; 
                clearInputBuffer();
                std::getline(std::cin, input);
                firstLetterUppercase(input);
                
                if(input.empty()){
                    exit = true;
                    is_running = false;
                }
                for(size_t i = 0; i < input.size(); i++ ){
                    if(!isalpha(input[i]) && input[i] != ' '){
                        std::cout << "Please input alpha numeric characters only for name" << std::endl;
                        i = input.size();
                        input.clear();
                        is_running = false;
                    }
                }
                if(is_running){
                    if(input.size() > NAMELEN || input.size() == 0){
                        std::cout << "Please input a name under 40 characters" << std::endl;
                        is_running = false;
                    }else{
                        parentList = input;
                    }
                }
                is_running = false;
            }
        }
    }
    // create input check
    if(name != EMPTY && description != EMPTY && dollarsStr != EMPTY && centsStr != EMPTY){
        menu->addFoodItem(newIdStr, name, description, dollars, cents, parentList);
    }
}

// Removes item from menu
void removeFoodItem(LinkedList* list) {
    std::string input;
    bool is_running = true;

    while (is_running) {
        std::cout << "Enter the food id of the food item to remove from the menu: ";
        std::getline(std::cin, input);
        if (std::cin.eof()) {
            std::cout << "EOF detected. Exiting..." << std::endl;
            is_running = false;
        } else if (list->removeFoodItem(input)) {
            std::cout << "Item removed from Menu" << std::endl;
            std::cout << std::endl;
            is_running = false;
        } else {
            std::cout << "Item not found, please enter a valid id, example format <F0001>" << std::endl;
            std::cout << std::endl;
        }
    }
}

// The function return a vector storing different various domination converted from totoal amount of money
std::vector<Denomination> calculateChanges(unsigned int amount) {
    std::vector<Denomination> change;
    while (amount > 0) {
        if (amount >= 5000) {
            change.push_back(FIFTY_DOLLARS);
            amount -= 5000;
        } else if (amount >= 2000) {
            change.push_back(TWENTY_DOLLARS);
            amount -= 2000;
        } else if (amount >= 1000) {
            change.push_back(TEN_DOLLARS);
            amount -= 1000;
        } else if (amount >= 500) {
            change.push_back(FIVE_DOLLARS);
            amount -= 500;
        } else if (amount >= 200) {
            change.push_back(TWO_DOLLARS);
            amount -= 200;
        } else if (amount >= 100) {
            change.push_back(ONE_DOLLAR);
            amount -= 100;
        } else if (amount >= 50) {
            change.push_back(FIFTY_CENTS);
            amount -= 50;
        } else if (amount >= 20) {
            change.push_back(TWENTY_CENTS);
            amount -= 20;
        } else if (amount >= 10) {
            change.push_back(TEN_CENTS);
            amount -= 10;
        } else if (amount >= 5) {
            change.push_back(FIVE_CENTS);
            amount -= 5;
        }
    }
    return change;
}

// The function Loop through the "change" vector and print out all the denomination that stored in the vectors 
void displayChanges(std::vector<Denomination> changes){
    int num_five_cents = 0;
    int num_ten_cents = 0;
    int num_twenty_cents = 0;
    int num_fifty_cents = 0;
    int num_one_dollar = 0;
    int num_two_dollars = 0;
    int num_five_dollars = 0;
    int num_ten_dollars = 0;
    int num_twenty_dollars = 0;
    int num_fifty_dollars = 0;
    // Count each denomination in the chnages vector
    for (Denomination denom : changes) {
        if (denom == FIVE_CENTS) {
            num_five_cents++;
        } else if (denom == TEN_CENTS) {
            num_ten_cents++;
        } else if (denom == TWENTY_CENTS) {
            num_twenty_cents++;
        } else if (denom == FIFTY_CENTS) {
            num_fifty_cents++;
        } else if (denom == ONE_DOLLAR) {
            num_one_dollar++;
        } else if (denom == TWO_DOLLARS) {
            num_two_dollars++;
        } else if (denom == FIVE_DOLLARS) {
            num_five_dollars++;
        } else if (denom == TEN_DOLLARS) {
            num_ten_dollars++;
        } else if (denom == TWENTY_DOLLARS) {
            num_twenty_dollars++;
        } else if (denom == FIFTY_DOLLARS) {
            num_fifty_dollars++;
        }
    }
    std::cout << "Your change is ";
    if (num_five_cents > 0) {
        std::cout << num_five_cents << " x 5c ";
    }
    if (num_ten_cents > 0) {
        std::cout << num_ten_cents << " x 10c ";
    }
    if (num_twenty_cents > 0) {
        std::cout << num_twenty_cents << " x 20c ";
    }
    if (num_fifty_cents > 0) {
        std::cout << num_fifty_cents << " x 50c ";
    }
    if (num_one_dollar > 0) {
        std::cout << num_one_dollar << " x $1 ";
    }
    if (num_two_dollars > 0) {
        std::cout << num_two_dollars << " x $2 ";
    }
    if (num_five_dollars > 0) {
        std::cout << num_five_dollars << " x $5 ";
    }
    if (num_ten_dollars > 0) {
        std::cout << num_ten_dollars << " x $10 ";
    }
    if (num_twenty_dollars > 0) {
        std::cout << num_twenty_dollars << " x $20 ";
    }
    if (num_fifty_dollars > 0) {
        std::cout << num_fifty_dollars << " x $50 ";
    }
    std::cout << "" << std::endl;
    std::cout << "" << std::endl;
}

//Displays the balence and total coins in register
void displayBalence(const std::vector<Coin>& coins){

    double value;
    double total = 0.0;

    std::cout << std::endl;
    std::cout << "Balence Summary" << std::endl;
    std::cout << "-------------" << std::endl;
    std::cout << "Denom | Quantity | Value" << std::endl;
    std::cout << "---------------------------" << std::endl;

    for(int i = coins.size() - 1; i >= 0 ; i--){
        double denomInt = stod(denominationToStr(coins[i].denom));
        value = (denomInt * coins[i].count) / 100;
        std::cout << std::fixed << std::setprecision(2);
        std::cout << std::left << std::setw(6) << denominationToStr(coins[i].denom) <<  "|" << std::left << std::setw(10) << coins[i].count << "|$" << 
            value << std::endl;
        total = total + value;
    }
    std::cout << std::right << std::setw(19) << "$" << total << std::endl;
    std::cout << std::endl;

}

// Save changes to coin data and menu data, currently saves to a new file to preserve origianls for testing
void saveAndExit(std::vector<Coin>& coins, LinkedList* list){
    std::string filenameCoins =  "coins.dat";
    std::string filenameFoods =  "foods.dat";

    std::ofstream coinsFile(filenameCoins);

    if(coinsFile.is_open()){
        for(const Coin& coin : coins){
        coinsFile << denominationToStr(coin.denom) << "," << coin.count << "\n";
        }
        coinsFile.close();
    }

    std::ofstream foodsFile(filenameFoods);
    if(foodsFile.is_open()){
        for(int i = 0; i < list-> size(); i++){
            foodsFile  << list->get(i)->id << "|" <<
                list->get(i)->name << "|" <<
                list->get(i)->description << "|" <<
                list->get(i)->price.dollars << "." << list->get(i)->price.cents << "|" <<
                list->get(i)->parentList << "\n";
        }
        foodsFile.close();
    }

}

// Checks input is valid for meal purchase
bool isValidDenomination(const std::string& str) {
    if (str == "5" || str == "10" || str == "20" || str == "50" || str == "100" ||
        str == "200" || str == "500" || str == "1000" || str == "2000" || str == "5000") {
        return true; // Return true if the input string matches any of the specified denominations
    } else {
        return false; // Return false otherwise
    }
}

void clearInputBuffer() {
    // Check if the next character is the newline character and ignore
    if (std::cin.peek() == '\n') {
        std::cin.ignore();
    }
}
