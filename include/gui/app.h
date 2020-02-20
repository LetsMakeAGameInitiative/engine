#ifndef LMAGIE_GUI_APP_H
#define LMAGIE_GUI_APP_H

namespace lmagie::gui {
  class app {
  public:
    app() = default;
    ~app() = default;
    int run(int argc, char** argv);
  };
}

#endif