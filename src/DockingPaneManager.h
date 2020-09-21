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

#ifndef DOCKINGPANEMANAGER_H
#define DOCKINGPANEMANAGER_H

#include <QObject>

#include "DockingPanes_global.h"

class QBoxLayout;
class QDomDocument;
class QDomNode;

class DockAutoHideButton;
class DockingFrameStickers;
class DockingPaneBase;
class DockingPaneFlyoutWidget;
class DockingPaneManagerPrivate;
class DockingPaneSplitterContainer;
class DockingPaneTitleWidget;
class DockingTargetWidget;

class DOCKINGPANESSHARED_EXPORT DockingPaneManager : QObject
{
    Q_OBJECT
    Q_DECLARE_PRIVATE(DockingPaneManager)
    public:
        enum DockPosition
        {
            dockLeft,
            dockRight,
            dockTop,
            dockBottom,
            dockFloat,
            dockTab
        };

        DockingPaneManager();

        QWidget *widget(void);
        DockingPaneBase *setClientWidget(QWidget *widget);
        DockingPaneBase *createPane(QString id, QString title, QWidget *widget, QSize initialSize, DockingPaneManager::DockPosition dockPosition, DockingPaneBase *neighbourPane = nullptr);
        void closePane(QString id);
        void closePane(DockingPaneBase *dockingPane);
        void hidePane(DockingPaneBase *dockingPane);
        void showPane(DockingPaneBase *dockingPane);
        void deletePane(DockingPaneBase *pane);
        void unpinPane(DockingPaneBase *pane);
        void closePinnedPane(DockingPaneBase *pane);
        void openFlyout(DockAutoHideButton *button);
        void replacePane(DockingPaneBase *oldPane, DockingPaneBase *newPane);
        void updateAutohideButton(DockingPaneBase *oldContainer, DockingPaneBase *oldPane, DockingPaneBase *newContainer, DockingPaneBase *newPane);
        DockingPaneBase *dockPane(DockingPaneBase *newPane, DockingPaneManager::DockPosition dockPosition, DockingPaneBase *neighbourPane);
        QString saveLayout(QString id);
        bool applyLayout(QString layout);
        void setMainWindow(QWidget *widget);
        QWidget *mainWindow(void);
        void dumpPaneList(void);
        void floatingPaneMoved(DockingPaneBase *pane, QPoint cursorPos);
        void floatingPaneEndMove(DockingPaneBase *pane, QPoint cursorPos);
        void floatingPaneStartMove(DockingPaneBase *pane, QPoint cursorPos);
        void removePinnedButton(DockingPaneBase *dockingPaneContainer, DockingPaneBase *dockingPane = nullptr);

    protected:
        virtual bool eventFilter(QObject *obj, QEvent *event) override;
        DockingPaneManagerPrivate *const d_ptr;
};

#endif // DOCKINGPANEMANAGER_H
