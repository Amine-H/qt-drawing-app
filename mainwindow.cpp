#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    m_scene = new QGraphicsScene(this);
    m_toolState = 0;
    m_backState = 0;
    m_view = new GraphicsView();
    m_view->setScene(m_scene);
    m_view->setRenderHint(QPainter::Antialiasing);
    m_view->setCursor(Qt::CrossCursor);
    m_view->setAlignment(Qt::AlignLeft | Qt::AlignTop);

    ui->statusBar->addWidget(&m_mouseCords);

    QObject::connect(m_view,SIGNAL(mouseReleased(QPoint)),this,SLOT(on_mouseClicked(QPoint)));
    QObject::connect(m_view,SIGNAL(mouseMoved(QPoint)),this,SLOT(on_mouseMoved(QPoint)));
    QObject::connect(m_view,SIGNAL(mouseButton(Qt::MouseButton)),this,SLOT(mouseButton(Qt::MouseButton)));

    ui->lineEdit->setFocus();

    brush = new Qt::BrushStyle;
    m_brush.setStyle(Qt::NoBrush);

    ui->verticalLayout_2->addWidget(m_view);

}

QString MainWindow::execute(QString str)
{
    QList<QString> strList = cutToPieces(str);
    QString result;
    if(strList.at(0) == "ajout")
    {
        if(strList.at(1) == "ligne")
        {
            if(strList.length() == 6)
            {
                for(int i = 2;i < strList.length();i++)
                {
                    strList.at(i).toInt(&m_isNumber);
                    if(!m_isNumber)
                    {
                        result = "erreur données incorrects";
                        goto RET;
                    }
                }
                int *p = new int[5];
                sync_BackState();
                m_scene->addLine(strList.at(2).toInt(),
                                     strList.at(3).toInt(),
                                     strList.at(4).toInt(),
                                     strList.at(5).toInt(),m_pen);
                p[0] = LIGNE;
                for(int i = 1;i < 5;i++)
                {
                    p[i] = strList.at(i + 1).toInt();
                }
                m_gList.append(p);
                return "<font color = 'blue'>[!] Ligne ajoutée</font>";
            }
            else
            {
                result = "erreur vous devez fournir 4 ENTIERs aprés ligne";
            }
        }
        else if(strList.at(1) == "rect")
        {
            if(strList.length() == 6)
            {
                for(int i = 2;i < strList.length();i++)
                {
                    strList.at(i).toInt(&m_isNumber);
                    if(!m_isNumber)
                    {
                        result = "erreur données incorrects";
                        goto RET;
                    }
                }
                int *p = new int[5];
                sync_BackState();
                m_scene->addRect(strList.at(2).toInt(),
                                     strList.at(3).toInt(),
                                     strList.at(4).toInt() - strList.at(2).toInt(),
                                     strList.at(5).toInt()- strList.at(3).toInt(),m_pen,m_brush);
                p[0] = RECT;
                for(int i = 1;i < 5;i++)
                {
                    p[i] = strList.at(i + 1).toInt();
                }
                m_gList.append(p);
                return "<font color = 'blue'>[!] Rectangle ajoutée</font>";
            }
            else
            {
                result = "erreur vous devez fournir 4 ENTIERs aprés rect";
            }
        }
        else if(strList.at(1) == "cercle")
        {
            if(strList.length() == 5)
            {
                for(int i = 2;i < strList.length();i++)
                {
                    strList.at(i).toInt(&m_isNumber);
                    if(!m_isNumber)
                    {
                        result = "[!] erreur données incorrects";
                        goto RET;
                    }
                }
                int *p = new int[4];
                sync_BackState();
                m_scene->addEllipse(strList.at(2).toInt(),
                                     strList.at(3).toInt(),
                                     strList.at(4).toInt()/2,
                                     strList.at(4).toInt()/2,m_pen,m_brush);
                p[0] = CERCLE;
                for(int i = 1;i < 4;i++)
                {
                    p[i] = strList.at(i + 1).toInt();
                }
                m_gList.append(p);
                return "<font color = 'blue'>[!] Cercle ajoutée</font>";
            }
            else
            {
                result = "erreur vous devez fournir 3 ENTIERs aprés cercle";
            }
        }
        else if(strList.at(1) == "polygon")
        {
            if(strList.length() > 2)
            {
                for(int i = 2;i < strList.length();i++)
                {
                    strList.at(i).toInt(&m_isNumber);
                    if(!m_isNumber)
                    {
                        result = "erreur données incorrects";
                        goto RET;
                    }
                }
                if(strList.length()%2 == 1)
                {
                    result = "erreur données incorrects";
                    goto RET;
                }
                for(int i = 2;i < strList.length();i++)
                {
                    strList.at(i).toInt(&m_isNumber);
                    if(!m_isNumber)
                    {
                        result = "valeurs non ENTIERs";
                        goto RET;
                    }
                }
                QPolygonF polygon;
                for(int i = 2;i < strList.length() - 1;i++)
                {
                    polygon << QPoint(strList.at(i).toInt(),strList.at(i+1).toInt());
                }
                int n = polygon.size();
                int *p = new int[n + 2];
                sync_BackState();
                m_scene->addPolygon(polygon,m_pen,m_brush);
                p[0] = POLYGON;
                p[1] = n;
                for(int i = 2;i < n + 1;i++)
                {
                    p[i] = polygon.at(i - 2).x();
                    p[i + 1] = polygon.at(i - 2).y();
                }
                m_gList.append(p);
                return "<font color = 'blue'>Polygon ajoutée</font>";
            }
            else
            {
                result = "erreur vous devez fournir des ENTIERs aprés polygon";
            }
        }
        else
        {
            result = "erreur du syntax (expected a shape)";
        }
    }
    else if(strList.at(0) == "effacer")
    {
        m_backState = 0;
        m_scene->clear();
        m_gList.clear();
        return "<font color = 'blue'>command executée</font>";
    }
    else if(strList.at(0) == "clear")
    {
        ui->textEdit->clear();
        return "<font color = 'blue'>command executée</font>";
    }
    else if (strList.at(0) == "set")
    {
        if(strList.length() == 3)
        {
            if(strList.at(1) == "color")
            {
                if(strList.at(2) == "black")
                {
                    QColor c(0,0,0);
                    m_pen.setColor(c);
                }
                else if (strList.at(2) == "blue")
                {
                    QColor c(0,0,255);
                    m_pen.setColor(c);
                }
                else if (strList.at(2) == "red")
                {
                    QColor c(255,0,0);
                    m_pen.setColor(c);
                }
                else if (strList.at(2) == "yellow")
                {
                    QColor c(255,255,0);
                    m_pen.setColor(c);
                }
                else if (strList.at(2) == "green")
                {
                    QColor c(0,255,0);
                    m_pen.setColor(c);
                }
                else if (strList.at(2) == "cyan")
                {
                    QColor c(0,255,255);
                    m_pen.setColor(c);
                }
                else
                {
                    result = "erreur du syntax, couleur non supporté";
                }
            }
            else
            {
                result = "erreur du syntax (expected color)";
            }
        }
        else if(strList.length() == 5)
        {
            strList.at(2).toInt(&m_isNumber);if(!m_isNumber){result = "erreur du syntax vous devez fournir des nombres set color r g b";goto RET;}
            strList.at(3).toInt(&m_isNumber);if(!m_isNumber){result = "erreur du syntax vous devez fournir des nombres set color r g b";goto RET;}
            strList.at(4).toInt(&m_isNumber);if(!m_isNumber){result = "erreur du syntax vous devez fournir des nombres set color r g b";goto RET;}

            int r,g,b;
            r = strList.at(2).toInt();
            g = strList.at(3).toInt();
            b = strList.at(4).toInt();
            if((r >= 0 && r < 256) &&
                (g >= 0 && g < 256) &&
                (b >= 0 && b < 256))
            {
                m_pen.setColor(QColor(r,g,b));
                result = "<font color = 'blue'>couleur modifié</font>";
            }
            else
            {
                result = "erreur du syntax valeurs erronée";
            }
        }
        else
        {
            result = "erreur du syntax, vous devez fournir 3/5 arguments set color nom/set color r g b)";
        }
    }
    else
    {
        result = "erreur du syntax (expected ajout/effacer/set/clear)";
    }
    RET:return "<font color = 'red'>[!] " + result + "</font>";
}

//cuts the string to substrings
QList<QString> MainWindow::cutToPieces(QString str)
{
    QList<QString> args;
    int from = 0;
    int to = 0;
    while((to = str.indexOf(" ",from)) != -1)
    {
        args.append(str.mid(from,to-from));
        from = to + 1;
    }
    args.append(str.mid(from,str.length() - from));
    return args;
}

Qt::BrushStyle MainWindow::selectBrush()
{
    QDialog* dialog = new QDialog(this);
    dialog->setWindowTitle("Remplissement");
    QVBoxLayout layout;
    comboBox = new QComboBox(dialog);
    QPushButton* ok = new QPushButton("Ok",dialog);
    QPushButton* cancel = new QPushButton("Annuler",dialog);

    QObject::connect(ok,SIGNAL(clicked()),this,SLOT(brushSelector_on_okClicked()));
    QObject::connect(ok,SIGNAL(clicked()),dialog,SLOT(close()));
    QObject::connect(cancel,SIGNAL(clicked()),dialog,SLOT(close()));

    dialog->setLayout(&layout);

    comboBox->addItem("Rien");
    comboBox->addItem("Solid Pattern");
    comboBox->addItem("Hor Pattern");
    comboBox->addItem("Ver Pattern");
    comboBox->addItem("Cross Pattern");
    comboBox->addItem("DiagCross Pattern");

    layout.addWidget(comboBox);
    layout.addWidget(ok);
    layout.addWidget(cancel);

    dialog->exec();

    return *brush;
}

void MainWindow::setSelectedTool(MainWindow::TOOL t)
{
    m_selectedTool = t;
    m_toolState = 0;
    m_infos.clear();
}

//show about box
void MainWindow::on_actionA_propos_triggered()
{
    QMessageBox::information(this,"A propos..","this application was developped by <b>Amine Hakkou</b>");
}

//closing the program
void MainWindow::on_actionQuitter_triggered()
{
    qApp->quit();
}

//execute command
void MainWindow::on_lineEdit_returnPressed()
{
    if(!ui->lineEdit->text().isEmpty())
    {
        QString command = ui->lineEdit->text().toLower();
        ui->textEdit->append(">" + command);
        QString res = execute(command);
        ui->textEdit->append(res);
        ui->lineEdit->setText("");
    }
}

MainWindow::~MainWindow()
{
    delete brush;
    while (!m_gList.isEmpty())
        delete m_gList.takeFirst();
    delete ui;
}

void MainWindow::on_actionRectangle_triggered()
{
    if(ui->actionRectangle->isChecked())
    {
        setSelectedTool(RECT);
        m_infos.clear();
        ui->actionLigne->setChecked(false);
        ui->actionCercle->setChecked(false);
        ui->actionPolygon->setChecked(false);
    }
    else
    {
        setSelectedTool(RIEN);
        m_toolState = 0;
    }
}

void MainWindow::on_actionCercle_triggered()
{
    if(ui->actionCercle->isChecked())
    {
        setSelectedTool(CERCLE);
        m_infos.clear();
        ui->actionLigne->setChecked(false);
        ui->actionRectangle->setChecked(false);
        ui->actionPolygon->setChecked(false);
    }
    else
    {
        setSelectedTool(RIEN);
        m_toolState = 0;
    }
}

void MainWindow::on_actionPolygon_triggered()
{
    if(ui->actionPolygon->isChecked())
    {
        setSelectedTool(POLYGON);
        m_infos.clear();
        ui->actionLigne->setChecked(false);
        ui->actionRectangle->setChecked(false);
        ui->actionCercle->setChecked(false);
    }
    else
    {
        setSelectedTool(RIEN);
        m_toolState = 0;
    }
}

void MainWindow::on_actionLigne_triggered()
{
    if(ui->actionLigne->isChecked())
    {
        setSelectedTool(LIGNE);
        m_infos.clear();
        ui->actionRectangle->setChecked(false);
        ui->actionCercle->setChecked(false);
        ui->actionPolygon->setChecked(false);
    }
    else
    {
        setSelectedTool(RIEN);
        m_toolState = 0;
    }
}

void MainWindow::on_mouseClicked(QPoint point)
{
    switch(m_selectedTool)
    {
        case RIEN:
            return;
        case LIGNE:
            if(m_toolState == 0)
            {
                m_infos.append(point.x());
                m_infos.append(point.y());
                m_toolState++;
            }
            else
            {
                int *p = new int[5];
                sync_BackState();
                m_scene->addLine(m_infos.at(0),m_infos.at(1),point.x(),point.y(),m_pen);
                p[0] = LIGNE;
                p[1] = m_infos.at(0);
                p[2] = m_infos.at(1);
                p[3] = point.x();
                p[4] = point.y();
                m_gList.append(p);
                m_infos.clear();
                m_toolState = 0;
            }
        break;
        case RECT:
            if(m_toolState == 0)
            {
                m_infos.append(point.x());
                m_infos.append(point.y());
                m_toolState++;
            }
            else
            {
                int *p = new int[5];
                int x1 = m_infos.at(0);
                int y1 = m_infos.at(1);
                int x2 = point.x();
                int y2 = point.y();
                int h = abs(y2 - y1);
                int w = abs(x2 - x1);
                if(x1 > x2)
                {
                    x1 -= w;
                }
                if(y1 > y2)
                {
                    y1 -= h;
                }
                sync_BackState();
                m_scene->addRect(x1,
                                     y1,
                                     w,
                                     h,
                                 m_pen,m_brush);
                p[0] = RECT;
                p[1] = m_infos.at(0);
                p[2] = m_infos.at(1);
                p[3] = abs(point.x() - m_infos.at(0));
                p[4] = abs(point.y() - m_infos.at(1));
                m_gList.append(p);
                m_infos.clear();
                m_toolState = 0;
            }
        break;
        case CERCLE:
            if(m_toolState == 0)
            {
                m_infos.append(point.x());
                m_infos.append(point.y());
                m_toolState++;
            }
            else
            {
                int *p = new int[4];
                int r = (int)qSqrt(qPow(m_infos.at(0)-point.x(),2)
                                   + qPow(m_infos.at(1)-point.y(),2));
                sync_BackState();
                m_scene->addEllipse(m_infos.at(0) - r,
                                        m_infos.at(1) - r,
                                        r*2,
                                        r*2,m_pen,m_brush);
                p[0] = CERCLE;
                p[1] = m_infos.at(0) - r;
                p[2] = m_infos.at(1) - r;
                p[3] = r*2;
                m_gList.append(p);
                m_infos.clear();
                m_toolState = 0;
            }
        break;
        case POLYGON:
            if(m_toolState == 0 || (m_mouseButton == Qt::LeftButton))
            {
                m_polygon << point;
                m_toolState++;
            }
            else
            {
                int n = m_polygon.size();
                int *p = new int[n + 2];
                sync_BackState();
                m_scene->addPolygon(m_polygon,m_pen,m_brush);
                p[0] = POLYGON;
                p[1] = n;
                for(int i = 2;i < n + 2;i+=2)
                {
                    p[i] = m_polygon.at(i).x();
                    p[i + 1] = m_polygon.at(i + 1).y();
                }
                m_gList.append(p);
                m_polygon.clear();
                m_toolState = 0;
            }
        break;
    }
}

void MainWindow::on_mouseMoved(QPoint point)
{
    m_mouseCords.setText("X = " + QString::number(point.x()) +
                         " Y = " + QString::number(point.y()));
}

void MainWindow::mouseButton(Qt::MouseButton button)
{
    m_mouseButton = button;
}

void MainWindow::on_actionNouveau_triggered()
{
    m_scene->clear();
    m_toolState = 0;
    m_backState = 0;
    m_infos.clear();
    while (!m_gList.isEmpty())
        delete m_gList.takeFirst();
}

void MainWindow::on_actionAnnuler_triggered()
{
    if(m_backState + 1 < m_gList.length())
    {
        m_backState++;
        m_scene->clear();
        loadBackItems();
    }
}

void MainWindow::on_actionRefaire_triggered()
{
    if(m_backState - 1 > 0)
    {
        m_backState--;
        m_scene->clear();
        loadBackItems();
    }
}

void MainWindow::loadBackItems()
{
    for(int i = 0;i < m_gList.length() - m_backState;i++)
    {
        switch(m_gList.at(i)[0])
        {
        case LIGNE:
            m_scene->addLine(m_gList.at(i)[1],
                             m_gList.at(i)[2],
                             m_gList.at(i)[3],
                             m_gList.at(i)[4],m_pen);
            break;
        case RECT:
            m_scene->addRect(m_gList.at(i)[1],
                             m_gList.at(i)[2],
                             m_gList.at(i)[3],
                             m_gList.at(i)[4],m_pen,m_brush);
            break;
        case CERCLE:
            m_scene->addEllipse(m_gList.at(i)[1],
                             m_gList.at(i)[2],
                             m_gList.at(i)[3],
                             m_gList.at(i)[3],m_pen,m_brush);
            break;
        case POLYGON:
            QPolygonF polygon;
            for(int j = 3;j < m_gList.at(i)[2] - 1;j++)
            {
                polygon << QPoint(m_gList.at(i)[j],m_gList.at(i)[j + 1]);
            }
            m_scene->addPolygon(polygon,m_pen,m_brush);
            break;
        }
    }
}

void MainWindow::on_actionEnregistrer_triggered()
{
    QString nom = QFileDialog::getSaveFileName(this,"Enregistrer");
    if (!nom.isEmpty())
    {
        QPixmap pixMap = QPixmap::grabWidget(m_view);
        pixMap.save(nom + ".png");
    }
}

void MainWindow::on_actionOuvrir_triggered()
{
    QString nom = QFileDialog::getOpenFileName(this,"Ouvrir");
    if(!nom.isEmpty())
    {
        QFile file(nom);
        if(nom.endsWith(".png"))
        {
            if (!file.open(QFile::ReadOnly))
            {
                QMessageBox::critical(this,"Erreur",
                                     "Erreur de lecture du fichier " +
                                     nom +
                                     file.errorString());
            }
            else
            {
                QPixmap pixMap;
                pixMap.load(nom);
                m_scene->addPixmap(pixMap);
            }
        }
        else if(nom.endsWith(".ESTO_CAO"))
        {
            QString line_command;
            QString res;
            if(!file.open(QFile::ReadOnly | QFile::Text))
            {
                QMessageBox::critical(this,"Erreur",
                                     "Erreur de lecture du fichier " +
                                     nom +
                                     file.errorString());
            }
            else
            {
                ui->textEdit->append("<font color = 'green'>Lecture des commandes du fichier " + nom + "</font>");
                QTextStream in(&file);
                while(!in.atEnd())
                {
                    line_command = in.readLine();
                    if(!line_command.isEmpty())
                    {
                        ui->textEdit->append(">" + line_command);
                        if(!line_command.startsWith("//"))
                        {
                            res = execute(line_command);
                            ui->textEdit->append(res);
                        }
                    }
                }

            }
        }
    }
}

void MainWindow::on_actionDocumentation_triggered()
{
    DocumentationDialog dDialog(this);
    dDialog.exec();
}

void MainWindow::on_actionCouleur_triggered()
{
    QColor couleur = QColorDialog::getColor(Qt::black,this);
    m_pen.setColor(couleur);
    m_brush.setColor(couleur);
}

void MainWindow::brushSelector_on_okClicked()
{
    switch(comboBox->currentIndex())
    {
    case 0:
        *brush = Qt::NoBrush;
        break;
    case 1:
        *brush = Qt::SolidPattern;
        break;
    case 2:
        *brush = Qt::HorPattern;
        break;
    case 3:
        *brush = Qt::VerPattern;
        break;
    case 4:
        *brush = Qt::CrossPattern;
    case 5:
        *brush = Qt::DiagCrossPattern;
    }
}

void MainWindow::on_actionRemplissement_triggered()
{
    m_brush.setStyle(selectBrush());
}

void MainWindow::sync_BackState()
{
    if(m_backState != 0)
    {
        for(int i = m_gList.length() - m_backState;i < m_gList.length();i++)
        {
            delete m_gList.takeAt(i);
        }
    }
    m_backState = 0;
}
