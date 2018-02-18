#pragma once

#include <QtWidgets/QMainWindow>
#include "ui_SimpleQtOGL.h"

class SimpleQtOGL : public QMainWindow
{
    Q_OBJECT

public:
    SimpleQtOGL(QWidget *parent = Q_NULLPTR);

private:
    Ui::SimpleQtOGLClass ui;
};
