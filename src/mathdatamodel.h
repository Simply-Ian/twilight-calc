#ifndef MATHDATAMODEL_H
#define MATHDATAMODEL_H
#include <memory>
#include "mathModel/model.h"
#include <QAbstractListModel>


class mathDataModel : public QAbstractListModel
{
    Q_OBJECT
protected:
    std::shared_ptr<const mathModel> math;
public:
    mathDataModel(const mathModel* m);
    QVariant data(const QModelIndex &index, int role) const = 0;
    int rowCount(const QModelIndex &parent) const = 0;
signals:

};

#endif // MATHDATAMODEL_H
