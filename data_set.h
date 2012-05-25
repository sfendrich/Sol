// Header for data set class
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


#ifndef DATA_SET_H
#define DATA_SET_H

#include <vector>

#include "common.h"
#include "sparse_vector.h"


class DataSet
{
  public:
    DataSet (int num_instances);
    id_t Read (const char *file_name);
    SparseVector operator[] (int index);
    const SparseVector operator[] (int index) const;
    size_t size () const;
  private:
    std::vector<SparseVector> data_set_;
};


inline SparseVector DataSet::operator[] (int index)
{
  return data_set_[index];
}


inline const SparseVector DataSet::operator[] (int index) const
{
  return data_set_[index];
}


inline size_t DataSet::size () const
{
  return data_set_.size ();
}

#endif
