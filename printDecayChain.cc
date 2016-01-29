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

void printQueue(queue<string> &q); // Helper function to cout queues

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

  for (int ev = 0; ev < 3/*t->GetEntries()*/; ev++) {
    t->GetEntry(ev);
    for (int np = 0; np < mcLen; np++){
      cout << "Event " << ev << ", Loop # " << np << ": mcLund is " << mcLund[np] << endl;
      if (mcLund[np] != 70553) continue;
      else { // Found an upsilon for root of tree
        queue<int> q; // q holds indices of particles
        vector<bool> v(mcLen, false); // has the particle been seen before
        vector<int> d(mcLen, 0); // depth of the particle

        // do BFS to explore the decay chain
        v[np] = true;
        d[np] = 0;
        int prevIdx = np;
        int prevDepth = 0;
        q.push(np);
        while (!q.empty()) {
          int curIdx = q.front();
          int curDepth = d[curIdx];
          q.pop();
          int curNumDau = dauLen[curIdx];
          for (int nd = 0; nd < curNumDau; nd++) {
            int curDauIdx = dauIdx[curIdx] + nd;
            if (!v[curDauIdx]) {
              d[curDauIdx] = d[curIdx] + 1;
              q.push(curDauIdx);
            }
          }
          // Set current node as visited and print it out
          v[curIdx] = true;
          if (curDepth != prevDepth) cout << endl; // when depth changes, newline
          if (mothIdx[curIdx] != mothIdx[prevIdx]) { // group siblings by mothIdx
            if (curDepth == prevDepth) cout << "| ";
            cout << "[" << mcLund[mothIdx[curIdx]] << "] ";
          }
          cout << mcLund[curIdx] << " ";
          prevIdx = curIdx;
          prevDepth = d[curIdx];
        }
        cout << endl;
      }
    }
  }
  f->Close();
  delete f;
  return 0;
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
