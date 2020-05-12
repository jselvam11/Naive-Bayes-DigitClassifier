//
//  digitclassifier.cpp
//  digitclassifier
//
//  Created by Jeeva Selvam on 3/30/19.
//  Copyright © 2019 Jeeva Selvam. All rights reserved.
//

#include "digitclassifier.hpp"

digitClassifier::digitClassifier(){
    num_train_exmp = 0;
    InitializeProbabilitySet();
    InitializeDataSet();
}

void digitClassifier::InitializeDataSet(){
    data_set = std::map<int,std::map<Coordinates,std::pair<int, int>>>();
    for (int digit = kFirstDigit; digit <= kLastDigit; digit++) {
        std::map<Coordinates,std::pair<int, int>> numRepeatance;
        for (int row = 0; row < kImgSideLen; row++) {
            for (int col = 0; col < kImgSideLen; col++) {
                // Adds a pair with two zeroes for each key in the map.
                // First value in each pair represents number of white pixels.
                // Second value in the pair is the number of grey or dark pixels
                numRepeatance[std::make_pair(col,row)] = kDefualtPairVal;
            }
        }
        
        data_set[digit] = numRepeatance;
    }
}

void digitClassifier::InitializeProbabilitySet(){
    class_prob = std::vector<double>();
    prob_set = std::map<int,std::map<Coordinates,std::pair<double, double>>>();
    for (int digit = kFirstDigit; digit <= kLastDigit; digit++) {
        std::map<Coordinates,std::pair<double,double>> numRepeatance;
        for (int row = 0; row < kImgSideLen; row++) {
            for (int col = 0; col < kImgSideLen; col++) {
                // Adds a pair with two zeroes for each key in the map.
                numRepeatance[std::make_pair(col, row)] = kDefualtPairVal;
            }
        }
        prob_set[digit] = numRepeatance;
    }
    
}

void digitClassifier::UserInterface(){
    while (true) {
    std::cout<< "Please Enter 0 to read training data to create a model, "
                "1 to write an existing model to a file, 2 to read an existing "
                "model from a file, 3 to clasify data" << std::endl;
    int user_choice;
    try {
        std::string input;
        getline(std::cin, input);
        user_choice = std::stoi(input);
    } catch (std::invalid_argument& e) {
        std::cout << "Please give a valid input" << std::endl;
        continue;
    }
        EvaluateUserInput(user_choice);
    }
}

void digitClassifier::EvaluateUserInput(const int &user_choice){
    switch (user_choice) {
        case 0:{
            std::cout << "Please provide a file path for data"
                         "or leave blank for default" << std::endl;
            std::string data_path;
            getline(std::cin, data_path);
            if (data_path == ""){
                //Default values are used when value entered is empty.
                ImportData(kDefTrainDataPath,kDefTrainLabelPath);
            } else {
            std::cout << "Please provide a file path"
                         " for training labels" << std::endl;
            std::string label_path;
            getline(std::cin, label_path);
                ImportData(data_path, label_path);
            }
            std::cout << "You have succesfully created a model" << std::endl;
            return;
            
        } case 1:{
            std::cout << "Please provide a file path to write a model "
                         "or leave blank for default" << std::endl;
            std::string file_path;
            getline(std::cin, file_path);
            if(file_path == ""){
                //Default value is used when value entered is empty.
                WriteModelToFile(kDefModelWriteReadPath);
            } else {
                WriteModelToFile(file_path);
            }
            std::cout << "You have succesfully wrote a model"
                         " to a file" << std::endl;
            return;
            
        } case 2:{
            std::cout << "Please provide a file path to read a model from "
                         "or leave blank for default" << std::endl;
            std::string file_path;
            getline(std::cin, file_path);
            if(file_path == ""){
                //Default value is used when value entered is empty.
                ImportModelFromFile(kDefModelWriteReadPath);
            } else {
                ImportModelFromFile(file_path);
            }
            std::cout << "You have succesfully read a model"
                         " from a file" << std::endl;
            return;
            
        } case 3:{
            std::cout << "Please provide a file path for data "
                         "or leave blank for default" << std::endl;
            std::string data_path;
            getline(std::cin, data_path);
            if (data_path == ""){
                //Default values are used when value entered is empty.
                ClassifyImages(kDefTestDataPath,kDefTestLabelPath);
            } else {
                std::cout << "Please provide a file path for"
                             " training labels" << std::endl;
                std::string label_path;
                getline(std::cin, label_path);
                ClassifyImages(data_path, label_path);
            }
            
            std::cout << "You have succesfully classified images"
                         " and created a confusion matrix" << std::endl;
            return;
            
        } default:
            std::cout << "Please enter a valid input"<< std::endl;
            return;
    }
}

void digitClassifier::ImportData(const std::string &data_path,
                                 const std::string &label_path) {
    std::ifstream train_file(data_path);
    std::string line;
    std::ifstream label_file(label_path);
    std::string label;
    
    while (true) {
        // Loop ends when labels are finished.
        // There are 28 lines in data file for each line in label file.
        if (!std::getline(label_file,label)) {
            break;
        }
        int label_ind = stoi(label);
        // num_train_exmp increases by 1 with each loop.
        num_train_exmp++;
        for (int row = 0; row < kImgSideLen; row++) {
            std::getline(train_file,line);
            for (int col = 0; col < kImgSideLen; col++) {
                Coordinates coord = std::make_pair(col,row);
                if (line.at(col) == kGrayPixelChar ||
                    line.at(col) == kBlackPixelChar) {
                    // The number of dark pixels at given coordinates is
                    // increased by one.
                    data_set[label_ind][coord].second++;
                } else{
                    // The number of white pixels at given coordinates is
                    // increased by one.
                    data_set[label_ind][coord].first++;
                }
            }
        }

    }
    
    CalculateProbabilities();
}

void digitClassifier::CalculateProbabilities(){
    for (int digit = kFirstDigit; digit <= kLastDigit; digit++){
        
        // Addition of first and second elements of the pair in any coordinate
        // with same digit will always be the same and reflect the number of
        // examples that digit has.
        int num_digit_repeat = data_set[digit][kDefualtPairVal].first +
                               data_set[digit][kDefualtPairVal].second;
        // Probability of each digit is calculated and added to a class var.
        double prob_digit = (kClassifyConst + num_digit_repeat) /
                            (num_train_exmp + 2 * kClassifyConst);
        class_prob.push_back(prob_digit);
        
        for (int row = 0; row < kImgSideLen; row++) {
            for (int col = 0; col < kImgSideLen; col++) {
                Coordinates coord = std::make_pair(col, row);
                int num_whit = data_set[digit][coord].first;
                int num_grblack = data_set[digit][coord].second;
                int num_repeatence = num_whit + num_grblack;
                
                // Formula to calculate
                // the probability of each coordinate in each digit for white
                // and black/gray pixels are calculated and stored in a class
                // variable.
                double prob_white = (num_whit + kClassifyConst) /
                                    (num_repeatence+ 2 * kClassifyConst);
                double prob_grblack = (num_grblack + kClassifyConst) /
                                      (num_repeatence+ 2 * kClassifyConst);
                
                prob_set[digit][coord] = std::make_pair(prob_white,
                                                        prob_grblack);
            }
        }
    }
}

int digitClassifier::GetMostLikelyDigit(std::map<Coordinates,int> &image_set){
    double highest_prob = GetDigitProbability(kFirstDigit, image_set);
    int digit_highest_prob = kFirstDigit;
    for (int digit = kFirstDigit + 1; digit <= kLastDigit; digit++) {
        // The probability of each digit is determined and then the highest one
        // is stored using a variable outside the loop.
        double digit_prob = GetDigitProbability(digit ,image_set);
        
        if (digit_prob > highest_prob) {
            highest_prob = digit_prob;
            digit_highest_prob = digit;
        }
    }
    
    // Digit with the highest probability is returned.
    return digit_highest_prob;
}

double digitClassifier::GetDigitProbability(const int &digit,
                                            std::map<Coordinates,int> &image_set
                                            ){
    
    double digit_prob = log(class_prob[digit]);
    
    for (int row = 0; row < kImgSideLen; row++) {
        for (int col = 0; col < kImgSideLen; col++) {
            // log of probability of each coordinate is added together.
            Coordinates coord = std::make_pair(col, row);
            digit_prob += GetPixelProbability(digit, image_set[coord], coord);
        }
    }
    
    return digit_prob;
}

double digitClassifier::GetPixelProbability(const int &digit, const int &color,
                                            const Coordinates &coord){
    if(color == kWhitePixVal){
        // If the color of the pixel is white then the corresponding value is
        // the first value in the pair with corresponding digit and coordinate.
        return log(prob_set[digit][coord].first);
    }
    
    // Otherwise the color of the pixel is black or gray, so the corresponding
    // value is the second in the pair with corresponding digit and coordinate.
    return log(prob_set[digit][coord].second);
}

bool digitClassifier::WriteModelToFile(const std::string &file_path){
    //If the data_set is empty or there're 0 examples then a false is returned.
    if (data_set.empty() || num_train_exmp == 0){
        return false;
    }
    
    std::ofstream output_file;
    output_file.open(file_path);
    
    // First line in the file is number of training examples.
    output_file << num_train_exmp;
    output_file << "\n";
    
    for (int digit = kFirstDigit; digit <= kLastDigit; digit++) {
        // Each line after the first represents each coordinate's corresponding
        // values in the given digit.
        output_file << GetDigitString(digit) + "\n";
        
    }
    
    output_file.close();
    return true;
}

std::string digitClassifier::GetDigitString(const int &digit){
    std::string digit_string;
    // Each coordinates pair values have a blank space between them and each
    // pair has a comma between them.
    for (int row = 0; row < kImgSideLen; row++) {
        for (int col = 0; col < kImgSideLen; col++) {
            Coordinates coord = std::make_pair(col, row);
            int num_whit = data_set[digit][coord].first;
            int num_grblack = data_set[digit][coord].second;
            digit_string += std::to_string(num_whit) +
                            kPixValSeparator +
                            std::to_string(num_grblack);
            
            digit_string += kCoorValSeperator;
        }
    }
    
    // Last element in the string is removed because it's an extra.
    digit_string.pop_back();
    return digit_string;
}

bool digitClassifier::ImportModelFromFile(const std::string &file_path) {
    // data_Set and prob_set are reset first.
    InitializeProbabilitySet();
    InitializeDataSet();
    
    std::ifstream input_file(file_path);
    std::string line;
    // Same formatting as file writting is used for reading, and split method
    // I implemented is used to achieve that.
    std::getline(input_file, line);
    num_train_exmp = std::stoi(line);
    for (int digit = kFirstDigit; digit <= kLastDigit; digit++) {
        std::getline(input_file, line);
        std::vector<std::string> pairs = SplitString(line, kCoorValSeperator);
        
        for (int row = 0; row < kImgSideLen; row++) {
            for (int col = 0; col < kImgSideLen; col++) {
                std::string pixel_data = pairs[col + kImgSideLen * row];
                std::vector<std::string> bwg_freq =
                SplitString(pixel_data,kPixValSeparator);
                int num_white = std::stoi(bwg_freq[0]);
                int num_grblack = std::stoi(bwg_freq[1]);
                Coordinates coord = std::make_pair(col,row);
                
                for (int i = 0; i < num_white; i++){
                    data_set[digit][coord].first++;
                }
                
                for (int i = 0; i < num_grblack; i++){
                    data_set[digit][coord].second++;
                }
            }
        }
    }
    
    // After the new data_set is implemented, probabilities are re-calculated.
    CalculateProbabilities();
    return true;
}

ConfusionMatrix digitClassifier::ClassifyImages(const std::string &file_path,
                                                const std::string &label_path) {
    ConfusionMatrix confusion_matrix = CreateConfusionMatrix();
    std::ifstream train_file(file_path);
    std::string line;
    
    std::ifstream label_file(label_path);
    std::string label;
    
    std::vector<int> digits;
    // These two values are used to calculate the number of correct guesses.
    int correct_digits = 0;
    int num_test_img = 0;
    
    while (true) {
        // Same logic used in creating a model is used here.
        if (!std::getline(label_file,label)) {
            break;
        }
        
        num_test_img++;
        int label_ind = stoi(label);
        std::map<Coordinates,int> img;
        for (int row = 0; row < kImgSideLen; row++) {
            std::getline(train_file,line);
            for (int col = 0; col < kImgSideLen; col++) {
                Coordinates coord = std::make_pair(col, row);
                if (line.at(col) == kGrayPixelChar ||
                   line.at(col) == kBlackPixelChar){
                    img[coord] = kGrayBlackPixVal;
                } else {
                    img[coord] = kWhitePixVal;
                }
            }
        }
        // Most likely digit is found is used to add values to confusion matrix
        // and to calculate correctness percentage. 
        int most_likely_digit = GetMostLikelyDigit(img);
        confusion_matrix[label_ind][most_likely_digit]++;
        
        if (most_likely_digit == label_ind) {
            correct_digits++;
        }
    }
    
    // Confusion matrix is updated to reflect the confusion in the code.
    for (int i = 0; i < confusion_matrix.size(); i++) {
        int num_elements = 0;
        for (int j = 0; j < confusion_matrix[i].size(); j++) {
            num_elements += confusion_matrix[i][j];
        }
        
        for (int j = 0; j < confusion_matrix[i].size(); j++) {
            confusion_matrix[i][j] /= num_elements;
        }
    }
    
    // A percentage of how correct the algorithm is printed to inform the user.
    double percentage = 100 * correct_digits/num_test_img;
    std::cout << percentage <<"% accurate" <<std::endl;
    return confusion_matrix;
}

std::vector<std::string> digitClassifier::SplitString(const std::string &string,
                                                      const char &split_point) {
    // workaround for splitting a string
    std::vector<std::string> strings;
    std::stringstream ss(string);
    std::string part;
    while (getline(ss, part, split_point)) {
        strings.push_back(part);
    }
    
    return strings;
}

ConfusionMatrix digitClassifier::CreateConfusionMatrix(){
    ConfusionMatrix matrix;
    for (int row = kFirstDigit; row <= kLastDigit; row++) {
        std::vector<double> values;
        for (int col = kFirstDigit; col <= kLastDigit; col++) {
            values.push_back(0);
        }
        
        matrix.push_back(values);
    }
    
    return matrix;
}
