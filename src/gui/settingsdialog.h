#ifndef SETTINGSDIALOG_H
#define SETTINGSDIALOG_H

#include <QDialog>

class QSpinBox;
class QLineEdit;
class QComboBox;
class QCheckBox;

class SettingsDialog : public QDialog
{
    Q_OBJECT
public:
    explicit SettingsDialog(QWidget *parent = nullptr);

    QString defaultSavePath() const;
    int maxDownloadSpeed() const;
    int maxUploadSpeed() const;
    int languageIndex() const;
    bool startMinimized() const;
    bool useDefaultPath() const;
    int themeIndex() const;

    void setDefaultSavePath(const QString &path);
    void setMaxDownloadSpeed(int kbps);
    void setMaxUploadSpeed(int kbps);
    void setLanguageIndex(int index);
    void setStartMinimized(bool val);
    void setUseDefaultPath(bool val);
    void setThemeIndex(int index);

private slots:
    void browseSavePath();

private:
    void setupStyle();

    QLineEdit *m_savePathEdit;
    QSpinBox *m_maxDownSpin;
    QSpinBox *m_maxUpSpin;
    QComboBox *m_languageCombo;
    QCheckBox *m_startMinimizedCheck;
    QCheckBox *m_useDefaultPathCheck;
    QComboBox *m_themeCombo;
};

#endif
