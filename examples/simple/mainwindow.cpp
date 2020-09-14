/*
 * This file is part of DockingPanes. (https://github.com/KestrelRadarSensors/dockingpanes)
 *
 * (C) 2020 Kestrel Radar Sensors (https://www.kestrelradarsensors.com)
 *
 * DockingPanes is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * DockingPanes is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.

 * You should have received a copy of the GNU General Public License
 * along with DockingPanes.  If not, see <https://www.gnu.org/licenses/>.
 */

#include <QAction>
#include <QActionGroup>
#include <QInputDialog>
#include <QLabel>
#include <QTextEdit>
#include <QMenu>
#include <QMessageBox>
#include <QUuid>

#include "DockingPaneManager.h"
#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    showMaximized();

    m_dockingPaneManager = new DockingPaneManager;

    auto clientWidget = new QTextEdit;

    m_dockingPaneManager->setClientWidget(clientWidget);

    setCentralWidget(m_dockingPaneManager->widget());

    DockingPaneBase *dockingWindow_1 = m_dockingPaneManager->createPane(QUuid::createUuid().toString(), "Tool Window 1", createLabel("Hello World 1"), QSize(200, 200), DockingPaneManager::dockBottom);
    DockingPaneBase *dockingWindow_2 = m_dockingPaneManager->createPane(QUuid::createUuid().toString(), "Tool Window 2", createLabel("Hello World 2"), QSize(200, 200), DockingPaneManager::dockLeft, nullptr);
    DockingPaneBase *dockingWindow_3 = m_dockingPaneManager->createPane(QUuid::createUuid().toString(), "Tool Window 3", createLabel("Hello World 3"), QSize(100, 200), DockingPaneManager::dockRight, nullptr);
    DockingPaneBase *dockingWindow_4 = m_dockingPaneManager->createPane(QUuid::createUuid().toString(), "Tool Window 4", createLabel("Hello World 4"), QSize(200, 200), DockingPaneManager::dockBottom, dockingWindow_2);
    DockingPaneBase *dockingWindow_5 = m_dockingPaneManager->createPane(QUuid::createUuid().toString(), "Tool Window 5", createLabel("Hello World 5"), QSize(200, 200), DockingPaneManager::dockFloat, nullptr);

    m_dockingPaneManager->hidePane(dockingWindow_3);

    Q_UNUSED(dockingWindow_1);
    Q_UNUSED(dockingWindow_2);
    Q_UNUSED(dockingWindow_4);
    Q_UNUSED(dockingWindow_5);

    m_layouts = new QActionGroup(this);
    m_layouts->setExclusive(true);
    connect(m_layouts, &QActionGroup::triggered, this, &MainWindow::onLayoutSelected);

    m_action_save_layout_as = new QAction("Save layout as...", this);
    connect(m_action_save_layout_as, &QAction::triggered, this, &MainWindow::onSaveLayout);

    m_menu_layout = new QMenu("Layout", this);
    ui->menuBar->addMenu(m_menu_layout);
    populateLayoutMenu();
}

MainWindow::~MainWindow()
{
    delete ui;
}

QLabel *MainWindow::createLabel(QString string)
{
    auto label = new QLabel(string);

    label->setAlignment(Qt::AlignCenter);

    return(label);
}

void MainWindow::populateLayoutMenu()
{
    m_menu_layout->clear();
    m_menu_layout->addAction(m_action_save_layout_as);
    m_menu_layout->addSeparator();
    m_menu_layout->addActions(m_layouts->actions());
}

void MainWindow::onSaveLayout()
{
    bool ok;
    QString name = QInputDialog::getText(this, "Add Layout...", "Layout name:", QLineEdit::Normal, "", &ok);
    if(ok && !name.isEmpty()) {
        for(auto action : m_layouts->actions()) {
            if(action->text() == name) {
                QMessageBox* box = new QMessageBox(this);
                box->setText("Name " + name + " already exists!");
                box->setAttribute(Qt::WA_DeleteOnClose);
                box->setIcon(QMessageBox::Warning);
                box->setWindowTitle("Warning");
                box->show();
                return;
            }
        }
        QString data = m_dockingPaneManager->saveLayout(name);
        QAction* action = m_layouts->addAction(name);
        m_layouts_data.insert(name, data);
        action->setCheckable(true);
        action->setChecked(true);
        m_current_layout = name;
        populateLayoutMenu();
    }
}

void MainWindow::onLayoutSelected(QAction* current)
{
    m_current_layout = current->text();
    if(!m_dockingPaneManager->applyLayout(m_layouts_data.value(m_current_layout))) {
        QMessageBox* box = new QMessageBox(this);
        box->setText("Error while applying layout " + m_current_layout);
        box->setAttribute(Qt::WA_DeleteOnClose);
        box->setIcon(QMessageBox::Critical);
        box->setWindowTitle("Error");
        box->show();
        return;
    }
}
