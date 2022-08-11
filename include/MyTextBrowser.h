#ifndef MYTEXTBROWSER_H
#define MYTEXTBROWSER_H
#include <QTextBrowser>

class MyTextBrowser :public QTextBrowser
{
public:
    MyTextBrowser(QWidget *parent=nullptr);

protected:
        void mouseDoubleClickEvent(QMouseEvent *event) override;
};

#endif // MYTEXTBROWSER_H
