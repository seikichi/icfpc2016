#include "input.h"
#include "score.h"
#include "geometry.h"
#include <iostream>
#include <tuple>
#include <string>
using namespace std;

vector<tuple<int, int, int>> angles = {
  make_tuple(1, 0, 1),  // 0.000000
  make_tuple(9940, 141, 9941),  // 0.812692
  make_tuple(1443, 76, 1445),  // 3.01487
  make_tuple(8265, 728, 8297),  // 5.03375
  make_tuple(84, 13, 85),  // 8.79741
  make_tuple(60, 11, 61),  // 10.3889
  make_tuple(40, 9, 41),  // 12.6804
  make_tuple(63, 16, 65),  // 14.25
  make_tuple(24, 7, 25),  // 16.2602
  make_tuple(35, 12, 37),  // 18.9246
  make_tuple(12, 5, 13),  // 22.6199
  make_tuple(77, 36, 85),  // 25.0576
  make_tuple(15, 8, 17),  // 28.0725
  make_tuple(56, 33, 65),  // 30.5102
  make_tuple(91, 60, 109),  // 33.3985
  make_tuple(4, 3, 5),  // 36.8699
  make_tuple(55, 48, 73),  // 41.1121
  make_tuple(21, 20, 29),  // 43.6028
  make_tuple(20, 21, 29),  // 46.3972
  make_tuple(65, 72, 97),  // 47.925
  make_tuple(48, 55, 73),  // 48.8879
  make_tuple(3, 4, 5),  // 53.1301
  make_tuple(60, 91, 109),  // 56.6015
  make_tuple(28, 45, 53),  // 58.1092
  make_tuple(8, 15, 17),  // 61.9275
  make_tuple(39, 80, 89),  // 64.0108
  make_tuple(5, 12, 13),  // 67.3801
  make_tuple(12, 35, 37),  // 71.0754
  make_tuple(7, 24, 25),  // 73.7398
  make_tuple(16, 63, 65),  // 75.75
  make_tuple(9, 40, 41),  // 77.3196
  make_tuple(820, 6699, 6749),  // 83.0214
  make_tuple(516, 7387, 7405),  // 86.0042
  make_tuple(141, 9940, 9941),  // 89.1873
  make_tuple(-1, 0, 1),  // 180
};

int main(int argc, char** argv) {
  if (argc != 5) {
    cerr << "Usage: tweak ANGLE_INDEX DIRECTION OFFSET_X OFFSET_Y\n";
    cerr << "Read solution from STDDIN and write tweaked solution to STDOUT.\n";
    cerr << "DIRECTION is 'FORWARD' or 'REVERSE'.\n";
    cerr << "OFFSET_X and OFFSET_Y are 32-bit integers.\n";
    exit(1);
  }
  int angle_index = stoi(argv[1]);
  auto& angle = angles.at(angle_index);
  string direction = argv[2];
  mpq_class offset_x = mpq_class(argv[3]);
  mpq_class offset_y = mpq_class(argv[4]);
  Point offset(offset_x, offset_y);  

  Output output;
  output.ReadOutput(stdin);

  Output tweaked;
  tweaked.source_points = output.source_points;
  tweaked.facet_indecies = output.facet_indecies;

  if (direction == "REVERSE") {
    tweaked.dest_points = TranslatePolygon(RotatePolygonReverse(output.dest_points, angle), offset);
  } else {
    tweaked.dest_points = TranslatePolygon(RotatePolygon(output.dest_points, angle), offset);
  }
  tweaked.WriteOutput(stdout);
}
