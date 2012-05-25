// Header file for multi-class learner
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


#ifndef LEARNER_MULTILABEL_H
#define LEARNER_MULTILABEL_H

#include "data_set.h"
#include "learner.h"


class MultiLabelLearner : public Learner
{
  public: 
    MultiLabelLearner ();
    int Init (int argc, char **argv);
  private:
    bool SingleUpdate (const DataSet &data_set);
    void Evaluate (const DataSet &data_set);
};

#endif
