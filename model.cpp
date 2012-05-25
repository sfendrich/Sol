// Implementation of linear learning models
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


#include <fstream>

#include "model.h"
#include "sparse_data_format.h"
#include "tiny_log.h"


void Model::Init (int num_submodels, int num_features)
{
  for (int i = 0; i < num_submodels; ++i)
  {
    WeightVector w (num_features);
    submodels_.push_back (w);
  }
}


void Model::Read (const char *file_name)
{
  std::ifstream ifs (file_name);
  std::string line;
  SparseVector s;
 
  for (int i = 0; i < submodels_.size (); ++i)
  {
    getline (ifs, line);
    const char *pos = sdf_parse_line (line.c_str (), s);
    if (*pos && (*pos != '#'))
    {
      FATAL << "Error in input:" << i + 1 << ':' << pos - line.c_str () + 1
        << std::endl; 
      // TODO: error handling
    }
    submodels_[i].clear ();
    submodels_[i].PlusEquals (s); 
    submodels_[i].set_bias (s.target ()); 
  }
}


void Model::Write (const char *file_name) // TODO: error handling
{
  std::ofstream ofs (file_name);
  for (int j = 0; j < submodels_.size (); ++j)
  {
    ofs << submodels_[j].bias () << " ";
    for (int i = 0; i < submodels_[0].size (); ++i)
    {
      float weight = submodels_[j].GetWeight (i); 
      if (weight != 0)
        ofs << i << ':' << weight << ' ';
    }
    ofs << std::endl;
  }
}


void Model::RegularizeL1 (const float factor)
{
  for (int i = 0; i < num_submodels (); ++i)
  {
    submodels_[i].RegularizeL1 (factor);
  }
}


void Model::RegularizeL2 (const float factor)
{
  for (int i = 0; i < num_submodels (); ++i)
  {
    submodels_[i].RegularizeL2 (factor);
  }
}

