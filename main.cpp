#include <string>
#include <cstdlib>
#include <iostream>
#include <sys/types.h>
#include <sys/socket.h>
#include <unistd.h>
#include <netinet/in.h>
#include <string.h>

#include "TSearch.hpp"
#include "TArray.hpp"
#include "TSyntaxTree.hpp"

int SaveSendMsg(int clientSock, char* buffer, std::string& str) {
    int n;
    strcpy(buffer, str.c_str());
    std::cout << "Write: " << buffer << std::endl;
    n = send(clientSock, buffer, strlen(buffer), 0);
    if (n == -1) {
        return n;
    }
    n = recv(clientSock, buffer, 500, 0);
    return n;
}

int caseSocket(TSearch& search) {
    
    char buffer[1000];
    int n;
    const int SERVER_PORT = 50007;

    sockaddr_in serverAddr;
    serverAddr.sin_family       = AF_INET;
    serverAddr.sin_port         = htons(SERVER_PORT);
    serverAddr.sin_addr.s_addr  = INADDR_ANY;

    sockaddr_in clientAddr;
    socklen_t   sin_size = sizeof(struct sockaddr_in);

    std::cout << "Create socket..." << std::endl;
    int serverSock = socket(AF_INET, SOCK_STREAM, 0);
    bind(serverSock, (struct sockaddr*)&serverAddr, sizeof(struct sockaddr));
    listen(serverSock, 1);

    while(true) {
        std::cout << "Accepting new client TCP connect" << std::endl;
        int clientSock = accept(serverSock, (struct sockaddr*) &clientAddr, &sin_size);

        bzero(buffer, 1000);

        std::cout << "Receive request..." << std::endl;
        n = recv(clientSock, buffer, 500, 0);
        if (n == -1 || n == 0) {
            std::cout << "Last request. Close connect." << std::endl;
            break;
        }
        std::string str = buffer;
        
        TArray<TFileData*> callback = search.Search(str, TSearch::REV_INDEX);
        TArray<std::string> translates = search.GetAlternate();
        str = "";
        for (std::size_t i = 0; i < translates.Size(); ++i) {
            n = SaveSendMsg(clientSock, buffer, translates[i]);
            if (n == -1 || n == 0) {
                break;
            }
        }
        if (n == -1 || n == 0) {
            break;
        }

        str = "END";
        n = SaveSendMsg(clientSock, buffer, str);
        if (n == -1 || n == 0) {
            break;
        }

        for(std::size_t i = 0; i < callback.Size(); ++i) {
            str = callback[i]->filepath + "&&&" + callback[i]->title + "&&&" + std::to_string(callback[i]->score);
            n = SaveSendMsg(clientSock, buffer, str);
            if (n == -1 || n == 0) {
                break;
            }
        }
        if (n == -1 || n == 0) {
            break;
        }

        str = "END";
        n = SaveSendMsg(clientSock, buffer, str);
        if (n == -1 || n == 0) {
            break;
        }

        std::cout << "Close socket" << std::endl;
        close(clientSock);
    }

    return 0;
}

int caseTerminal(TSearch& search, std::string prefixPath, std::string suffixPath) {
    std::string str = "";

    while (std::getline(std::cin, str)) {

        // std::cout << "Stupid Search" << std::endl;
        // callback = search.StupidSearch(str);
        // callback.Print();

        // std::cout << "Boolean Search" << std::endl;
        // callback = search.BooleanSearch(str);
        // callback.Print();

        std::cout << "Rev Search" << std::endl;
        TArray<TFileData*> callback = search.Search(str, TSearch::REV_INDEX);
        for(std::size_t i = 0; i < callback.Size(); ++i) {
            std::cout << "Path: "  << prefixPath << callback[i]->filepath << suffixPath << std::endl;
            std::cout << "Title: " << callback[i]->title << std::endl;
            std::cout << "Score: " << callback[i]->score << std::endl;
            std::cout << std::endl;
        }
    }

    return 0;
}

int main() {

    std::string path = "./templates/ruwiki/data";
    TSearch search = TSearch(path);
    std::string prefixPath = "ruwiki/html/";
    std::string suffixPath = ".html";

    search.LoadIndex(TSearch::REV_INDEX);
    caseSocket(search);
    //caseTerminal(search, prefixPath, suffixPath);
    std::cout << "End!" << std::endl;
    return 0;
}