#include <QWidget>

class QPushButton;
class QLineEdit;
class QTextBrowser;
class QNetworkReply;
class QNetworkAccessManager;

class MainWidget : public QWidget {
    Q_OBJECT

    public:
        explicit MainWidget(QWidget* parent = 0);
        ~MainWidget();

    private slots:
        void inputEntered();
        void onReply(QNetworkReply* reply); 

    private:
        QPushButton* m_searchButton;
        QLineEdit* m_input;
        QTextBrowser* m_output;
        QNetworkReply* m_reply;
        QNetworkAccessManager* m_manager;
        int m_phase;
};
