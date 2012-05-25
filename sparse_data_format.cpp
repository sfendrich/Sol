// Implementation of sparse data format reader
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


#include <cstddef>
#include <cstdlib>

#include <fstream>
#include <iostream>
#include <string>

#include "tiny_log.h"
#include "sparse_data_format.h"


// Parse a line in sparse data format and convert it to SparseVector.
// Return pointer to first unread chararcter, i.e. a successful
// parse ends at '\0' or at '#'.
const char *sdf_parse_line (const char *line, SparseVector &features)
{
  const char *pos = line;
  char *end;

  // target value
  float value = strtof (pos, &end);
  if (pos == end)
  {
    FATAL << "Can't read target value" << std::endl;
    return pos; 
  }
  features.set_target (value);
  pos = end;

  // TODO: optional query id

  // eat white space
  while (isspace (*pos)) ++pos;  

  // features
  int last_id = 0;
  while ((*pos) && (*pos != '#')) // until end of line or comment char
  {
    // read feature id
    unsigned long convert_buffer = strtoul (pos, &end, 10);
    if (pos == end)
    {
      FATAL << "Can't read feature id" << std::endl;
      return pos;
    }
    pos = end;
    id_t id (convert_buffer); // TODO: check for overflow

    // check increasing order
    if (last_id >= id)
    {
      FATAL << "Feature ids must be in increasing order, but " <<
        last_id << " >= " << id << std::endl;
      return pos;
    }
    last_id = id;

    // read separator
    if (*pos != ':')
    {
      FATAL << "Colon ':' expected" << std::endl;
      return pos; 
    }
    ++pos;

    // read feature value
    float value = strtof (pos, &end);
    if (pos == end)
    {
      FATAL << "Can't read feature value" << std::endl;
      return pos;    
    }
    pos = end;

    // append to vector
    features.push_back (std::make_pair (id, value));

    // eat white space
    while (isspace (*pos)) ++pos;  
  }  

  return pos; 
}
