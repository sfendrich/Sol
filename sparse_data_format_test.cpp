// Unit test for sparse data format reader
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


#include <iostream>
#include <string>

#include "sparse_data_format.h"
#include "sparse_vector.h"
#include "tiny_logging.h"


int main (int argc, char **argv)
{
  std::string line;
  int count = 0;

  TinyLog::SetLevel (TinyLog::kAll);
  std::cout << TinyLog::level () << std::endl;
  while (getline (std::cin, line))
  {
    count++;
    SparseVector features;
    const char *pos = sdf_parse_line (line.c_str (), features);
    if (*pos && (*pos != '#'))
    {
      FATAL << "Error in input:" << count << ':' << pos - line.c_str () + 1
        << std::endl; 
    }
  }
}
