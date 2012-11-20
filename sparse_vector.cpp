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


#include "sparse_vector.h"


SparseVector::SparseVector ()
: target_(0)
, squaredL2Norm_(0)
,max_id_(0)
{}


float SparseVector::InnerProduct (const SparseVector &rhs) const
{
  float result = 0;
  const_iterator left  = begin ();
  const_iterator right = rhs.begin ();
  while ((left != end ()) && (right != rhs.end ()))
  {
    if (left->first < right->first)
      ++left;
    else if (left->first > right->first)
      ++right;
    else
    {
      result += left->second * right->second;
      ++left;
      ++right;
    }
  }
  return result;
}
