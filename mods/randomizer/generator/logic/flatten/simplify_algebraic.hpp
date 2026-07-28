// Algebraic simplification techniques treat all requirements as unrelated variables
// and allow us to turn our two-level DNF/sum-of-products form into a simpler
// multi-level expression.

// The approach taken here is mostly used in hardware logic synthesis, and described in:

//  * https://faculty.sist.shanghaitech.edu.cn/faculty/zhoupq/Teaching/Spr16/07-Multi-Level-Logic-Synthesis.pdf
//    * Some lecture slides about the topic. These make the concepts of kernels,
//      algebraic division, and rectangles very accessible, but e.g. how to actually find rectangles is left open.
//  * Rudell 1989, Logic Synthesis for VLSI Design
//    https://www2.eecs.berkeley.edu/Pubs/TechRpts/1989/ERL-89-49.pdf (pp. 41-70)
//    * Rudell's PhD thesis is where this stuff was originally researched.
//      The pseudocode for generating prime rectangles is found there and quite useful.

// This approach does not exploit boolean properties like x & !x = false or x | !x = true
// but logic doesn't need this since we only have positive terms. The other thing
// these techniques don't handle are "implies" relations like Beetle x 2 => Beetle x 1,
// so we may use different techniques for those.

#pragma once

#include "bits.hpp"
#include <algorithm>
#include <iterator>
#include <functional>

struct FoundKernel
{
    std::vector<BitVector> kernel;
    BitVector coKernel;
};

randomizer::logic::requirement::Requirement DNFToExpr(BitIndex& bitIndex, DNF dnf);

randomizer::logic::requirement::Requirement createAnd(std::vector<randomizer::logic::requirement::Requirement> terms);

// Generates all prime rectangles in this matrix. A rectangle is a set of columns and rows
// such that for every row and column, matrix[row][colum] is not zero. A prime rectangle
// is a rectangle that is not included in any other rectangle.
template<typename Func>
void genRectangles(std::vector<int>& rows, std::vector<int>& cols, std::vector<std::vector<int>>& matrix, Func callback)
{
    // generate trivial prime rectangles first
    // trivial rectangles are rectangles with only
    // one row or one column
    for (const auto& row : rows)
    {
        // Find the ones in this row
        std::vector<int> ones = {};
        std::copy_if(cols.begin(), cols.end(), std::back_inserter(ones), [=](const int& c) { return matrix[row][c]; });
        // if this row has ones and there's no other row that
        // has ones in the same positions, this row is part of
        // a trivial row prime rectangle
        if (!ones.empty() and
            std::none_of(
                rows.begin(),
                rows.end(),
                [=](const int& r)
                { return r != row && std::all_of(ones.begin(), ones.end(), [=](const int& c) { return matrix[r][c]; }); }))
        {
            callback({row}, ones);
        }
    }

    for (const auto& col : cols)
    {
        // Same as above
        std::vector<int> ones = {};
        std::copy_if(rows.begin(), rows.end(), std::back_inserter(ones), [=](const int& r) { return matrix[r][col]; });

        if (!ones.empty() and
            std::none_of(
                rows.begin(),
                rows.end(),
                [=](const int& c)
                { return c != col && std::all_of(ones.begin(), ones.end(), [=](const int& r) { return matrix[r][c]; }); }))
        {
            callback(ones, {col});
        }
    }

    genRectanglesRecursive(rows, cols, matrix, 0, {}, {}, callback);
}

// Recursively generates non-trivial prime rectangles based on the
// existing prime rectangle given by matrix and rect_cols. Rectangles generated
// by this function will have fewer rows but more columns than the passed rectangle.

// Args:
//     all_rows:  A list of all row indices, for convenience.
//     all_cols:  A list of all column indices, for convenience.
//     matrix:    The matrix being searched for rectangles.
//     index:     Grow the rectangle starting from this column
//     rect_rows: Rows of the prime rectangle.
//     rect_cols: Columns of the prime rectangle.
//     callback:  Called for every prime rectangle.
template<typename Func>
void genRectanglesRecursive(std::vector<int>& allRows,
                            std::vector<int>& allCols,
                            std::vector<std::vector<int>>& matrix,
                            const int& index,
                            std::vector<int> rectRows,
                            std::vector<int> rectCols,
                            Func callback)
{
    // do not consider columns before the starting index, and require
    // this column to have two or more ones (otherwise we'd generate a trivial rectangle)
    for (const auto& c : allCols)
    {
        if (c >= index && std::count_if(allRows.begin(), allRows.end(), [=](const int& row) { return matrix[row][c]; }) >= 2)
        {
            // create submatrix, only keeping rows where the column has a one
            // all other rows are zeroed
            std::vector<std::vector<int>> m1 = {};
            for (int rowIdx = 0; rowIdx < matrix.size(); rowIdx++)
            {
                auto& row = matrix[rowIdx];
                m1.push_back(matrix[rowIdx][c] ? row : std::vector(row.size(), 0));
            }

            // create new rect rows based on this column. If we had an existing
            // rectangle in the recursive case, this shrinks the rectangle, otherwise
            // it creates the first rectangle
            std::vector<int> rect1Rows;
            std::copy_if(allRows.begin(),
                         allRows.end(),
                         std::back_inserter(rect1Rows),
                         [=](const int& row) { return matrix[row][c]; });
            std::vector<int> rect1Cols = rectCols;

            bool prune = false;
            // add column c and all columns with EXACTLY the same number of ones
            for (const auto& c1 : allCols)
            {
                if (std::count_if(allRows.begin(), allRows.end(), [=](const int& row) { return m1[row][c1]; }) ==
                    std::count_if(allRows.begin(), allRows.end(), [=](const int& row) { return matrix[row][c]; }))
                {
                    if (c1 < c)
                    {
                        // "if a column of 1's occurs for a column index less than
                        // the starting index, then all rectangles in the current
                        // submatrix have already been examined when that column
                        // was processed" (Rudell)
                        prune = true;
                        break;
                    }
                    else
                    {
                        // add the column to our rectangle
                        rect1Cols.push_back(c1);
                        for (const auto& row : allRows)
                        {
                            m1[row][c1] = 0;
                        }
                    }
                }
            }

            if (!prune)
            {
                callback(rect1Rows, rect1Cols);
                genRectanglesRecursive(allRows, allCols, m1, c, rect1Rows, rect1Cols, callback);
            }
        }
    }
}

std::vector<FoundKernel> findKernels(const std::vector<BitVector>& cubes,
                                     const std::vector<int>& variables,
                                     const BitVector& coKernelPath,
                                     std::vector<BitVector>& seenCoKernels,
                                     int minIdx = 0);

std::pair<std::vector<BitVector>, std::vector<BitVector>> algebraicDivision(const std::vector<BitVector>& expr,
                                                                            const std::vector<BitVector>& divisor);

template<typename Container>
std::vector<randomizer::logic::requirement::Requirement> lookupRequirements(BitIndex& bitIndex, Container r)
{
    std::vector<randomizer::logic::requirement::Requirement> reqs;
    for (auto& bit : r)
    {
        reqs.push_back(bitIndex.reverseIndex[bit]);
    }
    return reqs;
}
