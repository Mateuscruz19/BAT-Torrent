#include "speedgraph.h"
#include <QPainter>
#include <QPainterPath>
#include <algorithm>

SpeedGraph::SpeedGraph(QWidget *parent)
    : QWidget(parent)
{
    setMinimumHeight(80);
    setMaximumHeight(120);
}

void SpeedGraph::addDataPoint(int downloadSpeed, int uploadSpeed)
{
    m_downloadData.append(downloadSpeed);
    m_uploadData.append(uploadSpeed);

    if (m_downloadData.size() > MaxPoints) {
        m_downloadData.removeFirst();
        m_uploadData.removeFirst();
    }

    update();
}

void SpeedGraph::setAccentColor(const QColor &color)
{
    m_accentColor = color;
    update();
}

void SpeedGraph::paintEvent(QPaintEvent *)
{
    QPainter p(this);
    p.setRenderHint(QPainter::Antialiasing);

    int w = width();
    int h = height();

    // Background
    p.fillRect(rect(), QColor(0x14, 0x14, 0x14));

    if (m_downloadData.isEmpty())
        return;

    // Find max value for scaling
    int maxVal = 1024; // minimum 1 KB/s scale
    for (int v : m_downloadData) maxVal = std::max(maxVal, v);
    for (int v : m_uploadData) maxVal = std::max(maxVal, v);
    maxVal = static_cast<int>(maxVal * 1.2); // 20% headroom

    int n = m_downloadData.size();
    float xStep = static_cast<float>(w) / (MaxPoints - 1);
    float xOffset = (MaxPoints - n) * xStep;

    // Grid lines
    p.setPen(QPen(QColor(0x25, 0x25, 0x25), 1));
    for (int i = 1; i < 4; ++i) {
        int y = h * i / 4;
        p.drawLine(0, y, w, y);
    }

    // Draw upload area (behind download)
    QColor uploadColor(0x30, 0x60, 0xc4);
    {
        QPainterPath path;
        path.moveTo(xOffset, h);
        for (int i = 0; i < n; ++i) {
            float x = xOffset + i * xStep;
            float y = h - (static_cast<float>(m_uploadData[i]) / maxVal * (h - 4));
            if (i == 0) path.lineTo(x, y);
            else path.lineTo(x, y);
        }
        path.lineTo(xOffset + (n - 1) * xStep, h);
        path.closeSubpath();

        QColor fill = uploadColor;
        fill.setAlpha(40);
        p.fillPath(path, fill);
        // Line
        p.setPen(QPen(uploadColor, 1.5));
        for (int i = 1; i < n; ++i) {
            float x0 = xOffset + (i - 1) * xStep;
            float y0 = h - (static_cast<float>(m_uploadData[i - 1]) / maxVal * (h - 4));
            float x1 = xOffset + i * xStep;
            float y1 = h - (static_cast<float>(m_uploadData[i]) / maxVal * (h - 4));
            p.drawLine(QPointF(x0, y0), QPointF(x1, y1));
        }
    }

    // Draw download area
    {
        QPainterPath path;
        path.moveTo(xOffset, h);
        for (int i = 0; i < n; ++i) {
            float x = xOffset + i * xStep;
            float y = h - (static_cast<float>(m_downloadData[i]) / maxVal * (h - 4));
            if (i == 0) path.lineTo(x, y);
            else path.lineTo(x, y);
        }
        path.lineTo(xOffset + (n - 1) * xStep, h);
        path.closeSubpath();

        QColor fill = m_accentColor;
        fill.setAlpha(50);
        p.fillPath(path, fill);
        // Line
        p.setPen(QPen(m_accentColor, 1.5));
        for (int i = 1; i < n; ++i) {
            float x0 = xOffset + (i - 1) * xStep;
            float y0 = h - (static_cast<float>(m_downloadData[i - 1]) / maxVal * (h - 4));
            float x1 = xOffset + i * xStep;
            float y1 = h - (static_cast<float>(m_downloadData[i]) / maxVal * (h - 4));
            p.drawLine(QPointF(x0, y0), QPointF(x1, y1));
        }
    }

    // Scale label
    QString scaleText;
    if (maxVal >= 1024 * 1024)
        scaleText = QString::number(maxVal / (1024.0 * 1024.0), 'f', 1) + " MB/s";
    else
        scaleText = QString::number(maxVal / 1024.0, 'f', 0) + " KB/s";

    p.setPen(QColor(0x70, 0x70, 0x70));
    p.setFont(QFont(font().family(), 8));
    p.drawText(4, 12, scaleText);

    // Legend
    int legendX = w - 140;
    p.setPen(m_accentColor);
    p.drawLine(legendX, 8, legendX + 14, 8);
    p.setPen(QColor(0x90, 0x90, 0x90));
    p.drawText(legendX + 18, 12, "Download");

    p.setPen(uploadColor);
    p.drawLine(legendX + 80, 8, legendX + 94, 8);
    p.setPen(QColor(0x90, 0x90, 0x90));
    p.drawText(legendX + 98, 12, "Upload");
}
