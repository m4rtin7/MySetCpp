#include <sstream>
#include <vector>
#include <array>
#include <cmath>
#include <memory>

using namespace std;

//Cely projekt je len v jednom .h subore kedze som sa stretol s problemom ohladom linkera
//pri template classe a jedine riesenie, ktore som nasiel je prave toto.
//Projekt predstavuje implementaciu mojej triedy set, ktora je homogenna. Ma niektore funkcie, ktore ma aj klasicky set
//no obsahuje aj ine funkcie, ktore mne osobne prisli zaujimave.
//Dalej projekt obsahuje triedu SetIterator, ktora sa stara o iterovanie tymto setom.

template <typename Set>
class SetIterator{
private:
    using ValueType = typename Set::ValueType;
    using ConstReferenceType = const ValueType &;
    //zvolil som constRef, kedze potom by nastaval problem s tym,
    // ze by prvok po pozmeneni nemusel byt v spravnom buckete v hashTable

public:
    SetIterator(vector<vector<ValueType>>& ref, size_t arr, size_t vec):set(ref),arrIndex(arr),vecIndex(vec){};
    SetIterator& operator++(){
        if(! set[arrIndex].empty() && vecIndex < set[arrIndex].size()-1){
            vecIndex++;
            return *this;
        }
        arrIndex++;
        vecIndex = 0;
        while(set[arrIndex].empty()){
            arrIndex++;
        }
        return *this;
    }
    ConstReferenceType operator* (){
        return set[arrIndex][vecIndex];
    }
    bool operator==(const SetIterator& other) const{
        if(arrIndex == other.arrIndex && vecIndex == other.vecIndex){
            set[arrIndex].pop_back();
            return true;
        }
        return false;
    }
    bool operator!=(const SetIterator& other) const{
        if(arrIndex == other.arrIndex && vecIndex == other.vecIndex){
            set[arrIndex].pop_back();
            return false;
        }
        return true;
    }

private:
    vector<vector<ValueType>>& set;
    size_t arrIndex;
    size_t vecIndex;
};

template <typename T>
class MySet{
public:
    using Iterator = SetIterator<MySet<T>>;
    using ValueType = T;
private:
    size_t len = 0;
    size_t fullness = 0;
    size_t firstAr = 0;
    size_t lastAr = 0;
    size_t lastVe = 0;
    size_t hashconst;
    vector<vector<T>> hashTable;

    size_t myHash(const T& item, size_t num) const;
    void findFirst();
    void findLast();
    void expand(); // zvacsenie hashTable pri urcitej zaplnenosti
    void shrink(); // zmensenie hashTable pri urcitej zaplnenosti

public:
    MySet():hashconst(7){ hashTable.resize(hashconst);}
    ~MySet() = default;  //pre zachovanie rule of 3/5/0
    MySet(const MySet& other):hashconst(other.hashconst),hashTable(other.hashTable),
                                len(other.len),fullness(other.fullness){}
    MySet<T>& operator=(MySet<T> const & other);

    void add(T item); // prida item do Setu
    const T* find(T item); // zisti ci sa dany prvok nachadza v Sete, ak ano vrati nan ptr, ak nie vrati nullptr
    bool del(T item); // ak sa item nachadza v sete vymaze ho a vrati true, inak vrati false
    size_t length() const{return len;}; // vrati dlzku setu
    void print();
    void addByDelimiter(const string& str, char delimiter); //prida itemy zo stringu oddelene delimiterom
    //na rozdiel od malej verzie som sa rozhodol namiesto "string str" pouzit "const string& str"
    // preto aby bola tato funkcia funkcna ako pre lvalue tak aj pre rvalue

    void operator+=(MySet<T>& other); //prida do setu obsah druheho setu
    void operator-=(MySet<T>& other); //odobere zo setu obsah druheho setu
    bool operator==(MySet<T>& other);
    bool operator!=(MySet<T>& other);

    Iterator begin() {findFirst(); return Iterator(hashTable,firstAr,0);}
    //vrati iterator na pociatocny prvok setu

    Iterator end() {findLast(); return Iterator(hashTable,lastAr,lastVe);}
    //vrati iterator na koncovy prvok setu (presnejsie iterator na prvok za nim)

};

template<typename T>
void MySet<T>::expand() {
    hashconst = hashconst * 2 + 1;
    vector<vector<T>> newHashTab(hashconst);
    fullness = 0;

    auto first = begin();
    auto last = end();

    while(first != last){
        size_t h = myHash(*first, hashconst);
        if(newHashTab[h].empty()){
            fullness++;
        }
        newHashTab[h].push_back(*first);
        ++first;
    }
    hashTable = newHashTab;
}
template<typename T>
void MySet<T>::shrink() {
    hashconst = (hashconst-1) / 2;
    vector<vector<T>> newHashTab(hashconst);
    fullness = 0;

    auto first = begin();
    auto last = end();

    while(first != last){
        size_t h = myHash(*first, hashconst);
        if(newHashTab[h].empty()){
            fullness++;
        }
        newHashTab[h].push_back(*first);
        ++first;
    }
    hashTable = newHashTab;
}

template<typename T>
void MySet<T>::findFirst(){
    firstAr = 0;
    if(len == 0){
        return;
    }

    for(auto& v: hashTable){
        if(!v.empty()){
            return;
        }
        firstAr++;
    }
}
template<typename T>
void MySet<T>::findLast(){
    if(len == 0){
        lastAr = 0;
        lastVe = 0;
        return;
    }
    size_t ret = hashTable.size()-1;
    for(size_t i = ret; i >= 0; i-- ){
        if(!hashTable[i].empty()){
            lastAr = i;
            lastVe = hashTable[i].size();
            hashTable[i].push_back(hashTable[i][lastVe-1]);
            return;
        }
    }
}


template<> size_t MySet<string>::myHash(const string& item, size_t num) const{
    size_t ret = 0;
    for (char const &c: item) {
        ret += c;
    }
    return (ret * size_t(item.length())) % num;
}

template <class T>
size_t MySet<T>::myHash(const T& item, size_t num) const{
    return abs(int(item) * 2654435761) % num;
}

template <class T>
void MySet<T>::add(T item){
    if(find(item) != nullptr){
        return;
    }
    if( (double(fullness) / double(hashconst)) > 0.7){
        expand();
    }
    len += 1;
    auto vect = &hashTable[myHash(item, hashconst)];
    if(vect->empty()){
        fullness++;
    }
    vect->push_back(item);
}
template <class T>
const T* MySet<T>::find(T item){
    int num = myHash(item, hashconst);
    if(hashTable[num].empty()){
        return nullptr;
    }
    for(auto& item1: hashTable[num]){
        if(item1 == item){
            return &item1;
        }
    }
    return nullptr;
}
template <typename T>
bool MySet<T>::del(T item){
    if(find(item) == nullptr){
        return false;
    }
    int num = myHash(item, hashconst);
    auto vect = &hashTable[num];
    vect->erase(remove(vect->begin(), vect->end(), item), vect->end());
    len -= 1;
    if(vect->empty()){
        fullness--;
    }
    if( (double(fullness) / double(hashconst)) < 0.3){
        shrink();
    }
    return true;
}
template <typename T>
void MySet<T>::print(){
    stringstream s;
    s << '{';
    bool col = false;
    auto start = this->begin();
    auto end = this->end();

    while(start != end){
        if(col){
            s << ", " << *start ;
        } else{
            s << *start;
            col = true;
        }
        ++start;
    }
    cout << s.str() << '}' << endl;
}
template <typename T>
void MySet<T>::operator+=(MySet<T>& other){
    for(auto& item :other){
        add(item);
    }
}

template <typename T>
void MySet<T>::operator-=(MySet<T>& other){
    for(auto& item :other){
        del(item);
    }
}

template <typename T>
MySet<T>& MySet<T>::operator=(MySet<T> const & other){
    hashconst = other.hashconst;
    len = other.len;
    hashTable = other.hashTable;
    fullness = other.fullness;
    return *this;
}
template <typename T>
bool MySet<T>::operator==(MySet<T>& other){
    if(len != other.length()){
        return false;
    }
    for(auto& item: other){
        if(find(item) == nullptr){
            return false;
        }
    }
    return true;
}

template <typename T>
bool MySet<T>::operator!=(MySet<T>& other){
    return ! (this->operator==(other));
}
template <typename T>
void MySet<T>::addByDelimiter(const string& str, char delimiter){
    stringstream s;
    s << str;
    string item;
    while(getline(s,item,delimiter)){
        if(item.ends_with(delimiter)) {
            add(item.substr(0, item.length() - 1));
        }
        else{
            add(item);
        }
    }
}

/// MIMO CLASSU ///

//vrati set ako vysledok zlucenia 2 setov
template <typename T>
MySet<T> operator+(MySet<T>& first, MySet<T>& second){
    MySet<T> newSet = first;
    for(auto& item: second){
        newSet.add(item);
    }
    return newSet;
}

//vrati set s prvkami, ktore sa nachadzaju v prvom z nich no zaroven sa nenachadzaju v druhom
template <typename T>
MySet<T> operator-(MySet<T>& first, MySet<T>& second){
    MySet<T> newSet = first;
    for(auto& item: second){
        newSet.del(item);
    }
    return newSet;
}

//vrati novy set, ktory je tvoreny prienikom dvoch setov v parametroch
template <typename T>
MySet<T> intersection(MySet<T>& first, MySet<T>& second){
    MySet<T> newSet;
    for(auto& item: first){
        if(second.find(item) == nullptr){
            continue;
        }
        newSet.add(item);
    }
    return newSet;
};

//vrati set, ktory tvoria prvky zlucenia tychto dvoch setov, ale nie tie ktore su v prieniku (symetricka differnecia).
template <typename T>
MySet<T> symDiff(MySet<T>& first, MySet<T>& second){
    MySet<T> newSet = first;
    for(auto& item: second){
        auto ptr = newSet.find(item);
        if(ptr == nullptr){
            newSet.add(item);
            continue;
        }
        newSet.del(item);
    }
    return newSet;
}







