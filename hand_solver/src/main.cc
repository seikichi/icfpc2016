#include "in.h"
#include "out.h"
#include "fold.h"
#include "linenoise.h"

#include <iostream>
#include <sstream>
#include <string>
using namespace std;

int main() {
  linenoiseHistorySetMaxLen(10000);
  linenoiseHistoryLoad("hand_solver_history.txt");
  std::vector<Output> outputs;
  outputs.push_back(Output());
  outputs[0].Init();
  char *temp = nullptr;
  while ((temp = linenoise("> ")) != nullptr) {
    linenoiseHistoryAdd(temp);
    linenoiseHistorySave("hand_solver_history.txt");
    string command;
    string line = temp;
    linenoiseFree(temp);

    stringstream sin(line);
    if (!(sin >> command)) { continue; }
    if (command == "fold") {
      string x1, y1, x2, y2;
      if (!(sin >> x1 >> y1 >> x2 >> y2)) {
        fprintf(stderr, "Error: fold x1 y1 x2 y2 (x1 likes 123/456 without space)\n");
        goto next;
      }
      Line l = Line(Point(mpq_class(x1), mpq_class(y1)),
                    Point(mpq_class(x2), mpq_class(y2)));
      canonicalize(l[0]); canonicalize(l[1]);
      Output next = Fold(outputs.back(), l);
      outputs.push_back(next);
      outputs.back().WriteSVGSource("tmp_source.svg");
      outputs.back().WriteSVGDest("tmp_dest.svg");
      system("firefox tmp_dest.svg &");
    } else if (command == "move") {
      string x1, y1;
      if (!(sin >> x1 >> y1)) {
        fprintf(stderr, "Error: move x1 y1 (x1 likes 123/456 without space)\n");
        goto next;
      }
      Point vect = Point(mpq_class(x1), mpq_class(y1));
      canonicalize(vect);
      Output next = outputs.back();
      for (auto &p : next.dest_points) { p += vect; }
      outputs.push_back(next);
      outputs.back().WriteSVGSource("tmp_source.svg");
      outputs.back().WriteSVGDest("tmp_dest.svg");
      system("firefox tmp_dest.svg &");
    } else if (command == "undo") {
      if (outputs.size() == 1) {
        fprintf(stderr, "Error: Initial State!\n");
        goto next;
      } else {
        outputs.pop_back();
        outputs.back().WriteSVGSource("tmp_source.svg");
        outputs.back().WriteSVGDest("tmp_dest.svg");
        system("firefox tmp_dest.svg &");
      }
    } else if (command == "save") {
      string filename;
      if (!(sin >> filename)) {
        fprintf(stderr, "Error: save filename\n");
        goto next;
      }
      outputs.back().WriteOutput(filename.c_str());
      outputs.back().WriteSVGSource((filename + "_source.svg").c_str());
      outputs.back().WriteSVGDest((filename + "_dest.svg").c_str());
    } else if (command == "exit" || command == "quit") {
      break;
    } else {
      fprintf(stderr, "Error: command list = { fold move undo save exit quit  }\n");
    }
next:;
  }
}
