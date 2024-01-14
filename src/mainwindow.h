#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QPushButton>
#include <QPropertyAnimation>
#include <QParallelAnimationGroup>

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

private:
    Ui::MainWindow *ui;
    void populateKeyboard(bool phone_layout=true);
    QPushButton* newKeyboardButton(QString text, QString ch);
    void resizeEvent(QResizeEvent* event) override;
    void setTopPanelButtonsTransparent();
    std::map<QString, QFont> setUpFonts();
    void showFunsPanel();
    void hideFunsPanel();

//    QPropertyAnimation* pan_w_anim;
    QParallelAnimationGroup* a_group;
};
#endif // MAINWINDOW_H
