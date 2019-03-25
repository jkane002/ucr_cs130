#ifndef APPLICATION_H
#define APPLICATION_H

#include <vector>
using namespace std;

#include "gl_viewer.h"

class application : public gl_viewer
{
public:
    application();
    ~application();
    void init_triangle();
    vector<vector <float> > triangles;
    void init_event();
    void draw_event();
    void mouse_click_event(int button, int button_state, int x, int y);
    void mouse_move_event(int x, int y);
    void keyboard_event(unsigned char key, int x, int y);
};

#endif
