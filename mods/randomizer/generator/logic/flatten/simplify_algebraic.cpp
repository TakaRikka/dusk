#include "simplify_algebraic.hpp"
#include "../../utility/container.hpp"

// Turns a bit-based DNF (a two-level sum-of-products) back into
// a readable multi-level requirement.
randomizer::logic::requirement::Requirement DNFToExpr(BitIndex& bitIndex, DNF dnf)
{
    if (dnf.isTriviallyFalse())
    {
        return randomizer::logic::requirement::Requirement {randomizer::logic::requirement::Type::IMPOSSIBLE, {}};
    }

    if (dnf.isTriviallyTrue())
    {
        return randomizer::logic::requirement::Requirement {randomizer::logic::requirement::Type::NOTHING, {}};
    }

    // really make sure no dupes exist, not sure if needed
    dnf = dnf.dedup();

    // Map to BitVectors. Since DNFs don't offer bit-level access,
    // we have to manually go through every bit to build BitVectors.
    // This is definitely not cheap but it probably saves more time
    // than keeping the intsets around during search
    std::vector<BitVector> expr = {};
    for (const auto& t : dnf.terms)
    {
        std::list<int> bits = {};
        for (int bit = 0; bit < bitIndex.counter; bit++)
        {
            if (t.test(bit))
            {
                bits.push_back(bit);
            }
        }
        expr.emplace_back(bits);
    }

    // at this point we must remove weaker requirements. E.g.
    // imagine Beedle existed in this rando and an item required
    // (Wallet x1 and Wallet x2) or (Wallet x1 and ExtraWallet x1 and ExtraWallet x2)
    // then this code would pull out Wallet x1 first, resulting in
    // Wallet x1 and (Wallet x2 or ExtraWallet x1 and ExtraWallet x2) which is not
    // reasonable at all and at that point not even the TWWR-Tracker simplifications can save us
    for (auto& term : expr)
    {
        for (const auto& bit : term.ints())
        {
            auto& req = bitIndex.reverseIndex[bit];
            if (req._type == randomizer::logic::requirement::Type::COUNT)
            {
                auto count = std::get<int>(req._args[0]);
                auto item = std::get<randomizer::logic::item::Item*>(req._args[1]);
                for (int i = 1; i < count; i++)
                {
                    auto lesserBit = bitIndex.reqBit(
                        randomizer::logic::requirement::Requirement {randomizer::logic::requirement::Type::COUNT, {i, item}});
                    term.clear(lesserBit);
                }
            }
        }
    }

    auto commonFactors = expr[0].ints();
    for (const auto& term : expr)
    {
        std::set<int> intersection = {};
        std::set_intersection(commonFactors.begin(),
                              commonFactors.end(),
                              term.intset.begin(),
                              term.intset.end(),
                              std::inserter(intersection, intersection.begin()));
        commonFactors = intersection;
    }

    // build a list of variables that appear in our expression,
    // excluding common factors.
    std::set<int> varSet = {};
    for (auto& term : expr)
    {
        for (const auto& c : commonFactors)
        {
            term.clear(c);
        }
        for (const auto& b : term.ints())
        {
            varSet.insert(b);
        }
    }

    std::vector<int> variables = std::vector<int>(varSet.begin(), varSet.end());

    if (variables.empty())
    {
        return createAnd(lookupRequirements(bitIndex, commonFactors));
    }

    std::vector<BitVector> seen = {};
    auto kernels = findKernels(expr, variables, BitVector(), seen);
    kernels = randomizer::utility::container::FilterFromVector(kernels, [](const auto& k) { return !k.coKernel.isEmpty(); });

    // columns are unique cubes in all kernels
    std::vector<BitVector> columns = {};
    for (const auto& kernel : kernels)
    {
        for (const auto& kCube : kernel.kernel)
        {
            if (std::none_of(columns.begin(), columns.end(), [&](const auto& c) { return kCube.equals(c); }))
            {
                columns.push_back(kCube);
            }
        }
    }

    // rows are unique co-kernels
    auto& rows = kernels;
    if (!rows.empty() && !columns.empty())
    {
        std::vector<std::vector<int>> matrix = {};
        for (const auto& row : rows)
        {
            matrix.emplace_back(columns.size(), 0);
        }
        // create a matrix that is 1 where column cubes appear in row kernels.
        // since kernels are the result of a single division (by the co-kernel),
        // this essentially creates ones where division by another cube would be possible
        for (int col = 0; col < columns.size(); col++)
        {
            auto& kCube = columns[col];
            for (int row = 0; row < rows.size(); row++)
            {
                auto& coKernel = rows[row];
                if (std::any_of(coKernel.kernel.begin(), coKernel.kernel.end(), [&](const auto& k) { return kCube.equals(k); }))
                {
                    matrix[row][col] = 1;
                }
            }
        }

        // Find the best rectangle. This optimizes for #literals saved
        // in the resulting expression, which is a good heuristic for
        // minimizing the length of the expression.
        auto rowWeight = [&](const int& row) { return rows[row].coKernel.size() + 1; };
        auto colWeight = [&](const int& col) { return columns[col].size(); };

        auto value = [&](const int& col, const int& row)
        {
            auto cpy = rows[row].coKernel;
            cpy.or_(columns[col]);
            return cpy.size();
        };

        auto literalsSaved = [&](const std::tuple<std::vector<int>, std::vector<int>>& rect)
        {
            auto [rectRows, rectCols] = rect;
            int weight = 0;
            for (const auto& row : rectRows)
            {
                for (const auto& col : rectCols)
                {
                    if (matrix[row][col])
                    {
                        weight += value(col, row);
                    }
                }
            }

            for (const auto& row : rectRows)
            {
                weight -= rowWeight(row);
            }
            for (const auto& col : rectCols)
            {
                weight -= colWeight(col);
            }

            return weight;
        };

        std::vector<std::tuple<std::vector<int>, std::vector<int>>> allRects = {};
        std::vector<int> rows_;
        std::vector<int> cols_;
        for (int i = 0; i < rows.size(); i++)
        {
            rows_.push_back(i);
        }
        for (int i = 0; i < columns.size(); i++)
        {
            cols_.push_back(i);
        }
        genRectangles(rows_,
                      cols_,
                      matrix,
                      [&](const std::vector<int>& rows__, const std::vector<int>& cols__)
                      { allRects.push_back({rows__, cols__}); });

        if (!allRects.empty())
        {
            auto& [bestRows, bestCols] = *std::max_element(allRects.begin(),
                                                           allRects.end(),
                                                           [&](const auto& rect1, const auto& rect2)
                                                           { return literalsSaved(rect1) < literalsSaved(rect2); });

            // divisor is created by OR-ing column cubes
            std::vector<BitVector> divisor = {};
            for (const auto& c : bestCols)
            {
                divisor.push_back(columns[c]);
            }
            auto [quot, remainder] = algebraicDivision(expr, divisor);

            // and re-assemble a Requirement that sort of looks like
            // common_factors * (quotient * divisor + remainder)
            auto product = randomizer::logic::requirement::Requirement();
            product._type = randomizer::logic::requirement::Type::AND;
            std::vector<std::bitset<512>> quotBits;
            std::vector<std::bitset<512>> divisorBits;
            for (const auto& c : quot)
            {
                quotBits.push_back(c.bitset);
            }
            for (const auto& c : divisor)
            {
                divisorBits.push_back(c.bitset);
            }
            product._args.push_back(DNFToExpr(bitIndex, DNF(quotBits)));
            product._args.push_back(DNFToExpr(bitIndex, DNF(divisorBits)));

            auto sum = randomizer::logic::requirement::Requirement();
            if (!remainder.empty())
            {
                std::vector<std::bitset<512>> remainderBits;
                for (const auto& c : remainder)
                {
                    remainderBits.push_back(c.bitset);
                }
                sum._type = randomizer::logic::requirement::Type::OR;
                sum._args.push_back(product);
                sum._args.push_back(DNFToExpr(bitIndex, DNF(remainderBits)));
            }
            else
            {
                sum = product;
            }
            auto terms = lookupRequirements(bitIndex, commonFactors);
            terms.push_back(sum);
            return createAnd(terms);
        }
    }

    // here we didn't do our complicated rectangle extraction, so just extract
    // the common factors
    auto terms = randomizer::logic::requirement::Requirement();
    terms._type = randomizer::logic::requirement::Type::OR;
    for (const auto& c : expr)
    {
        terms._args.push_back(createAnd(lookupRequirements(bitIndex, c.ints())));
    }

    // common_factor1 AND common_factor2 AND ... AND (terms without common factors ORed)
    auto finalTerms = lookupRequirements(bitIndex, commonFactors);
    finalTerms.push_back(terms);
    return createAnd(finalTerms);
}

randomizer::logic::requirement::Requirement createAnd(std::vector<randomizer::logic::requirement::Requirement> terms)
{
    if (terms.size() > 1)
    {
        randomizer::logic::requirement::Requirement req;
        req._type = randomizer::logic::requirement::Type::AND;
        for (auto& term : terms)
        {
            req._args.push_back(term);
        }
        return req;
    }
    return terms[0];
}

// Recursively computes kernels and co-kernels of the expression `cubes`.
// A co-kernel is a cube (product term) such that for `expr / co-kernel = kernel`,
// `kernel` contains at least two terms but there's no factor to factor out.

// This effectively tries every combination of variables in this expression
// as a co-kernel. seenCoKernels is a bit of book-keeping to not create
// duplicate kernels, and min_idx ensures we don't try e.g. ab and ba separately
std::vector<FoundKernel> findKernels(const std::vector<BitVector>& cubes,
                                     const std::vector<int>& variables,
                                     const BitVector& coKernelPath,
                                     std::vector<BitVector>& seenCoKernels,
                                     int minIdx /* = 0 */)
{
    std::vector<FoundKernel> kernels = {};
    for (int idx = 0; idx < variables.size(); idx++)
    {
        auto& bit = variables[idx];
        // we won't find any useful kernels by trying these *again*
        if (idx < minIdx)
        {
            continue;
        }

        std::vector<BitVector> s = {};
        for (auto& c : cubes)
        {
            if (c.test(bit))
            {
                s.push_back(c);
            }
        }

        if (s.size() >= 2)
        {
            auto co = s[0];
            for (const auto& c : s)
            {
                co.and_(c);
            }
            auto subPath = coKernelPath;
            subPath.or_(co);
            auto [quot, remainder] = algebraicDivision(cubes, {co});
            auto subKernels = findKernels(quot, variables, subPath, seenCoKernels, idx + 1);

            for (const auto& sub : subKernels)
            {
                if (std::none_of(seenCoKernels.begin(),
                                 seenCoKernels.end(),
                                 [=](const auto& seenCo) { return seenCo.equals(sub.coKernel); }))
                {
                    seenCoKernels.push_back(sub.coKernel);
                    kernels.push_back(sub);
                }
            }
        }
    }

    // cube-free expr is always its own kernel, with trivial co-kernel 1
    if (std::none_of(seenCoKernels.begin(),
                     seenCoKernels.end(),
                     [=](const auto& seenCo) { return seenCo.equals(coKernelPath); }))
    {
        kernels.push_back(FoundKernel {cubes, coKernelPath});
    }

    return kernels;
}

// Computes the algebraic division of expr / divisor, returning
// the quotient and the remainder. These satisfy the formula

// expr = quotient * divisor + remainder
std::pair<std::vector<BitVector>, std::vector<BitVector>> algebraicDivision(const std::vector<BitVector>& expr,
                                                                            const std::vector<BitVector>& divisor)
{
    std::vector<BitVector> quot = {};
    // for every "cube"/product term in our divisor...
    for (const auto& divCube : divisor)
    {
        // get a list of all cubes that this can be divided by
        std::vector<BitVector> c = {};
        std::copy_if(expr.begin(), expr.end(), std::back_inserter(c), [=](const auto& e) { return divCube.isSubsetOf(e); });

        if (c.empty())
        {
            // division not possible, remainder is the entire expression
            return {{}, expr};
        }

        // "cross out" the bits of this divisor cube
        for (auto& ci : c)
        {
            for (const auto& bit : divCube.ints())
            {
                ci.clear(bit);
            }
        }

        // compute the intersection of the divided expr with the divided expr in other cubes
        if (quot.empty())
        {
            quot = c;
        }
        else
        {
            // this is literally set intersection, NOT an OR or an AND
            std::vector<BitVector> newQuot = {};
            std::copy_if(quot.begin(),
                         quot.end(),
                         std::back_inserter(newQuot),
                         [=](const auto& qc)
                         { return std::any_of(c.begin(), c.end(), [=](const auto& cc) { return cc.equals(qc); }); });
            quot = newQuot;
        }
    }

    // finally, compute the remainder essentially by computing
    // remainder = expr - quotient * divisor
    // * is AND
    std::vector<std::bitset<512>> quotBits = {};
    for (auto& i : quot)
    {
        quotBits.push_back(i.bitset);
    }
    std::vector<std::bitset<512>> divisorBits = {};
    for (auto& i : divisor)
    {
        divisorBits.push_back(i.bitset);
    }
    DNF product = DNF(quotBits).and_(DNF(divisorBits)).dedup();

    std::vector<BitVector> remainder = {};
    std::copy_if(expr.begin(),
                 expr.end(),
                 std::back_inserter(remainder),
                 [=](const auto& e)
                 {
                     return std::none_of(product.terms.begin(),
                                         product.terms.end(),
                                         [=](const auto& productTerm) { return includedIn(productTerm, e.bitset); });
                 });

    return {quot, remainder};
}
