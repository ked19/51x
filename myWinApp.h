#ifndef _MYWIN_APP_H
#define _MYWIN_APP_H

#include "define.h"
#include "myMath.h"
#include "cluster.h"
#include "texture.h"
#include "volumeData.h"
#include "draw.h"
#include "imageIO.h"

#include <windows.h>
#include <GL/glew.h>
#include <FL/Fl_Gl_Window.h>
#include <FL/Fl_File_Chooser.h>
#include <Fl/Fl_Value_Slider.H>
#include <cassert>
#include <vector>
#include <sstream>

class MyGlWindow: public Fl_Gl_Window
{
public:
	MyGlWindow(int x, int y, int w, int h, const char *l=0);
	void draw();
	int handle(int evnt);

private:
};

extern MyGlWindow *pRender_glWin;
extern Fl_Group *pColorFr_group;
extern Fl_Group *pColorTo_group;
extern Fl_Value_Slider *pLevelReduce_valScl;

void SetTarget		(Fl_Widget *w, void *v);
void DisplayTarget	(Fl_Widget *w, void *v);
void ClearTarget	(Fl_Widget *w, void *v);

void LoadData		(Fl_Widget *w, void *v);
void AddSelecter	(Fl_Widget *w, void *v);
void EditSelecter	(Fl_Widget *w, void *v);
void CopyColor		(Fl_Widget *w, void *v);
void SetColor		(Fl_Widget *w, void *v);
void GenCluster		(Fl_Widget *w, void *v);

void ReduceLevel	(Fl_Widget *w, void *v);
void Truncate		(Fl_Widget *w, void *v);

#endif