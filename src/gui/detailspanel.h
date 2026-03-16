#ifndef DETAILSPANEL_H
#define DETAILSPANEL_H

#include <QTabWidget>

class QLabel;
class QTableWidget;
class SessionManager;

class DetailsPanel : public QTabWidget
{
    Q_OBJECT
public:
    explicit DetailsPanel(SessionManager *session, QWidget *parent = nullptr);

public slots:
    void showTorrent(int index);
    void refresh();
    void retranslate();

private:
    QWidget *createGeneralTab();
    QWidget *createPeersTab();
    QWidget *createFilesTab();

    SessionManager *m_session;
    int m_currentIndex = -1;

    // General tab
    QLabel *m_nameLabel;
    QLabel *m_sizeLabel;
    QLabel *m_progressLabel;
    QLabel *m_downSpeedLabel;
    QLabel *m_upSpeedLabel;
    QLabel *m_stateLabel;
    QLabel *m_peersLabel;
    QLabel *m_downloadedLabel;
    QLabel *m_savePathLabel;

    // Peers tab
    QTableWidget *m_peersTable;

    // Files tab
    QTableWidget *m_filesTable;
};

#endif
