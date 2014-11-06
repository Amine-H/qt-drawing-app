#include "documentationdialog.h"

DocumentationDialog::DocumentationDialog(QWidget *parent) :
    QDialog(parent)
{
    setWindowTitle("Documentation");
    this->setMinimumSize(900,600);

    m_layout = new QVBoxLayout(this);
    m_txtEdit = new QTextEdit(this);
    m_txtEdit->setReadOnly(true);

    this->setLayout(m_layout);
    m_layout->addWidget(m_txtEdit);

    QFile fichier("doc.html");
    if(!fichier.open(QFile::ReadOnly | QFile::Text))
    {
        m_txtEdit->setText("<H1>erreur de lecture du fichier doc.html</H1>");
    }
    else
    {
        QTextStream in(&fichier);
        m_txtEdit->append(in.readAll());
    }
}
