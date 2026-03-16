#ifndef THEMEMANAGER_H
#define THEMEMANAGER_H

#include <QString>

class ThemeManager
{
public:
    enum Theme { Dark, Light, Midnight };

    static ThemeManager &instance();

    void setTheme(Theme theme);
    Theme theme() const { return m_theme; }
    QString styleSheet() const;
    QString accentColor() const;
    QString bgColor() const;
    QString surfaceColor() const;
    QString textColor() const;
    QString mutedColor() const;

    static QStringList themeNames();

private:
    ThemeManager() = default;
    Theme m_theme = Dark;
};

#endif
