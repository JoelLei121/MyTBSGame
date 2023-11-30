#ifndef RIGHTUI_H
#define RIGHTUI_H


#include <QWidget>
#include <QSize>

class RoleUnit;
class QPushButton;
class QLabel;
class QVBoxLayout;
class QHBoxLayout;
class QGridLayout;
class QPlainTextEdit;

class rightUi : public QWidget
{
    Q_OBJECT
public:
    rightUi(QWidget* parent = 0);
    ~rightUi();

private:
    // Title
    QLabel *gameTitle;
    QVBoxLayout *gameTitleLayout;

    // button of action
    QPushButton *btnAttack;
    QPushButton *btnMove;
    QHBoxLayout *btnLayout;

    // label of data
    QLabel *roleTeamLabel;
    QLabel *roleClassLabel;
    QLabel *roleHPLabel;
    QLabel *roleATKLabel;
    QLabel *roleActionLabel;
    QGridLayout *infoLayout;

    // battle record
    QLabel *battleRecordTitle;
    QPlainTextEdit *battleRecordBox;
    QPushButton *btnClearBattleRecord;
    QVBoxLayout *battleRecordLayout;

    // about turn
    QLabel *TurnsNumberLabel;
    QLabel *WhoseTurnLabel;
    QPushButton *btnEndTurn;
    QVBoxLayout *turnControllerLayout;
    
    QVBoxLayout *mainLayout;

    // font simple
    QFont *LabelFont;
    QFont *LargeButtonFont;
    QFont *SmallButtonFont;

    // function use to classify initialization
    void initTitleBox();
    void initDataBox();
    void initBattleRecord();
    void initTurnController();

signals:
    void sendRoleAction(std::string);
    void sendTurnEnding(); // to BattleSystem::RoundEnd

public slots:
    void RoleOnFocusChanged(RoleUnit* role);
    void BattleSystemMessageReceived(QString message);
    void RoundChanged(QString team, int round);

private slots:
    void RoleActionSelectionChanged(std::string status);
    

};

#endif