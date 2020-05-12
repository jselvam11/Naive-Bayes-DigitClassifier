//
//  digitclassifiertest.cpp
//  digitclassifier
//
//  Created by Jeeva Selvam on 3/30/19.
//  Copyright © 2019 Jeeva Selvam. All rights reserved.
//
#define CATCH_CONFIG_MAIN
#include "catch.hpp"
#include "digitclassifier.hpp"

TEST_CASE("Import Data Tests"){
    digitClassifier digit;
    digit.ImportData("/Users/jeeva/Naive-Bayes-DigitClassifier/"
                     "digitclassifier/digitdata/trainingimages",
                     "/Users/jeeva/Naive-Bayes-DigitClassifier/"
                     "digitclassifier/digitdata/traininglabels");
    
    SECTION("Number of Images Tests"){
        REQUIRE(digit.num_train_exmp == 5000);
        int num_elements = 0;
        for (int num = 0; num <= 9; num++) {
                    num_elements += digit.data_set[num]
                    [std::make_pair(0, 0)].first;
            num_elements += digit.data_set[num]
                    [std::make_pair(0, 0)].second;
            }
        REQUIRE(num_elements == 5000);
        
        //Since all images have 00 pixel as white.
        int num_white_pix_in00 = 0;
        for (int num = 0; num <= 9; num++) {
            num_white_pix_in00 += digit.data_set[num]
                                  [std::make_pair(0, 0)].first;
            
        }
        REQUIRE(num_white_pix_in00==5000);
    }
    SECTION("Probabilities Valid Tests"){
        for (int num = 0; num <= 9; num++) {
            for(int i=0;i<28;i++){
                for(int j=0;j<28;j++){
                    double prob_white = digit.prob_set[num]
                                        [std::make_pair(j, i)].first;
                    
                    double prob_blackgray = digit.prob_set[num]
                                            [std::make_pair(j, i)].second;
                    bool isValidWhite = prob_white <= 1 && prob_white >= 0;
                    bool isValidBlackGray = prob_blackgray <= 1 &&
                                            prob_blackgray >= 0;
                    bool isValid =isValidWhite && isValidBlackGray;
                    REQUIRE(isValid);
                }
            }
        }
    }
}

TEST_CASE("File Writing/ Reading Tests"){
    digitClassifier digit;
    digit.ImportData("/Users/jeeva/Naive-Bayes-DigitClassifier/"
                     "digitclassifier/digitdata/trainingimages",
                     "/Users/jeeva/Naive-Bayes-DigitClassifier/"
                     "digitclassifier/digitdata/traininglabels");
    
    SECTION("Check If File Writing/Reading is consistent") {
        digit.WriteModelToFile("/Users/jeeva/Naive-Bayes-DigitClassifier/"
                               "digitclassifier/datamodel.txt");
        int num_elem = digit.num_train_exmp;
        std::map<int,std::map<Coordinates,std::pair<int, int>>> data =
        digit.data_set;
        
        std::map<int,std::map<Coordinates,std::pair<double, double>>> prob =
        digit.prob_set;
        
        digitClassifier new_digit;
        new_digit.ImportModelFromFile("/Users/jeeva/Naive-Bayes-DigitClassifier/"
                                      "digitclassifier/datamodel.txt");
        REQUIRE(new_digit.num_train_exmp == num_elem);
        REQUIRE(new_digit.data_set == data);
        REQUIRE(new_digit.prob_set == prob);
    }
    
    SECTION("Check If reading/writing is successful"){
        
        REQUIRE(digit.WriteModelToFile("/Users/jeeva/Naive-Bayes-DigitClassifier/"
                                       "digitclassifier/datamodel.txt"));
        REQUIRE(digit.ImportModelFromFile("/Users/jeeva/Naive-Bayes-DigitClassifier/"
                                          "digitclassifier/datamodel.txt"));
        
    }
    
}

TEST_CASE("Image Classification Tests"){
    digitClassifier digit;
    digit.ImportData("/Users/jeeva/Naive-Bayes-DigitClassifier/"
                     "digitclassifier/digitdata/trainingimages",
                     "/Users/jeeva/Naive-Bayes-DigitClassifier/"
                     "digitclassifier/digitdata/traininglabels");
    
    SECTION("Check If Confusion matrix is valid"){
        ConfusionMatrix matrix = digit.ClassifyImages("/Users/jeeva/Naive-Bayes-DigitClassifier/"
                                                      "digitclassifier/digitdata/testimages",
                                                      "/Users/jeeva/Naive-Bayes-DigitClassifier/"
                                                      "digitclassifier/digitdata/testlabels");
        for (int i=0; i<=9; i++) {
            double prob = 0;
            for (int j=0; j<=9; j++) {
                prob += matrix[i][j];
            }
            int total_prob = std::round(prob);
            REQUIRE(total_prob == 1);
        }
    }
    
    SECTION("Check if Same Models Provide Same Matrix") {
        ConfusionMatrix matrix = digit.ClassifyImages("/Users/jeeva/Naive-Bayes-DigitClassifier/"
                                                      "digitclassifier/digitdata/testimages",
                                                      "/Users/jeeva/Naive-Bayes-DigitClassifier/"
                                                      "digitclassifier/digitdata/testlabels");
        digit.WriteModelToFile("/Users/jeeva/Naive-Bayes-DigitClassifier/"
                               "digitclassifier/datamodel.txt");
        digitClassifier importedDigit;
        importedDigit.ImportModelFromFile("/Users/jeeva/Naive-Bayes-DigitClassifier/"
                                          "digitclassifier/datamodel.txt");
        ConfusionMatrix matrix2 = importedDigit.ClassifyImages("/Users/jeeva/Naive-Bayes-DigitClassifier/"
                                                               "digitclassifier/digitdata/testimages",
                                                               "/Users/jeeva/Naive-Bayes-DigitClassifier/"
                                                               "digitclassifier/digitdata/testlabels");
        REQUIRE(matrix == matrix2);
    }
    
    SECTION("Check If Each Digit is at least %60 Correct") {
        ConfusionMatrix matrix = digit.ClassifyImages("/Users/jeeva/Naive-Bayes-DigitClassifier/"
                                                      "digitclassifier/digitdata/testimages",
                                                      "/Users/jeeva/Naive-Bayes-DigitClassifier/"
                                                      "digitclassifier/digitdata/testlabels");
        double minimum_correct = 0.6;
        for (int i=9; i<=9; i++) {
            REQUIRE(matrix[i][i] >= minimum_correct);
        }
    }
}
