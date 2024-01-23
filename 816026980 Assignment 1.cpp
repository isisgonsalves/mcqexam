#include <iostream>
#include <iomanip>
#include <fstream>
#include <cmath>
#include <ctime>
#include <cstdlib>
#include <conio.h>

using namespace std;

#define NUM_CHOICES 4

struct Responses {
	string text[4];
	string answer;
	bool shuffle;
};

struct Question {
	string ID;
	string text;
	Responses responses;
	int points;
};

struct Summary {
	string questionID;
	bool correct;
	bool skipped;
	string answer;
	string givenAnswer;
	int points;
};


int readQuestions (Question questionBank[]) {
	
	// This function reads all the questions from questions.txt into 
	// the Question Bank. It returns the amount of questions in the 
	// Question Bank.

	int numQuestions=0;
	int c=0;
	int i;
	
	ifstream input;
	
	string line;
	string text;
	string choices;
	string answer;
	bool shuf;
	int points;
	
	
	input.open("Questions.txt");
	if (!input.is_open()) {
		cout << "\nERROR: File cannot be found...Try again\n";
		return 0;
	}
	else 
		
		cout<<"File was Read Successfully... "<<endl<<endl; 
	
	getline (input, line);
	
	
	while (line != "END"){

	questionBank[c].ID=line;
	
	getline (input, text);
	questionBank[c].text=text;
	
	for (i=0;i<4;i++){
	
		getline (input, choices);
		questionBank[c].responses.text[i]=choices;
		
	}
	
	getline (input, answer);
	questionBank[c].responses.answer=answer;

	input>>shuf;
	questionBank[c].responses.shuffle=shuf;
	
	
	input>>points;
	questionBank[c].points=points;
	
	
	numQuestions++;
	c++;
	
	getline (input, line);
	getline (input, line);
	getline (input, line);

	}

	return numQuestions;
}


void printList(Question questionBank[],  int numQuestions){
	
	// This function displays the contents of a question. It can
	// be used for debugging purposes to ensure that the data is
	// being read correctly from the file.

/*	int i;
	int c;
	
	for (i=0;i<numQuestions;i++){
		cout<<"QUESTION "<<i+1<<endl;
		cout<<questionBank[i].ID<<endl<<endl;
		cout<<questionBank[i].text<<endl<<endl;
		
		for (c=0;c<NUM_CHOICES;c++){
			cout<<questionBank[i].responses.text[c]<<endl<<endl;
		}
		cout<<"Answer: "<<questionBank[i].responses.answer<<endl<<endl;
		cout<<"Shuffle: "<<questionBank[i].responses.shuffle<<endl<<endl;
		cout<<"Points: "<<questionBank[i].points<<endl<<endl<<endl;
	}
*/

}


bool containsQuestion( Question exam[], int numExamQuestions, string examQ){
	
	// This function returns true if the exam already 
    // contains the given question, and false otherwise.
	
	int i;
	for (i=0;i<numExamQuestions;i++){
		if (exam[i].ID == examQ){
			return true;
		}
	}
	return false;
}


bool containsResponse (Question exam[], int num, string response, int shuffleNum) {
	
	// This function returns true if the set of responses already 
    // contains the given response, and false otherwise.
	
	int i;
	for (i=0;i<NUM_CHOICES;i++){
		if (exam[i].responses.text[shuffleNum] == response){
			return true;
		}
	}
	
	return false;
}


void shuffleResponses (Question exam[],int numExamQuestions) {
	
	// This function accepts the question passed as a parameter, 
	// shuffles the responses if the responses should be shuffled, 
	// and returns the modified question.

	int shuffleNum;
	int i;
	string response;
	int c;
	string tempSwap[4];
	
	for (i=1;i<=numExamQuestions;i++){
		
		for (c=0;c<NUM_CHOICES;c++){
	
			if (exam[i].responses.shuffle == true){
				shuffleNum = rand () % NUM_CHOICES;
				
				tempSwap[c] = exam[i].responses.text[shuffleNum];
				exam[i].responses.text[shuffleNum] = exam[i].responses.text[c];
				exam[i].responses.text[c] = tempSwap[c];
				
			}
					
				if (containsResponse(exam,i,response,shuffleNum)== false ){
						exam[i].responses.text[c]= exam[i].responses.text[shuffleNum];
				}
				
		}
		
	}
}


void prepareExam (Question questionBank[], int numQuestions, Question exam[], int numExamQuestions){

	// This function creates the exam containing numExamQuestions, 
	// where the questions are taken randomly from the Question Bank, 
	// which contains numQuestions.Duplicate questions are not
	// allowed in the exam.
	
	int i=1;
	int c;
	int randQuestion;
	string examQ;
	Question ShuffleResponses;
	Question shuffle;
	
	srand(time(NULL));
	
	
	
	while (i<=numExamQuestions){
		randQuestion= rand() % numQuestions;
		examQ= questionBank[randQuestion].ID;
		if (containsQuestion(exam,i,examQ)== false ){
			exam[i].ID = examQ;
			exam[i].text = questionBank[randQuestion].text;
			
			for (c=0;c<NUM_CHOICES;c++){
			exam[i].responses.text[c]=questionBank[randQuestion].responses.text[c];
			}
			
			exam[i].responses.answer = questionBank[randQuestion].responses.answer;
			exam[i].responses.shuffle = questionBank[randQuestion].responses.shuffle;
			exam[i].points = questionBank[randQuestion].points;

			i++;
		}
	}

}


void generateExam (Question exam[], int numExamQuestions, Summary summary[]) {
	// After the exam is prepared, this function presents the 
	// questions on the exam to the user, one by one. It also
	// records how the user responded to each question, for 
	// the summary at the end.
	int i=1;
	int c=0;
	int givenAnswer;
	int j=1;
	int skipped=0;
	int correct=0;
	int wrong =0;
	
	
		while(i<=numExamQuestions){
			
				cout<<"\n\n\nQuestion "<<i<< ": "<<exam[i].ID<<endl<<endl;
				cout<<exam[i].text<<endl<<endl;
			
				shuffleResponses( exam,numExamQuestions);
				
				for (c=0;c<NUM_CHOICES;c++){	
					cout<<c+1<<". "<<exam[i].responses.text[c]<<endl<<endl;
			
				}
				
				cout<<"Response= ";
				cin>> givenAnswer;
				
			
				summary[i].questionID = exam[i].ID;
				summary[i].answer = exam[i].responses.answer;
				
				

					if (givenAnswer == 5){
								cout<<"This Question was Skipped."<<endl;
								summary[i].givenAnswer = "skipped";
								summary[i].skipped= true;
					}
					else
						if(exam[i].responses.text[givenAnswer-1] != exam[i].responses.answer){
									cout<<"I'm sorry but this answer is Incorrect :( "<<endl;
									summary[i].givenAnswer = exam[i].responses.text[givenAnswer-1];
									summary[i].correct= false;
								}
							else
								if(exam[i].responses.text[givenAnswer-1] == exam[i].responses.answer){
									cout<<"This Answer is Correct, Keep it up. "<<endl;
									summary[i].points = exam[i].points;
									summary[i].givenAnswer = exam[i].responses.text[givenAnswer-1];
									summary[i].correct= true;
								}	
								
									
		i++;

	}
	
}


void displaySummary (Summary summary[], int numExamQuestions) {

	// This function displays a summary of the user's performance on 
	// each question of the exam, after the exam is completed.
	int i=1;
	int c;
	int totalP=0;
	int correct=0;
	int wrong=0;
	int skipped=0;
	
	//tabular question summary 
	
	//I had some problems with this table as some Answers were too long
	//Any feedback on how I couldve gotten it to be better and more sufficient would be appreciated.
	//Thank youuuuuu!!!
	
		cout<<"\n\n   __________________________________________________________________________________________________________\n";	
		cout<<"    NO. Question ID"<<setw(28)<<"Answer" <<setw(30)<<"Given Answer"<<setw(30)<<"Points Awarded \n";
	
		while(i<=numExamQuestions){
			cout<<"   __________________________________________________________________________________________________________\n";
			
			cout<<"    "<<i<<".  "<<summary[i].questionID<<setw(28)<<summary[i].givenAnswer<<setw(29)<<summary[i].answer<<setw(20)<<summary[i].points<<endl<<endl<<endl;
			
			i++;
		}
		
		cout<<"   __________________________________________________________________________________________________________\n\n\n";
		cout<<"\nSUMMARY OF EXAM QUESTIONS:  \n\n";
		
			for(i=1;i<=numExamQuestions;i++){
				
				totalP=totalP + summary[i].points;
				
				if(summary[i].correct == true){
					correct=correct + 1; 
					cout<<"Question NO:     "<< i<<endl;
					cout<<"Question:        "<< summary[i].questionID <<endl;
					cout<<"Correct:         YES"<<endl;
					cout<<"Marks Awarded:   "<< summary[i].points<<endl;
					cout<<"Correct Answer:  "<< summary[i].answer<<endl;
					cout<<"Given Answer:    "<< summary[i].givenAnswer<<endl<<endl<<endl;
				}
				else
					if(summary[i].skipped == true){
						skipped=skipped + 1; 
						cout<<"Question NO:     "<< i<<endl;
						cout<<"Question:        "<< summary[i].questionID <<endl;
						cout<<"Correct:         Skipped"<<endl;
						cout<<"Marks Awarded:   "<< summary[i].points<<endl<<endl<<endl;
					}
					else 
						if(summary[i].correct == false){
						wrong=wrong + 1; 
						cout<<"Question NO:     "<< i<<endl;
						cout<<"Question:        "<< summary[i].questionID <<endl;
						cout<<"Correct:         NO"<<endl;
						cout<<"Marks Awarded:   "<< summary[i].points<<endl;
						cout<<"Correct Answer:  "<< summary[i].answer<<endl;
						cout<<"Given Answer:    "<< summary[i].givenAnswer<<endl<<endl<<endl;
						} 
			}
		
		
		cout<<"\n\n\n\t\t_______________________________S U M M A R Y_______________________________\n";
		
		cout<<"\t\tNum of Questions      Correct      Incorrect       Skipped     Total Points\n";
		cout<<"\t\t___________________________________________________________________________\n";
		cout<<"\t\t      "<<numExamQuestions<<"\t\t\t"<<correct<<"\t\t"<<wrong<<"\t      "<<skipped<<"\t\t  "<<totalP<<endl;
		cout<<"\t\t___________________________________________________________________________\n\n\n\n";
		
		cout<<"\t\t***********************Thank You & Stay Covid Safe :)**********************\n\n\n\n";

		
	}
	

int main(){
	
	// The main function calls readQuestions, prepareExam, 
	// generateExam, and displaySummary to achieve the functionality
	// of Assignment 1.
	
	Question questionBank[100];
	int numQuestions;
	
	Question exam [50];
	int numExamQuestions;
	
	Summary summary [50];
	
	numQuestions = readQuestions(questionBank);
	
	cout<<"Number of Questions in the Question Bank: "<<numQuestions<<endl;
	
	printList(questionBank,numQuestions);
	
	cout<<"\nPlease enter the amount of questions for the test: ";
	cin>> numExamQuestions;
	
	prepareExam( questionBank, numQuestions, exam, numExamQuestions);
	
	cout<<"\n\nExam has been prepared... \n\n";
	
	cout<<"\n\n\t\t\tYou are about to take a Multiple Choice Exam.\n";
	cout<<"\t\tChoose between options 1, 2, 3, 4 or 5 to skip the question.\n";
	cout<<"\t\t\t     Check over before pressing ENTER.\n";
	cout<<"\t\t\t\t\t  Goodluck!\n\n\n";
	
	generateExam(exam, numExamQuestions, summary);
	
	cout<<"\n\t\t SUMMARY OF EXAM QUESTIONS \n\n";

	displaySummary(summary, numExamQuestions);
	
	
	return 0;
}
