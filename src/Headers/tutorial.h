#ifndef TUTORIAL_H
#define TUTORIAL_H

#include <QMainWindow>

class QCloseEvent;

namespace Ui {
class Tutorial;
}

class Tutorial : public QMainWindow
{
    Q_OBJECT

public:
    explicit Tutorial(QWidget *parent = nullptr);
    ~Tutorial();

protected:
    void closeEvent (QCloseEvent *event);

private:
    Ui::Tutorial *ui;

signals:
    void backToMenu();

private slots:
    void on_btnBack_clicked();
};

#endif // TUTORIAL_H
