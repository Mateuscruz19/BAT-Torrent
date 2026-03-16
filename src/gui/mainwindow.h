#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>

class QTableView;
class QLabel;
class QSplitter;
class QSystemTrayIcon;
class SessionManager;
class TorrentModel;
class DetailsPanel;
class SpeedGraph;

class MainWindow : public QMainWindow
{
    Q_OBJECT
public:
    explicit MainWindow(SessionManager *session, QWidget *parent = nullptr);
    ~MainWindow();

protected:
    void closeEvent(QCloseEvent *event) override;
    void dragEnterEvent(QDragEnterEvent *event) override;
    void dropEvent(QDropEvent *event) override;

private slots:
    void openTorrent();
    void openMagnet();
    void removeSelected();
    void pauseSelected();
    void resumeSelected();
    void updateStatusBar();
    void onSelectionChanged();
    void onTorrentFinished(const QString &name);
    void trayActivated();
    void openSettings();
    void showWelcome();
    void retranslateUi();

private:
    void applyTheme();
    void setupMenuBar();
    void setupToolBar();
    void setupCentralWidget();
    void setupStatusBar();
    void setupTrayIcon();
    void saveSettings();
    void loadSettings();
    int selectedRow() const;
    void addTorrentFile(const QString &filePath);

    SessionManager *m_session;
    TorrentModel *m_model;
    QTableView *m_tableView;
    DetailsPanel *m_detailsPanel;
    SpeedGraph *m_speedGraph;
    QLabel *m_statusLabel;
    QSystemTrayIcon *m_trayIcon;
    QString m_lastSavePath;
    bool m_startMinimized = false;
    bool m_useDefaultPath = false;
};

#endif
