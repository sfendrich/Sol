// Implementation of binary learner
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


#include <cstdio>
#include <cstdlib>

#include <iostream>

#include "tiny_log.h"

#include "learner_binary.h"
#include "data_set.h"
#include "learner.h"
#include "model.h"
#include "weight_vector.h"

namespace po = boost::program_options;


int BinaryLearner::Init (int argc, char **argv)
{
  int rv = Learner::Init (argc, argv);
  num_submodels_ = 1;
  return rv;
}


bool BinaryLearner::SingleUpdate (const DataSet &data_set)
{
  int   instance      = rand () % data_set.size ();
  float bias          = model_[0].bias (); 
  float model_score   = model_[0].InnerProduct (data_set[instance]) + bias;
  float target_value  = data_set[instance].target ();
  float target_sign   = sign (target_value);
  bool  model_updated = false;

  // Update from loss 
  if (target_sign * model_score < margin_) 
  {
    model_[0].PlusEquals (learning_rate_ * target_sign, data_set[instance]);
    model_[0].set_bias (bias + learning_rate_ * target_sign);
    model_updated = true;
  }
  return model_updated;
}


void BinaryLearner::Evaluate (const DataSet &data_set)
{
  int positive = 0; // TODO: true and false positives/negatives
  int negative = 0;
  int count    = data_set.size ();

  for (int i = 0; i < count; ++i)
  {
    // apply model
    float model_score  = model_[0].InnerProduct (data_set[i])
      + model_[0].bias ();
    float target_value = data_set[i].target ();

    // compare prediction with target
    if ((model_score * target_value > 0) || (model_score == target_value))
      positive++;
    else 
      negative++;

    // print predictions
    if (print_predictions_)
      std::cout << sign (model_score) << std::endl;

    // report progress
    if ((progress_interval_ > 0) && (i % progress_interval_ == 0))
      INFO << i << '/' << count << '\r';
  }
  float result = float (positive) / float (positive + negative);

  // log result
  INFO << "result: " << result
    << " (" << positive << '/' << positive + negative << ')' << std::endl;
  
  // print result to stdout
  if (print_result_)
    std::cout << result << std::endl;
}

