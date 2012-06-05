// Header file for learner class
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


#ifndef LEARNER_H
#define LEARNER_H

#include <string>

#include <boost/program_options.hpp>

#include "data_set.h"
#include "model.h"


class Learner
{
  public:
    typedef enum { kRegNone, kRegL1, kRegL2 } RegType; // Regulatization type
    Learner ();                       // Constructor
    int Init (int argc, char **argv); // Initialize options
    int Run ();                       // Run learning process
  private:
    void Learn (const DataSet &data_set);                    // SGD loop
    virtual bool SingleUpdate (const DataSet &data_set) = 0; // Loss-update
    virtual void Evaluate (const DataSet &data_set) = 0;     // Evaluation
  protected:
    boost::program_options::options_description options_;    // Program options
    Model model_;                     // Learning model
    bool  learn_;                     // Learn model on input data
    bool  evaluate_;                  // Evaluate model on input data
    bool  print_result_;              // Print evaluation result to std::cout
    bool  print_predictions_;         // Print predictions to std::cout
    bool  pegasos_projection_;        // Use pegasos L2-ball projection
    std::string data_in_;             // Read data from file
    std::string model_in_;            // Read an initial model from file
    std::string model_out_;           // Write model to file
    bool  write_intermediate_models_; // Write model at each iteration
    bool  decreasing_lr_;             // Use decreasing learning rate
    float initial_learning_rate_;     // Initial learning rate
    float learning_rate_;             // Current learning rate
    float margin_;                    // Margin
    RegType reg_type_;                // Regularization type
    float reg_param_;                 // Regularization parameter
    int   reg_interval_;              // Updates between regularization steps
    int   num_classes_;               // Number of classes (multi-class)
    int   num_labels_;                // Number of labels (multi-label)
    int   num_features_;              // Number of features
    int   num_iterations_;            // Number of iterations
    int   num_instances_;             // Number of instances in data set
    int   num_submodels_;             // Number of submodels
    int   progress_interval_;         // Updates between progress reports
    unsigned random_seed_;            // Random seed
};

std::istream& operator>> (std::istream& in, Learner::RegType& reg_type);

#endif
