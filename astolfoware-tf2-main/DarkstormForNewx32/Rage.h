#pragma once
#include <windows.h>
#include <wininet.h>
#include <string>
#include <mutex>
#include <iostream>
#include <fstream>
#include <vector>
#include <sstream>

#include <unordered_map>
#include <unordered_set>
#include <fstream>
#include <string>
#include <mutex>
#include <sstream>
#include <iostream>

class CMarkingSystem {
public:
    CMarkingSystem() {
        LoadFromFile();
    }

    void Save(int id, const std::string& name);
    std::string IsMarked(int id);
    void Flush();
    void LoadFromFile();
    void Remove(int id, const std::string& name); // New remove function

private:
    void LoadLine(std::ifstream& input); // Recursive function to load lines
    std::unordered_map<std::string, std::unordered_set<int>> cache;
    bool cacheDirty = false;
    std::mutex cacheMutex; // Mutex for thread safety
};

inline CMarkingSystem gMarkingSystem;