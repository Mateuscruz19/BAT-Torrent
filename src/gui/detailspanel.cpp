#include "detailspanel.h"
#include "../core/sessionmanager.h"
#include "../core/translator.h"
#include <QLabel>
#include <QFormLayout>
#include <QVBoxLayout>
#include <QTableWidget>
#include <QHeaderView>

DetailsPanel::DetailsPanel(SessionManager *session, QWidget *parent)
    : QTabWidget(parent), m_session(session)
{
    addTab(createGeneralTab(), tr_("detail_general"));
    addTab(createPeersTab(), tr_("detail_peers"));
    addTab(createFilesTab(), tr_("detail_files"));

    setMaximumHeight(200);

    connect(m_session, &SessionManager::torrentsUpdated, this, &DetailsPanel::refresh);
}

void DetailsPanel::showTorrent(int index)
{
    m_currentIndex = index;
    refresh();
}

static QString fmtSize(qint64 bytes)
{
    if (bytes < 1024) return QString::number(bytes) + " B";
    if (bytes < 1024 * 1024) return QString::number(bytes / 1024.0, 'f', 1) + " KB";
    if (bytes < 1024LL * 1024 * 1024) return QString::number(bytes / (1024.0 * 1024.0), 'f', 1) + " MB";
    return QString::number(bytes / (1024.0 * 1024.0 * 1024.0), 'f', 2) + " GB";
}

static QString fmtSpeed(int bps)
{
    if (bps < 1024) return QString::number(bps) + " B/s";
    if (bps < 1024 * 1024) return QString::number(bps / 1024.0, 'f', 1) + " KB/s";
    return QString::number(bps / (1024.0 * 1024.0), 'f', 1) + " MB/s";
}

void DetailsPanel::refresh()
{
    if (m_currentIndex < 0 || m_currentIndex >= m_session->torrentCount()) {
        m_nameLabel->setText("-");
        m_sizeLabel->setText("-");
        m_progressLabel->setText("-");
        m_downSpeedLabel->setText("-");
        m_upSpeedLabel->setText("-");
        m_stateLabel->setText("-");
        m_peersLabel->setText("-");
        m_downloadedLabel->setText("-");
        m_savePathLabel->setText("-");
        m_peersTable->setRowCount(0);
        m_filesTable->setRowCount(0);
        return;
    }

    TorrentInfo info = m_session->torrentAt(m_currentIndex);
    m_nameLabel->setText(info.name);
    m_stateLabel->setText(info.stateString);
    m_peersLabel->setText(QString("%1 (%2 seeds)").arg(info.numPeers).arg(info.numSeeds));
    m_progressLabel->setText(QString::number(info.progress * 100.0, 'f', 1) + "%");
    m_sizeLabel->setText(fmtSize(info.totalSize));
    m_downloadedLabel->setText(fmtSize(info.totalDone));
    m_downSpeedLabel->setText(fmtSpeed(info.downloadRate));
    m_upSpeedLabel->setText(fmtSpeed(info.uploadRate));
    m_savePathLabel->setText(info.savePath);

    // Peers tab
    if (currentIndex() == 1) {
        auto peers = m_session->peersAt(m_currentIndex);
        m_peersTable->setRowCount(static_cast<int>(peers.size()));
        for (int i = 0; i < static_cast<int>(peers.size()); ++i) {
            m_peersTable->setItem(i, 0, new QTableWidgetItem(peers[i].ip));
            m_peersTable->setItem(i, 1, new QTableWidgetItem(QString::number(peers[i].port)));
            m_peersTable->setItem(i, 2, new QTableWidgetItem(peers[i].client));
            m_peersTable->setItem(i, 3, new QTableWidgetItem(fmtSpeed(peers[i].downloadRate)));
            m_peersTable->setItem(i, 4, new QTableWidgetItem(fmtSpeed(peers[i].uploadRate)));
            m_peersTable->setItem(i, 5, new QTableWidgetItem(
                QString::number(peers[i].progress * 100.0, 'f', 1) + "%"));
        }
    }

    // Files tab
    if (currentIndex() == 2) {
        auto files = m_session->filesAt(m_currentIndex);
        m_filesTable->setRowCount(static_cast<int>(files.size()));
        for (int i = 0; i < static_cast<int>(files.size()); ++i) {
            m_filesTable->setItem(i, 0, new QTableWidgetItem(files[i].path));
            m_filesTable->setItem(i, 1, new QTableWidgetItem(fmtSize(files[i].size)));
            m_filesTable->setItem(i, 2, new QTableWidgetItem(
                QString::number(files[i].progress * 100.0, 'f', 1) + "%"));
        }
    }
}

QWidget *DetailsPanel::createGeneralTab()
{
    auto *widget = new QWidget;
    auto *layout = new QFormLayout(widget);
    layout->setContentsMargins(12, 8, 12, 8);
    layout->setSpacing(4);

    m_nameLabel = new QLabel("-");
    m_sizeLabel = new QLabel("-");
    m_progressLabel = new QLabel("-");
    m_downSpeedLabel = new QLabel("-");
    m_upSpeedLabel = new QLabel("-");
    m_stateLabel = new QLabel("-");
    m_peersLabel = new QLabel("-");
    m_downloadedLabel = new QLabel("-");
    m_savePathLabel = new QLabel("-");

    auto addRow = [&](const QString &label, QLabel *value) {
        auto *lbl = new QLabel(label);
        lbl->setStyleSheet("color: #c43030; font-weight: bold;");
        layout->addRow(lbl, value);
    };

    addRow("Name:", m_nameLabel);
    addRow("Save Path:", m_savePathLabel);
    addRow("Size:", m_sizeLabel);
    addRow("Downloaded:", m_downloadedLabel);
    addRow("Progress:", m_progressLabel);
    addRow("Down Speed:", m_downSpeedLabel);
    addRow("Up Speed:", m_upSpeedLabel);
    addRow("State:", m_stateLabel);
    addRow("Peers:", m_peersLabel);

    return widget;
}

QWidget *DetailsPanel::createPeersTab()
{
    auto *widget = new QWidget;
    auto *layout = new QVBoxLayout(widget);
    layout->setContentsMargins(0, 0, 0, 0);

    m_peersTable = new QTableWidget(0, 6);
    m_peersTable->setHorizontalHeaderLabels({"IP", "Port", "Client", "Down", "Up", "Progress"});
    m_peersTable->horizontalHeader()->setStretchLastSection(true);
    m_peersTable->verticalHeader()->hide();
    m_peersTable->setEditTriggers(QAbstractItemView::NoEditTriggers);
    m_peersTable->setSelectionBehavior(QAbstractItemView::SelectRows);
    m_peersTable->setShowGrid(false);
    m_peersTable->setAlternatingRowColors(true);
    m_peersTable->setStyleSheet(R"(
        QTableWidget {
            background-color: #141414;
            alternate-background-color: #181818;
            color: #b0b0b0;
            border: none;
        }
        QHeaderView::section {
            background-color: #1a1a1a;
            color: #c43030;
            border: none;
            border-bottom: 1px solid #c43030;
            padding: 4px;
            font-weight: bold;
        }
    )");

    layout->addWidget(m_peersTable);
    return widget;
}

QWidget *DetailsPanel::createFilesTab()
{
    auto *widget = new QWidget;
    auto *layout = new QVBoxLayout(widget);
    layout->setContentsMargins(0, 0, 0, 0);

    m_filesTable = new QTableWidget(0, 3);
    m_filesTable->setHorizontalHeaderLabels({"File", "Size", "Progress"});
    m_filesTable->horizontalHeader()->setStretchLastSection(true);
    m_filesTable->horizontalHeader()->setSectionResizeMode(0, QHeaderView::Stretch);
    m_filesTable->verticalHeader()->hide();
    m_filesTable->setEditTriggers(QAbstractItemView::NoEditTriggers);
    m_filesTable->setSelectionBehavior(QAbstractItemView::SelectRows);
    m_filesTable->setShowGrid(false);
    m_filesTable->setAlternatingRowColors(true);
    m_filesTable->setStyleSheet(R"(
        QTableWidget {
            background-color: #141414;
            alternate-background-color: #181818;
            color: #b0b0b0;
            border: none;
        }
        QHeaderView::section {
            background-color: #1a1a1a;
            color: #c43030;
            border: none;
            border-bottom: 1px solid #c43030;
            padding: 4px;
            font-weight: bold;
        }
    )");

    layout->addWidget(m_filesTable);
    return widget;
}

void DetailsPanel::retranslate()
{
    setTabText(0, tr_("detail_general"));
    setTabText(1, tr_("detail_peers"));
    setTabText(2, tr_("detail_files"));

    m_peersTable->setHorizontalHeaderLabels({
        tr_("peer_ip"), tr_("peer_port"), tr_("peer_client"),
        tr_("peer_down"), tr_("peer_up"), tr_("peer_progress")
    });
    m_filesTable->setHorizontalHeaderLabels({
        tr_("file_name"), tr_("file_size"), tr_("file_progress")
    });
}
