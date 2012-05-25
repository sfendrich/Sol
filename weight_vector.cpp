// Implementation of dense weight vectors
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


#include <cstdlib>
#include <cstring>

#include "sparse_vector.h"
#include "weight_vector.h"


WeightVector::WeightVector (int size)
  :size_(size)
  ,bias_(0)
  ,squaredL2Norm_(0)
  ,scale_(1.0)
{
  vector_ = new float[size_];
  memset (vector_, 0, size_ * sizeof (float));
}


WeightVector::WeightVector (const WeightVector &copy)
{
  size_          = copy.size_;
  bias_          = copy.bias_;
  scale_         = copy.scale_;
  squaredL2Norm_ = copy.squaredL2Norm_;
  vector_        = new float[size_];
  memcpy (vector_, copy.vector_, size_* sizeof (float));
}


WeightVector::~WeightVector ()
{
  delete[] vector_;
}


void WeightVector::PlusEquals (const SparseVector &rhs)
{
  float accum = 0;
  for (SparseVector::const_iterator i = rhs.begin (); 
    i != rhs.end (); ++i)
  {
    accum += i->second * vector_[i->first]; 
    vector_[i->first] += i->second / scale_;
  }
  squaredL2Norm_ += rhs.squaredL2Norm () - 2 * scale_ * accum;
}


void WeightVector::PlusEquals (float scalar, const SparseVector &rhs)
{
  float accum = 0;
  for (SparseVector::const_iterator i = rhs.begin (); 
    i != rhs.end (); ++i)
  {
    accum += i->second * vector_[i->first]; 
    vector_[i->first] += scalar * i->second / scale_;
  }
  squaredL2Norm_ += scalar *
    (scalar * rhs.squaredL2Norm () - 2 * scale_ * accum);
}


float WeightVector::InnerProduct (const SparseVector &rhs) const
{
  float ip = 0;
  for (SparseVector::const_iterator i = rhs.begin ();
    i != rhs.end (); ++i)
  {
    ip += vector_[i->first] * i->second;
  }
  return scale_ * ip;
}


void WeightVector::RegularizeL1 (const float factor)
{ 
  for (int i = 0; i < size (); ++i)
  {
    float weight = GetWeight (i);
    weight -= sign (weight) * factor;
    // Truncate
    if (abs (weight) < factor)
      weight = 0;
    SetWeight (i, weight);
  }
}
