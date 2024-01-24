#ifndef VARSMODEL_H
#define VARSMODEL_H
#include "mathdatamodel.h"


/// @brief Обертка над mathModel, предоставляющая доступ к словарю доступных переменных в стиле QAbstractListModel
class varsModel : public mathDataModel
{
    Q_OBJECT
public:
    varsModel(const mathModel* m);
    QVariant data(const QModelIndex &index, int role) const override;
    int rowCount(const QModelIndex &parent) const override;
    void newRow();
    void fixNewRow();
private:
    /// @brief Флаг, показывающий, была ли при вычислении текущего выражения добавлена новая переменная
    /// @details Нужен для корректного добавления новой строки в модель и обновления View
    bool newVarInserted = false;
};

#endif // VARSMODEL_H
