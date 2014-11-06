#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QMessageBox>
#include <QGraphicsScene>
#include <QGraphicsItem>
#include <QApplication>
#include <QPoint>
#include <QtCore/qmath.h>
#include <QFileDialog>
#include <QTextStream>
#include <QLabel>
#include <QColorDialog>
#include <QComboBox>
#include <QPushButton>
#include "documentationdialog.h"
#include "graphicsview.h"

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    enum TOOL{RIEN,LIGNE,RECT,CERCLE,POLYGON};
    explicit MainWindow(QWidget *parent = 0);
    QString execute(QString str);
    QList<QString> cutToPieces(QString str);
    Qt::BrushStyle selectBrush();
    void loadBackItems();
    void setSelectedTool(TOOL t);
    ~MainWindow();

private slots:
    void on_mouseClicked(QPoint);
    void on_mouseMoved(QPoint);
    void mouseButton(Qt::MouseButton);
    void on_actionA_propos_triggered();
    void on_actionQuitter_triggered();
    void on_lineEdit_returnPressed();
    void on_actionRectangle_triggered();
    void on_actionCercle_triggered();
    void on_actionPolygon_triggered();
    void on_actionLigne_triggered();
    void on_actionNouveau_triggered();
    void on_actionAnnuler_triggered();
    void on_actionRefaire_triggered();
    void on_actionEnregistrer_triggered();
    void on_actionOuvrir_triggered();
    void on_actionDocumentation_triggered();
    void on_actionCouleur_triggered();
    void brushSelector_on_okClicked();
    void on_actionRemplissement_triggered();
    void sync_BackState();

private:
    Ui::MainWindow *ui;
    QGraphicsScene *m_scene;
    GraphicsView *m_view;
    QPen m_pen;
    QList<int*> m_gList;
    QList<int> m_infos;
    TOOL m_selectedTool;
    int m_toolState;
    int m_backState;
    Qt::MouseButton m_mouseButton;
    QPolygonF m_polygon;
    QLabel m_mouseCords;
    bool m_isNumber;
    QComboBox* comboBox;
    Qt::BrushStyle* brush;
    QBrush m_brush;
};

#endif // MAINWINDOW_H
