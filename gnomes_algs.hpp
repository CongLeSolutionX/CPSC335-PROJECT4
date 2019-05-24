///////////////////////////////////////////////////////////////////////////////
// gnomes_algs.hpp
//
// Algorithms that solve the greedy gnomes problem.
//
// All of the TODO sections for this project reside in this file.
//
// This file builds on gnomes_types.hpp, so you should familiarize yourself
// with that file before working on this file.
//
///////////////////////////////////////////////////////////////////////////////

#pragma once

#include <cassert>
#include <vector>
#include "gnomes_types.hpp"
#include <optional>
using namespace std;
namespace gnomes {
    
    // Solve the greedy gnomes problem for the given grid, using an exhaustive
    // search algorithm.
    //
    // This algorithm is expected to run in exponential time, so the grid's
    // width+height must be small enough to fit in a 64-bit int; this is enforced
    // with an assertion.
    //
    // The grid must be non-empty.
    path greedy_gnomes_exhaustive(const grid& setting) {
        
        // grid must be non-empty.
        assert(setting.rows() > 0);
        assert(setting.columns() > 0);
        
        // Compute maximum path length, and check that it is legal
        // maxlen = r + c - 2
        const size_t max_steps = setting.rows() + setting.columns() - 2;
        // make max_steps small enough to fit in a 64-bit int
        assert(max_steps < 64);
        // best = None
        path best(setting);
        
        // for len from 0 to maxlen inclusive
        for(size_t len = 0; len <= max_steps; len++){
            // for bits from 0 to 2^len - 1 inclusive
            for(size_t bits = 0; bits <= pow(2,len)-1; bits++){
                // candidate = [start]
                path candidate(setting);
                // for k from 0 to len-1 inclusive
                for(size_t k = 0; k < len; ++k){
                    int bit;
                    bit = (bits >> k) & 1;
                    // if bit is 1, then we will move →
                    if(bit == 1){
                        // make sure the candidate stays inside the grid and never crosses an X cell
                        if(candidate.is_step_valid(STEP_DIRECTION_RIGHT)){
                            // adding a right move to candidate
                            candidate.add_step(STEP_DIRECTION_RIGHT);
                        }
                     // else bit is 0, then we will move ↓
                    }else {
                        // make sure the candidate stays inside the grid and never crosses an X cell
                        if(candidate.is_step_valid(STEP_DIRECTION_DOWN)){
                            // adding a down move to candidate
                            candidate.add_step(STEP_DIRECTION_DOWN);
                        }
                    }
                }
                // if best is None or candidate harvests more gold than best
                if(best.last_step() == STEP_DIRECTION_START || candidate.total_gold() > best.total_gold()){
                    best = candidate;
                }
            }
        }
        return best;
    }
    
    // Solve the greedy gnomes problem for the given grid, using a dynamic
    // programming algorithm.
    //
    // The grid must be non-empty.
    path greedy_gnomes_dyn_prog(const grid& setting) {
        
        // grid must be non-empty.
        assert(setting.rows() > 0);
        assert(setting.columns() > 0);
        
        // A = new row⨉column matrix
        vector<vector<optional<path>> > A(setting.rows(),vector<optional<path>> (setting.columns(), nullopt));
        // base case
        // A[0][0] = [start]
        A[0][0] = path(setting) ;
        
        // general cases
        // for i from 0 to r-1 inclusive
        for (size_t i = 0; i <= setting.rows() - 1; ++i) {
            // for j from 0 to c-1 inclusive
            for ( size_t j = 0; j <= setting.columns() - 1; ++j) {
                // if we are hitting a rock
                if (setting.get(i,j) == CELL_ROCK) {
                    // A[i][j]=None
                    A[i][j].reset();
                    continue;
                }
                // from_above = from_left = None
                optional <path> from_above;
                optional <path> from_left;
                // if i>0 and A[i-1][j] is not None
                if (i > 0 && A[i-1][j].has_value()) {
                    // from_above = A[i-1][j] + [↓]
                    from_above = A[i-1][j];
                    if(from_above->is_step_valid(STEP_DIRECTION_DOWN)){
                        from_above->add_step(STEP_DIRECTION_DOWN);
                    }
                }
                // if j>0 and A[i][j-1] is not None
                if (j > 0&&A[i][j-1].has_value())  {
                    // from_left = A[i][j-1] + [→]
                    from_left = A[i][j-1];
                    if(from_left->is_step_valid(STEP_DIRECTION_RIGHT)){
                        from_left->add_step(STEP_DIRECTION_RIGHT);
                    }
                }
                // A[i][j] will be assigned to the following cases:
                // if from_above and from_left is non-None
                if(from_above.has_value() && from_left.has_value()){
                    // if from_above has more gold than from_left
                    if  (from_above->total_gold() > from_left->total_gold()) {
                        A[i][j] = from_above;
                    }
                    // else from_left has more gold than from_above, then
                    else  {
                        A[i][j] = from_left;
                    }
                }
                // if from_above is non-None
                else if(from_above.has_value()) {
                    A[i][j] = from_above;
                }
                // if from_left is non-None
                else if (from_left.has_value()){
                    A[i][j] = from_left;
                }
                
            }
            
        }
        // post-processing to find maximum-gold path
        // best = A[0][0]
        optional <path> best = A[0][0];     // This path is always legal, but not necessarily optimal
        // for i from 0 to r-1 inclusive
        for (size_t i = 0; i <= setting.rows() - 1; ++i) {
            // for j from 0 to c-1 inclusive
            for (size_t j = 0; j <= setting.columns() - 1; ++j) {
                // if A[i][j] is not None and A[i][j] harvests more gold than best
                if ((A[i][j].has_value())&&(A[i][j]->total_gold() > best->total_gold())) {
                    best = A[i][j];
                }
            }
        }
        return  *best;
    }
    
}
