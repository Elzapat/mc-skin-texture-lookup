#include <QtWidgets>
#include <QNetworkAccessManager>
#include <QNetworkReply>
#include <QJsonDocument>
#include <QJsonObject>
#include <QJsonArray>
#include <QByteArray>
#include <QStringList>
#include "mainwidget.h"

MainWidget::MainWidget(QWidget* parent) : QWidget(parent), m_phase(0) {
    this->setWindowTitle(tr("Minecraft Skin Texture Lookup"));
    this->setMinimumWidth(600);
    this->setMinimumHeight(200);

    m_searchButton = new QPushButton(tr("Search"));
    m_input = new QLineEdit();
    m_output = new QTextBrowser();
    m_manager = new QNetworkAccessManager(this);

    QVBoxLayout* mainLayout = new QVBoxLayout;

    QHBoxLayout* h1 = new QHBoxLayout;
    h1->addWidget(m_input);
    h1->addWidget(m_searchButton);

    mainLayout->addLayout(h1);
    mainLayout->addWidget(m_output);

    setLayout(mainLayout);

    connect(m_searchButton, SIGNAL(released()), this, SLOT(inputEntered()));
    connect(m_input, SIGNAL(returnPressed()), this, SLOT(inputEntered()));
    connect(m_manager, SIGNAL(finished(QNetworkReply*)), this, SLOT(onReply(QNetworkReply*)));
}

MainWidget::~MainWidget() {
    delete m_searchButton;
    delete m_input;
    delete m_output;
    delete m_manager;
}

void MainWidget::inputEntered() {
    m_output->clear();
    m_output->append(tr("Loading..."));
    QString username = m_input->text();

    const QUrl url("https://api.mojang.com/users/profiles/minecraft/" + username);
    QNetworkRequest request(url);
    m_reply = m_manager->get(request);
}

void MainWidget::onReply(QNetworkReply* reply) {
    if (m_reply->error() != QNetworkReply::NoError) {
        m_output->append("Error (this username most likely doesn't exist)");
        return;
    }

    QString strReply = reply->readAll();
    QJsonDocument jsonResponse = QJsonDocument::fromJson(strReply.toUtf8());
    QJsonObject jsonObject = jsonResponse.object();

    if (m_phase == 0) {
        QString id = jsonObject["id"].toString();
        
        const QUrl url("https://sessionserver.mojang.com/session/minecraft/profile/" + id);
        QNetworkRequest request(url);
        m_reply = m_manager->get(request);

        m_phase = 1;
    } else if (m_phase == 1) {
        QJsonArray properties = jsonObject["properties"].toArray();
        QString value = jsonObject["properties"]
            .toArray()
            .at(0)
            .toObject()["value"]
            .toString();

        QString textureJsonStr = QString(QByteArray::fromBase64(value.toUtf8()));

        QJsonDocument textureJsonDoc = QJsonDocument::fromJson(textureJsonStr.toUtf8());
        QJsonObject textureJson = textureJsonDoc.object();

        QString textureUrl = textureJson["textures"]
            .toObject()["SKIN"]
            .toObject()["url"]
            .toString();

        QStringList explodedUrl = textureUrl.split('/');
        QString skinTexture = explodedUrl.at(explodedUrl.size() - 1);

        m_output->clear();
        m_output->append(m_input->text() + "'s skin texture:\n");
        m_output->append(skinTexture);

        m_phase = 0;
    }
}
