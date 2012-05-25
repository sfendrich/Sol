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


#ifndef WEIGHT_VECTOR_H
#define WEIGHT_VECTOR_H

#include <cstring>

#include "sparse_vector.h"


class WeightVector
{
  public:
    WeightVector (int size);
    WeightVector (const WeightVector &copy);
    ~WeightVector ();

    int   size () const;
    float bias () const;
    void  set_bias (float bias);
    void  clear ();
    float GetWeight (int index) const;
    void  SetWeight (int index, float value);
    void  PlusEquals (const SparseVector &rhs);
    void  PlusEquals (float scalar, const SparseVector &rhs);
    float InnerProduct (const SparseVector &rhs) const;
    void  Scale (float factor);
    float squaredL2Norm () const;
    void  RegularizeL1 (const float factor);
    void  RegularizeL2 (const float factor);
  private:
    float *vector_;
    float bias_;
    float scale_;
    int   size_;
    float squaredL2Norm_;
};


inline int WeightVector::size () const
{
  return size_;
}


inline float WeightVector::bias () const
{
  return bias_;
}


inline void WeightVector::set_bias (float bias)
{
  bias_ = bias;
}


inline void WeightVector::clear ()
{
  memset (vector_, 0, size_ * sizeof (float)); 
  squaredL2Norm_ = 0;
}


inline float WeightVector::GetWeight (int index) const
{
  return scale_ * vector_[index];
}


inline void WeightVector::SetWeight (int index, float value)
{
  vector_[index] = value / scale_;
}


inline void WeightVector::Scale (float factor)
{
  scale_ *= factor;
}


inline float WeightVector::squaredL2Norm () const
{
  return squaredL2Norm_;
}


inline void WeightVector::RegularizeL2 (const float factor)
{
  Scale (1.0 - factor); // TODO: need minimum?
}

#endif
