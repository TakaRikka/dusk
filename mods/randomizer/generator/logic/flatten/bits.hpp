#pragma once

#include "../requirement.hpp"

#include <list>
#include <vector>
#include <bitset>
#include <set>
#include <unordered_map>

class BitVector
{
   public:
    BitVector() = default;
    explicit BitVector(const std::list<int>& bits);

    bool isEmpty() const;
    std::set<int> ints() const;
    void set(const int& i);
    void clear(const int& i);
    bool test(const int& i) const;
    int size() const;
    void and_(const BitVector& other);
    void or_(const BitVector& other);
    bool isSubsetOf(const BitVector& other) const;
    bool equals(const BitVector& other) const;

    std::bitset<512> bitset;
    std::set<int> intset;
};

bool includedIn(const std::bitset<512>& a, const std::bitset<512>& b);

// A logical expression in disjunctive normal form.
// Disjuncts are bit-vectors, but we don't use the BitVector class here
// because it doesn't seem necessary since our bitvectors are small
// and we only need bit set access after the propagation code is done
class DNF
{
   public:
    DNF() = default;
    DNF(std::vector<std::bitset<512>> terms);

    static DNF True() { return DNF({0}); }

    static DNF False() { return DNF(std::vector<std::bitset<512>> {}); }

    bool isTriviallyFalse() const;
    bool isTriviallyTrue() const;
    DNF or_(const DNF& other);
    DNF dedup();
    std::pair<bool, DNF> or_useful(const DNF& other);
    DNF and_(const DNF& other);

    std::vector<std::bitset<512>> terms = {};
};

class BitIndex
{
   public:
    BitIndex() = default;

    int bump();
    int reqBit(const randomizer::logic::requirement::Requirement& req);

    std::unordered_map<std::string, int> itemBits = {};
    std::unordered_map<std::string, int> heartCount = {};
    std::unordered_map<std::string, int> goldenBugCount = {};
    std::unordered_map<std::string, int> dungeonCompletedCount = {};
    std::vector<randomizer::logic::requirement::Requirement> reverseIndex = {};
    int counter = 0;
};
