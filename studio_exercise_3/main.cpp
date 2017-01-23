//
//  main.cpp
//  studio2
//
//  Created by Fred Wu on 8/31/15.
//  Copyright (c) 2015 Fred. All rights reserved.
//

#include <iostream>
#include <vector>
#include <numeric>
#include <thread>
#include <mutex>

using namespace std;


mutex testMutex;


class SalesItems{
private:
    vector<int> items;
    mutex myMutex;
public:
    
    SalesItems();
    
    
    friend SalesItems& operator<<(SalesItems& rh, const int& i);
    void printAll();
    int operator()();
};


SalesItems::SalesItems(){
    
    items = vector<int>();
    
//    for(unsigned i=0; i<20; ++i){
//        items.push_back(i);
//    }
}


// Exercises 2
SalesItems& operator<<(SalesItems& rh, const int& i){
    lock_guard<mutex> guard(rh.myMutex);
    rh.items.push_back(i);
    return rh;
}

int SalesItems::operator()() {
    lock_guard<mutex> guard(myMutex);
    return accumulate(items.begin(), items.end(), 0);
}


void SalesItems::printAll(){
    lock_guard<mutex> guard(myMutex);
    printf("Count: %lu\n", items.size());
    for(unsigned i = 0; i<items.size(); ++i){
        printf("%d: %d\n", i, items[i]);
    }
    printf("Finished.\n");
}






void insertItem(SalesItems& s, int i){
    s<<i;
}

void printSum(SalesItems& s){
    printf("Sum: %d", s());
}


void insert10Items(SalesItems& s, int i){
    
    for (unsigned x=i; x<10+i; ++x){
        s<<x;
    }
}

void printAll(SalesItems &s){
    s.printAll();
}

void func2(){
    lock_guard<mutex> guard(testMutex);
    printf("Func2");
}

void func1(){
    printf("Before Func1 locked");
    lock_guard<mutex> guard(testMutex);
    printf("Func1");
    func2();
}



int main(int argc, const char * argv[]) {
    // insert code here...
    
    /*
    
    SalesItems s;
    
//    
//    insertItem(s, 42);
//    printSum(s);
    
    
    thread t1(insertItem, ref(s), 21);
//
    thread t11(insertItem, ref(s), 42);

//
    thread t2(printSum, ref(s));
    
    thread t3(insert10Items, ref(s), 99);
    
    thread t4(printAll, ref(s));

    t1.join();
    t11.join();
    
    t2.join();
    t3.join();
    t4.join();
     
     */
    
    func1();
    
    
    
    return 0;
}
