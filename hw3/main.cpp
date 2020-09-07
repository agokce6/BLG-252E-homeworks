/*
 * Ahmet Gokce
 * 150180076
 * 24.05.2020
 * can be complied with -std=c++11 or -std=c++14
 */
#include <iostream>
#include <cmath>
#include <istream>
#include <fstream>
#include <sstream>
#include <string>
using namespace std;

class Matrix{ // a matrix which all values are same
    private:
        int rows;
        int columns;
        float * data;
    public:
        Matrix(int,int,float );
        ~Matrix();
        float & operator() (int,int); // access with index
        float* operator*(const float*); //vector multiplication
        float* operator+(const float*); //vector addition
};

Matrix::Matrix(int r, int c , float num) {
    if(r>0 && c>0){
        rows=r;
        columns=c;
        data = new float [rows*columns];
    }
    for(int i =0;i<rows*columns;i++){
        data[i] = num;
    }
}

Matrix::~Matrix(){
    delete [] data;
}

float & Matrix::operator()(int r, int c) {
    if(r<=rows && c<=columns){
        return data[columns* r + c];
    }
}

float * Matrix::operator*(const float* vector) {
    float* ptr = new float[rows];
    for (int k = 0; k < rows; ++k) {
        ptr[k]= 0;
    }
    for (int i = 0; i < rows; ++i) {
        for (int j = 0; j < columns; ++j) {
            ptr[i]+=data[columns*i+j]*vector[j];
        }
    }
    return ptr;
}

float * Matrix::operator+(const float * vector) {
    if(columns==1){
        float *ptr = new float[rows];
        for (int k = 0; k < rows; ++k) {
            ptr[k] = 0;
        }
        for (int i = 0; i < rows; ++i) {
            for (int j = 0; j < columns; ++j) {
                ptr[i] += data[columns * i + j] + vector[j];
            }
        }
        return ptr;
    }
}

class Neuron{
    protected:
        float value;
        float activated_value;
    public:
        Neuron();
        void setValue(float);
        float getValue() const;
        float getActivatedValue() const;
        virtual void activate() = 0;
        virtual ~Neuron() = default;
};
Neuron::Neuron() {
    value=0;activated_value=0;
}

void Neuron::setValue(float n) {
    value = n;
}

float Neuron::getValue() const {
    return value;
}

float Neuron::getActivatedValue() const {
    return activated_value;
}

class SigmoidNeuron: public Neuron{
    public:
        SigmoidNeuron();
        void activate();
};

void SigmoidNeuron::activate() {
    activated_value = 1/(1+exp(-value));
}

SigmoidNeuron::SigmoidNeuron():Neuron() {}

class ReluNeuron: public Neuron{
    public:
        ReluNeuron();
        void activate();

};
ReluNeuron::ReluNeuron():Neuron(){}

void ReluNeuron::activate() {
    activated_value = fmax(0,value);
}

class LReluNeuron: public Neuron{
    public:
        LReluNeuron();
        void activate();
};
LReluNeuron::LReluNeuron():Neuron(){}

void LReluNeuron::activate() {
    activated_value = fmax(0.1*value,value);
}

class Layer{
    private:
        Neuron* neurons;
        int neuronCount;
    public:
        Layer();
        Layer(int,int);
        ~Layer();
        void setValues(float *);
        int getNeuronCount() const;
        float* getActivatedValues();
        void printValues();
        float * getValues();
        void printActivatedValues();
        Layer& operator=(Layer const &);
};

Layer::Layer() {
    neurons = NULL;
    neuronCount=0;
}

Layer::Layer(int t, int c) {
    if (t == 0) {
        neurons = new SigmoidNeuron[c];
    }
    if (t == 1) {
        neurons = new LReluNeuron[c];
    }
    if (t == 2) {
        neurons = new ReluNeuron[c];
    }
    neuronCount = c;
}

Layer::~Layer() {
    delete [] neurons;
}

void Layer::setValues(float * x) {
    for(int i=0;i<neuronCount;i++){
        neurons[i].setValue(x[i]);
        neurons[i].activate();
    }
}

int Layer::getNeuronCount() const {
    return neuronCount;
}
float * Layer::getValues() {
    float* tm = new float[neuronCount];
    for(int i=0;i<neuronCount;i++){
        tm[i]=neurons[i].getValue();
    }
    return tm;
}

float * Layer::getActivatedValues() {
    float* tm = new float[neuronCount];
    for(int i=0;i<neuronCount;i++){
        tm[i]=neurons[i].getActivatedValue();
    }
    return tm;
}
void Layer::printValues() {
    for(int i=0;i<neuronCount;i++){
        cout<<neurons[i].getValue()<<endl;
    }
}

void Layer::printActivatedValues() {
    for(int i=0;i<neuronCount;i++){
        cout<<neurons[i].getActivatedValue()<<endl;
    }
}

Layer& Layer::operator=(Layer const & L) {
    this->neurons = L.neurons;
    this->neuronCount = L.neuronCount;
    return *this;
}

class Network{
    private:
        Layer* layers;
        int layerCount;
    public:
        Network(Layer*,int);
        ~Network();
        void calculate(int);
        void run();
        void print();
};

Network::Network(Layer*layers,int n) {
    this->layers = layers;
    layerCount = n;
}

Network::~Network() {
    delete [] layers;
}

void Network::calculate(int n) {
    if(n>0){
        float* previousValues;
        if(n==1)
            previousValues = layers[n-1].getValues();
        else
            previousValues = layers[n-1].getActivatedValues();
        Matrix W(layers[n].getNeuronCount(),layers[n-1].getNeuronCount(),0.1);
        float* tmp = NULL;
        tmp = W*previousValues;
        Matrix B(layers[n].getNeuronCount(),1,0.1);
        tmp = B+tmp;
        layers[n].setValues(tmp);
    }
}

void Network::run() {
    for (int i = 0; i < layerCount; ++i) {
        calculate(i);
    }
}

void Network::print() {
    cout<<"Layer "<<0<<":"<<endl;
    layers[0].printValues();
    for(int i =1;i<layerCount;i++){
        cout<<"Layer "<<i<<":"<<endl;
        layers[i].printActivatedValues();
    }
}

int main(int argc, char* argv[]) {
    string NetworkFilePath = argv[1];
    ifstream NetworkFile;
    NetworkFile.open(NetworkFilePath);
    if(!NetworkFile){
        cout<<"Network file cannot be found";
    }
    int layerCount;
    int* neuronCounts;
    int* neuronTypes;
    float* x;
    NetworkFile>>layerCount;
    neuronCounts = new int[layerCount];
    neuronTypes = new int[layerCount];
    for(int i=0;i<layerCount;i++){
        NetworkFile>>neuronCounts[i];
    }
    for(int i=0;i<layerCount;i++){
        try{
            NetworkFile >> neuronTypes[i];
            if(neuronTypes[i]>3 || neuronTypes[i]<0) throw "ERROR";
        }
        catch (char const *) {
            cout<<"Unidentified activation function!"<<endl;
            return 9999;
        }
    }
    NetworkFile.seekg(4*layerCount+5,ios::beg);
    string inputValues;
    getline(NetworkFile,inputValues);
    stringstream sss(inputValues);
    int sizeofX=0;
    float input;
    while (!sss.eof()){
        sss>>input;
        int newsizeofX = sizeofX+1;
        float* tmp = new float[newsizeofX];
        for (int i=0; i< sizeofX; ++i)
        {
            tmp[i] = x[i];
        }
        tmp[newsizeofX-1] = input;
        delete[] x;
        x = tmp;
        sizeofX = newsizeofX;
    }
    NetworkFile.close();

    Layer* layers = new Layer[layerCount];
    for(int i=0;i<layerCount;i++){
        Layer* tmp= new Layer(neuronTypes[i],neuronCounts[i]);
        *(layers+i) = *tmp;
    }
    try{
        if(sizeofX!=layerCount) throw "ERROR";
        layers[0].setValues(x);
    }
    catch (char const*) {
        cout<<"Input shape does not match!"<<endl;
        return 9999;
    }
    Network nw(layers,layerCount);
    nw.run();
    nw.print();

    return 0;
}
