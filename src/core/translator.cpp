#include "translator.h"

Translator &Translator::instance()
{
    static Translator t;
    return t;
}

Translator::Translator()
{
    loadEnglish();
}

void Translator::setLanguage(Language lang)
{
    m_lang = lang;
    if (lang == Portuguese)
        loadPortuguese();
    else
        loadEnglish();
}

QString Translator::tr(const QString &key) const
{
    return m_strings.value(key, key);
}

void Translator::loadEnglish()
{
    m_strings = {
        // Menu
        {"menu_file", "&File"},
        {"menu_torrent", "&Torrent"},
        {"menu_settings", "&Settings"},
        {"menu_help", "&Help"},
        {"action_open", "&Open Torrent..."},
        {"action_magnet", "Open &Magnet Link..."},
        {"action_quit", "&Quit"},
        {"action_pause", "&Pause"},
        {"action_resume", "&Resume"},
        {"action_remove", "&Remove"},
        {"action_settings", "&Preferences..."},
        {"action_language", "&Language"},
        {"action_about", "&About BATorrent"},
        {"action_welcome", "&Welcome Guide"},

        // Toolbar
        {"tb_open", "Open"},
        {"tb_magnet", "Magnet"},
        {"tb_pause", "Pause"},
        {"tb_resume", "Resume"},
        {"tb_remove", "Remove"},
        {"tb_settings", "Settings"},

        // Table headers
        {"col_name", "Name"},
        {"col_size", "Size"},
        {"col_progress", "Progress"},
        {"col_down", "Down"},
        {"col_up", "Up"},
        {"col_state", "State"},
        {"col_peers", "Peers"},

        // Details
        {"detail_general", "General"},
        {"detail_peers", "Peers"},
        {"detail_files", "Files"},
        {"detail_name", "Name:"},
        {"detail_save_path", "Save Path:"},
        {"detail_size", "Size:"},
        {"detail_downloaded", "Downloaded:"},
        {"detail_progress", "Progress:"},
        {"detail_down_speed", "Down Speed:"},
        {"detail_up_speed", "Up Speed:"},
        {"detail_state", "State:"},
        {"detail_peers_count", "Peers:"},

        // Peer table
        {"peer_ip", "IP"},
        {"peer_port", "Port"},
        {"peer_client", "Client"},
        {"peer_down", "Down"},
        {"peer_up", "Up"},
        {"peer_progress", "Progress"},

        // File table
        {"file_name", "File"},
        {"file_size", "Size"},
        {"file_progress", "Progress"},

        // Status
        {"status_no_torrents", "No torrents"},
        {"status_format", "%1 torrent(s)  |  Down: %2 KB/s  |  Up: %3 KB/s"},

        // States
        {"state_checking", "Checking"},
        {"state_metadata", "Metadata"},
        {"state_downloading", "Downloading"},
        {"state_finished", "Finished"},
        {"state_seeding", "Seeding"},
        {"state_paused", "Paused"},
        {"state_unknown", "Unknown"},

        // Dialogs
        {"dlg_open_torrent", "Open Torrent"},
        {"dlg_save_to", "Save To"},
        {"dlg_add_magnet", "Add Magnet Link"},
        {"dlg_paste_magnet", "Paste magnet link:"},
        {"dlg_torrent_filter", "Torrent Files (*.torrent)"},
        {"dlg_download_complete", "Download Complete"},
        {"dlg_finished_msg", "%1 has finished downloading."},

        // Settings
        {"settings_title", "Preferences"},
        {"settings_general", "General"},
        {"settings_speed", "Speed Limits"},
        {"settings_default_save", "Default save path:"},
        {"settings_browse", "Browse..."},
        {"settings_language", "Language:"},
        {"settings_max_down", "Max download (KB/s, 0=unlimited):"},
        {"settings_max_up", "Max upload (KB/s, 0=unlimited):"},
        {"settings_start_tray", "Start minimized to tray"},
        {"settings_use_default_path", "Always use default save path (skip folder dialog)"},
        {"settings_theme", "Theme:"},

        // Tray
        {"tray_show", "Show"},
        {"tray_quit", "Quit"},

        // Welcome
        {"welcome_title", "Welcome to BATorrent!"},
        {"welcome_subtitle", "Your lightweight BitTorrent client"},
        {"welcome_step1_title", "Add a Torrent"},
        {"welcome_step1_desc", "Click 'Open' in the toolbar to add a .torrent file, or use 'Magnet' for magnet links. You can also drag & drop files directly into the window."},
        {"welcome_step2_title", "Manage Downloads"},
        {"welcome_step2_desc", "Use Pause, Resume, and Remove buttons to control your downloads. Select a torrent to see details, peers, and files in the panel below."},
        {"welcome_step3_title", "Settings & Tray"},
        {"welcome_step3_desc", "Go to Settings > Preferences to set speed limits and default save path. Closing the window minimizes to system tray — right-click the tray icon to quit."},
        {"welcome_got_it", "Got it!"},
        {"welcome_dont_show", "Don't show again"},
    };
}

void Translator::loadPortuguese()
{
    m_strings = {
        // Menu
        {"menu_file", "&Arquivo"},
        {"menu_torrent", "&Torrent"},
        {"menu_settings", "&Configuracoes"},
        {"menu_help", "A&juda"},
        {"action_open", "&Abrir Torrent..."},
        {"action_magnet", "Abrir Link &Magnet..."},
        {"action_quit", "&Sair"},
        {"action_pause", "&Pausar"},
        {"action_resume", "&Continuar"},
        {"action_remove", "&Remover"},
        {"action_settings", "&Preferencias..."},
        {"action_language", "&Idioma"},
        {"action_about", "&Sobre o BATorrent"},
        {"action_welcome", "&Guia de Boas-vindas"},

        // Toolbar
        {"tb_open", "Abrir"},
        {"tb_magnet", "Magnet"},
        {"tb_pause", "Pausar"},
        {"tb_resume", "Continuar"},
        {"tb_remove", "Remover"},
        {"tb_settings", "Config"},

        // Table headers
        {"col_name", "Nome"},
        {"col_size", "Tamanho"},
        {"col_progress", "Progresso"},
        {"col_down", "Download"},
        {"col_up", "Upload"},
        {"col_state", "Estado"},
        {"col_peers", "Peers"},

        // Details
        {"detail_general", "Geral"},
        {"detail_peers", "Peers"},
        {"detail_files", "Arquivos"},
        {"detail_name", "Nome:"},
        {"detail_save_path", "Salvar em:"},
        {"detail_size", "Tamanho:"},
        {"detail_downloaded", "Baixado:"},
        {"detail_progress", "Progresso:"},
        {"detail_down_speed", "Vel. Download:"},
        {"detail_up_speed", "Vel. Upload:"},
        {"detail_state", "Estado:"},
        {"detail_peers_count", "Peers:"},

        // Peer table
        {"peer_ip", "IP"},
        {"peer_port", "Porta"},
        {"peer_client", "Cliente"},
        {"peer_down", "Download"},
        {"peer_up", "Upload"},
        {"peer_progress", "Progresso"},

        // File table
        {"file_name", "Arquivo"},
        {"file_size", "Tamanho"},
        {"file_progress", "Progresso"},

        // Status
        {"status_no_torrents", "Nenhum torrent"},
        {"status_format", "%1 torrent(s)  |  Down: %2 KB/s  |  Up: %3 KB/s"},

        // States
        {"state_checking", "Verificando"},
        {"state_metadata", "Metadados"},
        {"state_downloading", "Baixando"},
        {"state_finished", "Concluido"},
        {"state_seeding", "Semeando"},
        {"state_paused", "Pausado"},
        {"state_unknown", "Desconhecido"},

        // Dialogs
        {"dlg_open_torrent", "Abrir Torrent"},
        {"dlg_save_to", "Salvar em"},
        {"dlg_add_magnet", "Adicionar Link Magnet"},
        {"dlg_paste_magnet", "Cole o link magnet:"},
        {"dlg_torrent_filter", "Arquivos Torrent (*.torrent)"},
        {"dlg_download_complete", "Download Concluido"},
        {"dlg_finished_msg", "%1 terminou de baixar."},

        // Settings
        {"settings_title", "Preferencias"},
        {"settings_general", "Geral"},
        {"settings_speed", "Limites de Velocidade"},
        {"settings_default_save", "Pasta padrao para salvar:"},
        {"settings_browse", "Procurar..."},
        {"settings_language", "Idioma:"},
        {"settings_max_down", "Max download (KB/s, 0=ilimitado):"},
        {"settings_max_up", "Max upload (KB/s, 0=ilimitado):"},
        {"settings_start_tray", "Iniciar minimizado na bandeja"},
        {"settings_use_default_path", "Sempre usar pasta padrao (pular dialogo de pasta)"},
        {"settings_theme", "Tema:"},

        // Tray
        {"tray_show", "Mostrar"},
        {"tray_quit", "Sair"},

        // Welcome
        {"welcome_title", "Bem-vindo ao BATorrent!"},
        {"welcome_subtitle", "Seu cliente BitTorrent leve e rapido"},
        {"welcome_step1_title", "Adicionar um Torrent"},
        {"welcome_step1_desc", "Clique em 'Abrir' na barra de ferramentas para adicionar um arquivo .torrent, ou use 'Magnet' para links magnet. Voce tambem pode arrastar e soltar arquivos diretamente na janela."},
        {"welcome_step2_title", "Gerenciar Downloads"},
        {"welcome_step2_desc", "Use os botoes Pausar, Continuar e Remover para controlar seus downloads. Selecione um torrent para ver detalhes, peers e arquivos no painel abaixo."},
        {"welcome_step3_title", "Configuracoes e Bandeja"},
        {"welcome_step3_desc", "Va em Configuracoes > Preferencias para definir limites de velocidade e pasta padrao. Fechar a janela minimiza para a bandeja do sistema — clique com o botao direito no icone da bandeja para sair."},
        {"welcome_got_it", "Entendi!"},
        {"welcome_dont_show", "Nao mostrar novamente"},
    };
}
