#include <iostream>
#include <string>
#include <iomanip>
#include <vector>
#include <algorithm>
#include <random>

using namespace std;

class Question {
public:
    virtual void display() const = 0;
    virtual bool checkAnswer(char userAnswer) const = 0;
    virtual string getExplanation() const = 0;
    virtual void fiftyFifty() = 0; // Added virtual function for 50/50 lifeline
};

class MCQ : public Question {
private:
    string question;
    string options[4];
    char correctAnswer;
    string explanation;

public:
    MCQ(string question, string optionA, string optionB, string optionC, string optionD, char correctAnswer, string explanation)
        : question(question), correctAnswer(correctAnswer), explanation(explanation) {
        options[0] = optionA;
        options[1] = optionB;
        options[2] = optionC;
        options[3] = optionD;
    }

    void display() const override {
        cout << "Question: " << question << endl;
        cout << "Options:\n";
        for (int i = 0; i < 4; ++i) {
            char option = 'A' + i;
            cout << option << ") " << options[i] << endl;
        }
    }

    bool checkAnswer(char userAnswer) const override {
        return toupper(userAnswer) == correctAnswer;
    }

    string getExplanation() const override {
        return explanation;
    }

    void fiftyFifty() override {
        vector<int> indices = {0, 1, 2, 3};
        indices.erase(remove(indices.begin(), indices.end(), correctAnswer - 'A'), indices.end());
        shuffle(indices.begin(), indices.end(), default_random_engine{});

        // Hide two incorrect options
        for (int i = 0; i < 2; ++i) {
            options[indices[i]] = " ";
        }
    }
};

int main() {
    vector<Question*> questions;
    questions.push_back(new MCQ("Who holds the record for the most Formula 1 World Championships?",
                                "A) Lewis Hamilton", "B) Michael Schumacher", "C) Juan Manuel Fangio", "D) Alain Prost", 'B',
                                "Michael Schumacher and Lewis Hamilton both share the record with 7 championships."));
    questions.push_back(new MCQ("Which team did Fernando Alonso win his two World Championships with?",
                                "A) Ferrari", "B) McLaren", "C) Renault", "D) Red Bull", 'C',
                                "Fernando Alonso won both of his World Championships with Renault in 2005 and 2006."));
    questions.push_back(new MCQ("Who won the 2020 Formula 1 World Championship?",
                                "A) Lewis Hamilton", "B) Sebastian Vettel", "C) Max Verstappen", "D) Valtteri Bottas", 'A',
                                "Lewis Hamilton won the 2020 Formula 1 World Championship, his 7th title."));
    questions.push_back(new MCQ("In which year did the first Formula 1 race take place?",
                                "A) 1950", "B) 1947", "C) 1962", "D) 1965", 'A',
                                "The first Formula 1 World Championship race took place in 1950 at Silverstone, UK."));
    questions.push_back(new MCQ("Which F1 driver is known as the 'Flying Finn'?",
                                "A) Mika Häkkinen", "B) Kimi Räikkönen", "C) Valtteri Bottas", "D) Heikki Kovalainen", 'A',
                                "Mika Häkkinen, a two-time World Champion, is known as the 'Flying Finn' due to his flying driving style."));

    cout << "Welcome to F1 Quiz Competition!\n\n";

    string player1, player2;
    cout << "Enter Player 1's Name: ";
    cin >> player1;
    cout << "Enter Player 2's Name: ";
    cin >> player2;

    int score1 = 0, score2 = 0;
    try {
        for (int i = 0; i < questions.size(); ++i) {
            if (i % 2 == 0) {
                cout << "\n" << player1 << ", ";
            } else {
                cout << "\n" << player2 << ", ";
            }

            questions[i]->display();

            char userAnswer;
            cout << "\nEnter your choice (A/B/C/D) or type 'L' for Lifeline: ";
            cin >> userAnswer;

            if (!isalpha(userAnswer)) {
                throw runtime_error("Invalid input! Please enter A, B, C, D, or L.");
            }

            if (toupper(userAnswer) == 'L') {
                questions[i]->fiftyFifty();
                cout << "\nYou've used the 50/50 lifeline! Here are the remaining options:\n";
                questions[i]->display();
                cout << "\nEnter your choice (A/B/C/D): ";
                cin >> userAnswer;
            }

            if (questions[i]->checkAnswer(userAnswer)) {
                cout << "\nCorrect Answer! You earn 500 points.\n";
                if (i % 2 == 0) {
                    score1 += 500;
                } else {
                    score2 += 500;
                }
            } else {
                cout << "\nWrong Answer! The correct answer is " << questions[i]->getExplanation() << ".\n";
            }
        }

        cout << "\nQuiz completed!\n";
        cout << player1 << " scored: " << score1 << " points\n";
        cout << player2 << " scored: " << score2 << " points\n";

        if (score1 > score2) {
            cout << "\nCongratulations, " << player1 << " wins!\n";
        } else if (score1 < score2) {
            cout << "\nCongratulations, " << player2 << " wins!\n";
        } else {
            cout << "\nIt's a tie!\n";
        }
    } catch (const exception& e) {
        cerr << "Exception: " << e.what() << endl;
    }

    for (auto& q : questions) {
        delete q;
    }
    questions.clear();

    return 0;
}

