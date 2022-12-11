#include <string>
#include <cstdlib>
#include <iostream>

#include "TSearch.hpp"
#include "TArray.hpp"
#include "TSyntaxTree.hpp"

int testSynTree() {

    TSyntaxTree tree;

    std::cout << "=========================" << std::endl;
    tree.Insert("T1");
    tree.Print();
    std::cout << "=========================" << std::endl;
    tree.Insert("||");
    tree.Print();
    std::cout << "=========================" << std::endl;
    tree.Insert("!");
    tree.Print();
    std::cout << "=========================" << std::endl;
    tree.Insert("(");
    tree.Print();
    std::cout << "=========================" << std::endl;
    tree.Insert("!");
    tree.Print();
    std::cout << "=========================" << std::endl;
    tree.Insert("(");
    tree.Print();
    std::cout << "=========================" << std::endl;
    tree.Insert("T2");
    tree.Print();
    std::cout << "=========================" << std::endl;
    tree.Insert("T3");
    tree.Print();
    std::cout << "=========================" << std::endl;
    tree.Insert("||");
    tree.Print();
    std::cout << "=========================" << std::endl;
    tree.Insert("T4");
    tree.Print();
    std::cout << "=========================" << std::endl;
    tree.Insert(")");
    tree.Print();
    std::cout << "=========================" << std::endl;
    tree.Insert(")");
    tree.Print();
    std::cout << "=========================" << std::endl;
    tree.Insert("T5");
    tree.Print();
    std::cout << "=========================" << std::endl;
    tree.Insert("T6");
    tree.Print();
    // T1 || !(!(T2 T3 || T4)) && T5 T6
    return 0;
}

int testTArray() {

    TArray<std::string> test;
    std::string str = "test";
    std::size_t testSize = 643;

    for(std::size_t i = 0; i < testSize; ++i) {
        std::string strN = str + std::to_string(i); 
        test.Push(strN);
    }

    for(std::size_t i = 0; i < testSize; ++i) {
        std::cout << test[i] << std::endl;
    }

    for(std::size_t i = 0; i < testSize; ++i) {
        std::string strN = test.Pop();
        std::cout << strN << std::endl;
    }

    TArray<std::string> test2;

    TArray<std::string> test3;

    test3.Push("test");

    TArray<std::size_t> test4;

    test4.Push(123);
    std::cout << test4[0] << std::endl;
    test4[0] = 321;
    std::cout << test4[0] << std::endl;
    std::size_t tmpvar = test4[0];
    tmpvar = 5421;
    std::cout << "<<<<<<<<<<<<<<<<" << std::endl;
    std::cout << test4[0] << std::endl;
    std::cout << tmpvar << std::endl;

    std::string tmpstr = test3[0];

    tmpstr = "tmpstr";
    std::cout << "<<<<<<<<<<<<<<<<" << std::endl;
    std::cout << test3[0] << std::endl;
    std::cout << tmpstr << std::endl;

    return 0;
}

int main() {
    std::string str = "";
    TArray<std::string> callback;
    TSearch search = TSearch();

    // testSynTree();
    
    search.LoadIndex();
    while (std::getline(std::cin, str)) {
        // std::cout << "Search: " << str << std::endl;
        // std::cout << "Stupid Search" << std::endl;
        // callback = search.StupidSearch(str);
        // callback.Print();
        std::cout << "Boolean Search" << std::endl;
        callback = search.BooleanSearch(str);
        callback.Print();
    }

    std::cout << "End!" << std::endl;
    return 0;
}