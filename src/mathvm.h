#ifndef MATHVM_H
#define MATHVM_H

#undef slots
#include "mathModel/model.h"
#define slots
#include <QObject>
#include <memory>
#include <exception>
#include <QMap>

class mathVM : public QObject
{
    Q_OBJECT

private:
    std::unique_ptr<mathModel> model = 0;

    QString m_expression;
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

    /// @todo: сообщения об ошибке на заданном в настройках языке
    QString calcIt();

    /// @brief Обновляет выражение и очищает словарь исключения
    void setExpression(QString new_expr);
signals:
    void modelExceptionEvent(const QMap<QString, QString>& arg);
};

#endif // MATHVM_H
