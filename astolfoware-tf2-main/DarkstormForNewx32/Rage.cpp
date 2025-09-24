#include "Rage.h"
#include "CConsole.h"
// Load entries from the file into the cache
// Load entries from the file into the cache and remove deleted lines
void CMarkingSystem::LoadFromFile() {
    std::unordered_map<std::string, std::unordered_set<int>> fileCache;

    std::ifstream input("Marked.txt");
    if (input.is_open()) {
        std::string line;
        while (std::getline(input, line)) {
            std::istringstream iss(line);
            int id;
            std::string name;
            if (iss >> id >> name) {
                fileCache[name].insert(id); // Populate a temporary cache from the file
            }
        }
        input.close();
    }

    std::lock_guard<std::mutex> lock(cacheMutex); // Lock the cache during modification
    // Remove entries from the cache that are not present in the fileCache
    for (auto it = cache.begin(); it != cache.end();) {
        auto fileEntry = fileCache.find(it->first);
        if (fileEntry == fileCache.end()) {
            it = cache.erase(it); // Name not found in the file, remove from the cache
        }
        else {
            // Remove IDs from the cache that are not in the file
            for (auto idIt = it->second.begin(); idIt != it->second.end();) {
                if (fileEntry->second.find(*idIt) == fileEntry->second.end()) {
                    idIt = it->second.erase(idIt); // ID not found in the file, remove it
                }
                else {
                    ++idIt;
                }
            }
            // If the name no longer has any IDs, remove it
            if (it->second.empty()) {
                it = cache.erase(it);
            }
            else {
                ++it;
            }
        }
    }

    // Add new entries from the fileCache to the cache
    for (const auto& [name, ids] : fileCache) {
        for (int id : ids) {
            cache[name].insert(id);
        }
    }
}

// Recursive function to load lines from the file
void CMarkingSystem::LoadLine(std::ifstream& input) {
    std::string line;
    if (std::getline(input, line)) { // Read a line
        std::istringstream iss(line);
        int id;
        std::string name;
        if (iss >> id >> name) { // Read ID and name from line
            std::lock_guard<std::mutex> lock(cacheMutex); // Lock while modifying the cache
            cache[name].insert(id);
        }
        LoadLine(input); // Recursively call to load the next line
    }
}

// Save a new ID and name to the cache
void CMarkingSystem::Save(int id, const std::string& name) {
    std::lock_guard<std::mutex> lock(cacheMutex); // Lock while modifying the cache
    if (cache[name].find(id) == cache[name].end()) {
        cache[name].insert(id);
        cacheDirty = true;
    }
}

// Check if an ID is marked and return the associated name
std::string CMarkingSystem::IsMarked(int id) {
    std::lock_guard<std::mutex> lock(cacheMutex); // Lock while reading the cache
    for (const auto& entry : cache) {
        if (entry.second.find(id) != entry.second.end()) {
            return entry.first; // Return the name if a match is found
        }
    }
    return ""; // Return empty string if not found
}

// Flush cached entries to the file
void CMarkingSystem::Flush() {
    std::lock_guard<std::mutex> lock(cacheMutex); // Lock while writing to file
    if (cacheDirty) {
        std::ofstream output("Marked.txt");
        if (output.is_open()) {
            for (const auto& entry : cache) {
                for (int id : entry.second) {
                    output << id << " " << entry.first << "\n"; // Write ID and name to file
                }
            }
            output.close();
            cacheDirty = false;
        }
    }
}

void CMarkingSystem::Remove(int id, const std::string& name) {
    std::lock_guard<std::mutex> lock(cacheMutex); // Lock while modifying the cache
    auto it = cache.find(name);
    if (it != cache.end() && it->second.erase(id) > 0) { // Erase ID if it exists
        cacheDirty = true; // Mark cache as dirty for saving
        if (it->second.empty()) {
            cache.erase(it); // Remove the name entry if no IDs are left
        }
    }
}