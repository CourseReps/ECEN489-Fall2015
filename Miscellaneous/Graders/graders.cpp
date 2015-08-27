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
  ecen489names.push_back("Nathaniel Glaser");
  ecen489names.push_back("Karl Griesser");
  ecen489names.push_back("Kyle Malone");
  ecen489names.push_back("Nneamaka Njoku");
  ecen489names.push_back("Steven Porter");
  ecen489names.push_back("Anthony Ramirez");
  ecen489names.push_back("Cooper Rehn");
  ecen489names.push_back("April Walker");
  ecen489names.push_back("Kaimen Walters");
  ecen489names.push_back("Jinpeng You");
  ecen489names.push_back("Xiangdong Yu");
  // Graduate Students 689
  ecen489names.push_back("Abhay Anand");
  ecen489names.push_back("Pranay Eedara");
  ecen489names.push_back("Rohit Gangrade");
  ecen489names.push_back("Kartic Kaveripuram Ramasamy");
  ecen489names.push_back("Udaya Bhaskar Kothapalli");
  ecen489names.push_back("Hai Li");
  ecen489names.push_back("Shashank Naphade");
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
    cout << *giterator << " will grade work by " << name << endl;
    giterator++;
  }
  cout << endl;

  cout << endl << "==== Students and Graders ===="  << endl << endl;
  giterator = ecen489graders.begin();
  for (string name : ecen489names) {
    cout << name << " will be graded by " << *giterator << endl;
    giterator++;
  }
  cout << endl;
}

