#include "welcomedialog.h"
#include "../core/translator.h"
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QLabel>
#include <QPushButton>
#include <QCheckBox>
#include <QPixmap>

WelcomeDialog::WelcomeDialog(QWidget *parent)
    : QDialog(parent)
{
    setWindowTitle("BATorrent");
    setFixedSize(520, 520);

    setStyleSheet(R"(
        QDialog {
            background-color: #121212;
            color: #d0d0d0;
        }
        QLabel {
            color: #d0d0d0;
        }
        QPushButton {
            background-color: #2a1015;
            color: #e0e0e0;
            border: 1px solid #6b2020;
            border-radius: 6px;
            padding: 10px 32px;
            font-size: 14px;
            font-weight: bold;
        }
        QPushButton:hover {
            background-color: #6b2020;
            color: #ffffff;
        }
        QCheckBox {
            color: #888888;
            spacing: 8px;
            font-size: 11px;
        }
        QCheckBox::indicator {
            width: 14px;
            height: 14px;
            border: 1px solid #333333;
            border-radius: 3px;
            background-color: #1a1a1a;
        }
        QCheckBox::indicator:checked {
            background-color: #6b2020;
        }
    )");

    auto *mainLayout = new QVBoxLayout(this);
    mainLayout->setContentsMargins(32, 24, 32, 24);
    mainLayout->setSpacing(8);

    // Logo
    auto *logoLabel = new QLabel;
    QPixmap logo(":/images/logo1.png");
    logoLabel->setPixmap(logo.scaled(80, 80, Qt::KeepAspectRatio, Qt::SmoothTransformation));
    logoLabel->setAlignment(Qt::AlignCenter);
    mainLayout->addWidget(logoLabel);

    // Title
    auto *titleLabel = new QLabel(tr_("welcome_title"));
    titleLabel->setStyleSheet("font-size: 22px; font-weight: bold; color: #c43030;");
    titleLabel->setAlignment(Qt::AlignCenter);
    mainLayout->addWidget(titleLabel);

    auto *subtitleLabel = new QLabel(tr_("welcome_subtitle"));
    subtitleLabel->setStyleSheet("font-size: 12px; color: #888888; margin-bottom: 12px;");
    subtitleLabel->setAlignment(Qt::AlignCenter);
    mainLayout->addWidget(subtitleLabel);

    // Steps
    auto addStep = [&](const QString &num, const QString &titleKey, const QString &descKey) {
        auto *stepLayout = new QHBoxLayout;
        stepLayout->setSpacing(12);

        auto *numLabel = new QLabel(num);
        numLabel->setFixedSize(32, 32);
        numLabel->setAlignment(Qt::AlignCenter);
        numLabel->setStyleSheet(
            "background-color: #2a1015; color: #c43030; border-radius: 16px;"
            "font-size: 14px; font-weight: bold; border: 1px solid #6b2020;");

        auto *textLayout = new QVBoxLayout;
        auto *stepTitle = new QLabel(tr_(titleKey));
        stepTitle->setStyleSheet("font-size: 13px; font-weight: bold; color: #e0e0e0;");
        auto *stepDesc = new QLabel(tr_(descKey));
        stepDesc->setWordWrap(true);
        stepDesc->setStyleSheet("font-size: 11px; color: #999999; line-height: 1.4;");

        textLayout->addWidget(stepTitle);
        textLayout->addWidget(stepDesc);
        textLayout->setSpacing(2);

        stepLayout->addWidget(numLabel, 0, Qt::AlignTop);
        stepLayout->addLayout(textLayout, 1);

        mainLayout->addLayout(stepLayout);
    };

    addStep("1", "welcome_step1_title", "welcome_step1_desc");
    addStep("2", "welcome_step2_title", "welcome_step2_desc");
    addStep("3", "welcome_step3_title", "welcome_step3_desc");

    mainLayout->addStretch();

    // Bottom
    m_dontShowCheck = new QCheckBox(tr_("welcome_dont_show"));

    auto *btnLayout = new QHBoxLayout;
    btnLayout->addWidget(m_dontShowCheck);
    btnLayout->addStretch();
    auto *gotItBtn = new QPushButton(tr_("welcome_got_it"));
    connect(gotItBtn, &QPushButton::clicked, this, &QDialog::accept);
    btnLayout->addWidget(gotItBtn);

    mainLayout->addLayout(btnLayout);
}

bool WelcomeDialog::dontShowAgain() const
{
    return m_dontShowCheck->isChecked();
}
