#include <iostream>
#include <cstring>
using namespace std;

///------------------------------------------------------------------------------CLASA COMPLEX---------------------------------------------------------------------------------

class Complex{

    float re, im;

public:

    Complex(float x, float y);

    Complex(const Complex & ob);

    ~Complex();

    void update(float x, float y);

    void update_re(float x);

    void update_im(float y);

    float get_re();

    float get_im();

    bool check_imaginary();

    friend ostream & operator <<(ostream &o, const Complex &ob);
    friend istream & operator >>(istream &i, Complex &ob);

    operator =(const Complex & ob){

        this -> re = ob.re;
        this -> im = ob.im;

        ///cout<<"Assignment operator for complex number "<<re<<" +"<<im<<"i called"<<'\n';
    }
};

///-----------------------------------------------------------------------------METODE CLASA COMPLEX--------------------------------------------------------------------------------------------------

Complex::Complex(float x = 0, float y = 0){

    re = x;
    im = y;

    ///cout<<"Counstructor for complex number "<<re<<" +"<<im<<"i called"<<'\n';
}

Complex::Complex(const Complex & ob){

    this -> re = ob.re;
    this -> im = ob.im;

    ///cout<<"Copy counstructor for complex number "<<re<<" +"<<im<<"i called"<<'\n';
}

Complex::~Complex(){

    ///cout<<"Destructor for complex number "<<re<<" + "<<im<<"i called"<<'\n';
}

void Complex::update(float x, float y){

    re = x;
    im = y;
}

void Complex::update_re(float x){

    re = x;
}

void Complex::update_im(float y){

    im = y;
}

float Complex::get_re(){

    return re;
}


float Complex::get_im(){

    return im;
}

bool Complex::check_imaginary(){

    if(re == 0 && im != 0)
        return 1;

    return 0;
}

ostream & operator <<(ostream &o, const Complex & ob){

    if(ob.im == 0)
        o<<ob.re;
    else if(ob.re == 0)
        o<<ob.im<<"i";
    else
        o<<ob.re<<" + "<<ob.im<<"i";

    return o;
}

istream & operator >>(istream &i, Complex &ob){

    i>>ob.re>>ob.im;

    return i;
}

///--------------------------------------------------------------------------------------CLASA VECTOR-------------------------------------------------------------------------------------

class Vector{

protected:

    int dim;
    int capacity;

    Complex *v;
    Complex *org; /// pointer la pozitia initiala a vectorului, pentru a ajuta la delete

public:

    Vector(int k);

    Vector(const Vector & vaux);

    virtual ~Vector();

    int get_dim();

    int get_capacity();

    Complex get_element(int i);

    bool check_imaginary();

    void add(Complex k);

    virtual void change(int i, Complex k);

    virtual void pop();

    virtual void print(ostream & o) const; /// folosit pentru conceptul de run time polymorphism , la fel ca si functia urmatoare read()

    virtual void read(istream & i);

    friend ostream & operator <<(ostream & o, const Vector & ob);
    friend istream & operator >>(istream & i, Vector & ob);

    operator =(const Vector & ob){

        ///cout<<"Assignment operator for vector of complex numbers with dim "<<dim<<" and capacity "<<capacity<<" called"<<'\n';

        dim = ob.dim;
        capacity = ob.capacity;

        if(dim > 0){

            delete[] org;

            v = new Complex[capacity];
            org = v;

            for(int i = 0; i < dim; i++){

                v[i] = ob.v[i];
            }
        }
        else{

            v = NULL;
            org = NULL;
        }
    }
};

///----------------------------------------------------------------------------METODE CLASA VECTOR------------------------------------------------------------------------------------

Vector::Vector(int k = 0){

    if(k < 0)
        throw "EXCEPTION: invalid lenght; vector intialized with lenght 0 to avoid future errors";

    dim = k;
    capacity = k;

    ///cout<<"Constructor for vector of complex numbers with dim "<<dim<<" and capacity "<<capacity<<" called"<<'\n';

    if(dim > 0){

        v = new Complex[capacity];
        org = v;
    }
    else{

        v = NULL;
        org = NULL;
    }
}

Vector::Vector(const Vector & vaux){

    this -> dim = vaux.dim;
    this -> capacity = vaux.capacity;

    if(dim > 0){

        ///delete[] org;

        this -> v = new Complex[capacity];
        this -> org = this -> v;

        for(int i = 0; i < dim ; i++){

            ///*(this -> v + i)= *(vaux.v + i);
            v[i] = vaux.v[i];
        }
    }
    else{

        this -> v = NULL;
        this -> org = NULL;
    }

    ///cout<<"Copy constructor for vector of complex numbers with dim "<<dim<<" and capacity "<<capacity<<" called"<<'\n';
}

Vector::~Vector(){

    ///cout<<"Destructor for vector of complex numbers with dim "<<dim<<" and capacity "<<capacity<<" called"<<'\n';

    if(capacity > 0)
        delete[] org;
}

int Vector::get_dim(){

    return dim;
}

int Vector::get_capacity(){

    return capacity;
}

Complex Vector::get_element(int i){

    if(i < 0 || i >= dim)
        throw "EXCEPTION: index i does not refer to any position in container!";

    return v[i];
}

bool Vector::check_imaginary(){

    if(dim == 0)
        throw "EXCEPTION: Invalid call to bool Vector::check_imaginary(); Vector is empty!";

    for(int i = 0; i < dim; i++){

        if(!v[i].check_imaginary())
            return 0;
    }

    return 1;
}

void Vector::add(Complex k){

    dim += 1;

    if(dim > capacity){

        capacity = dim;

        Complex *resized = new Complex[dim];

        for(int i = 0; i < dim - 1; i++){

            resized[i] = v[i];
        }

        resized[dim - 1] = k;

        delete[] org;

        v = resized;
        org = resized;
    }
    else{

        v[dim] = k;
    }
}

void Vector::change(int i, Complex k){

    if(i < 0 || i >= dim)
        throw "EXCEPTION: index i does not refer to any position in container!";

    v[i].update(k.get_re(), k.get_im());
}

void Vector::pop(){

    if(dim == 0)
        throw "EXCEPTION: Container already empty!";

    dim -= 1;
}

void Vector::print(ostream & o) const{

    if(dim == 0){

        o<<"(Empty container) ";

        return;
    }

    o<<"|  ";
    for(int i = 0; i < dim; i++){

        o<<v[i]<<"  |  ";
    }
}

void Vector::read(istream & i){

    int auxdim;
    i >> auxdim;

    if(auxdim < 0)
        throw "EXCEPTION: dimension input is negative; returning without any change to the data structure";

    Complex aux;

    int k;

    for(k = 0; k < min(dim, auxdim); k++){

        i >> aux;
        v[k].update(aux.get_re(), aux.get_im());
    }

    for(int j = k; j < auxdim; j++){

        i >> aux;
        add(aux);
    }

    dim = auxdim;

    if(dim > capacity)
        capacity = dim;
}

ostream & operator <<(ostream & o, const Vector & ob){

    ob.print(o);

    return o;
}

istream & operator >>(istream & i, Vector & ob){

    try{

        ob.read(i);
    }
    catch(const char * err){

        throw;
    }

    return i;
}

///-------------------------------------------------------------------------CLASA STACK: VECTOR------------------------------------------------------------------------------------------

class Stack: public Vector{

public:

    Stack(int k);

    Stack(const Stack & ob);

    ~Stack();

    void change(int i, Complex k);

    void pop();
};

///--------------------------------------------------------------------------METODE CLASA STACK: VECTOR--------------------------------------------------------------------------------------------

Stack::Stack(int k = 0): Vector(k){

    ///cout<<"Constructor for stack of complex numbers with dim "<<this -> get_dim()<<" and capacity "<<this -> get_capacity()<<" called"<<'\n';
}

Stack::Stack(const Stack & ob): Vector(ob){

    ///cout<<"Copy constructor for stack of complex numbers with dim "<<this -> get_dim()<<" and capacity "<<this -> get_capacity()<<" called"<<'\n';
}

Stack::~Stack(){

    ///cout<<"Destructor for stack of complex numbers with dim "<<this -> get_dim()<<" and capacity "<<this -> get_capacity()<<" called"<<'\n';
}

void Stack::change(int i, Complex k){

    cout<<'\n'<<"FORBIDDEN OPERATION: attempt to change element in stack;"<<'\n';
}

void Stack::pop(){

    Vector::pop();
}

///--------------------------------------------------------------------------CLASA QUEUE: VECTOR------------------------------------------------------------------------------------------

class Queue: public Vector{

public:

    Queue(int k);

    Queue(const Queue & ob);

    ~Queue();

    void change(int i, Complex k);

    void pop();
};

///-----------------------------------------------------------------------------METODE CLASA QUEUE: VECTOR---------------------------------------------------------------------------------------

Queue::Queue(int k = 0): Vector(k){

    ///cout<<"Constructor for queue of complex numbers with dim "<<this -> get_dim()<<" and capacity "<<this -> get_capacity()<<" called"<<'\n';
}

Queue::Queue(const Queue & ob): Vector(ob){

    ///cout<<"Copy constructor for queue of complex numbers with dim "<<this -> get_dim()<<" and capacity "<<this -> get_capacity()<<" called"<<'\n';
}

Queue::~Queue(){

    ///cout<<"Destructor for queue of complex numbers with dim "<<this -> get_dim()<<" and capacity "<<this -> get_capacity()<<" called"<<'\n';
}

void Queue::change(int i, Complex k){

    cout<<'\n'<<"FORBIDDEN OPERATION: attempt to change element in queue;"<<'\n';
}

void Queue::pop(){

    if(dim == 0)
        throw "EXCEPTION: Container already empty!";

    v = v + 1;

    dim -= 1;
    capacity -= 1;
}

///--------------------------------------------------------------

Vector ** read_array(int n){         ///functie de citire a n obiecte

    Vector **aux = new Vector*[n];

    int type;

    for(int i = 0; i < n; i++){

        cin>>type;

        if(type == 0)
            aux[i] = new Vector;
        else if(type == 1)
            aux[i] = new Stack;
        else if(type == 2)
            aux[i] = new Queue;

        try{

            cin>>*aux[i];
        }
        catch(const char * err){

            cout<<"EXCEPTION: array of objects could not be read; problem"<<" at element "<<i<<" : "<<err<<'\n';
        }
    }

    return aux;
}

void print_array(Vector **aux, int n){

    for(int i = 0; i < n; i++)
        cout<<*aux[i]<<'\n';
}

int main(){

    /*Vector v(3);                        /// initializare vector cu dimensiune prestabilita si afisare
    cout<<v<<'\n'<<'\n';

    cin>>v;                               ///citire vector
    cout<<v<<'\n'<<'\n';

    Complex aux(5, 6);                  ///schimbare element din vector
    v.change(0, aux);
    cout<<v<<'\n'<<'\n';

    Stack s;    /// initializare fara parametru             ///exemplificare stack

    Complex c[4];
    for(int i = 0; i <= 3; i++){

        cin>>c[i];

        s.add(c[i]);
    }

    s.change(1, c[3]);

    cout<<s<<'\n'<<'\n';
    s.pop();                                            /// pop pentru stack
    cout<<s<<'\n'<<'\n';
    s.pop();
    cout<<s<<'\n'<<'\n';
    s.pop();
    cout<<s<<'\n'<<'\n';
    s.pop();
    cout<<s<<'\n'<<'\n';
    s.pop();
    cout<<s<<'\n'<<'\n';

    Queue q(2);

    cin>>q;
    cout<<q<<'\n'<<'\n';

    Complex aux2(-8, 7.88);
    q.add(aux2);
    cout<<q<<'\n'<<'\n';

    q.pop();
    cout<<q<<'\n'<<'\n';
    q.pop();
    cout<<q<<'\n'<<'\n';

    Vector &qq = q;

    Stack s1(4);
    Vector &ss1 = s1;

    ss1 = qq;

    cout<<s1<<'\n'<<'\n';*/

    /*Vector a1;
    cin>>a1;

    cout<<a1<<'\n'<<'\n';

    Vector a2 = a1;                 ///overload copy constr
    Vector a3(7);                   ///overload atribuire
    a3 = a1;

    a2.pop();

    a3.pop();
    a3.pop();

    cout<<a1<<'\n'<<a2<<'\n'<<a3<<'\n'<<'\n';*/

///----------------------------------------------------------------READ ARRAY OF OBJECTS-----------------------------------------------------------------------------------

    /*int n;                        ///functie de citit si afisat array de obiecte diferite (vector/ stack/ queue)
    cin>>n;                         /// am inclus o exceptie si in aceasta functie

    Vector **a;

    a = read_array(n);
    print_array(a, n);*/

///------------------------------------------------------------------------EXCEPTION HANDLING-----------------------------------------------------------------------------

    try{

        Vector a(-2);
    }
    catch(const char * err){

        cout<<err<<'\n';
    }

    try{
        Vector v(3);

        Complex x(5, 6);

        v.change(1000, x);
    }
    catch(const char * err){

        cout<<err<<'\n';
    }

    try{
        Vector v(3);
        v.get_element(1000);
    }
    catch(const char * err){

        cout<<err<<'\n';
    }

    try{

        Vector v;
        v.pop();
    }
    catch(const char * err){

        cout<<err<<'\n';
    }

    try{
        Vector v;

        cout<<v.check_imaginary()<<'\n';
    }
    catch(const char * err){

        cout<<err<<'\n';
    }

    try{

        Queue q;
        q.pop();
    }
    catch(const char * err){

        cout<<err<<'\n';
    }

    try{

        Vector v;

        cin>>v;             /// exceptie afisata daca se da o dimensiune < 0; am folosit si un throw; intermediar in orverload ul lui >>
    }
    catch(const char * err){

        cout<<err<<'\n';
    }



    return 0;
}
