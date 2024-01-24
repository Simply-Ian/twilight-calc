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
    /// @param vm Указатель на объект VM
    /// @param fm Указатель на объект funsModel
    /// @param varsm Указатель на объект varModel
    MainWindow(mathVM* vm, QAbstractItemModel* fm, QAbstractItemModel* varsm, QWidget *parent = nullptr);
    ~MainWindow();

private:
    std::unique_ptr<Ui::MainWindow> ui;

    /// @brief Хранит анимации показа (скрытия) боковой панели
    std::unique_ptr<QParallelAnimationGroup> funsPanelAnimGroup;
    /// @brief Анимация показа (скрытия) боковой панели. Управляет ее минимальной шириной
    QPropertyAnimation* pan_w_anim;
    /// @brief Анимация расширения (сужения) окна.
    /// @details Запускается и выполняется одновременно с MainWindow::pan_w_anim, таким образом, при раскрытии боковая панель занимает
    /// освободившееся место, а остальные виджеты не меняют свое положение и размер
    QPropertyAnimation* win_w_anim;
    /// @brief Ширина боковой панели
    const int funs_tab_w = 165;
    /// @brief Хранит ширину экрана, доступную приложениям (т. е. без системной панели приложений в Ubuntu и т. п.)
    int screen_w;

    QAbstractItemModel* funs_model;
    QAbstractItemModel* vars_model;
    std::unique_ptr<mathVM> math_vm;

    /// @brief Создает клавиши экранной клавиатуры и добавляет их в keyboardLayout.
    /// @param phone_layout Обозначает один из вариантов расположения цифровых кнопок: как на мобильном телефоне (верхний ряд 1 2 3), либо как на
    /// клавиатуре компьютера (верхний ряд 7 8 9).
    void populateKeyboard(bool phone_layout=true);

    /// @brief Создает новую кнопку экранной клавиатуры с надписью text, задает ее внешний вид и назначает коллбэк: при нажатии кнопка дописывает
    /// в Ui::expressionEdit строку ch
    /// @returns Указатель на созданную кнопку. Добавить ее в компоновщик нужно самостоятельно
    QPushButton* newKeyboardButton(QString text, QString ch);

    /// @brief Загружает из ресурсов два шрифта
    /// @returns QMap: название font-family / объект шрифта
    QMap<QString, QFont> setUpFonts();

    /// @brief Раскрывает боковую панель, запуская анимации из MainWindow::funsPanelAnimGroup
    void showFunsPanel();
    /// @brief Скрывает боковую панель, запуская анимации из MainWindow::funsPanelAnimGroup
    void hideFunsPanel();

    /// @brief Создает объекты анимаций MainWindow::pan_w_anim и MainWindow::win_w_anim, производит первоначальную настройку.
    void setUpAnimations();

    /// @brief Если при расчете выражения возникает ошибка, подсвечивает в  Ui::expressionEdit символ (токен), вызвавший ошибку и показывает
    /// QToolTip с текстом сообщения об ошибке
    void showErrorMessage(const QMap<QString, QString>& error);
};
#endif // MAINWINDOW_H
