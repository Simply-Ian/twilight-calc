#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QPushButton>
#include <QPropertyAnimation>
#include <QParallelAnimationGroup>
#include <memory>
#include "mathvm.h"
#include "funsmodel.h"
#include "varsmodel.h"

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(mathVM* vm, QAbstractItemModel* fm, QAbstractItemModel* varsm, QWidget *parent = nullptr);
    ~MainWindow();

private:
    std::unique_ptr<Ui::MainWindow> ui;
    std::unique_ptr<QParallelAnimationGroup> funsPanelAnimGroup;
    std::unique_ptr<mathVM> math_vm;

    QPropertyAnimation* pan_w_anim;
    QPropertyAnimation* win_w_anim;
    const int funs_tab_w = 165;
    int screen_w;
    QAbstractItemModel* funs_model;
    QAbstractItemModel* vars_model;

    void populateKeyboard(bool phone_layout=true);
    QPushButton* newKeyboardButton(QString text, QString ch);
    void resizeEvent(QResizeEvent* event) override;
    void setTopPanelButtonsTransparent();
    QMap<QString, QFont> setUpFonts();
    void showFunsPanel();
    void hideFunsPanel();
    void setUpAnimations();
    void showErrorMessage(const QMap<QString, QString>& error);
};
#endif // MAINWINDOW_H
