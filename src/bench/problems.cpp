#include "problems.hpp"

#include <algorithm>
#include <charconv>
#include <filesystem>
#include <fstream>
#include <sstream>
#include <stdexcept>
#include <unordered_map>
#include <utility>

namespace fs = std::filesystem;

namespace {

struct KnownSolution {
    int bestUb;
    bool solved;
};

bool parseInt(const std::string& s, int& out) {
    const char* end = s.data() + s.size();
    auto [ptr, ec] = std::from_chars(s.data(), end, out);
    return ec == std::errc() && ptr == end;
}

// Maps "SetName/file" to its best known solution
std::unordered_map<std::string, KnownSolution> loadSolutions(const fs::path& csvPath) {
    std::ifstream in(csvPath);
    if (!in) throw std::runtime_error("cannot open solutions file: " + csvPath.string());

    const std::string header = "file,set,best_lb,best_ub,status";
    std::unordered_map<std::string, KnownSolution> solutions;
    std::string line;
    int lineNo = 0;

    while (std::getline(in, line)) {
        ++lineNo;
        if (!line.empty() && line.back() == '\r') line.pop_back();

        if (lineNo == 1) {
            if (line != header)
                throw std::runtime_error(csvPath.string() + ": expected header \"" + header + "\", got \"" + line + "\"");
            continue;
        }
        if (line.empty()) continue;

        std::vector<std::string> cols;
        std::stringstream ss(line);
        std::string col;
        while (std::getline(ss, col, ',')) cols.push_back(col);

        int bestUb;
        if (cols.size() != 5 || !parseInt(cols[3], bestUb))
            throw std::runtime_error(csvPath.string() + ":" + std::to_string(lineNo) + ": malformed row \"" + line + "\"");

        solutions[cols[0]] = {bestUb, cols[4] == "solved"};
    }
    if (lineNo == 0) throw std::runtime_error(csvPath.string() + ": file is empty");

    return solutions;
}

// Format: item count, capacity, then one item size per line
Problem loadInstance(const fs::path& path, const std::string& set) {
    std::ifstream in(path);
    if (!in) throw std::runtime_error("cannot open instance file: " + path.string());

    Problem p;
    p.name = set + "/" + path.filename().string();
    p.set = set;

    int n;
    if (!(in >> n >> p.binCapacity) || n <= 0 || p.binCapacity <= 0)
        throw std::runtime_error(path.string() + ": expected a positive item count and capacity on the first two lines");

    for (int i = 0; i < n; ++i) {
        int size;
        if (!(in >> size))
            throw std::runtime_error(path.string() + ": expected " + std::to_string(n) + " item sizes, could only read " + std::to_string(i));
        if (size <= 0 || size > p.binCapacity)
            throw std::runtime_error(path.string() + ": item size " + std::to_string(size) + " is outside 1.." + std::to_string(p.binCapacity));
        p.nums.push_back(size);
    }

    std::string extra;
    if (in >> extra)
        throw std::runtime_error(path.string() + ": more than the " + std::to_string(n) + " item sizes declared");

    return p;
}

} // namespace

std::vector<Problem> loadProblemSets(const std::string& dataDir, const std::vector<std::string>& sets) {
    const fs::path root(dataDir);
    const auto solutions = loadSolutions(root / "bpplib_solutions.csv");

    std::vector<Problem> problems;
    for (const std::string& set : sets) {
        const fs::path setDir = root / "bpplib" / set;
        if (!fs::is_directory(setDir))
            throw std::runtime_error("problem set not found: " + setDir.string());

        // Skip hidden files like .DS_Store; no extension filter since SSTriples files have none
        std::vector<fs::path> paths;
        for (const fs::directory_entry& entry : fs::directory_iterator(setDir)) {
            const std::string fileName = entry.path().filename().string();
            if (entry.is_regular_file() && fileName.front() != '.') paths.push_back(entry.path());
        }
        std::sort(paths.begin(), paths.end());

        for (const fs::path& path : paths) {
            Problem p = loadInstance(path, set);
            auto it = solutions.find(p.name);
            if (it != solutions.end()) {
                p.expectedNumBins = it->second.bestUb;
                p.knownOptimal = it->second.solved;
            }
            problems.push_back(std::move(p));
        }
    }
    return problems;
}
