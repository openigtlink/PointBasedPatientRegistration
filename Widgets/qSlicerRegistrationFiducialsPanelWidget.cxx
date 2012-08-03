/*==============================================================================

  Program: 3D Slicer

  Copyright (c) Kitware Inc.

  See COPYRIGHT.txt
  or http://www.slicer.org/copyright/copyright.txt for details.

  Unless required by applicable law or agreed to in writing, software
  distributed under the License is distributed on an "AS IS" BASIS,
  WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
  See the License for the specific language governing permissions and
  limitations under the License.

  This file was originally developed by Jean-Christophe Fillion-Robin, Kitware Inc.
  and was partially funded by NIH grant 3P41RR013218-12S1

==============================================================================*/

// RegistrationFiducialsPanel Widgets includes
#include "qSlicerRegistrationFiducialsPanelWidget.h"
#include "ui_qSlicerRegistrationFiducialsPanelWidget.h"

#include <QList>
#include <QTableWidgetSelectionRange>


//-----------------------------------------------------------------------------
/// \ingroup Slicer_QtModules_PointBasedPatientRegistration
class qSlicerRegistrationFiducialsPanelWidgetPrivate
  : public Ui_qSlicerRegistrationFiducialsPanelWidget
{
  Q_DECLARE_PUBLIC(qSlicerRegistrationFiducialsPanelWidget);
protected:
  qSlicerRegistrationFiducialsPanelWidget* const q_ptr;

public:
  qSlicerRegistrationFiducialsPanelWidgetPrivate(
    qSlicerRegistrationFiducialsPanelWidget& object);
  virtual void setupUi(qSlicerRegistrationFiducialsPanelWidget*);
};

// --------------------------------------------------------------------------
qSlicerRegistrationFiducialsPanelWidgetPrivate
::qSlicerRegistrationFiducialsPanelWidgetPrivate(
  qSlicerRegistrationFiducialsPanelWidget& object)
  : q_ptr(&object)
{
}

// --------------------------------------------------------------------------
void qSlicerRegistrationFiducialsPanelWidgetPrivate
::setupUi(qSlicerRegistrationFiducialsPanelWidget* widget)
{
  this->Ui_qSlicerRegistrationFiducialsPanelWidget::setupUi(widget);
}

//-----------------------------------------------------------------------------
// qSlicerRegistrationFiducialsPanelWidget methods

//-----------------------------------------------------------------------------
qSlicerRegistrationFiducialsPanelWidget
::qSlicerRegistrationFiducialsPanelWidget(QWidget* parentWidget)
  : Superclass( parentWidget )
  , d_ptr( new qSlicerRegistrationFiducialsPanelWidgetPrivate(*this) )
{
  Q_D(qSlicerRegistrationFiducialsPanelWidget);
  d->setupUi(this);

  QStringList list;
  list << tr("Point #") << tr("R") << tr("A") << tr("S");

  d->ImagePointsTable->setRowCount(1);
  d->ImagePointsTable->setColumnCount(4);
  d->ImagePointsTable->setHorizontalHeaderLabels(list);
  d->ImagePointsTable->verticalHeader()->hide();
  d->ImagePointsTable->setSelectionBehavior(QAbstractItemView::SelectRows);
  d->ImagePointsTable->setSelectionMode(QAbstractItemView::SingleSelection);
  d->ImagePointsTable->setEditTriggers(QAbstractItemView::NoEditTriggers);

  d->PhysicalPointsTable->setRowCount(1);
  d->PhysicalPointsTable->setColumnCount(4);
  d->PhysicalPointsTable->setHorizontalHeaderLabels(list);
  d->PhysicalPointsTable->verticalHeader()->hide();
  d->PhysicalPointsTable->setSelectionBehavior(QAbstractItemView::SelectRows);
  d->PhysicalPointsTable->setSelectionMode(QAbstractItemView::SingleSelection);
  d->PhysicalPointsTable->setEditTriggers(QAbstractItemView::NoEditTriggers);
}

//-----------------------------------------------------------------------------
qSlicerRegistrationFiducialsPanelWidget
::~qSlicerRegistrationFiducialsPanelWidget()
{
}
