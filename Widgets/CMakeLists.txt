project(qSlicer${MODULE_NAME}ModuleWidgets)

set(KIT ${PROJECT_NAME})

set(${KIT}_EXPORT_DIRECTIVE "Q_SLICER_MODULE_${MODULE_NAME_UPPER}_WIDGETS_EXPORT")

set(${KIT}_INCLUDE_DIRECTORIES
  )

set(${KIT}_SRCS
  qSlicerRegistrationFiducialsPanelWidget.cxx
  qSlicerRegistrationFiducialsPanelWidget.h
  qSlicerRegistrationFiducialsTableModel.cxx
  qSlicerRegistrationFiducialsTableModel.h
  #qSlicerFooBarWidget.cxx
  #qSlicerFooBarWidget.h
  )

set(${KIT}_MOC_SRCS
  #qSlicerFooBarWidget.h
  qSlicerRegistrationFiducialsPanelWidget.h
  qSlicerRegistrationFiducialsTableModel.h
  )

set(${KIT}_UI_SRCS
  ../Resources/UI/qSlicerRegistrationFiducialsPanelWidget.ui
  )

set(${KIT}_RESOURCES
  ../Resources/UI/qSlicerRegistrationFiducialsPanelWidget.ui
  )

set(${KIT}_TARGET_LIBRARIES
  vtkSlicerAnnotationsModuleMRML
  vtkSlicerAnnotationsModuleLogic
  vtkSlicer${MODULE_NAME}ModuleLogic
  )

#-----------------------------------------------------------------------------
SlicerMacroBuildModuleWidgets(
  NAME ${KIT}
  EXPORT_DIRECTIVE ${${KIT}_EXPORT_DIRECTIVE}
  INCLUDE_DIRECTORIES ${${KIT}_INCLUDE_DIRECTORIES}
  SRCS ${${KIT}_SRCS}
  MOC_SRCS ${${KIT}_MOC_SRCS}
  UI_SRCS ${${KIT}_UI_SRCS}
  TARGET_LIBRARIES ${${KIT}_TARGET_LIBRARIES}
  RESOURCES ${${KIT}_RESOURCES}
  WRAP_PYTHONQT
  )

