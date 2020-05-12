//
//  digitclassifier.hpp
//  digitclassifier
//
//  Created by Jeeva Selvam on 3/30/19.
//  Copyright © 2019 Jeeva Selvam. All rights reserved.
//

#ifndef DIGITCLASSIFIER_hpp
#define DIGITCLASSIFIER_hpp

#include <iostream>
#include <vector>
#include <string>
#include <fstream>
#include <map>
#include <math.h>
#include <sstream>

typedef std::pair<int,int> Coordinates;
typedef std::vector<std::vector<double>> ConfusionMatrix;

class digitClassifier{
public:
    // Class variables that represent the "model" and probabilities.
    // All set to public for testing.
    int num_train_exmp;
    std::map<int,std::map<Coordinates,std::pair<int, int>>> data_set;
    std::map<int,std::map<Coordinates,std::pair<double, double>>> prob_set;
    std::vector<double> class_prob;
    
    // Constructor of the class, takes no arguments.
    // Initializes class variables.
    digitClassifier();
    
    // Basically is the user Interface of the program.
    // Runs everything in a while true loop.
    void UserInterface();
    
    // These methods are set to public for testing.
    
    // Converts data from files to a model for naive bayes algorithm.
    // Takes 2 std::strings that represent the label and data files path.
    // Values in data_set are updated according to the data and labels.
    void ImportData(const std::string &data_path,
                    const std::string &label_path);
    
    // Writes the existing data_set and number of examples to a file.
    // Takes an std::string that represents the file path as the only argument.
    // Returns a boolean that represents if the file writing was succesfull.
    bool WriteModelToFile(const std::string &file_path);
    
    // Uses the same file format as WriteModelToFile to import a data_set.
    // Takes an std::string that represents the file path as the only argument.
    // Returns a boolean that represents if the file reading was succesfull.
    bool ImportModelFromFile(const std::string &file_path);
    
    // Classifies the given images and prints out the correctnes percentage
    // Takes 2 std::strings that represent the label and data files path.
    // Returns a confusion matrix as described in the assignment sheet.
    ConfusionMatrix ClassifyImages(const std::string &file_path,
                        const std::string &label_path);
private:
    // Various constant values that are self explanatory
    const std::pair<int, int> kDefualtPairVal = std::make_pair(0,0);
    const double kClassifyConst = 0.00000009;
    const int kFirstDigit = 0;
    const int kLastDigit = 9;
    const int kImgSideLen = 28;
    const int kWhitePixVal = 0;
    const int kGrayBlackPixVal = 1;
    const char kPixValSeparator = ' ';
    const char kCoorValSeperator = ',';
    const char kGrayPixelChar = '+';
    const char kBlackPixelChar = '#';
    const std::string kDefTrainDataPath = "/Users/jeeva/Naive-Bayes-DigitClassifier/"
                                        "digitclassifier/digitdata/trainingimages";
    const std::string kDefTrainLabelPath = "/Users/jeeva/Naive-Bayes-DigitClassifier/"
                                        "digitclassifier/digitdata/traininglabels";
    const std::string kDefTestDataPath = "/Users/jeeva/Naive-Bayes-DigitClassifier/"
                                        "digitclassifier/digitdata/testimages";
    const std::string kDefTestLabelPath = "/Users/jeeva/Naive-Bayes-DigitClassifier/"
                                        "digitclassifier/digitdata/testlabels";
    const std::string kDefModelWriteReadPath = "/Users/jeeva/Naive-Bayes-DigitClassifier/"
                                            "digitclassifier/datamodel.txt";
    
    // Creates a 2D vector of type double of size 10 by 10.
    // Sets every value in the 2d vector to 0 by default.
    // Returns the created 2d vector/confusion matrix.
    ConfusionMatrix CreateConfusionMatrix();
    
    // Initializes data_set.
    // Sets every value inside pairs to 0 by default.
    void InitializeDataSet();
    
    // Initializes prob_set.
    // Sets every value inside pairs to 0 by default.
    void InitializeProbabilitySet();
    
    // Calculates the probability of each pixel of each digit using data_set.
    // Calculates the probability of each pixel and each class and stores them.
    void CalculateProbabilities();
    
    // Returns the most likely digit for an image.
    // Takes an std::map that represents an image as the only parameter.
    // Returns a value of int that represents what the image most likely is.
    int GetMostLikelyDigit(std::map<Coordinates,int> &image_set);
    
    // Checks the probability of a given image being the given digit.
    // Takes an std::map that represents an image and a digit as parameters.
    // Returns a double value that represents the probability of the image
    // being  the  given digit.
    double GetDigitProbability(const int &digit,
                               std::map<Coordinates,int> &image_set);
    
    // Returns the log value of the probability at given parameters.
    // Takes the digit, color of the pixel and the coordinates as the parameter.
    // Gets the value specified by the parameters from prob_set and returns the
    // log of that value.
    double GetPixelProbability(const int &digit, const int &color,
                               const Coordinates &coord);
    
    // Converts the part of the data_set associated with a digit to a string.
    // Takes an int representing a digit as the only parameter.
    // Returns an std::string that represents data_set[digit].
    std::string GetDigitString(const int &digit);
    
    // Splits a string into different strings by the given char.
    // Basically is the .split() method in Java strings.
    // Takes a string and where we want to split it as the parameters.
    // Returns a vector ccontaining the string split by the given char.
    std::vector<std::string> SplitString(const std::string &string,
                                         const char &split_point);
    
    // Performs the necessary action by evaluating the user input.
    // Takes an int value as the only input.
    // Calls almost every other method directly or indirectly.
    void EvaluateUserInput(const int &user_choice);
};
#endif /* digitclassifier_hpp */
