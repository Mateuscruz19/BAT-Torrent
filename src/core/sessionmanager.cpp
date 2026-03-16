#include "sessionmanager.h"
#include "translator.h"
#include <libtorrent/load_torrent.hpp>
#include <libtorrent/torrent_info.hpp>
#include <libtorrent/magnet_uri.hpp>
#include <libtorrent/write_resume_data.hpp>
#include <libtorrent/read_resume_data.hpp>
#include <libtorrent/alert_types.hpp>
#include <libtorrent/peer_info.hpp>
#include <QDir>
#include <QStandardPaths>
#include <QFile>

SessionManager::SessionManager(QObject *parent)
    : QObject(parent)
{
    lt::settings_pack pack;
    pack.set_int(lt::settings_pack::alert_mask,
                 lt::alert_category::status | lt::alert_category::error
                 | lt::alert_category::storage);
    m_session.apply_settings(pack);

    loadResumeData();

    connect(&m_updateTimer, &QTimer::timeout, this, &SessionManager::updateStats);
    m_updateTimer.start(1000);
}

SessionManager::~SessionManager()
{
    saveResumeData();
}

void SessionManager::addTorrent(const QString &filePath, const QString &savePath)
{
    lt::add_torrent_params atp = lt::load_torrent_file(filePath.toStdString());
    atp.save_path = savePath.toStdString();

    lt::torrent_handle h = m_session.add_torrent(atp);
    m_torrents.push_back(h);

    emit torrentAdded(static_cast<int>(m_torrents.size()) - 1);
}

void SessionManager::addMagnet(const QString &uri, const QString &savePath)
{
    lt::add_torrent_params atp = lt::parse_magnet_uri(uri.toStdString());
    atp.save_path = savePath.toStdString();

    lt::torrent_handle h = m_session.add_torrent(atp);
    m_torrents.push_back(h);

    emit torrentAdded(static_cast<int>(m_torrents.size()) - 1);
}

void SessionManager::removeTorrent(int index, bool deleteFiles)
{
    if (index < 0 || index >= static_cast<int>(m_torrents.size()))
        return;

    lt::remove_flags_t flags{};
    if (deleteFiles)
        flags = lt::session::delete_files;

    m_session.remove_torrent(m_torrents[index], flags);
    m_torrents.erase(m_torrents.begin() + index);
    emit torrentRemoved(index);
}

void SessionManager::pauseTorrent(int index)
{
    if (index < 0 || index >= static_cast<int>(m_torrents.size()))
        return;
    m_torrents[index].pause();
}

void SessionManager::resumeTorrent(int index)
{
    if (index < 0 || index >= static_cast<int>(m_torrents.size()))
        return;
    m_torrents[index].resume();
}

int SessionManager::torrentCount() const
{
    return static_cast<int>(m_torrents.size());
}

TorrentInfo SessionManager::torrentAt(int index) const
{
    TorrentInfo info{};
    if (index < 0 || index >= static_cast<int>(m_torrents.size()))
        return info;

    lt::torrent_status st = m_torrents[index].status();
    info.handle = m_torrents[index];
    info.name = QString::fromStdString(st.name);
    info.savePath = QString::fromStdString(st.save_path);
    info.totalSize = st.total_wanted;
    info.totalDone = st.total_wanted_done;
    info.progress = st.progress;
    info.downloadRate = st.download_rate;
    info.uploadRate = st.upload_rate;
    info.numPeers = st.num_peers;
    info.numSeeds = st.num_seeds;
    info.stateString = stateToString(st.state);
    info.paused = (st.flags & lt::torrent_flags::paused) != lt::torrent_flags_t{};
    if (info.paused)
        info.stateString = tr_("state_paused");
    return info;
}

std::vector<PeerInfo> SessionManager::peersAt(int index) const
{
    std::vector<PeerInfo> result;
    if (index < 0 || index >= static_cast<int>(m_torrents.size()))
        return result;

    std::vector<lt::peer_info> peers;
    m_torrents[index].get_peer_info(peers);

    for (const auto &p : peers) {
        PeerInfo pi;
        pi.ip = QString::fromStdString(p.ip.address().to_string());
        pi.port = p.ip.port();
        pi.downloadRate = p.down_speed;
        pi.uploadRate = p.up_speed;
        pi.progress = p.progress;
        pi.client = QString::fromStdString(p.client);
        result.push_back(pi);
    }
    return result;
}

std::vector<FileInfo> SessionManager::filesAt(int index) const
{
    std::vector<FileInfo> result;
    if (index < 0 || index >= static_cast<int>(m_torrents.size()))
        return result;

    auto ti = m_torrents[index].torrent_file();
    if (!ti) return result;

    std::vector<std::int64_t> fileProgress;
    m_torrents[index].file_progress(fileProgress);

    const auto &fs = ti->files();
    for (lt::file_index_t i(0); i < fs.end_file(); ++i) {
        FileInfo fi;
        fi.path = QString::fromStdString(fs.file_path(i));
        fi.size = fs.file_size(i);
        fi.progress = fi.size > 0 ? static_cast<float>(fileProgress[static_cast<int>(i)]) / fi.size : 1.0f;
        fi.priority = static_cast<int>(m_torrents[index].file_priority(i));
        result.push_back(fi);
    }
    return result;
}

void SessionManager::setDownloadLimit(int kbps)
{
    lt::settings_pack pack;
    pack.set_int(lt::settings_pack::download_rate_limit, kbps > 0 ? kbps * 1024 : 0);
    m_session.apply_settings(pack);
}

void SessionManager::setUploadLimit(int kbps)
{
    lt::settings_pack pack;
    pack.set_int(lt::settings_pack::upload_rate_limit, kbps > 0 ? kbps * 1024 : 0);
    m_session.apply_settings(pack);
}

int SessionManager::downloadLimit() const
{
    return m_session.get_settings().get_int(lt::settings_pack::download_rate_limit) / 1024;
}

int SessionManager::uploadLimit() const
{
    return m_session.get_settings().get_int(lt::settings_pack::upload_rate_limit) / 1024;
}

void SessionManager::saveResumeData()
{
    QDir dir(resumeDataDir());
    if (!dir.exists())
        dir.mkpath(".");

    for (size_t i = 0; i < m_torrents.size(); ++i) {
        lt::torrent_status st = m_torrents[i].status();
        if (!st.has_metadata) continue;

        m_torrents[i].save_resume_data(lt::torrent_handle::save_info_dict);
    }

    // Process save_resume_data alerts
    m_session.wait_for_alert(std::chrono::seconds(3));
    std::vector<lt::alert *> alerts;
    m_session.pop_alerts(&alerts);

    for (auto *a : alerts) {
        if (auto *rd = lt::alert_cast<lt::save_resume_data_alert>(a)) {
            auto buf = lt::write_resume_data_buf(rd->params);
            lt::torrent_status st = rd->handle.status();
            QString name = QString::fromStdString(st.name);
            QString filePath = dir.filePath(name + ".resume");
            QFile file(filePath);
            if (file.open(QIODevice::WriteOnly)) {
                file.write(buf.data(), static_cast<qint64>(buf.size()));
            }
        }
    }
}

void SessionManager::loadResumeData()
{
    QDir dir(resumeDataDir());
    if (!dir.exists()) return;

    QStringList files = dir.entryList({"*.resume"}, QDir::Files);
    for (const auto &fileName : files) {
        QFile file(dir.filePath(fileName));
        if (!file.open(QIODevice::ReadOnly)) continue;

        QByteArray data = file.readAll();
        lt::error_code ec;
        lt::add_torrent_params atp = lt::read_resume_data(
            lt::span<const char>(data.data(), data.size()), ec);
        if (ec) continue;

        lt::torrent_handle h = m_session.add_torrent(atp);
        m_torrents.push_back(h);
    }

    if (!m_torrents.empty())
        emit torrentsUpdated();
}

void SessionManager::updateStats()
{
    processAlerts();
    if (!m_torrents.empty())
        emit torrentsUpdated();
}

void SessionManager::processAlerts()
{
    std::vector<lt::alert *> alerts;
    m_session.pop_alerts(&alerts);

    for (auto *a : alerts) {
        if (auto *fa = lt::alert_cast<lt::torrent_finished_alert>(a)) {
            QString name = QString::fromStdString(fa->torrent_name());
            emit torrentFinished(name);
        }
    }
}

QString SessionManager::resumeDataDir() const
{
    return QStandardPaths::writableLocation(QStandardPaths::AppDataLocation) + "/resume";
}

QString SessionManager::stateToString(lt::torrent_status::state_t state)
{
    switch (state) {
    case lt::torrent_status::checking_files: return tr_("state_checking");
    case lt::torrent_status::downloading_metadata: return tr_("state_metadata");
    case lt::torrent_status::downloading: return tr_("state_downloading");
    case lt::torrent_status::finished: return tr_("state_finished");
    case lt::torrent_status::seeding: return tr_("state_seeding");
    case lt::torrent_status::checking_resume_data: return tr_("state_checking");
    default: return tr_("state_unknown");
    }
}
