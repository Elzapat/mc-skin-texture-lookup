#include <QWidget>

class QPushButton;
class QLineEdit;
class QTextBrowser;
class QNetworkReply;

class MainWidget : public QWidget {
    Q_OBJECT

    public:
        explicit MainWidget(QWidget* parent = 0);
        ~MainWidget();

    private slots:
        void inputEntered();

    private:
        QPushButton* m_searchButton;
        QLineEdit* m_input;
        QTextBrowser* m_output;
};
