#include <toaru/toaru.h>

using namespace toaru;

int main(int argc, char *argv[]) {
  toaru::init(argc, argv);

  Scene scene;

  Viewer viewer(scene);
  viewer.start();

  toaru::exit();
}