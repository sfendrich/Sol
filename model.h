// Header file for linear learning models
//
// Copyright (C) Heidelberg University
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


#ifndef MODEL_H
#define MODEL_H

#include "weight_vector.h"


class Model
{
  public:
    void Init (int num_submodels, int num_features);
    void Read  (const char *file_name);
    void Write (const char *file_name);
    WeightVector &operator[] (int index); // TODO: do we really want this?
    int num_submodels () const;
    int num_features () const;
    void RegularizeL1 (const float factor);
    void RegularizeL2 (const float factor);
  private:
    std::vector<WeightVector> submodels_;
};


inline WeightVector &Model::operator[] (int index)
{
  return submodels_[index];
}


inline int Model::num_submodels () const
{
  return submodels_.size ();
}


inline int Model::num_features () const
{
  return submodels_[0].size ();
}

#endif
