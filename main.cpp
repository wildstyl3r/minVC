#include <QApplication>
#include <vc_ui.h>

int main(int argc, char *argv[])
{
  QApplication a(argc, argv);
  VC_ui gui;
  gui.show();
  return a.exec();
}
