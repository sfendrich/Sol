// Implementation of multi-class learner
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

#include <iostream>
#include <limits>

#include <boost/program_options.hpp>
#include "tiny_log.h"

#include "data_set.h"
#include "learner_multiclass.h"
#include "weight_vector.h"

namespace po = boost::program_options;


MultiClassLearner::MultiClassLearner ()
{
  // Add multi-class options 
  po::options_description opt_special ("Multi-class options");
  opt_special.add_options ()
    ("num-classes,c", po::value<int> (&num_classes_),
      "number of classes (class labels in 0 ... arg - 1)") 
  ;
  options_.add (opt_special);
}


// Initialize multi-class learner
int MultiClassLearner::Init (int argc, char **argv)
{
  // Call parent member
  int rv = Learner::Init (argc, argv);

  // Setup multi-class specific configuration
  num_submodels_ = num_classes_;

  return rv;
}


// Learn multi-class classifier
bool MultiClassLearner::SingleUpdate (const DataSet &data_set)
{
  int   index  = rand () % data_set.size ();
  float target = data_set[index].target ();
  float bias   = model_[target].bias ();
  float score  = model_[target].InnerProduct (data_set[index]) + bias;
  bool  model_updated = false;

  // Find max margin violator
  int   max_class = target;
  float max_bias  = 0;
  float max_score = - std::numeric_limits<float>::max ();
  for (int c = 0; c < model_.num_submodels (); ++c)
  {
    if (c != target)
    {
      float tmp_bias  = model_[c].bias ();
      float tmp_score = model_[c].InnerProduct (data_set[index]) + tmp_bias;
      if (max_score < tmp_score)
      {
        max_class = c;
        max_bias  = tmp_bias;
        max_score = tmp_score;
      } 
    } 
  }

  // Update from loss 
  if ((max_class != target) && (score - max_score < margin_))
  {
    model_[target].PlusEquals (learning_rate_, data_set[index]);
    model_[target].set_bias (bias + learning_rate_);
    model_[max_class].PlusEquals (- learning_rate_, data_set[index]);
    model_[max_class].set_bias (max_bias - learning_rate_);
    model_updated = true;
  }

  return model_updated;
}


void MultiClassLearner::Evaluate (const DataSet &data_set)
{
  int positive = 0;
  int negative = 0;
  int count    = data_set.size ();

  for (int i = 0; i < count; ++i)
  {
    // Apply model
    float target_value = data_set[i].target ();
    float model_score  = - std::numeric_limits<float>::max();
    int   predicted_class = -1; 
  
    for (int j = 0; j < model_.num_submodels (); ++j)
    {
      float tmp_score = model_[j].InnerProduct (data_set[i]) + 
        model_[j].bias ();
      if (tmp_score > model_score)
      {
        model_score = tmp_score;
        predicted_class = j;
      }
    }

    // Compare prediction with target
    if (predicted_class == target_value)
      positive++;
    else 
      negative++;

    // Write predictions
    if (print_predictions_)
      std::cout << predicted_class << std::endl;

    // Report progress
    if ((progress_interval_ > 0) && (i % progress_interval_ == 0))
      INFO << i << '/' << count << '\r';
  }
  float result = float (positive) / float (positive+negative);

  // Log result
  INFO << "result: " << result
    << " (" << positive << '/' << positive+negative << ')' << std::endl;
  
  // Write result to stdout
  if (print_result_)
    std::cout << result << std::endl;
}
