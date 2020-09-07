/*
Ahmet Gökçe
150180076
29.042020
*/

#include <iostream>
using namespace std;

class Person{
    protected:
        string name;
        string surname;
    public:
        Person();
        Person(string,string);
        string getName();
        string getSurname();
};

Person::Person() {
    name="";
    surname="";
}

Person::Person(string name, string surname) {
    this->name=name;
    this->surname=surname;
}

string Person::getName() {
    return name;
}

string Person::getSurname() {
    return surname;
}

class Owner:public Person{
    private:
        float ownership;
    public:
        Owner();
        Owner(string,string);
        void setOwnership(float );
        float getOwnership() const;
};

Owner::Owner():Person() {ownership=0;}

Owner::Owner(string name, string surname):Person(name,surname) {
    ownership=0;
}
void Owner::setOwnership(float ownership) {
    this->ownership=ownership;
}
float Owner::getOwnership() const {
    return ownership;
}

class Courier:public Person{
    private:
        string vehicle;
        int capacity;
    public:
        Courier();
        Courier(string,string,string);
        string getVehicle() const;
        int getCapacity() const;
        bool operator==(const Courier&);
};

Courier::Courier():Person() {capacity=0;}

Courier::Courier(string name,string surname,string vehicle) : Person(name, surname) {
    if(vehicle!="car" && vehicle!="motorcycle" && vehicle!="bicycle"){
        cerr<<"Vehicle Name is invalid!!!";
        this->vehicle="?";
        return;
    }
    if(vehicle=="car")
        capacity = 200;
    if(vehicle=="motorcycle")
        capacity = 35;
    if(vehicle=="bicycle")
        capacity = 10;
    this->vehicle=vehicle;
}

string Courier::getVehicle() const {
    return vehicle;
}

int Courier::getCapacity() const{
    return capacity;
}

bool Courier::operator==(const Courier& c) {
    return this->name == c.name && this->surname == c.surname && this->vehicle == c.vehicle;
}

class Business{
    private:
        string name;
        string address;
        Owner* owners;
        int ownercount;
        Courier* couriers;
        int couriercount;
    public:
        Business(string,string,Owner*,int);
        void hire_courier(Courier&);
        void fire_courier(Courier&);
        void list_couriers() const ;
        void list_owners() const ;
        int calculate_shipment_capacity();
        void operator()();
        Courier operator[](int);
};

Business::Business(string name, string address, Owner* owners, int ownercount) {
    this->name = name;
    this->address=address;
    float ownership = 100.0/ownercount;
    for(int i=0;i<ownercount;i++){
        owners[i].setOwnership(ownership);
    }
    this->owners = owners;
    this->ownercount=ownercount;
    couriers = NULL;
    couriercount = 0;
}

void Business::hire_courier(Courier & newCourier) {
    Courier* tmp = new Courier[couriercount + 1];
    copy(couriers,couriers+couriercount,tmp);
    tmp[couriercount] = newCourier;
    couriercount++;
    delete [] couriers;
    couriers = new Courier[couriercount]();
    copy(tmp,tmp+couriercount,couriers);
    delete [] tmp;
    return;
}

void Business::fire_courier(Courier & firedCourier) {
    if(couriers == NULL)
        return;
    for(int i=0;i<couriercount;i++){
        Courier c = couriers[i];
        if(c == firedCourier){
            delete &c;
            Courier* tmp = new Courier[couriercount-1];
            copy(couriers,couriers+i,tmp);
            couriercount--;
            for(int y=i;y<couriercount;y++){
                tmp[y]=couriers[y+1];
            }
            delete [] couriers;
            couriers = new Courier[couriercount];
            copy(tmp,tmp+couriercount,couriers);
            delete [] tmp;
            return;
        }
    }
    cerr<<"Courier could not found!";
}

void Business::list_couriers() const {
    for(int i=0;i<couriercount;i++){
        Courier c= couriers[i];
        cout<<c.getName()<<" "<<c.getSurname()<<" "<<c.getVehicle()<<endl;
    }
}

void Business::list_owners() const {
    for(int i=0;i<ownercount;i++){
        Owner o= owners[i];
        cout<<o.getName()<<" "<<o.getSurname()<<" "<<o.getOwnership()<<endl;
    }
}

int Business::calculate_shipment_capacity() {
    int capacity=0;
    for(int i =0;i<couriercount;i++){
        capacity+=couriers[i].getCapacity();
    }
    return capacity;
}

void Business::operator()() {
    cout<<this->name<<" ";
    cout<<this->address<<endl;
    list_owners();
    list_couriers();
}

Courier Business::operator[](int index) {
    if(index<0 || index>couriercount){
        cerr<<"Courier could not found with given index,first courier is returned";
        return couriers[0];}
    return couriers[index];
}