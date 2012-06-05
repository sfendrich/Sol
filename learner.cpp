// Implementation of base learner class
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


#include <cmath>
#include <cstdio>

#include <iostream>

#include <boost/program_options.hpp>
#include "tiny_log.h"

#include "learner.h"


namespace po = boost::program_options;

std::istream& operator>> (std::istream& in, Learner::RegType& reg_type)
{
  std::string token;
  in >> token;
  if (token == "none")
    reg_type = Learner::kRegNone;
  else if (token == "l1")
    reg_type = Learner::kRegL1;
  else if (token == "l2")
    reg_type = Learner::kRegL2;
  return in;
}


Learner::Learner ()
: options_ ("Allowed options", po::options_description::m_default_line_length)
{
  // Add options
  po::options_description opt_general ("General options");
  opt_general.add_options ()
    ("help,h", "display this help message")
    ("eval,e",
      po::value<bool> (&evaluate_)->zero_tokens ()->default_value (false),
      "evaluate on data")
    ("input-file", po::value<std::string> (&data_in_), "name of data file")
    ("learn,l",
      po::value<bool> (&learn_)->zero_tokens ()->default_value (false),
      "learn from data")
    ("lr", po::value<float> (&initial_learning_rate_)->default_value (0.02),
      "learning rate")
    ("decreasing-lr",
      po::value<bool> (&decreasing_lr_)->default_value(false)->zero_tokens(),
      "use decreasing learning rate lr/(1+iteration)")
    ("margin,m", po::value<float> (&margin_)->default_value (1.0),
      "margin size")
    ("model-in", po::value<std::string> (&model_in_)->default_value (""),
      "initialize model from file")
    ("model-out",
      po::value<std::string> (&model_out_)->default_value (""),
      "write final model to file")
    ("intermediate-models",
      po::value<bool> (&write_intermediate_models_)->zero_tokens()
        ->default_value (false),
      "write model at each update")
    ("num-features,f",
      po::value<int> (&num_features_)->default_value(1, "input-file"),
      "number of features")
    ("num-iterations,i",
      po::value<int> (&num_iterations_)->default_value (100000),
      "number of iterations")
    ("num-instances,n", po::value<int> (&num_instances_)->default_value (0),
      "hint about number of instances in data")
    ("pegasos-projection",
      po::value<bool> (&pegasos_projection_)->zero_tokens ()
        ->default_value (false), "use pegasos style L2-ball projection")
    ("progress-interval",
      po::value<int> (&progress_interval_)->default_value (0), 
      "report progress every arg items")
    ("print-predictions,p",
      po::value<bool> (&print_predictions_)->zero_tokens ()
        ->default_value (false), "write predictions to stdout")
    ("print-result",
      po::value<bool> (&print_result_)->zero_tokens ()->default_value (false),
      "write result to stdout")
    ("random-seed",
      po::value<unsigned int> (&random_seed_)->default_value(time (NULL),
      "time (NULL)"), "random seed")
    ("reg-param,r", po::value<float> (&reg_param_)->default_value (1.0),
      "regularization parameter")
    ("reg-interval", po::value<int> (&reg_interval_)->default_value (1000),
      "regularization will take place every arg updates")
    ("reg-type,t", po::value<Learner::RegType> (&reg_type_)
      ->default_value (Learner::kRegL2, "l2"),
      "regularization type (none | l1 | l2)")
    ("verbosity", po::value<int> (), "verbosity level (0 ... 7)")
  ;
  options_.add (opt_general);
}


int Learner::Init (int argc, char ** argv)
{
  po::variables_map vm;
  po::store (po::parse_command_line (argc, argv, options_), vm);
  po::notify (vm);

  // TODO: Check consistency of given options

  // Print help if no option or help option is given 
  if (argc <= 1 || vm.count ("help"))
  {
    std::cerr << options_ << std::endl;
    return 1;
  }

  // Log level
  if (vm.count ("verbosity"))
    TinyLog::SetLevel (TinyLog::Level (vm["verbosity"].as<int> ()));

  return 0;
}


int Learner::Run ()
{
  // Initialize random number generator
  srand (random_seed_); 

  // Read data set
  DataSet data_set (num_instances_);
  INFO << "reading data (" << data_in_ << ") ..." << std::endl;
  id_t max_id = data_set.Read (data_in_.c_str ());
  if (max_id >= num_features_)
  {
    if (num_features_ != 0)
      WARN << "Maximum id in '" << data_in_ << "' greater than num-features ("
        << max_id << " >= " << num_features_ << ")" << std::endl;
    num_features_ = max_id + 1;
  }

  // Initialize model
  model_.Init (num_submodels_, num_features_);
  if (model_in_ != "")
  {
    INFO << "reading model (" << model_in_ << ") ..." << std::endl;
    model_.Read (model_in_.c_str ());
  }

  // Learn
  if (learn_)
  {
    INFO << "learning ..." << std::endl;
    Learn (data_set);
  }

  // Evaluate
  if (evaluate_)
  {
    INFO << "evaluating ..." << std::endl;
    Evaluate (data_set);
  }

  // Write model
  if (model_out_ != "")
  {
    INFO << "writing model (" << model_out_ << ") ..." << std::endl;
    model_.Write (model_out_.c_str ());
  }

  return 0;
}


void Learner::Learn (const DataSet &data_set)
{
  for (int i = 0; i < num_iterations_; ++i)
  {
    // Setup learning rate
    if (decreasing_lr_)
      learning_rate_ = initial_learning_rate_/(1.0 + reg_param_ * float(i));
    else
      learning_rate_ = initial_learning_rate_;

    // Update from loss
    bool model_updated = SingleUpdate (data_set);

    // Update from regularization
    if (i % reg_interval_ == 0)
    {
      switch (reg_type_)
      {
        // L1-regularization
        case kRegL1: 
          model_.RegularizeL1 (reg_param_ * learning_rate_);
          model_updated = true;
          break;

        // L2-regularization
        case kRegL2: 
          model_.RegularizeL2 (reg_param_ * learning_rate_);
          model_updated = true;
          break;

        // Unregularized
        case kRegNone: 
        default:
          break;
      }
    }

    // Update from pegasos ball projection
    if (pegasos_projection_)
    {
      for (int j = 0; j < model_.num_submodels (); ++j)
      {
        float factor = 1.0/(sqrt (reg_param_) * model_[0].squaredL2Norm ());
        if (factor < 1.0)
        {
          model_[j].Scale (factor);
          model_updated = true;
        }
      }
    }

    // Write intermediate models    
    if (write_intermediate_models_ && model_updated)
    {
      const int kBufSize = 1000;
      char buffer[kBufSize];
      snprintf (buffer, kBufSize, "%s.%08x", model_out_.c_str (), i);
      model_.Write (buffer);
    }

    // report progress
    if ((progress_interval_ > 0) &&  (i % progress_interval_) == 0)
      INFO << i << '/' << num_iterations_ << '\r';
  }
}
