#include "MyTextBrowser.h"

MyTextBrowser::MyTextBrowser(QWidget *parent):QTextBrowser(parent)
{
}

void MyTextBrowser::mouseDoubleClickEvent(QMouseEvent *event)
{
   this->parentWidget()->hide();
}
