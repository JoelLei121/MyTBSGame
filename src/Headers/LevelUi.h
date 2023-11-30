#ifndef LEVELUI_H
#define LEVELUI_H

#include <QMainWindow>
#include <QPushButton>
#include <QVector>
class QGraphicsScene;
class QGraphicsView;
class QLabel;
class QVBoxLayout;
class QGridLayout;
class QColseEvent;

class LevelButton : public QPushButton
{
    Q_OBJECT

public:
    explicit LevelButton(const QString &text, QWidget *parent = nullptr);

    QSize sizeHint() const override;
};

class LevelUi : public QMainWindow
{
    Q_OBJECT

public:
    explicit LevelUi(QWidget *parent = nullptr);
    
protected:
    void closeEvent(QCloseEvent *event);

private:
    QGraphicsScene *scene;
    QGraphicsView *view;
    QImage *image;
    QBrush *brush;

    QLabel *LevelLabel;
    QVBoxLayout *labelLayout;

    QVector<LevelButton*> btnLevel;
    QGridLayout *btnLayout;
    void btnInit(int n);

    QVBoxLayout *mainLayout;

    QFont *LabelFont;
    QFont *ButtonFont;

signals:
    void setLevel(int);
    void sendLevelCreationRequset(int);
    void backToMenu();

private slots:
    void createLevel(int i);

public slots:
    void enteringLevelUi();
};


#endif
