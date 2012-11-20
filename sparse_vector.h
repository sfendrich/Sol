// Implementation of sparse vectors
//
// Copyright (C) 2012 Heidelberg University
//
// Author: Sascha Fendrich
//
// This file is part of Sol.
// 
// Sol is free software: you can redistribute it and/or modify
// it under the terms of the GNU Lesser General Public License as published by
// the Free Software Foundation, either version 3 of the License, or
// (at your option) any later version.
// 
// Sol is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
// GNU Lesser General Public License for more details.
// 
// You should have received a copy of the GNU Lesser General Public License
// along with Sol.  If not, see <http://www.gnu.org/licenses/>.


#ifndef SPARSE_VECTOR_H
#define SPARSE_VECTOR_H

#include <cstring>

#include <vector>

#include "common.h"


// TODO: if we define SparseVector as a template<Id, Value> we would
// TODO: become independent of id_t and common.h 
class SparseVector
{
  public:
    typedef std::pair<id_t, float> elem_t;
    typedef std::vector<elem_t>::const_iterator  const_iterator;
    typedef std::vector<elem_t>::const_reference const_reference;

    SparseVector ();
    float target () const;                // Get target value
    void  set_target (float target);      // Set target value
    float squaredL2Norm () const;         // Get squared L2-norm
    int   size () const;                  // Get vector size
    id_t  max_id () const;                // Get maximum id
    void  push_back (const elem_t &elem); // Append component
    float InnerProduct (const SparseVector &rhs) const; // inner product
    const_iterator begin () const;        // Iterator
    const_iterator end () const;          // Iterator
  private:
    std::vector<elem_t> vector_; // Vector data
    float target_;               // Target value
    float squaredL2Norm_;        // Squared L2-norm
    id_t  max_id_;               // Maximum id in vector
};


inline float SparseVector::target () const
{
  return target_;
}


inline void SparseVector::set_target (float target)
{
  target_ = target;
}


inline float SparseVector::squaredL2Norm () const
{
  return squaredL2Norm_;
}


inline int SparseVector::size () const
{
  return vector_.size ();
}


inline id_t SparseVector::max_id () const
{
  return max_id_;
}


inline void SparseVector::push_back (const SparseVector::elem_t &elem)
{
  // TODO: ensure increasing ids 
  vector_.push_back (elem);
  squaredL2Norm_ += elem.second * elem.second;
  if (elem.first > max_id_)
    max_id_ = elem.first;
}


inline SparseVector::const_iterator SparseVector::begin () const
{
  return vector_.begin ();
} 


inline SparseVector::const_iterator SparseVector::end () const
{
  return vector_.end ();
}


#endif
