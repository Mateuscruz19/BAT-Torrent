#include "thememanager.h"
#include <QStringList>

ThemeManager &ThemeManager::instance()
{
    static ThemeManager tm;
    return tm;
}

void ThemeManager::setTheme(Theme theme)
{
    m_theme = theme;
}

QStringList ThemeManager::themeNames()
{
    return {"Dark", "Light", "Midnight"};
}

QString ThemeManager::accentColor() const
{
    switch (m_theme) {
    case Light: return "#a02020";
    case Midnight: return "#ff3333";
    default: return "#c43030";
    }
}

QString ThemeManager::bgColor() const
{
    switch (m_theme) {
    case Light: return "#f0f0f0";
    case Midnight: return "#080810";
    default: return "#141414";
    }
}

QString ThemeManager::surfaceColor() const
{
    switch (m_theme) {
    case Light: return "#ffffff";
    case Midnight: return "#0e0e18";
    default: return "#1a1a1a";
    }
}

QString ThemeManager::textColor() const
{
    switch (m_theme) {
    case Light: return "#1a1a1a";
    case Midnight: return "#d0d0d0";
    default: return "#b0b0b0";
    }
}

QString ThemeManager::mutedColor() const
{
    switch (m_theme) {
    case Light: return "#666666";
    case Midnight: return "#555555";
    default: return "#707070";
    }
}

QString ThemeManager::styleSheet() const
{
    QString bg = bgColor();
    QString sf = surfaceColor();
    QString tx = textColor();
    QString mt = mutedColor();
    QString ac = accentColor();

    // Derive hover/selection colors from accent
    QString acDark, acLight, acSurface, border;
    switch (m_theme) {
    case Light:
        acDark = "#801818";
        acLight = "#c82828";
        acSurface = "#f5e0e0";
        border = "#d0d0d0";
        break;
    case Midnight:
        acDark = "#aa1111";
        acLight = "#ff4444";
        acSurface = "#1a0808";
        border = "#1a1a2a";
        break;
    default: // Dark
        acDark = "#3d1520";
        acLight = "#6b2020";
        acSurface = "#2a1015";
        border = "#252525";
        break;
    }

    return QString(R"(
        QMainWindow { background-color: %1; }
        QMenuBar {
            background-color: %2; color: %3;
            border-bottom: 1px solid %8; font-weight: bold;
        }
        QMenuBar::item:selected { background-color: %7; color: %3; }
        QMenu {
            background-color: %2; color: %3; border: 1px solid %8;
        }
        QMenu::item:selected { background-color: %7; }
        QMenu::separator { background-color: %8; height: 1px; margin: 4px 8px; }
        QToolBar {
            background-color: %2; border-bottom: 1px solid %7;
            spacing: 4px; padding: 4px 6px;
        }
        QToolButton {
            color: %3; background-color: transparent;
            border: 1px solid transparent; border-radius: 4px;
            padding: 4px 10px; font-weight: bold; font-size: 11px;
        }
        QToolButton:hover {
            background-color: %7; color: %3; border-color: %5;
        }
        QToolButton:pressed { background-color: %5; }
        QTableView {
            background-color: %1; alternate-background-color: %2;
            color: %3; gridline-color: transparent; border: none;
            selection-background-color: %7; selection-color: %3;
            font-size: 12px;
        }
        QHeaderView::section {
            background-color: %2; color: %4;
            border: none; border-bottom: 1px solid %4;
            padding: 8px 6px; font-weight: bold; font-size: 11px;
        }
        QTabWidget::pane {
            background-color: %1; border: 1px solid %8;
            border-top: 1px solid %6;
        }
        QTabBar::tab {
            background-color: %2; color: %9;
            padding: 8px 20px; border: 1px solid %8;
            border-bottom: none; margin-right: 1px; font-weight: bold;
        }
        QTabBar::tab:selected {
            background-color: %7; color: %4; border-top: 2px solid %4;
        }
        QTabBar::tab:hover { background-color: %7; color: %4; }
        QLabel { color: %3; }
        QStatusBar {
            background-color: %2; color: %9;
            border-top: 1px solid %8; font-size: 11px;
        }
        QSplitter::handle { background-color: %8; height: 2px; }
        QProgressBar {
            background-color: %8; border: 1px solid %8;
            border-radius: 4px; text-align: center;
            color: %3; font-weight: bold; font-size: 11px;
        }
        QProgressBar::chunk {
            background: qlineargradient(x1:0, y1:0, x2:1, y2:0,
                stop:0 %5, stop:1 %6);
            border-radius: 3px;
        }
        QScrollBar:vertical {
            background-color: %1; width: 8px; border: none;
        }
        QScrollBar::handle:vertical {
            background-color: %8; border-radius: 4px; min-height: 20px;
        }
        QScrollBar::handle:vertical:hover { background-color: %5; }
        QScrollBar::add-line:vertical, QScrollBar::sub-line:vertical { height: 0; }
        QScrollBar:horizontal {
            background-color: %1; height: 8px; border: none;
        }
        QScrollBar::handle:horizontal {
            background-color: %8; border-radius: 4px; min-width: 20px;
        }
        QScrollBar::handle:horizontal:hover { background-color: %5; }
        QScrollBar::add-line:horizontal, QScrollBar::sub-line:horizontal { width: 0; }
    )").arg(bg, sf, tx, ac, acDark, acLight, acSurface, border, mt);
}
