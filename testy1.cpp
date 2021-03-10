// Copyright 2005, Google Inc.
// All rights reserved.

#include <iostream>
#include <fstream>
#include <iosfwd>
#include "gtest/gtest.h"

#include "MySet.h"

using namespace std;
using namespace ::testing;
TEST(TestFirstTask, AddInt) {
    MySet<int> t;
    t.add(1);
    t.add(2);
    ASSERT_EQ(t.length(), 2);
}

TEST(TestFirstTask, AddFindInt) {
    MySet<int> t;
    for(int i = 0; i < 100; i++){
        t.add(i);
    }
    ASSERT_EQ(t.length(),100);
    ASSERT_EQ(*t.find(3),3);
    ASSERT_EQ(*t.find(9),9);
    ASSERT_EQ(*t.find(12),12);
    ASSERT_EQ(t.find(122),nullptr);


}
TEST(TestFirstTask, DelInt) {
    MySet<int> t;
    t.add(1);
    t.add(2);
    ASSERT_EQ(t.length(), 2);
    t.del(1);
    ASSERT_EQ(t.length(), 1);
    t.del(2);
    ASSERT_EQ(t.length(), 0);
}
TEST(TestFirstTask, FindString) {
    MySet<string> t;
    string arr[] = {"jedna", "dva", "tri", "nejakevelmidlheslovo", "a"};
    string n = "neexistuje";
    for (auto & i : arr){
        t.add(i);
    }
    ASSERT_EQ(*t.find(arr[0]),"jedna");
    ASSERT_EQ(*t.find(arr[2]),"tri");
    ASSERT_EQ(t.del(arr[2]),true);
    ASSERT_EQ(t.find(arr[2]),nullptr);
    ASSERT_EQ(t.del(arr[2]),false);
    ASSERT_EQ(*t.find(arr[3]),"nejakevelmidlheslovo");
    ASSERT_EQ(*t.find(arr[4]),"a");
    ASSERT_EQ(t.find(n),nullptr);
}

TEST(TestFirstTask, AddSameAndDel) {
    MySet<int> t;
    t.add(3);
    t.add(4);
    t.add(3);
    t.add(4);
    ASSERT_EQ(*t.find(4), 4);
    ASSERT_EQ(*t.find(3), 3);
    t.del(4);
    t.del(3);
    ASSERT_EQ(t.find(4), nullptr);
    ASSERT_EQ(t.find(3), nullptr);

}
TEST(TestFirstTask, Iterator){
    MySet<char> c;
    for(int i = 32; i < 126; i++){
        c.add(i);
    }
    int l = 0;
    for(auto& item: c){
        l++;
    }
    ASSERT_EQ(c.length(),l);
}
TEST(TestFirstTask, AddText) {
    MySet<string> s;
    string text = "jeden,dva,tri,neviem,co,dalej,dalej,dalej,jeden,dva,dv,5,6";
    s.addByDelimiter(text,',');
    ASSERT_EQ(s.length(), 9);
    s.del("dalej");
    ASSERT_EQ(s.length(),8);
    s.addByDelimiter("jeden,dva,tri,neviem,co,dv,5,6",',');
//    s.print();
    for(auto& x: s){
        s.add(x);
    }
    ASSERT_EQ(s.length(),8);

}
TEST(TestFirstTask, LvalueRvalueTest) {
    MySet<string> s1;
    string text = "jeden,dva,tri,neviem,co,dalej,dalej,dalej,jeden,dva,dv,5,6";
    s1.addByDelimiter(text,',');

    MySet<string> s2;
    s2.addByDelimiter("jeden,dva,tri,neviem,co,dalej,dalej,dalej,jeden,dva,dv,5,6",',');

    ASSERT_EQ(s1==s2,true);
}
TEST(TestFirstTask, AddLongFile){
    MySet<string> s;
    ifstream ss("../sub.txt");
    string word;
    while(ss >> word) {
        s.add(word);
    }
    ASSERT_EQ(*s.find("aceton"),"aceton");
    ASSERT_EQ(s.length(), 213622);
}

TEST(TestFirstTask, copy){
    MySet<int> s;
    for(int i = 0; i < 20; i++){
        s.add(i);
    }
    MySet<int> t = s;
    ASSERT_EQ(t==s,true);
    ASSERT_EQ(t!=s,false);
    t.del(11);
    ASSERT_EQ(t==s,false);
    ASSERT_EQ(t!=s,true);
}
TEST(TestFirstTask, copy2){
    MySet<int> old;
    for(int i = 0; i < 30; i++){
        old.add(i);
    }
    MySet<int>neww(old);
    ASSERT_EQ(neww.length(),old.length());
    neww.del(9);
    ASSERT_NE(neww.length(),old.length());
    old.del(22);
    ASSERT_EQ(neww.length(),old.length());
}
TEST(TestFirstTask, symDiffAndInretsection){
    MySet<string> s;
    MySet<string> ss;
    s.addByDelimiter("pes,macka,set,dom,strom",',');
    ss.addByDelimiter("nula,tri,set,jeden,dva,dom",',');
    MySet<string> prienik = intersection(s,ss);
    MySet<string> other = symDiff(s,ss);
    ASSERT_EQ(prienik.length(), 2); //set,dom
    ASSERT_EQ(other.length(), 7); // pes,macka,strom,nula,tri,jeden,dva
}
TEST(TestFirstTask, plus){
    MySet<char> c1;
    c1.add('a');
    MySet<char> c2;
    c2.add('b');
    MySet<char> c0;
    MySet<char> c3;
    c3.add('d');
    MySet<char> c4;
    c4.add('e');
    MySet<char> c5;
    c5.add('f');
    c1 += c2;
    c3 += c4;
    c1 += c3;
    c1 += c5;
    ASSERT_EQ(c1.length(), 5);

}
TEST(TestFirstTask, plus2){
    MySet<int> a;
    for(int i = 0; i<20; i++){
        a.add(i);
    }
    MySet<int> b;
    for(int i = 0; i<10; i++){
        b.add(i);
    }
    MySet<int> c;
    for(int i = 10; i<20; i++){
        c.add(i);
    }

    MySet<int> d = b + c;
    ASSERT_EQ(a==d, true);

}
TEST(TestFirstTask, plusEmpty){
    MySet<int> a;
    MySet<int> b;
    MySet<int> c;

    c = a + b;

    ASSERT_EQ(a.length(), 0);

}
TEST(TestFirstTask, minusEmpty){
    MySet<int> a;
    MySet<int> b;
    MySet<int> c;

    c = a - b;

    ASSERT_EQ(a.length(), 0);

}
TEST(TestFirstTask, minus){
    MySet<char> c;
    char arr[] = {'a','b','c','d','e','f','g','h'};
    for(char& ch: arr){
    c.add(ch);
    }
    MySet<char> cc;
    cc.add('a');
    cc.add('b');
    cc.add('Z');
    ASSERT_EQ(cc.length(),3);
    cc -= c;
    ASSERT_EQ(cc.length(),1);
    MySet<char> z;
    z.add('Z');
    cc -= z;
    ASSERT_EQ(cc.length(),0);
}

TEST(TestFirstTask, emptyIter){
    MySet<int> empty;
    int x = 0;
    for(auto& n: empty){
        x++;
    }
    ASSERT_EQ(x,0);
}
TEST(TestFirstTask, SetOperations){
    MySet<int> a;
    for(int i = 0; i<10; i++){
        a.add(i);
    }
    MySet<int> b;
    for(int i = 5; i<15; i++){
        b.add(i);
    }
    MySet<int> prienik = intersection(a,b);
    MySet<int> zvysok = symDiff(b,a);
    MySet<int> x = a + b;
    MySet<int> y = prienik + zvysok;
    ASSERT_EQ(x == y, true);
}

TEST(TestFirstTask, Shrink){
    MySet<int> a;
    for(int i = 0; i<100; i++){
        a.add(i);
    }
    for(int i = 0; i<100; i++){
        a.del(i);
        i++;
    }

    ASSERT_EQ(a.length(), 50);
}








