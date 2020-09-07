/*
Name: Ahmet Gökçe (Gokce if it cannot be reading due to encoding issue)
Number: 150180076
Date 04.04.2020
*/
// complied with -std=c++11
#include <iostream>
#include <string>
#include<bits/stdc++.h>

using namespace std;

#define SMALL "small"
#define MEDIUM "medium"
#define BIG "big"
#define THICK "thick"
#define NORMAL "normal"
#define THIN "thin"

class Ingredient{
    private:
        string nameOfIng;
    public:
        Ingredient* next; //Ingredient pointer for linked list structure
        Ingredient(string);  //constructor
        ~Ingredient(); //destructor
        string getName() const;
        void addIngredient(string); //add new ingredient to this ingredient
};

Ingredient::Ingredient(const string ingr){
    nameOfIng = ingr;
    next = NULL;
}

string Ingredient::getName() const{
    return this->nameOfIng;
}


void Ingredient::addIngredient(const string newingr) {
    Ingredient* newIngr = new Ingredient(newingr);
    Ingredient *tmp;
    tmp = this;
    while(tmp->next) tmp = tmp->next;
    tmp->next = newIngr;
}

Ingredient::~Ingredient() {
    next = NULL;
}

class Drink{
    private:
        string nameOfDrink;
        int amount;
    public:
        Drink* next; //drink pointer for linked list structure
        Drink(string); //constructor
        ~Drink(); //destructor
        string getName()const;
        int getAmonut()const;
        void addDrink(string); //add new drink to this drink
};

Drink::Drink(const string drink) {
    nameOfDrink = drink;
    amount = 1;
    next = NULL;
}

Drink::~Drink() {
    this->next = NULL;
}

void Drink::addDrink(const string newdrink) {
    Drink* traverse;
    traverse=this;
    while(traverse){
        if(traverse->getName()==newdrink){
            traverse->amount+=1;
            return;
        }
        traverse = traverse->next;
    }
    traverse = this;
    while(traverse->next) traverse = traverse->next;
    Drink* drink = new Drink(newdrink);
    traverse->next = drink;
}

string Drink::getName() const{
    return this->nameOfDrink;
}

int Drink::getAmonut() const{
    return this->amount;
}

class Pizza{
    private:
        string pizzaName;
        string size;
        Ingredient* ingredients;
        string crustType;

    public:
        Pizza* next; //drink pointer for linked list structure
        Pizza(); //default constructor
        Pizza(string,string,int); //constructor with size, crust type, pizza type
        Pizza(Pizza&); //copy constructor
        ~Pizza(); //destructor
        string getName()const;
        string getSize()const;
        Ingredient* getIngredient()const;
        Ingredient* getIngredient(int)const; //getting with index number
        void printIngredients()const; //print all ingredients
        string getCrustType()const;
        void addPizza(Pizza&); //add new pizza to this pizza
        void removeIngredient(string); //remove ingredient from this pizza
};

Pizza::Pizza() {
    pizzaName = "default";
    size = MEDIUM;
    ingredients = new Ingredient("mozarella");
    crustType = "normal";
    next = NULL;
}

Pizza::Pizza(const string size, const string crust_type, const int pizza_type) {
    this->size = size;
    this->crustType = crust_type;

    ingredients = new Ingredient("mozarella");

    if(pizza_type == 1){
        pizzaName = "Chicken Pizza";
        ingredients->addIngredient("chicken");ingredients->addIngredient("mushroom");
        ingredients->addIngredient("corn");ingredients->addIngredient("onion");
        ingredients->addIngredient("tomato");
    }
    else if(pizza_type == 2){
        pizzaName = "Broccoli Pizza";
        ingredients->addIngredient("broccoli");ingredients->addIngredient("pepperoni");
        ingredients->addIngredient("olive");ingredients->addIngredient("corn");
        ingredients->addIngredient("onion");
    }
    else if(pizza_type == 3){
        pizzaName = "Sausage Pizza";
        ingredients->addIngredient("sausage");ingredients->addIngredient("tomato");
        ingredients->addIngredient("olive");ingredients->addIngredient("mushroom");
        ingredients->addIngredient("corn");
    }

    next = NULL;
}

Pizza::Pizza(Pizza& srcPizza) {
    this->pizzaName = srcPizza.getName();
    this->size = srcPizza.size;
    this->crustType = srcPizza.crustType;

    Ingredient* head = new Ingredient(srcPizza.getIngredient()->getName());
    ingredients = head;
    Ingredient* tmp = srcPizza.getIngredient()->next;
    while(tmp){ //copying ingredients of source pizza
        ingredients->addIngredient(tmp->getName());
        tmp = tmp->next;
    }
    this->next=NULL;
}

Pizza::~Pizza() {
    this->next = NULL;
    Ingredient* tmp ;
    while(ingredients){ //deleting all ingredients
        tmp = ingredients;
        ingredients = ingredients->next;
        delete tmp;
    }
}

string Pizza::getName() const{
    return this->pizzaName;
}

string Pizza::getSize()const {
    return this->size;
}

Ingredient* Pizza::getIngredient() const{ //returning first ingredient
    return this->ingredients;
}

Ingredient* Pizza::getIngredient(const int index) const{ //searching ingredient by index
    Ingredient* traverse = this->ingredients;
    for(int i=1;i<index;i++) traverse = traverse->next;
    return traverse;
}

void Pizza::printIngredients() const{
    Ingredient* traverse = NULL;
    traverse = getIngredient();
    int i = 1;
    while(traverse){ //traversing all ingredients and printing
        cout<<i<<". "<<traverse->getName()<<endl;
        traverse = traverse->next;
        i++;
    }
}

string Pizza::getCrustType() const{
    return this->crustType;
}

void Pizza::addPizza(Pizza &newPiz) {
    Pizza* traverse;
    traverse=this;
    while(traverse->next) traverse = traverse->next;
    traverse->next = &newPiz;
}

void Pizza::removeIngredient(const string toRemove) {
    Ingredient* traverse = this->ingredients;

    if(this->ingredients->getName() == toRemove){ //if removed ingredient is first element
        Ingredient * tmp = this->ingredients;
        ingredients = ingredients->next;
        delete tmp;
        return;
    }
    Ingredient* behind;
    behind = traverse;
    traverse = traverse->next;
    while(traverse){ //otherwise traversing and finding it
        if(traverse->getName()==toRemove){
            behind->next = traverse->next;
            delete traverse;
            return;
        }
        behind = traverse;
        traverse = traverse->next;
    }
}

class Order{
    private:
        string customerName;
        Pizza* pizza_s;
        Drink* drink_s;
    public:
        Order* next; //order pointer for linked list structure
        Order(string,Pizza&); //constructor with customer name and pizzas
        Order(string,Pizza&,Drink&); //constructor with customer name, pizzas and drinks
        ~Order(); //destructor
        string getCustomerName()const;
        float getPrice()const;
        void printOrder()const; //printing order content
};

Order::Order(const string cusName , Pizza& pizzas) {
    customerName = cusName;
    this->pizza_s = NULL;
    pizza_s = &pizzas;
    this->drink_s = NULL;
    this->next = NULL;
}

Order::Order(const string cusName, Pizza & pizzas, Drink & drinks) {
    customerName = cusName;
    this->pizza_s = NULL;
    pizza_s = &pizzas;
    this->drink_s = NULL;
    drink_s = &drinks;
    this->next = NULL;
}

Order::~Order() {
    this->next = NULL;
    Pizza* tmpP;
    Drink* tmpD;
    while(pizza_s){ //deleting pizzas
        tmpP = pizza_s;
        pizza_s = pizza_s->next;
        delete tmpP;
    }
    while(drink_s){ //deleting drinks if there are
        tmpD = drink_s;
        drink_s = drink_s->next;
        delete tmpD;
    }
}

string Order::getCustomerName() const {
    return this->customerName;
}

float Order::getPrice() const{
    Pizza* traverseP;
    Drink* traverseD;
    traverseP = this->pizza_s;
    traverseD = this->drink_s;

    float totalPrice = 0;

    while(traverseP){ //calculating price of pizzas
        if(traverseP->getSize() == SMALL) totalPrice+=15;
        else if(traverseP->getSize()==MEDIUM) totalPrice+=20;
        else if(traverseP->getSize()==BIG) totalPrice+=25;
        traverseP = traverseP->next;
    }

    while(traverseD){ //calculating price of drinks if there are
        if(traverseD->getName() == "cola" || traverseD->getName() == "Cola") totalPrice+=4*traverseD->getAmonut();
        else if(traverseD->getName() == "soda" || traverseD->getName() == "Soda") totalPrice+=2*traverseD->getAmonut();
        else if(traverseD->getName() == "ice tea" || traverseD->getName() == "Ice Tea") totalPrice+=3*traverseD->getAmonut();
        else if(traverseD->getName() == "fruit juice" || traverseD->getName() == "Fruit Juice") totalPrice+=3.5*traverseD->getAmonut();
        traverseD = traverseD->next;
    }

    return totalPrice;
}

void Order::printOrder() const{
    cout<<"- - - - - - - - - - - -"<<endl;
    cout<<"Name :"<<customerName<<endl<<endl;
    Pizza* traverseP = this->pizza_s;
    while (traverseP){ //printing pizzas
        cout<<traverseP->getName()<<"(";
        Ingredient* traverseI = traverseP->getIngredient();
        while(traverseI){
            cout<<traverseI->getName()<<", ";
            traverseI = traverseI->next;
        }cout<<")"<<endl;
        cout<<"size: "<<traverseP->getSize()<<", "<<"crust: "<<traverseP->getCrustType()<<endl<<endl;
        traverseP = traverseP->next;
    }
    Drink* traverseD = this->drink_s;
    while(traverseD){ //printing drinks there are
        cout<<traverseD->getAmonut()<<" "<<traverseD->getName()<<", ";
        traverseD = traverseD->next;
    }cout<<endl;
    cout<<"- - - - - - - - - - - -"<<endl;
}

class OrderList{
    private:
        int n;
        Order* head;
    public:
        OrderList(); //default constructor
        ~OrderList(); //destructor
        void takeOrder();
        void listOrders() const; //printing all orders
        void deliverOrder();
};

OrderList::OrderList(){
    n = 0;
    this->head = NULL;
}

OrderList::~OrderList() {
    Order* tmp;
    while(head){ //deleting orders
        tmp = head;
        head = head->next;
        delete tmp;
    }
}

void OrderList::takeOrder() {
    int type;
    string size;
    string crust_type;
    int amount;
    string name;

    cout<<"Pizza Menu"<<endl;
    cout<<"1. "<<"Chicken Pizza (mozarella, chicken, mushroom, corn, onion, tomato)"<<endl;
    cout<<"2. "<<"Broccoli Pizza (mozarella, broccoli, pepperoni, olive, corn, onion)"<<endl;
    cout<<"3. "<<"Sausage Pizza (mozarella, sausage, tomato, olive, mushroom, corn)"<<endl;
    cout<<"0. Back to main menu"<<endl;
    while(true){
        cin>>type; //taking type
        if(type == 0) return;
        else if(type>3 || type<0) {
            cout<<"invalid input try again"<<endl;
            continue;
        }
        else break;
    }

    cout<<"Select size: small (15 TL), medium(20 TL), big (25 TL)"<<endl;
    while(true){
        cin>>size; //taking size
        transform(size.begin(), size.end(), size.begin(), ::tolower);
        if(size != SMALL && size != MEDIUM && size != BIG) {
            cout<<"invalid input"<<endl;
            continue;
        }
        else break;
    }

    cout<<"Select crust type: thin, normal, thick"<<endl;
    while(true){
        cin>>crust_type; //taking crust type
        transform(crust_type.begin(), crust_type.end(), crust_type.begin(), ::tolower);
        if(crust_type != THIN && crust_type != NORMAL && crust_type != THICK) {
            cout<<"invalid input try again"<<endl;
            continue;
        }
        else break;
    }

    cout<<"Enter the amount:";
    while(true){
        cin>>amount; //taking amount of pizzas
        if(amount<1) {
            cout<<"invalid input try again"<<endl;
            continue;
        }
        else break;
    }

    Pizza* newPizza = new Pizza(size,crust_type,type); //creating a pizza with given preferences
    if(amount>1){ //if there are more than one pizza order
        for(int i=0;i<amount-1;i++){
            Pizza* cpyPizza = new Pizza(*newPizza);
            newPizza->addPizza(*cpyPizza);
        }
        cout<<"Please enter the number of the ingredient you want to remove from your pizza"<<endl;
        newPizza->printIngredients();
        cout<<"Press 0 to save it."<<endl;
        int n=-1;
        while(n!=0){ //removing ingredients
            cin>>n;
            if(n==0) break;
            if(n>6 || n<0){
                cout<<"invalid input try again"<<endl;
                continue;
            }
            Pizza* traverse = newPizza->next;
            while(traverse){
                traverse->removeIngredient(newPizza->getIngredient(n)->getName());
                traverse = traverse->next;
            }
        }
    }

    cout<<"Please choose a drink:"<<endl;
    cout<<"0. "<<"no drink"<<endl;
    cout<<"1. "<<"cola "<<"4 TL"<<endl;
    cout<<"2. "<<"soda "<<"2 TL"<<endl;
    cout<<"3. "<<"ice tea "<<"3 TL"<<endl;
    cout<<"4. "<<"fruit juice "<<"3.5 TL"<<endl;
    cout<<"Press -1 for save your order";
    int n = -2;
    bool isThereDrink = true;
    Drink* newDrink = NULL;
    while(n !=-1){ //adding drinks
        cin>>n;
        if(n ==-1) break;
        if(n<-1 || n>4){
            cout<<"invalid input try again"<<endl;
            continue;
        }
        if(n == 0 ){
            isThereDrink = false;
            Drink* tmp;
            while(newDrink){
                tmp = newDrink;
                newDrink = newDrink->next;
                delete tmp;
            }
            break;
        }
        else if(n == 1){
            if(newDrink != NULL) newDrink->addDrink("cola");
            else newDrink = new Drink("cola");
        }
        else if(n == 2){
            if(newDrink != NULL) newDrink->addDrink("soda");
            else newDrink = new Drink("soda");
        }
        else if(n == 3){
            if(newDrink != NULL) newDrink->addDrink("ice tea");
            else newDrink = new Drink("ice tea");
        }
        else if(n == 4){
            if(newDrink != NULL) newDrink->addDrink("fruit juice");
            else newDrink = new Drink("fruit juice");
        }
    }

    cout<<"Please enter your name:"<<endl;
    getline(cin.ignore(),name);
    cout<<"Your order is saved, it will be delivered when it is ready..."<<endl;

    Order* newOrder;
    if(isThereDrink) newOrder = new Order(name,*newPizza,*newDrink); //if any drink is ordered
    else newOrder = new Order(name,*newPizza); //otherwise

    if(head == NULL) head = newOrder; //if there is no order head is new order
    else{ //otherwise find end of orders and add to there
        Order* traverse = head;
        while(traverse->next) traverse = traverse->next;
        traverse->next = newOrder;
    }
    n+=1;
    newOrder->printOrder();
}

void OrderList::listOrders() const{
    if(head == NULL){ //if there is no order
        cout<<"empty"<<endl;
        return;
    }
    Order* traverse = head;
    int n = 1;
    while(traverse){ //traversing orders and printing them
        cout<<n<<endl;
        traverse->printOrder();
        traverse = traverse->next;n++;
    }
}

void OrderList::deliverOrder() {
    string customerName;
    float totalPrice;
    if(head == NULL){ //if there is no order
        cout<<"empty"<<endl;
        return;
    }
    this->listOrders();
    cout<<"Please write the customer name in order to deliver his/her order: ";
    getline(cin.ignore(),customerName);

    Order* toDeliver=NULL;
    Order* traverse = head;

    if(head->getCustomerName() == customerName){ //if head will be delivered
        toDeliver = head;
        head = head->next;
        traverse = NULL;
    }
    Order* behind;
    if(toDeliver==NULL){
        behind = traverse;
        traverse = traverse->next;
    }
    while(traverse){ //if delivered order is in orders
        if(traverse->getCustomerName() == customerName){
            toDeliver = traverse;
            behind->next = traverse->next;
            break;
        }
        behind = traverse;
        traverse = traverse->next;
    }

    if(toDeliver==NULL) { //if the name could not find
        cout<<"Wrong name"<<endl;
        return;
    }
    cout<<"Following order is delivering..."<<endl;
    totalPrice = toDeliver->getPrice();
    toDeliver->printOrder();
    cout<<"Total price: "<<totalPrice<<endl;
    string pCode;
    while(true){
        cout<<"Do you have a promotion code? (y/n)"<<endl;
        cin>>pCode;
        if(pCode=="n") break;
        else if(pCode=="y") { //if user has promotion code
            string promotionCode;
            cout<<"Please enter your code:";
            getline(cin.ignore(),promotionCode);
            if(promotionCode == "I am a student"){
                totalPrice *= 0.9; //appling a 10% discount
                cout<<"Discounted price: "<<totalPrice<<endl;
                break;
            }
            else{
                cout<<"Promotion code is not accepted."<<endl;
                continue;
            }
        }
        else{
            cout<<"invalid input try again"<<endl;
            continue;
        }
    }
    delete toDeliver;
    cout<<"The order is delivered successfully!"<<endl<<endl;
}

int main() {
    OrderList ol;
    int c=0;
    while(true){
        cout<<"Welcome to Unicorn Pizza!"<<endl;
        cout<<"1. "<<"Add an order"<<endl;
        cout<<"2. "<<"List orders"<<endl;
        cout<<"3. "<<"Deliver order"<<endl;
        cout<<"4. "<<"Exit"<<endl;
        cout<<"Choose what to do: ";cin>>c;
        if(c==1){
            ol.takeOrder();
            continue;
        }
        else if (c==2){
            ol.listOrders();
            continue;
        }
        else if (c==3){
            ol.deliverOrder();
            continue;
        }
        else if(c==4){
            cout<<"Goodbye...";
            break;
        }
        else{
            cout<<"invalid input try again"<<endl;
        }
    }
    return 0;
}
