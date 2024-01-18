#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <utility>
#include <functional>
#include <QGraphicsOpacityEffect>
#include <QFontDatabase>
#include <QScreen>
#include <array>
#include <map>
#define RESIZE_RECT(a) QRect(geometry().x(), geometry().y(), a, height())

MainWindow::MainWindow(mathVM *vm, QWidget *parent):
    QMainWindow(parent),
    ui(new Ui::MainWindow),
    math_vm(vm)
{
    auto fonts = setUpFonts();
    QApplication::setFont (fonts.at("Roboto"));
    ui->setupUi(this);
    populateKeyboard();
    setTopPanelButtonsTransparent();
    ui->funsButton->setFont(fonts.at("Georgia"));
    ui->answerLabel->adjustSize ();
    setUpAnimations ();

    connect(ui->backspaceButton, &QPushButton::pressed, this, [&]{
        int cursor_pos = ui->expressionEdit->cursorPosition ();
        if (cursor_pos) {
            QString new_text = ui->expressionEdit->text().remove (cursor_pos - 1, 1);
            ui->expressionEdit->setText(new_text);
        }
    });
    connect(ui->clearButton, &QPushButton::clicked, this, [&]{
        ui->expressionEdit->clear ();
        ui->answerLabel->setText ("");
    });
    connect(ui->funsButton, &QPushButton::toggled, this, [&](bool checked){
        if (checked)
            showFunsPanel();
        else
            hideFunsPanel ();
    });
    connect(ui->expressionEdit, &QLineEdit::textEdited, this, [&] {
        math_vm->setExpression(ui->expressionEdit->text());
    });
    connect(ui->expressionEdit, &QLineEdit::returnPressed, this, [&] {
        findChild<QPushButton*>("eqButton")->click();
    });
    connect(findChild<QPushButton*>("eqButton"), &QPushButton::clicked, this, [&] {
        QString result = math_vm->calcIt();
        ui->answerLabel->setText (result);
        ui->answerLabel->adjustSize ();
    });
//    ui->funsButton->move (ui->historyWidget->x() + (ui->historyWidget->width() - ui->funsButton->width()), ui->funsButton->y());
}

MainWindow::~MainWindow(){}

std::map<QString, QFont> MainWindow::setUpFonts() {
    std::array<QString, 2> res_names = {":/fonts/Georgia_Italic.ttf", ":/fonts/Robotolight.ttf"};
    std::map<QString, QFont> ready_fonts;
    for (QString font_name: res_names){
        int font_id = QFontDatabase::addApplicationFont(font_name);
        QString family = QFontDatabase::applicationFontFamilies(font_id).back();
        QFont font(family);
        ready_fonts.insert({family, font});
    }
    ready_fonts.at("Georgia").setPointSize(16);
    ready_fonts.at("Georgia").setItalic(true);
    return ready_fonts;
}

void MainWindow::setTopPanelButtonsTransparent(){
    QGraphicsOpacityEffect * effect = new QGraphicsOpacityEffect(ui->settingsButton);
    effect->setOpacity(0.7);
    ui->settingsButton->setGraphicsEffect (effect);

    effect = new QGraphicsOpacityEffect(ui->funsButton);
    effect->setOpacity(0.7);
    ui->funsButton->setGraphicsEffect (effect);
}

void MainWindow::setUpAnimations() {
    const int duration = 500;
    pan_w_anim = new QPropertyAnimation(ui->funVarPanel, "minimumWidth");
    QEasingCurve curve(QEasingCurve::OutCubic);
    pan_w_anim->setDuration(duration);
    pan_w_anim->setStartValue (0); // Нужно, чтобы корректно работало скрытие панели, где мы меняем направление анимации
    pan_w_anim->setEndValue(funs_tab_w);
    pan_w_anim->setEasingCurve (curve);

    win_w_anim = new QPropertyAnimation((QObject*)this, "geometry");
    win_w_anim->setDuration(duration);
    win_w_anim->setEasingCurve (curve);

    funsPanelAnimGroup = std::make_unique<QParallelAnimationGroup>();
    funsPanelAnimGroup->addAnimation(win_w_anim);
    funsPanelAnimGroup->addAnimation(pan_w_anim);

    QScreen* screen = QApplication::primaryScreen();
    screen_w = screen->availableSize().width();
}

void MainWindow::resizeEvent(QResizeEvent* event) {
    QMainWindow::resizeEvent(event);
//    ui->historyWidget->resize (width(), ui->historyWidget->height ());
    ui->historyListView->resize(ui->centralwidget->width(), ((QWidget*)ui->historyListView->parent())->height());
    ui->funsButton->move (ui->historyWidget->x() + (ui->historyWidget->width() - ui->funsButton->width()), ui->funsButton->y());
}

QPushButton* MainWindow::newKeyboardButton (QString text, QString ch) {
    QPushButton* but = new QPushButton(text);
    but->setFlat(true);
    connect(but, &QPushButton::clicked, this, std::bind([&](QString ch_){
        QLineEdit* box = this->ui->expressionEdit;
        unsigned int box_cursor_pos = box->cursorPosition();
        box->setText(box->text().insert(box_cursor_pos, ch_));
        box->setCursorPosition (box_cursor_pos + 1);
    }, ch));
    but->setMinimumSize (75, 75);
    but->setSizePolicy (QSizePolicy::Preferred, QSizePolicy::Expanding);
    but->setFocusPolicy(Qt::FocusPolicy::NoFocus);
    return but;
}

void MainWindow::populateKeyboard(bool phone_layout) {
    std::vector<std::pair<QString, QString>> keys_n_codes = {
        {"a=...", "="}, {"(", "("}, {")", ")"}, {"%", "%"}, {"^", "^"},
        {"", ""}, {"", ""}, {"", ""}, {"/", "/"}, {"//", "//"},
        {"", ""}, {"", ""}, {"", ""}, {"-", "-"}, {"*", "*"},
        {"", ""}, {"", ""}, {"", ""}, {"", ""}, {"", ""},
        {",", ","}, {"0", "0"}, {".", "."}, {"", ""}, {"", ""}
    };
    std::vector<std::pair<QString, QString>> digit_keys = {
        {"1", "1"}, {"2", "2"}, {"3", "3"},
        {"4", "4"}, {"5", "5"}, {"6", "6"},
        {"7", "7"}, {"8", "8"}, {"9", "9"}
    };
    for (int i = 0; i < 5; i++){
        for (int j = 0; j < 5; j++){
            auto cur_pair = keys_n_codes.at(i * 5 + j);
            if (cur_pair.first != "" && cur_pair.second != ""){
                ui->keyboardLayout->addWidget(newKeyboardButton (cur_pair.first, cur_pair.second), i, j);
            }
        }
    }
    for (int i = 0; i < 3; i++){
        for (int j = 0; j < 3; j++){
            auto cur_pair = digit_keys.at(i * 3 + j);
            ui->keyboardLayout->addWidget(newKeyboardButton (cur_pair.first, cur_pair.second),
                                          1 + (phone_layout? i : 2 - i),
                                          j);
        }
    }
    QPushButton* plus_button = newKeyboardButton ("+", "+");
    plus_button->setMinimumHeight (150);
    ui->keyboardLayout->addWidget(plus_button, 3, 4, 2, 1);

    QPushButton* eq_button = new QPushButton("=");
    eq_button->setObjectName("eqButton");
    eq_button->setMinimumHeight (150);
    eq_button->setStyleSheet("QPushButton{background-color:rgb(61, 61, 61)}"
                             "QPushButton:hover {color: rgb(52, 52, 52); background-color: rgb(164, 164, 164)}"
                             "QPushButton:hover:pressed {color: rgb(52, 52, 52); background-color: rgb(190, 190, 190)}");
    eq_button->setSizePolicy (QSizePolicy::Preferred, QSizePolicy::Expanding);
    ui->keyboardLayout->addWidget(eq_button, 3, 3, 2, 1);
}

void MainWindow::showFunsPanel() {
    pan_w_anim->setStartValue(0);
    pan_w_anim->setEndValue(funs_tab_w);
    const int screen_place_left = screen_w - (pos().x() + size().width());
    if (screen_place_left >= funs_tab_w)
        win_w_anim->setEndValue(RESIZE_RECT(width() + funs_tab_w));
    else
        win_w_anim->setEndValue(RESIZE_RECT(screen_w - pos().x()));
    funsPanelAnimGroup->start();
}

void MainWindow::hideFunsPanel() {
    win_w_anim->setEndValue(RESIZE_RECT(width() - funs_tab_w));
    pan_w_anim->setEndValue(0);
    pan_w_anim->setStartValue(funs_tab_w);
    funsPanelAnimGroup->start();
}
