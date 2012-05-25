// Implementation of data set class
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


#include <fstream>
#include <string>

#include "tiny_log.h"

#include "data_set.h"
#include "sparse_data_format.h"


DataSet::DataSet (int num_instances)
{
  if (num_instances > 0)
    data_set_.reserve (num_instances);
}


id_t DataSet::Read (const char *file_name)
{
  std::ifstream ifs (file_name);
  std::string line;
  id_t max_id = 0;
  int line_count = 0;

  while (getline (ifs, line))
  {
    line_count++;
    SparseVector temp;
    const char *pos = sdf_parse_line (line.c_str (), temp);
    if (*pos && (*pos != '#'))
    {
      FATAL << "Error in input:" << line_count << ':' 
        << pos - line.c_str () + 1 << std::endl; 
      return 0;
    }
    data_set_.push_back (temp);
    if (temp.max_id () > max_id)
      max_id = temp.max_id ();
  }   
  return max_id;
}
