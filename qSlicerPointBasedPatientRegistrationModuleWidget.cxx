/*==============================================================================

  Program: 3D Slicer

  Portions (c) Copyright Brigham and Women's Hospital (BWH) All Rights Reserved.

  See COPYRIGHT.txt
  or http://www.slicer.org/copyright/copyright.txt for details.

  Unless required by applicable law or agreed to in writing, software
  distributed under the License is distributed on an "AS IS" BASIS,
  WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
  See the License for the specific language governing permissions and
  limitations under the License.

==============================================================================*/

// Qt includes
#include <QDebug>

// SlicerQt includes
#include "qSlicerPointBasedPatientRegistrationModuleWidget.h"
#include "ui_qSlicerPointBasedPatientRegistrationModule.h"

//-----------------------------------------------------------------------------
/// \ingroup Slicer_QtModules_ExtensionTemplate
class qSlicerPointBasedPatientRegistrationModuleWidgetPrivate: public Ui_qSlicerPointBasedPatientRegistrationModule
{
public:
  qSlicerPointBasedPatientRegistrationModuleWidgetPrivate();
};

//-----------------------------------------------------------------------------
// qSlicerPointBasedPatientRegistrationModuleWidgetPrivate methods

//-----------------------------------------------------------------------------
qSlicerPointBasedPatientRegistrationModuleWidgetPrivate::qSlicerPointBasedPatientRegistrationModuleWidgetPrivate()
{
}

//-----------------------------------------------------------------------------
// qSlicerPointBasedPatientRegistrationModuleWidget methods

//-----------------------------------------------------------------------------
qSlicerPointBasedPatientRegistrationModuleWidget::qSlicerPointBasedPatientRegistrationModuleWidget(QWidget* _parent)
  : Superclass( _parent )
  , d_ptr( new qSlicerPointBasedPatientRegistrationModuleWidgetPrivate )
{
}

//-----------------------------------------------------------------------------
qSlicerPointBasedPatientRegistrationModuleWidget::~qSlicerPointBasedPatientRegistrationModuleWidget()
{
}

//-----------------------------------------------------------------------------
void qSlicerPointBasedPatientRegistrationModuleWidget::setup()
{
  Q_D(qSlicerPointBasedPatientRegistrationModuleWidget);
  d->setupUi(this);
  this->Superclass::setup();
}

