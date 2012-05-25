// Implementation of multi-label learner
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


#include <iostream>

#include <boost/program_options.hpp>
#include "tiny_log.h"

#include "learner_multilabel.h"
#include "weight_vector.h"
#include "sparse_vector.h"

namespace po = boost::program_options;


MultiLabelLearner::MultiLabelLearner ()
{
  // Add multi-label-options
  po::options_description opt_special ("Multi-label options");
  opt_special.add_options ()
    ("num-labels,c", po::value<int> (&num_labels_), // TODO: required
      "number of labels (class labels in 0 ... 2^arg - 1)")
  ;
  options_.add (opt_special);
}


// Initialize multi-label learner
int MultiLabelLearner::Init (int argc, char **argv)
{
  // Call parent member
  int rv = Learner::Init (argc, argv);

  // Setup multi-label specific configuration
  num_submodels_ = num_labels_;
  num_classes_   = 1 << (num_labels_ - 1);

  return rv;
}


// Learn multi-label classifier
bool MultiLabelLearner::SingleUpdate (const DataSet &data_set)
{
  int index  = rand () % data_set.size ();
  int target = int (data_set[index].target ());
  bool model_updated = false;

  // Update from loss 
  for (int j = 0; j < model_.num_submodels ();  ++j)
  {
    int   current_class = 1 << j;
    float target_sign   = (target & current_class)?1:-1;
    float bias          = model_[j].bias ();
    float score         = model_[j].InnerProduct (data_set[index]) + bias;

    if (target_sign * score < 1)
    {
      model_[j].PlusEquals (target_sign * learning_rate_, data_set[index]);
      model_[j].set_bias (bias + learning_rate_ * target_sign);
      model_updated = true;
    }
  }
  return model_updated;
}


// Evaluate multi-label classifier
void MultiLabelLearner::Evaluate (const DataSet &data_set)
{
  int positive = 0;
  int negative = 0;
  int count    = data_set.size ();

  for (int i = 0; i < count; ++i)
  {
    // Apply model
    int predicted_class = 0; 
  
    for (int j = 0; j < model_.num_submodels (); ++j)
    {
      float tmp_score = model_[j].InnerProduct (data_set[i]) + 
        model_[j].bias ();
      if (tmp_score > 0)
      {
        predicted_class |= 1 << j;
      }
    }

    // Compare prediction with target
    int target_value = int (data_set[i].target ());
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
  float result = float (positive) / float (positive + negative);

  // Log result
  INFO << "result: " << result
    << " (" << positive << '/' << positive + negative << ')' << std::endl;
  
  // Write result to stdout
  if (print_result_)
    std::cout << result << std::endl;
}

