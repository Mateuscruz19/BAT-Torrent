#include <QApplication>
#include <libtorrent/session.hpp>
#include <libtorrent/load_torrent.hpp>
#include <iostream>
#include <thread>
#include "gui/mainwindow.h"

int main(int argc, char *argv[]) {
    lt::session ses;
    lt::add_torrent_params atp = lt::load_torrent_file("/home/baiano/Downloads/torrent.torrent");
    atp.save_path = "/tmp/downloads";
    ses.add_torrent(atp);

QApplication app(argc, argv);
MainWindow window;
window.show();

return app.exec();
}
