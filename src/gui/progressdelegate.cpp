#include "progressdelegate.h"
#include <QPainter>
#include <QApplication>
#include <QStyleOptionProgressBar>

void ProgressDelegate::paint(QPainter *painter, const QStyleOptionViewItem &option,
                             const QModelIndex &index) const
{
    QVariant progressVar = index.data(Qt::UserRole);
    if (!progressVar.isValid()) {
        QStyledItemDelegate::paint(painter, option, index);
        return;
    }

    float progress = progressVar.toFloat();

    QStyleOptionProgressBar progressBar;
    progressBar.rect = option.rect.adjusted(2, 2, -2, -2);
    progressBar.minimum = 0;
    progressBar.maximum = 100;
    progressBar.progress = static_cast<int>(progress * 100);
    progressBar.text = QString::number(progress * 100.0, 'f', 1) + "%";
    progressBar.textVisible = true;

    QApplication::style()->drawControl(QStyle::CE_ProgressBar, &progressBar, painter);
}
