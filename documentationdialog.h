#ifndef DOCUMENTATIONDIALOG_H
#define DOCUMENTATIONDIALOG_H

#include <QDialog>
#include <QVBoxLayout>
#include <QFile>
#include <QTextEdit>
#include <QUrl>
#include <QTextStream>

class DocumentationDialog : public QDialog
{
    Q_OBJECT
public:
    explicit DocumentationDialog(QWidget *parent = 0);
    
signals:
    
public slots:

private:
    QVBoxLayout *m_layout;
    QTextEdit *m_txtEdit;
};

#endif // DOCUMENTATIONDIALOG_H
