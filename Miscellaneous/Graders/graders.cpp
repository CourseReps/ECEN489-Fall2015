#include <iostream>
#include <algorithm>
#include <vector>
#include <cstdlib>
#include <ctime>
using namespace std;

// random generator
int ngrandom (int i) { return rand() % i; }

int main() {
  srand ( unsigned ( std::time(NULL) ) );

  // initialize vector with student names.
  vector<string> ecen489names;
  // Undergraduate Students 489
  ecen489names.push_back("Thomas Branyon");
  ecen489names.push_back("Keaton Brown");
  ecen489names.push_back("Anthony Ramirez");
  ecen489names.push_back("Cooper Rehn");
  ecen489names.push_back("Kaimen Walters");
  ecen489names.push_back("Steven Jinpeng You");
  ecen489names.push_back("Henry Xiangdong Yu");
  ecen489names.push_back("Blade Roybal");
  ecen489names.push_back("Seungwon Yoon");
  // Graduate Students 689
  ecen489names.push_back("Abhay Anand");
  ecen489names.push_back("Kartic Bhargav");
  ecen489names.push_back("Pranay Eedara");
  ecen489names.push_back("Udaya Bhaskar Kothapalli");
  ecen489names.push_back("Hai Li");
  ecen489names.push_back("Austin Taghavi");
  ecen489names.push_back("Travis Taghavi");
  ecen489names.push_back("Kan Zheng");

  // initialize vector with grader names.
  vector<string> ecen489graders;
  ecen489graders = ecen489names;

  // generate a derangement using the rejection method.
  bool derangement = false;
  vector<string>::iterator niterator;
  vector<string>::iterator giterator;

  while (derangement == false) {
    // permute grader names using built-in random generator.
    random_shuffle ( ecen489graders.begin(), ecen489graders.end(), ngrandom);
    // checking that permuation is derangement.
    derangement = true;
    niterator = ecen489names.begin();
    giterator = ecen489graders.begin();
    while ((niterator != ecen489names.end()) && (derangement == true)) {
      if (*niterator == *giterator) {
        derangement = false;
      }
      niterator++;
      giterator++;
    }
  }

  cout << endl << "==== Graders and Students ===="  << endl << endl;
  giterator = ecen489graders.begin();
  for (string name : ecen489names) {
    cout << "Grader: " << *giterator << ";    Student: " << name << endl;
    giterator++;
  }
  cout << endl;

  cout << endl << "==== Students and Graders ===="  << endl << endl;
  giterator = ecen489graders.begin();
  for (string name : ecen489names) {
    cout << "Student: " << name << ";    Grader: " << *giterator << endl;
    giterator++;
  }
  cout << endl;
}

