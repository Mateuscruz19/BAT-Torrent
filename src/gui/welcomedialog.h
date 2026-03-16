#ifndef WELCOMEDIALOG_H
#define WELCOMEDIALOG_H

#include <QDialog>

class QCheckBox;

class WelcomeDialog : public QDialog
{
    Q_OBJECT
public:
    explicit WelcomeDialog(QWidget *parent = nullptr);
    bool dontShowAgain() const;

private:
    QCheckBox *m_dontShowCheck;
};

#endif
