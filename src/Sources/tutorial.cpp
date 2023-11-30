#include "Headers/tutorial.h"
#include "ui_tutorial.h"

#include <QPushButton>
#include <QCloseEvent>

Tutorial::Tutorial(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::Tutorial)
{
    ui->setupUi(this);
}

Tutorial::~Tutorial()
{
    delete ui;
}

void Tutorial::on_btnBack_clicked()
{
    this->hide();
    emit backToMenu();
}

void Tutorial::closeEvent (QCloseEvent *event)
{
    this->hide();
    emit backToMenu();
    event->ignore();
}