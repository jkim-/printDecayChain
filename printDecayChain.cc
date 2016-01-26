#include <iostream>
#include <vector>
#include <queue>
#include <deque>
#include <algorithm>
#include <string>
// ROOT headers
#include <TDirectory.h>
#include <TFile.h>
#include <TTree.h>
#include <TBranch.h>

using namespace std;

void printFront(queue<string> &q, string &curIdx);
void printFrontLund(queue<string> &q, string &index, int mcLund[]);
void printQueue(queue<string> &q);
int getNumBars(queue<string> &q);

int main() {
  // Read in file
  TFile *f = new TFile("jake_test.root");
  TTree *t = (TTree*) f->Get("ntp1");

  // Get MC block variables for each event
  int mcLen;
  int mcLund[500], mothIdx[500], 
      dauLen[500], dauIdx[500]; 
  t->SetBranchAddress("mcLen", &mcLen);
  t->SetBranchAddress("mcLund", &mcLund);
  t->SetBranchAddress("mothIdx", &mothIdx);
  t->SetBranchAddress("dauLen", &dauLen);
  t->SetBranchAddress("dauIdx", &dauIdx);

  for (int ev = 0; ev < 1/*t->GetEntries()*/; ev++) {
    t->GetEntry(ev);
    queue<string> q; // q holds indices of particles
    for (int np = 0; np < mcLen; np++){
      cout << "Loop # " << np << ": mcLund is " << mcLund[np] << endl;
      //cout << "Loop # " << np << ": mothIdx is " << mothIdx[np] << endl;
      if (mcLund[np] == 70553) { // Find an upsilon for root of tree
        q.push(to_string(np));
        q.push("n");
        int numDau = 0; // set numDau = 0 whenever you push("n");
        while (q.size() > 0) {
          // Get the first particle in queue and print it
          string curIdx;
          //printFront(q, curIdx);
          printFrontLund(q, curIdx, mcLund);
          // Add daughters to a vector of daughters
          // and add each to the queue 
          if (curIdx == " ") {
            continue;
          }
          int curIdxInt = stoi(curIdx);
          int curNumDau = dauLen[curIdxInt];
          for (int nd = 0; nd < curNumDau; nd++) {
            q.push(to_string(dauIdx[curIdxInt] + nd));
          }
          if (curNumDau == 0) q.push(" ");
          numDau += curNumDau;
          int qSize = (int) q.size();
          int nBars = getNumBars(q);
          if (numDau == (qSize - nBars)) {
            q.push("n");
            numDau = 0;
          }
          else if (q.size() > 0) q.push("|");
        }
        break;
      }
    }
  }
  return 0;
}

// Print the first element of queue
// If next in line is not a number, print it also
void printFront(queue<string> &q, string &index) {
  index = q.front();
  cout << index + " ";
  q.pop();
  if (q.size() > 0) {
    string next = q.front();
    if (next == "n") {cout << endl; q.pop();}
    else if (next == "|") {cout << "| "; q.pop();}
  }
}

void printFrontLund(queue<string> &q, string &index, int mcLund[]) {
  index = q.front();
  if (index != " ") cout << mcLund[stoi(index)] << " ";
  else cout << index + " ";
  q.pop();
  if (q.size() > 0) {
    string next = q.front();
    if (next == "n") {cout << endl; q.pop();}
    else if (next == "|") {cout << "| "; q.pop();}
  }
}

void printQueue(queue<string> &q) {
  queue<string> qCopy = q;
  cout << "q: ";
  int qSize = (int) q.size();
  for (int i = 0; i < qSize; i++) {
    cout << qCopy.front() + " ";
    qCopy.pop();
  }
  cout << endl;
}

int getNumBars(queue<string> &q) {
  queue<string> qCopy = q;
  int qSize = (int) q.size();
  int nBars = 0;
  for (int i = 0; i < qSize; i++) {
    string front = qCopy.front();
    if (front == "|" || front == "n" || front == " ") nBars++;
    qCopy.pop();
  }
  return nBars;
}
