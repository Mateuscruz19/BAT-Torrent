#ifndef TORRENTFILTER_H
#define TORRENTFILTER_H

#include <QSortFilterProxyModel>

class TorrentFilter : public QSortFilterProxyModel
{
    Q_OBJECT
public:
    explicit TorrentFilter(QObject *parent = nullptr);

    void setStateFilter(const QString &state);
    void setNameFilter(const QString &text);

protected:
    bool filterAcceptsRow(int sourceRow, const QModelIndex &sourceParent) const override;
    bool lessThan(const QModelIndex &left, const QModelIndex &right) const override;

private:
    QString m_stateFilter;
    QString m_nameFilter;
};

#endif
