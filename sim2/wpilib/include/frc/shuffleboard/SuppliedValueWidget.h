/*----------------------------------------------------------------------------*/
/* Copyright (c) 2019 FIRST. All Rights Reserved.                             */
/* Open Source Software - may be modified and shared by FRC teams. The code   */
/* must be accompanied by the FIRST BSD license file in the root directory of */
/* the project.                                                               */
/*----------------------------------------------------------------------------*/

#pragma once

#include <functional>
#include <memory>

#include <networktables/NetworkTable.h>
#include <networktables/NetworkTableEntry.h>
#include <wpi/Twine.h>

#include "frc/shuffleboard/ShuffleboardComponent.h"
#include "frc/shuffleboard/ShuffleboardComponent.inc"
#include "frc/shuffleboard/ShuffleboardComponentBase.h"
#include "frc/shuffleboard/ShuffleboardContainer.h"
#include "frc/shuffleboard/ShuffleboardWidget.h"

namespace frc {
template <typename T>
class SuppliedValueWidget : public ShuffleboardWidget<SuppliedValueWidget<T> > {
 public:
  SuppliedValueWidget(ShuffleboardContainer& parent, const wpi::Twine& title,
                      std::function<T()> supplier,
                      std::function<void(nt::NetworkTableEntry, T)> setter)
      : ShuffleboardValue(title),
        ShuffleboardWidget<SuppliedValueWidget<T> >(parent, title),
        m_supplier(supplier),
        m_setter(setter) {}

  void BuildInto(std::shared_ptr<nt::NetworkTable> parentTable,
                 std::shared_ptr<nt::NetworkTable> metaTable) override {
    this->BuildMetadata(metaTable);
    metaTable->GetEntry("Controllable").SetBoolean(false);

    auto entry = parentTable->GetEntry(this->GetTitle());
    m_setter(entry, m_supplier());
  }

 private:
  std::function<T()> m_supplier;
  std::function<void(nt::NetworkTableEntry, T)> m_setter;
};
}  // namespace frc
