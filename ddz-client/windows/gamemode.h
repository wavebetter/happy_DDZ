#ifndef GAMEMODE_H
#define GAMEMODE_H

#include <QDialog>

namespace Ui {
class GameMode;
}

class GameMode : public QDialog
{
    Q_OBJECT

public:
    explicit GameMode(QWidget *parent = nullptr);
    ~GameMode();

    void showInfo(int count);

protected:
    void closeEvent(QCloseEvent* ev);

private:
    Ui::GameMode *ui;
};

#endif // GAMEMODE_H
