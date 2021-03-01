#include <QtWidgets>
#include <QNetworkAccessManager>
#include <QNetworkReply>
#include "mainwidget.h"

MainWidget::MainWidget(QWidget* parent): QWidget(parent) {

    this->setWindowTitle(tr("Minecraft Skin Texture Lookup"));
    this->setMinimumWidth(500);
    this->setMinimumHeight(300);

    m_searchButton = new QPushButton(tr("Search"));
    m_input = new QLineEdit();
    m_output = new QTextBrowser();

    QVBoxLayout* mainLayout = new QVBoxLayout;

    QHBoxLayout* h1 = new QHBoxLayout;
    h1->addWidget(m_input);
    h1->addWidget(m_searchButton);

    mainLayout->addLayout(h1);
    mainLayout->addWidget(m_output);

    setLayout(mainLayout);

    connect(m_searchButton, SIGNAL(released()), this, SLOT(inputEntered()));
    connect(m_input, SIGNAL(returnPressed()), this, SLOT(inputEntered()));
}

MainWidget::~MainWidget() {

    delete m_searchButton;
    delete m_input;
    delete m_output;
}

void MainWidget::inputEntered() {

    QNetworkAccessManager *manager = new QNetworkAccessManager(this);

    m_output->clear();
    QString username = m_input->text();

    const QUrl url("https://api.mojang.com/users/profiles/minecraft/" + username);
    QNetworkRequest request(url);
    QNetworkReply* reply = manager->get(request);

    if (reply->error() == QNetworkReply::NetworkError::NoError) {
        m_output->append("no error:" + reply->readAll());
        m_output->append(url.url());
    } else {
        m_output->append("Error: " + reply->errorString());
    }
}
