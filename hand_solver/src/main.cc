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
      Output next = Fold(outputs.back(), l);
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
      fprintf(stderr, "Error: command list = { fold undo save exit quit  }\n");
    }
next:;
  }

  // int count = 0;
  // for (auto& polygon : silhouette) {
  //   for (auto& point : polygon) {
  //     xg += point.real();
  //     yg += point.imag();
  //     count++;
  //   }
  // }
  // xg /= count;
  // yg /= count;
  //
  // mpq_class min_x = silhouette[0][0].real();
  // mpq_class max_x = silhouette[0][0].real();
  // mpq_class min_y = silhouette[0][0].imag();
  // mpq_class max_y = silhouette[0][0].imag();
  //
  // for (auto& polygon : silhouette) {
  //   for (auto& point : polygon) {
  //     if (point.real() < min_x) min_x = point.real();
  //     if (point.real() > max_x) max_x = point.real();
  //     if (point.imag() < min_y) min_y = point.imag();
  //     if (point.imag() > max_y) max_y = point.imag();
  //   }
  // }
  //
  // mpq_class width = max_x - min_x;
  // if (width < mpq_class(1, 2))
  //   width = mpq_class(1, 2);
  // if (width > 1)
  //   width = mpq_class(1, 1);
  // width.canonicalize();
  // mpq_class height = max_y - min_y;
  // if (height < mpq_class(1, 2))
  //   height = mpq_class(1, 2);
  // if (height > 1)
  //   height = mpq_class(1, 1);
  // height.canonicalize();
  //
  // vector<mpq_class> xs, ys;
  // if (width == mpq_class(1, 1))
  //   xs = {0, 1};
  // else
  //   xs = {0, width, 1};
  // if (height == mpq_class(1, 1))
  //   ys = {0, 1};
  // else
  //   ys = {0, height, 1};
  //
  // // source positions part
  // cout << xs.size() * ys.size() << "\n";
  // for (auto& y : ys) {
  //   for (auto& x : xs) {
  //     cout << PointToString(Point(x, y)) << "\n";
  //   }
  // }
  // // facets part
  // cout << (xs.size()-1)*(ys.size()-1) << "\n";
  // for (int i = 0; i < (int)ys.size()-1; ++i) {
  //   for (int j = 0; j < (int)xs.size()-1; ++j) {
  //     cout << "4 "
  //          << (i*xs.size() + j) << " "
  //          << (i*xs.size() + j+1) << " "
  //          << ((i+1)*xs.size() + j+1) << " "
  //          << ((i+1)*xs.size() + j) << "\n";
  //   }
  // }
  // // destination positions part
  // mpq_class offset_x = xg - width/2;
  // mpq_class offset_y = yg - height/2;
  // for (auto& y : ys) {
  //   mpq_class dest_y = (ys.size() == 3 && y == 1) ? (2*height-1) : y;
  //   for (auto& x : xs) {
  //     mpq_class dest_x = (xs.size() == 3 && x == 1) ? (2*width-1) : x;
  //     cout << PointToString(Point(dest_x + offset_x, dest_y + offset_y)) << "\n";
  //   }
  // }
}
