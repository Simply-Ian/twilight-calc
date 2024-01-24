#ifndef MATHVM_H
#define MATHVM_H

#undef slots
#include "mathModel/model.h"
#define slots
#include <QObject>
#include <memory>
#include <exception>
#include <QMap>

/// @brief Главная (и единственная) VM.
/// @details Связывает mathModel и компоненты главного окна, за исключением списков функций и переменных (ими занимаются отдельные модели-обертки)
class mathVM : public QObject
{
    Q_OBJECT

private:
    std::shared_ptr<mathModel> model = 0;

    /// @brief Текущее выражение
    QString m_expression;

    /// @brief Хранит описание исключения, выброшенного моделью при расчете текущего выражения
    QMap<QString, QString> m_excMap;

    Q_PROPERTY(QString result READ calcIt)
    Q_PROPERTY(QString Expression MEMBER m_expression WRITE setExpression)
    Q_PROPERTY(QMap<QString, QString> excMap READ getCurExc)

    /// @brief Обрезает незначащие нули после точки в числе (и саму точку, если она не нужна).
    /// @details Пример: 1.00000 => 1
    QString stripTrailingZeroes(QString number);

public:
    mathVM(std::string scripts_folder);

    const QMap<QString, QString>& getCurExc() const{
        return m_excMap;
    }

    /// @brief Вычисляет текущее выражение. При выбросе исключения записывает информацию о нем в m_excMap. После получения ответа оповещает
    /// varsModel через событие mathVM::calculationFinished()
    QString calcIt();

    /// @brief Обновляет выражение и очищает m_excMap
    void setExpression(QString new_expr);

    /// @brief Возвращает указатель на математическую модель
    const mathModel* getModelRef() const {
        return model.get();
    }
signals:
    /// @brief Оповещает, что при вычислении выражения произошла ошибка
    void modelExceptionEvent(const QMap<QString, QString>& arg);
    /// @brief Оповещает, что выичсление прошло успешно и получен ответ
    void calculationFinished();
};

#endif // MATHVM_H
