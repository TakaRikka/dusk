#include <algorithm>

#include "bits.hpp"
#include "../item.hpp"

#include <iterator>

BitVector::BitVector(const std::list<int>& bits)
{
    for (auto& i : bits)
    {
        this->set(i);
    }
}

bool BitVector::isEmpty() const
{
    return bitset.none();
}

std::set<int> BitVector::ints() const
{
    return intset;
}

void BitVector::set(const int& i)
{
    bitset.set(i, true);
    intset.insert(i);
}

void BitVector::clear(const int& i)
{
    if (intset.contains(i))
    {
        intset.erase(i);
        bitset.set(i, false);
    }
}

bool BitVector::test(const int& i) const
{
    return intset.contains(i);
}

int BitVector::size() const
{
    return intset.size();
}

void BitVector::and_(const BitVector& other)
{
    std::set<int> intersection = {};
    std::set_intersection(intset.begin(),
                          intset.end(),
                          other.intset.begin(),
                          other.intset.end(),
                          std::inserter(intersection, intersection.begin()));
    intset = intersection;
    bitset &= other.bitset;
}

void BitVector::or_(const BitVector& other)
{
    intset.insert(other.intset.begin(), other.intset.end());
    bitset |= other.bitset;
}

bool BitVector::isSubsetOf(const BitVector& other) const
{
    return (bitset | other.bitset) == other.bitset;
}

bool BitVector::equals(const BitVector& other) const
{
    return bitset == other.bitset;
}

bool includedIn(const std::bitset<512>& a, const std::bitset<512>& b)
{
    return (a | b) == b;
}

DNF::DNF(std::vector<std::bitset<512>> terms_): terms(terms_) {}

bool DNF::isTriviallyFalse() const
{
    return terms.size() == 0;
}

bool DNF::isTriviallyTrue() const
{
    return std::any_of(terms.begin(), terms.end(), [](const auto& i) { return i == 0; });
}

DNF DNF::or_(const DNF& other)
{
    auto new_terms = terms;
    new_terms.insert(new_terms.end(), other.terms.begin(), other.terms.end());
    return DNF(new_terms);
}

// Removes all redundent terms
DNF DNF::dedup()
{
    std::vector<std::bitset<512>> filtered = {};
    for (const auto& candidate : terms)
    {
        std::vector<int> toPop = {};
        bool nextTerm = false;
        for (int existing_idx = 0; existing_idx < filtered.size(); existing_idx++)
        {
            const auto& existing = filtered[existing_idx];
            if (includedIn(existing, candidate))
            {
                // Existing requires fewer or equal things than candidate
                nextTerm = true;
                break;
            }
            else if (includedIn(candidate, existing))
            {
                // Candidate requires strictly fewer things than existing
                toPop.push_back(existing_idx);
            }
        }

        if (!nextTerm)
        {
            // Did not break to next term
            for (auto c_iter = toPop.rbegin(); c_iter != toPop.rend(); c_iter++)
            {
                const auto& c = *c_iter;
                if (c == filtered.size() - 1)
                {
                    filtered.pop_back();
                }
                else
                {
                    // Remove c without shifting elements by replacing
                    // it with the last element
                    filtered[c] = filtered.back();
                    filtered.pop_back();
                }
            }
            filtered.push_back(candidate);
        }
    }

    return DNF(filtered);
}

// Returns useful, self.or_(other)
// useful is True if other contained at least one term that
// was not redundant.
std::pair<bool, DNF> DNF::or_useful(const DNF& other)
{
    auto filtered_this = terms;
    std::vector<std::bitset<512>> filtered_other = {};
    bool useful = false;

    for (const auto& candidate : other.terms)
    {
        bool nextTerm = false;
        for (const auto& existing : filtered_this)
        {
            if (includedIn(existing, candidate))
            {
                nextTerm = true;
                break;
            }
        }

        if (!nextTerm)
        {
            filtered_other.push_back(candidate);
            useful = true;
        }
    }

    filtered_this.insert(filtered_this.end(), filtered_other.begin(), filtered_other.end());
    return {useful, DNF(filtered_this)};
}

DNF DNF::and_(const DNF& other)
{
    std::vector<std::bitset<512>> d = {};
    for (const auto& t1 : terms)
    {
        for (const auto& t2 : other.terms)
        {
            d.push_back(t1 | t2);
        }
    }

    // Dedup incase things are getting too big
    DNF dnf = DNF(d);
    if (d.size() > 500)
    {
        dnf = dnf.dedup();
    }
    return dnf;
}

int BitIndex::bump()
{
    auto c = counter;
    counter++;
    return c;
}

int BitIndex::reqBit(const randomizer::logic::requirement::Requirement& req)
{
    uint32_t expectedCount;
    randomizer::logic::item::Item* item;
    std::string key;

    switch (req._type)
    {
        case randomizer::logic::requirement::Type::ITEM:
            item = std::get<randomizer::logic::item::Item*>(req._args[0]);
            key = item->GetName() + "::1";
            if (itemBits.contains(key))
            {
                return itemBits[key];
            }
            else
            {
                itemBits[key] = counter;
                reverseIndex.push_back(req);
                return bump();
            }
        case randomizer::logic::requirement::Type::COUNT:
            expectedCount = std::get<int>(req._args[0]);
            item = std::get<randomizer::logic::item::Item*>(req._args[1]);
            key = item->GetName() + "::" + std::to_string(expectedCount);
            if (itemBits.contains(key))
            {
                return itemBits[key];
            }
            else
            {
                itemBits[key] = counter;
                reverseIndex.push_back(req);
                return bump();
            }
        case randomizer::logic::requirement::Type::GOLDEN_BUGS:
            key = std::to_string(std::get<int>(req._args[0]));
            if (goldenBugCount.contains(key))
            {
                return goldenBugCount[key];
            }
            else
            {
                goldenBugCount[key] = counter;
                reverseIndex.push_back(req);
                return bump();
            }
        case randomizer::logic::requirement::Type::HEARTS:
            key = std::to_string(std::get<int>(req._args[0]));
            if (heartCount.contains(key))
            {
                return heartCount[key];
            }
            else
            {
                heartCount[key] = counter;
                reverseIndex.push_back(req);
                return bump();
            }
        case randomizer::logic::requirement::Type::DUNGEONS_COMPLETED:
            key = std::to_string(std::get<int>(req._args[0]));
            if (dungeonCompletedCount.contains(key))
            {
                return dungeonCompletedCount[key];
            }
            else
            {
                dungeonCompletedCount[key] = counter;
                reverseIndex.push_back(req);
                return bump();
            }
        default:
            // Not a flattening requirement
            return -1;
    }
    return -1;
}
